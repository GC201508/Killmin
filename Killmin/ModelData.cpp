#include "ModelData.h"
#include "Animation.h"
#include <vector>

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
			return hr; //S_OK !! S_OK ?
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
			{
				goto e_Exit;
			}

			//FVFは、私たちの宣言子と一致する必要があります。頂点シェーダは、FFパイプラインほど寛容ではありません【原文直訳】

			//FVF.. 柔軟な頂点フォーマットですって。
			//	単一のデータ ストリームにインターリーブされて格納されている頂点の内容を記述するために使われる。
			//	FVF コードは、通常、固定機能の頂点処理によって処理されるデータを指定するために使われる。

			//_POSITION_MASK.. 位置座標ビットを抽出するマスク

			//_NORMAL.. 頂点フォーマットが頂点法線ベクトルを含む。
			//		このフラグを、D3DFVF_XYZRHW フラグと共に使うことはできない。

			//_TEX0 ~ TEX8.. この頂点に対応するテクスチャ座標セットの番号。
			//			これらのフラグに対応する数字は連続番号になっていない。

			//_LASTBETA_UBYTE4.. 頂点位置データの最後のベータフィールドは UBYTE4 型になる。
			//			ベータフィールドのデータは、行列パレットスキニングで行列インデックスを指定するために使う。


			//NewFVFに頂点フォーマット色んな情報をビット演算で求めた結果を放り込んでるんじゃないかな？
			DWORD NewFVF = (pMeshContainer->MeshData.pMesh->GetFVF() & D3DFVF_POSITION_MASK) | D3DFVF_NORMAL |
				D3DFVF_TEX1 | D3DFVF_LASTBETA_UBYTE4;

			//NewFVFがメッスコンテナのメッシュデータのメッシュの頂点フォーマットと異なる時
			if (NewFVF != pMeshContainer->MeshData.pMesh->GetFVF())
			{
				LPD3DXMESH pMesh; //LPだからメッシュのアドレスか何かでしょ？

				//->CloneMeshFVF.. 柔軟な頂点フォーマット (FVF) コードを使ってメッシュのコピーを作成する。
				//成功したらS_OKを返す
				//失敗したら以下のパターン
				//	D3DERR_INVALIDCALL.. メソッドの呼び出しが無効である。(メソッドのパラメータに無効な値が設定されている場合とか）
				//	E_OUTOFMEMORY.. Microsoft® Direct3D® が呼び出しを完了するための十分なメモリを割り当てることができなかった。

				hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(
					pMeshContainer->MeshData.pMesh->GetOptions(), //[in] メッシュの作成オプションを指定する、D3DXMESH列挙からの1つまたは複数のフラグの組み合わせ。
					NewFVF, //[in] FVF コードの組み合わせ。出力メッシュに含まれる頂点の頂点フォーマットを指定する。コードの値については「D3DFVF」を参照しろ。
					pd3dDevice, //[in] メッシュに関連付けられているデバイスオブジェクトを表すIDirect3DDevice9インターフェイスへのポインタ。
					&pMesh //[out] コピーされたメッシュを表す ID3DXPMesh インターフェイスへのポインタのアドレス。
					);
				if (!FAILED(hr))	//S_OKもらえなかったぞ！！
				{
					pMeshContainer->MeshData.pMesh->Release();	//解放してやる！！
					pMeshContainer->MeshData.pMesh = pMesh;	//こんなもんいらねーよ！！ くれてやる！！
					pMesh = NULL; //NULLったる！！
				}
			}

			//D3DVERTEXELEMENT9.. 頂点データのレイアウトを定義します。
			//				各頂点には 1 つ以上のデータ型を含めることができ、各データ型は頂点要素によって記述されすま。
			//MAX_FVF_DECL_SIZE.. 頂点宣言における要素の最大数。D3DDECL_END の場合は(+1) を追加すしま。
			D3DVERTEXELEMENT9 pDecl[MAX_FVF_DECL_SIZE];
			LPD3DVERTEXELEMENT9 pDeclCur; //アドレスかな
			hr = pMeshContainer->MeshData.pMesh->GetDeclaration(pDecl);
			if (FAILED(hr))
			{
				goto e_Exit;
			}

			// 頂点シェーダはD3DCOLORとしてUBYTE4を解釈するので、タイプを更新するために期待しています
			// 注：これはそれがD3DCOLORに、次にUBYTE4データが浮いているように変換してだろう、CloneMeshで行うことができません
			// これは「キャスト」操作の詳細です... 【原文直訳】
			
			pDeclCur = pDecl; //アドレスにくれてやる

			//頂点データのストリーム番号が0xff(８ビット?)でない間ループしたろ
			while (pDeclCur->Stream != 0xff)
			{
				//->Usage.. データの使用方法、つまり頂点データのレイアウトと頂点シェーダーの間の相互接続性を定義してやす
				//->UsageIndex.. 使用データを変更してユーザーが複数の使用方法を指定できるようにしやす

				//D3DDECLUSAGE(コア列挙型).. 頂点データの使用目的を指定しやーす。たくさん種類あんよ
				//_BLENDINDICES.. ブレンドインデックスデータでーす。
				//			インデックス付きパレットスキニング用の行列インデックスを指定するにゃ、
				//			使用方法インデックスを0として使ってめぇ。 


				if ((pDeclCur->Usage == D3DDECLUSAGE_BLENDINDICES) and(pDeclCur->UsageIndex == 0))
				{
					//->Type.. D3DDECLTYPEとして指定されているデータ型。
					//		データのサイズを定義する、事前定義された複数の型のいずれかになまりす。
					//		暗黙的な型が決まっているメソッドもあります　モアリ・マス。

					//D3DDECLTYPE(コア列挙型).. 頂点の宣言データ型を定義ますし。
					//_D3DCOLOR.. 0 ～ 1 の範囲にマップされる 4 成分の圧縮された符号なしバイトどぅえす。
					//		入力は D3DCOLOR で、RGBA の順序に展開されまぁーす。
					pDeclCur->Type = D3DDECLTYPE_D3DCOLOR;//入れたれ
				}
				pDeclCur++; //なにインクリメントしてんだよ
			}

			//->UpdateSemantics.. このメソッドを利用すると、ユーザーは頂点バッファのデータレイアウトを変更せずに、
			//				メッシュの宣言を変更できる。
			//				古い宣言と新しい宣言のフォーマットが同じ頂点サイズである場合にのみ、呼び出しが有効となる。
			//成功した場合は、D3D_OKを返す。
			//失敗した場合は、D3DERR_INVALIDCALLを返す。

			hr = pMeshContainer->MeshData.pMesh->UpdateSemantics(pDecl);//[in, out] メッシュの頂点の頂点フォーマットを記述する
																		//D3DVERTEXELEMENT9 要素の配列。
																		//この宣言子配列の上限は MAX_FVF_DECL_SIZE である。
			if (FAILED(hr))
			{
				goto e_Exit;
			}

			//骨基質のためのバッファを割り当てますが、別のメッシュは同じ大きさの1以上割り当てられていない場合にのみ
			//げんぶんちょくやく

			//骨Matrixの最大値がメッシュコンテナのスキン情報の骨より少ないやんけ！ てな時。
			if (g_NumBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones())
			{
				//ボーン数更新したろ。
				g_NumBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

				//ブレンド行列のためのスペースを割り当て(原文翻)
				delete[] g_pBoneMatrices; //ポインタ消すんすか？
				g_pBoneMatrices = new D3DXMATRIXA16[g_NumBoneMatricesMax]; //消したぽいんたに新しいMATRIX16化した骨数入れるんすか？
				if (g_pBoneMatrices == NULL) //中身無いですやん
				{
					hr = E_OUTOFMEMORY; //out of memory (メモリ不足?)エラー返すんすか？
					goto e_Exit;
				}
			}
		}
	e_Exit:
		return hr;
	}

	//名前を割り当て(翻訳まんま)
	HRESULT AllocateName(LPCSTR Name, LPSTR* pNewName)
	{
		UINT cdLength; //cdって何の略よ。 何の長さよ。

		if (Name != NULL) //大変！ この子名前が無い！！
		{
			cdLength = (UINT)strlen(Name) + 1; //名前の長さ一文字増やすん？
			*pNewName = new CHAR[cdLength]; //新しいCHAR型のcd長さポインタだァー！
			if (*pNewName == NULL) //アドレス入ってないやん！！
				return E_OUTOFMEMORY; //out of memory (19行ぶり２回目)
			memcpy(*pNewName, Name, cdLength * sizeof(CHAR)); //pNewNameのメモリブロックにNameのCHARサイズ×cd長さをコピったる！
		}
		else
		{
			*pNewName = NULL;
		}

		return S_OK;
	}
	//--------------------------------------------------------------------------------------
	// ポインタは、その変換行列に、指定された骨のためにセットアップするために呼び出されます
	//--------------------------------------------------------------------------------------[♡原文翻訳♡]
	
	//骨行列ポインタでメッシュの設定(翻)
	HRESULT SetupBoneMatrixPointersOnMesh(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME rootFrame)
	{
		UINT iBone, cBones;
		D3DXFRAME_DERIVED* pFrame;

		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		//スキンその後、メッシュ、セットアップ骨行列が存在する場合(原文翻訳)
		if (pMeshContainer->pSkinInfo != NULL)
		{
			cBones = pMeshContainer->pSkinInfo->GetNumBones(); //c骨にメッシュコンテナスキン情報の骨数

			pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones]; //骨行列のダブルポインタにc骨行列のポインタを新しく授けよう
			if (pMeshContainer->ppBoneMatrixPtrs == NULL) //Wぽ骨行列に何もないんやけど
				return E_OUTOFMEMORY;// O	U	T		O	F		M	E	M	O	R	Y

			//i骨がc骨より少ない間はループしますよ
			for (iBone = 0; iBone < cBones; iBone++)
			{
				//D3DXFrameFind.. ルートフレームの子フレームを検索します
				//			子フレームが見つかった場合はそれを返。見つからなかった場合は NULL を返

				pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame,	//[in] ルートフレームへのポインナ
					pMeshContainer->pSkinInfo->GetBoneName(iBone));		//[in] 検索する子フレーナの名前
				if (pFrame == NULL) //pFrameに何もないぞ
					return E_FAIL; //失敗します

				//i骨くんの骨行列にpFrameちゃんの合成済み変換行列をプレゼントしちゃう
				pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->CombinedTransformationMatrix;
			}
		}

		//"_face"…顔? ルートフレームから顔を検索してるんすかこれ？
		pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(rootFrame, "_face");
		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	//ポインタは、その変換行列に、指定された骨のためにセットアップするために呼び出されます
	//--------------------------------------------------------------------------------------[♡原文翻訳♡]
	HRESULT SetupBoneMatrixPointers(LPD3DXFRAME pFrame, LPD3DXFRAME pRootFrame)
	{
		HRESULT hr;

		if (pFrame->pMeshContainer != NULL) //メッシュコンテナに情報ありますめぇ！
		{
			hr = SetupBoneMatrixPointersOnMesh(pFrame->pMeshContainer, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameSibling != NULL) //pFrameに兄弟姉妹ありますめぅ！
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameSibling, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		if (pFrame->pFrameFirstChild != NULL) //pFrameに子供めう！
		{
			hr = SetupBoneMatrixPointers(pFrame->pFrameFirstChild, pRootFrame);
			if (FAILED(hr))
				return hr;
		}

		return S_OK;
	}


/*--------------------------------------------------------------------------------------------------------------------------*/


	//何だこのクラス!?
	//割り当て階層って何だよ何を割り当てる階層だよ
	class AllocateHierarchy : public ID3DXAllocateHierarchy
	{
	public:
		STDMETHOD(CreateFrame)(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame);
		STDMETHOD(CreateMeshContainer)(THIS_
			LPCSTR Name,
			CONST D3DXMESHDATA *pMeshData,
			CONST D3DXMATERIAL *pMaterials,
			CONST D3DXEFFECTINSTANCE *pEffectInstances,
			DWORD NumMaterials,
			CONST DWORD *pAdjacency,
			LPD3DXSKININFO pSkinInfo,
			LPD3DXMESHCONTAINER *ppNewMeshContainer);
		STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME pFrameToFree);
		STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER pMeshContainerBase);

		AllocateHierarchy()
		{
		}
	};

	//--------------------------------------------------------------------------------------
	// Name: AllocateHierarchy::CreateFrame()
	// Desc: メッシュコンテナを作成しまぁーす。
	//--------------------------------------------------------------------------------------
	HRESULT AllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME* ppNewFrame)
	{
		HRESULT hr = S_OK;
		D3DXFRAME_DERIVED* pFrame;

		*ppNewFrame = NULL;

		pFrame = new D3DXFRAME_DERIVED;
		if (pFrame == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		hr = AllocateName(Name, &pFrame->Name);
		if (FAILED(hr))
			goto e_Exit;

		// initialize other data members of the frame
		D3DXMatrixIdentity(&pFrame->TransformationMatrix);
		D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

		pFrame->pMeshContainer = NULL;
		pFrame->pFrameSibling = NULL;
		pFrame->pFrameFirstChild = NULL;

		*ppNewFrame = pFrame;
		pFrame = NULL;

	e_Exit:
		delete pFrame;
		return hr;
	}
	//--------------------------------------------------------------------------------------
	// Name: AllocateHierarchy::CreateMeshContainer()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT AllocateHierarchy::CreateMeshContainer(
		LPCSTR Name,
		CONST D3DXMESHDATA *pMeshData,
		CONST D3DXMATERIAL *pMaterials,
		CONST D3DXEFFECTINSTANCE *pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD *pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER *ppNewMeshContainer)
	{
		HRESULT hr;
		D3DXMESHCONTAINER_DERIVED *pMeshContainer = NULL;
		UINT NumFaces;
		UINT iMaterial;
		UINT iBone, cBones;
		LPDIRECT3DDEVICE9 pd3dDevice = NULL;

		LPD3DXMESH pMesh = NULL;

		*ppNewMeshContainer = NULL;

		// this sample does not handle patch meshes, so fail when one is found
		if (pMeshData->Type != D3DXMESHTYPE_MESH)
		{
			hr = E_FAIL;
			goto e_Exit;
		}

		// get the pMesh interface pointer out of the mesh data structure
		pMesh = pMeshData->pMesh;
		DWORD numVert = pMesh->GetNumVertices();
		// this sample does not FVF compatible meshes, so fail when one is found
		if (pMesh->GetFVF() == 0)
		{
			hr = E_FAIL;
			goto e_Exit;
		}

		// allocate the overloaded structure to return as a D3DXMESHCONTAINER
		pMeshContainer = new D3DXMESHCONTAINER_DERIVED;
		if (pMeshContainer == NULL)
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}
		memset(pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_DERIVED));

		// make sure and copy the name.  All memory as input belongs to caller, interfaces can be addref'd though
		hr = AllocateName(Name, &pMeshContainer->Name);
		if (FAILED(hr))
			goto e_Exit;

		pMesh->GetDevice(&pd3dDevice);
		NumFaces = pMesh->GetNumFaces();

		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef();

		D3DVERTEXELEMENT9 decl[] = {
			{ 0, 0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
			{ 0, 16, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT, 0 },
			{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0 },
			{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
			{ 0, 60, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },
			{ 0, 72, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
			D3DDECL_END()
		};


		// allocate memory to contain the material information.  This sample uses
		//   the D3D9 materials and texture names instead of the EffectInstance style materials
		pMeshContainer->NumMaterials = max(1, NumMaterials);
		pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
		pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
		pMeshContainer->pAdjacency = new DWORD[NumFaces * 3];
		if ((pMeshContainer->pAdjacency == NULL) || (pMeshContainer->pMaterials == NULL))
		{
			hr = E_OUTOFMEMORY;
			goto e_Exit;
		}

		memcpy(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD)* NumFaces * 3);
		memset(pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9)* pMeshContainer->NumMaterials);

		// if materials provided, copy them
		if (NumMaterials > 0)
		{
			memcpy(pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL)* NumMaterials);

			for (iMaterial = 0; iMaterial < NumMaterials; iMaterial++)
			{
				if (pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL)
				{
					char* baseDir = "Assets/model/";
					char filePath[64];
					strcpy(filePath, baseDir);
					strcat(filePath, pMeshContainer->pMaterials[iMaterial].pTextureFilename);
					if (FAILED(D3DXCreateTextureFromFile(
						pd3dDevice,
						filePath,
						&pMeshContainer->ppTextures[iMaterial]))
						) {
						pMeshContainer->ppTextures[iMaterial] = NULL;
					}

					// don't remember a pointer into the dynamic memory, just forget the name after loading
					pMeshContainer->pMaterials[iMaterial].pTextureFilename = NULL;
				}
			}
		}
		else // if no materials provided, use a default one
		{
			pMeshContainer->pMaterials[0].pTextureFilename = NULL;
			memset(&pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}

		// if there is skinning information, save off the required data and then setup for HW skinning
		if (pSkinInfo != NULL)
		{
			// first save off the SkinInfo and original mesh data
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();

			pMeshContainer->pOrigMesh = pMesh;
			pMesh->AddRef();

			// Will need an array of offset matrices to move the vertices from the figure space to the bone's space
			cBones = pSkinInfo->GetNumBones();
			pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
			if (pMeshContainer->pBoneOffsetMatrices == NULL)
			{
				hr = E_OUTOFMEMORY;
				goto e_Exit;
			}

			// get each of the bone offset matrices so that we don't need to get them later
			for (iBone = 0; iBone < cBones; iBone++)
			{
				pMeshContainer->pBoneOffsetMatrices[iBone] = *(pMeshContainer->pSkinInfo->GetBoneOffsetMatrix(iBone));
			}

			// GenerateSkinnedMesh will take the general skinning information and transform it to a HW friendly version
			hr = GenerateSkinnedMesh(pd3dDevice, pMeshContainer);
			if (FAILED(hr))
				goto e_Exit;

			LPD3DXMESH pOutMesh;
			hr = pMeshContainer->MeshData.pMesh->CloneMesh(
				pMeshContainer->MeshData.pMesh->GetOptions(),
				decl,
				pd3dDevice, &pOutMesh);
			if (FAILED(hr))
				goto e_Exit;
			hr = D3DXComputeTangentFrameEx(
				pOutMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				NULL,
				0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
				0.25f,
				0.01f,
				&pOutMesh,
				NULL
				);
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pOutMesh;


			std::vector<DWORD> adjList;
			adjList.resize(3 * pOutMesh->GetNumFaces());
			pOutMesh->GenerateAdjacency(1.0f / 512.0f, &adjList[0]); // EPSIONは適当な値(1.0f/512とか)

			DWORD numVert = pOutMesh->GetNumVertices();  // Optimizeの一種
			pOutMesh->OptimizeInplace(D3DXMESHOPT_COMPACT, &adjList[0], NULL, NULL, NULL);
			numVert = pOutMesh->GetNumVertices();

			if (FAILED(hr))
				goto e_Exit;
		}
		else {
			LPD3DXMESH pOutMesh;
			DWORD numVert = pMeshContainer->MeshData.pMesh->GetNumVertices();
			hr = pMeshContainer->MeshData.pMesh->CloneMesh(
				pMeshContainer->MeshData.pMesh->GetOptions(),
				decl,
				pd3dDevice, &pOutMesh);

			numVert = pMeshContainer->MeshData.pMesh->GetNumVertices();
			hr = D3DXComputeTangentFrameEx(
				pOutMesh,
				D3DDECLUSAGE_TEXCOORD,
				0,
				D3DDECLUSAGE_TANGENT,
				0,
				D3DX_DEFAULT,
				0,
				D3DDECLUSAGE_NORMAL,
				0,
				0,
				NULL,
				0.01f,    //ボケ具合.値をおおきくするとぼけなくなる
				0.25f,
				0.01f,
				&pOutMesh,
				NULL
				);
			numVert = pOutMesh->GetNumVertices();
			pMeshContainer->MeshData.pMesh->Release();
			pMeshContainer->MeshData.pMesh = pOutMesh;
			if (FAILED(hr))
				goto e_Exit;
			LPD3DXMESH optMesh;
			std::vector<DWORD> adjList;
			adjList.resize(3 * pOutMesh->GetNumFaces());
			pOutMesh->GenerateAdjacency(1.0f, &adjList[0]); // EPSIONは適当な値(1.0f/512とか)
			numVert = pOutMesh->GetNumVertices();  // Optimizeの一種
			pOutMesh->Optimize(D3DXMESHOPT_COMPACT, &adjList[0], NULL, NULL, NULL, &optMesh);
			numVert = optMesh->GetNumVertices();

		}

		*ppNewMeshContainer = pMeshContainer;
		pMeshContainer = NULL;

	e_Exit:
		pd3dDevice->Release();

		// call Destroy function to properly clean up the memory allocated 
		if (pMeshContainer != NULL)
		{
			DestroyMeshContainer(pMeshContainer);
		}

		return hr;
	}
	//--------------------------------------------------------------------------------------
	// Name: AllocateHierarchy::DestroyFrame()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT AllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
	{
		SAFE_DELETE_ARRAY(pFrameToFree->Name);
		SAFE_DELETE(pFrameToFree);
		return S_OK;
	}




	//--------------------------------------------------------------------------------------
	// Name: AllocateHierarchy::DestroyMeshContainer()
	// Desc: 
	//--------------------------------------------------------------------------------------
	HRESULT AllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase)
	{
		UINT iMaterial;
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;

		SAFE_DELETE_ARRAY(pMeshContainer->Name);
		SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
		SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);
		SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrices);

		// release all the allocated textures
		if (pMeshContainer->ppTextures != NULL)
		{
			for (iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++)
			{
				SAFE_RELEASE(pMeshContainer->ppTextures[iMaterial]);
			}
		}

		SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);
		SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrixPtrs);
		SAFE_RELEASE(pMeshContainer->pBoneCombinationBuf);
		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		SAFE_RELEASE(pMeshContainer->pSkinInfo);
		SAFE_RELEASE(pMeshContainer->pOrigMesh);
		SAFE_DELETE(pMeshContainer);
		return S_OK;
	}


}
ModelData::ModelData() :
frameRoot(nullptr),
lpAnimController(nullptr)
{
}
ModelData::~ModelData()
{
}
void ModelData::Release()
{
	if (lpAnimController) {
		lpAnimController->Release();
	}
}

void ModelData::LoadModelData(const char* filePath, Animation* anim)
{
	AllocateHierarchy alloc;
	HRESULT hr = D3DXLoadMeshHierarchyFromX(
		filePath,
		D3DXMESH_VB_MANAGED,
		g_pd3dDevice,
		&alloc,
		nullptr,
		&frameRoot,
		&lpAnimController
		);
	//m_pAnimController->(0);
	SetupBoneMatrixPointers(frameRoot, frameRoot);
	if (anim && lpAnimController) {
		anim->Init(lpAnimController);
	}
}

void ModelData::UpdateBoneMatrix(const D3DXMATRIX& matWorld)
{
	UpdateFrameMatrices(frameRoot, &matWorld);
}