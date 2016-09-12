#include "ModelData.h"

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif    
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }

#endif

UINT                        g_NumBoneMatricesMax = 0;
D3DXMATRIXA16*              g_pBoneMatrices = NULL;

namespace
{
	/* フレーム行列を更新 */
	
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, const Matrix* pParentMatrix)
	{
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase; //変換してpFrameに渡しとる

		if (pParentMatrix != NULL) //親行列に情報ありますねぇ！
		{
			//２つの行列（pFrameとpParentMatrix）の積を合成済み行列格納変数MATRIX16くんに出力
			D3DXMatrixMultiply(&pFrame->CombinedTransformationMatrix, &pFrame->TransformationMatrix, pParentMatrix);
		}
		else //親行列に情報ないです
		{
			//pFrameの変換行列だけ合成して終わりでいいんじゃない？
			pFrame->CombinedTransformationMatrix = pFrame->TransformationMatrix;
		}

		if (pFrame->pFrameSibling != NULL) //pFrameの兄弟姉妹ってなんだよ。 情報ありますあります
		{//またUpdateFrameMatrices関数使うの？　これあれじゃない？　再帰呼び出しとか言うやつ
		
			//pFrameの兄弟姉妹をベースに親を親Matrixにして実行ゥー！
			UpdateFrameMatrices(pFrame->pFrameSibling, pParentMatrix);
		}
		
		if (pFrame->pFrameFirstChild != NULL) //pFrameの第一子？　子持ちなの？
		{//またこの関数だよ

			//pFrameの第一子をFrameベースに、pFrameの合成済み行列を親に。　複雑な家庭環境かな
			UpdateFrameMatrices(pFrame->pFrameFirstChild, &pFrame->CombinedTransformationMatrix);
		}
	}

	//スキニングメッシュを生成します【直訳】
	HRESULT GenerateSkinnedMesh(
		IDirect3DDevice9* pd3dDevice,//DrawPrimitive ベースのレンダリングの実行、リソースの作成、システムレベル変数の処理、ガンマ曲線レベルの調整、パレットの処理、シェーダの作成を行うには、IDirect3DDevice9 インターフェイスのメソッドを使いましょう（説明長い）
		D3DXMESHCONTAINER_DERIVED* pMeshContainer //満を持して登場の派生メッシュコンテナーくん
		)
	{
		HRESULT hr = S_OK;

		//D3DCAPS9.. Microsoft® Direct3D® オブジェクトが公開するハードウェアの能力を表す。 表すだけ？
		D3DCAPS9 d3dCaps;

		//->GetDeviceCaps ..指定されたデバイスに関するデバイス固有の情報を取得!! ハードウェア能力でも手に入れたの？
		pd3dDevice->GetDeviceCaps(&d3dCaps);

		if (pMeshContainer->pSkinInfo == NULL)	//スキン情報ありませんよ－－－、な時。
		{
			return hr; //S_OK !! S_OK !!
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);	//メッシュ解放してんの？
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);//骨コンビネーションバフ【直訳】のリリー酢

		{
			/*	【原文直訳】-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-
				・パレットのサイズを取得
				・最初の9定数が他のデータのために使用されます。各4x3の行列は3定数を占めています。
				・（96 ~ 9) / 3 、すなわち最大一定の数 - 使用される定数
			-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	-	*/

			UINT MaxMatrices = 26; //まっくすぎょるれつ

			//メッシュコンテナのパレットエントリーの数をパレット☆コンテナのスキン情報の骨の数(最小)に書き換えたる！！ みたいな
			pMeshContainer->NumPaletteEntries = min(MaxMatrices, pMeshContainer->pSkinInfo->GetNumBones());

						//D3DXMESHOPT 列挙型.. 実行するメッシュ最適化の種類を指定する。
			DWORD Flags = D3DXMESHOPT_VERTEXCACHE;	//_VERTEXCACHE.. 面の順番を変更し、頂点キャッシュのキャッシュ ヒット率を向上させる。
			
			
			//.VertexShaderVersion.. ピクセルシェーダのメインバージョンとサブバージョンを表す 2 つの数字
			//D3DVS_VERSION.. 頂点シェーダー バージョン番号トークンを作成しす。
			//バージョン番号.. メジャーバージョン番号と、頂点シェーダーのマイナーバージョン番号との組み合わせ。(1,1)(2,0)(3,0)が有効ですって
			if (d3dCaps.VertexShaderVersion >= D3DVS_VERSION(1, 1))
			{
				
				pMeshContainer->UseSoftwareVP = false; //使いますソフトウェアVP【直訳】…なにこれ？ falseぶち込んだろ！

				//「 |= 」演算子.. ビットごとの包括的 OR 代入
				//D3DXMESH列挙型.. メッシュの作成オプションを指定するフラグ。
				//_MANAGED.. D3DXMESH_VB_MANAGED と D3DXMESH_IB_MANAGED の両方を指定したことになる。　なんだよそいつら
				//_VB_MANAGED.. 頂点バッファに D3DPOOL_MANAGEDメモリクラスを使用する。D3DPOOL_MANAGEDってなんだよ
				//_IB_MANAGED.. インデックスバッファに D3DPOOL_MANAGEDメモリクラスを使用する。だからD3DPOOL_MANAGEDってなｎ

				//D3DPOOL_MANAGEDってなに--!?
				//D3DPOOL列挙型.. リソースのバッファーを保持するメモリークラスを定義します。らしい
				//_MANAGED.. リソースは、必要に応じて、デバイスからアクセス可能なメモリーへ自動的にコピーされます。マネージ リソースはシステム メモリーによって保持されるので、デバイスが失われても再作成する必要はありません。詳細については、「リソースの管理 (Direct3D 9)」を参照してください。マネージ リソースはロック可能です。直接変更できるのは、システムメモリーのコピーのみです。Direct3D は、加えた変更を、必要に応じてドライバーからアクセス可能なメモリーにコピーします。(MSDNコピペ)
				//原文長い（憤怒） IDirect3DDevice9Exの時は無効ですってさ。

				Flags |= D3DXMESH_MANAGED; //頂点バッファとインデックスバッファをMANEDED（どうにかする??）オプションをOR代入で指定してやりましょ
			}
			else
			{
				pMeshContainer->UseSoftwareVP = true; //VPって言うソフトウェア使うんでしょ？ trueだし。

				//_SYSTEMMEM.. D3DXMESH_VB_SYSTEMMEMとD3DXMESH_IB_SYSTEMMEMの両方を指定したことになる。 まぁーた両方指定かこの野郎
				//_VB_SYSTEMMEM.. 頂点バッファにD3DPOOL_SYSTEMMEMメモリクラスを使用する。 はいはい、どうせ次はインデックスバッファなんでしょ？
				//_IB_SYSTEMMEM.. インデックスバッファにD3DPOOL_SYSTEMMEMメモリクラスを使用する。 ほぉーら当たった！！

				//D3DPOOL_SYSTEMMEM.. リソースは、通常は Direct3D デバイスからアクセスできないメモリーに配置されます。
				//ま				このようなメモリー割り当ては、システム RAM を消費しますが、ページ可能 RAM を減らすことはありません。
				//た				これらのリソースは、デバイスが失われても再作成の必要はありません。
				//原				このプール内のリソースは、ロックが可能で、D3DPOOL_DEFAULT を使用して作成されたメモリー リソースに対する IDirect3DDevice9::UpdateSurface または IDirect3DDevice9::UpdateTexture 処理のソースとして使用できます。
				//文長い（憤怒）
				
				Flags |= D3DXMESH_SYSTEMMEM; //頂点バッファとインデックスバッファのSYSTEMMEM（システム男性??）オプションをOR代入で指定してやりましょ
			}

			SAFE_RELEASE(pMeshContainer->MeshData.pMesh); //メッシュッシュ解ー放

			//->ConvertToBlendedMesh.. メッシュを受け取り、
			//						頂点単位のブレンドの重みとボーンの組み合わせテーブルを適用した新しいメッシュを返す。
			//						このテーブルは、ボーンパレットとメッシュのサブセットの関係を表す。
			//成功した場合は、D3D_OK を返す。
			//失敗した場合は、D3DERR_INVALIDCALL を返す。
			hr = pMeshContainer->pSkinInfo->ConvertToIndexedBlendedMesh
				(
				pMeshContainer->pOrigMesh,			//[in]入力メッシュ
				Flags,								//[in]現 在 は 使 わ れ て い な い
				pMeshContainer->NumPaletteEntries,	//[in]行列パレットスキニングに使用可能なボーン行列の数
				pMeshContainer->pAdjacency,			//[in]入力メッシュの隣接性情報　　なにそれ
				NULL, NULL, NULL,					//NULLでいいらしい。	出力メッシュも 各面の新しいインデックスを格納している転送先バッファへのポインタも各頂点の新しいインデックスを格納する、ID3DXBuffer インターフェイスへのポインタのアドレスも必要ねぇーんだよ！！
				&pMeshContainer->NumInfl,			//[out]このスキニングメソッドに必要なボーンの影響の1面あたりの最大数を格納する DWORDへのポインタ
				&pMeshContainer->NumAttributeGroups,//[out]ボーンの組み合わせテーブルに含まれるボーンの数へのポインタ。
				&pMeshContainer->pBoneCombinationBuf,//[out]ボーンの組み合わせテーブルへのポインタ。このデータは、D3DXBONECOMBINATION 構造体に格納される。
				&pMeshContainer->MeshData.pMesh//[out]新しいメッシュへのポインタ。
				);

			//What何このThisなに
			if (FAILED(hr))
				//goto e_Exit;


			//FVFは、私たちの宣言子と一致する必要があります。頂点シェーダは、FFパイプラインほど寛容ではありません【原文直訳】
			DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
				D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;
		}
	}

}