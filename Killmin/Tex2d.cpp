#include "Tex2d.h"

Tex2d::Tex2d(){}

Tex2d::~Tex2d(){}

void Tex2d::Initialize()
{
	/*D3DXIMAGE_INFO*/ //imgInfo;	//画像情報格納用構造体

	D3DXCreateTextureFromFileEx(
		g_pd3dDevice     ,				 //  0Direct3Dデバイス
		this->texFileName,				//  画像ファイル名
		0                ,			   //  幅（画像サイズでそのまま使用する場合は０で良い。
		0                ,			  //  高さ（画像サイズそのまま使用する場合は０で良い。
		0                ,			 //  要求Mipmap数（０指定で完全なミップマップチェーン"全ての解像度を"作成
		0                ,			//  ０で良い
		D3DFMT_UNKNOWN   ,		   //  この記述でファイルからフォーマット取得
		D3DPOOL_DEFAULT  ,		  //  テクスチャの配置先メモリクラス。この記述で良い
		D3DX_FILTER_NONE ,	     //  イメージフィルタリング方法。この記述で良い
		D3DX_DEFAULT     ,		//  これでよい
		0                ,	   //  透明となるD3DCOLORの値。　無効にする場合は０指定
		&imgInfo         ,	  //  画像ファイル情報格納先
		NULL             ,	 //  NULLで良い
		&this->pTexture);	//  テクスチャ出力先

	//テクスチャの中点セット
	this->texCenter = D3DXVECTOR2(//2D空間のベクトルを記述する
		(float)imgInfo.Width / 2,	//
		(float)imgInfo.Height / 2);	//

	RECT rec = { 0, 0, imgInfo.Width, imgInfo.Height };//描画領域
	memcpy(&this->rect, &rec, sizeof(RECT));//描画領域セット
}

void Tex2d::Draw(LPD3DXSPRITE pSprite)
{
	pSprite->Begin(D3DXSPRITE_ALPHABLEND);//スプライト描画開始

	pSprite->SetTransform(&this->transformMatrix);//変換行列セット

	pSprite->Draw(		//スプライトにテクスチャ貼付け
		this->pTexture,	//スプライトに使うソース イメージを表す IDirect3DTexture9 インターフェイスへのポインタ
		&this->rect,		//ソース テクスチャのどの部分をスプライトに使うかを示す RECT 構造体へのポインタ
		&D3DXVECTOR3(this->texCenter.x, this->texCenter.y, 0.0f),	//スケーリング ベクトルを格納する D3DXVECTOR2 構造体へのポインタ
		NULL,	//スクリーン ピクセルでの平行移動値を格納する D3DXVECTOR2 構造体へのポインタ(なのか...？) この引数が NULL の場合は、ポイント(0, 0) を使う
		this->backColor);/*D3DCOLOR 型。この値によりカラー チャンネルとアルファ チャンネルが乗算される。
						   0xFFFFFFFF という値を使うと、元のソース カラーおよびアルファ データが維持される。*/

	pSprite->End();//スプライト描画終了
}

//アニメーションDraw
void Tex2d::Draw(LPD3DXSPRITE pSprite, D3DXVECTOR3& position, RECT& rect, D3DXVECTOR2& center)
{
	D3DXMatrixIdentity(&this->transformMatrix);
	D3DXMatrixTransformation2D(&this->transformMatrix, NULL, 0.0f, &D3DXVECTOR2(this->scale.x, this->scale.y),
		NULL, D3DXToRadian(this->angle), &D3DXVECTOR2(position.x, position.y));

	pSprite->Begin(D3DXSPRITE_ALPHABLEND);//スプライト描画開始
	pSprite->SetTransform(&this->transformMatrix);//変換行列セット

	pSprite->Draw(
		this->pTexture,
		&rect,
		&D3DXVECTOR3(center.x, center.y, 0.0f),
		NULL,
		this->backColor);

	pSprite->End();
}


void Tex2d::SetupMatrices()
{
	D3DXMatrixIdentity(&this->transformMatrix);//変換行列初期化

	D3DXMatrixTransformation2D(//変換行列の算出
		&this->transformMatrix,	//出力先アドレス
		NULL,						//NULLでいい
		0.0f,						//０でいい
		&this->scale,				//スケーリング値アドレス
		NULL,						//NULLでいい
		D3DXToRadian(this->angle),//回転アングル（ラジアン）	DirectXのマクロ関数であるD3DXToRadianを呼び出し、角度をラジアンに変換する。
		&this->position);			//位置アドレス
}