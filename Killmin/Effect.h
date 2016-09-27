/*!
* @brief	エフェクトマネージャー
*/

#pragma once
#include "System.h"
#include <map>
class Effect;
/*!
* @brief	エフェクトマネージャ。
*/
class Effect{
public:
	/*!
	* @brief	コンストラクタ。
	*/
	Effect();
	/*!
	* @brief	デストラクタ。
	*/
	~Effect();
	/*!
	*@brief	エフェクトのロード。
	*@details
	* 読み込み済みのエフェクトの場合はロードは行われず、</br>
	* 既存のエフェクトが返ってきます。
	*@param[in]	filePath	ロードするエフェクトのファイルパス。
	*@return	ロードしたエフェクトファイル。
	*/
	ID3DXEffect* LoadEffect(const char* filePath);
	/*!
	* @brief	リリース。
	*/
	void Release();
private:
	std::map<int, ID3DXEffect*>		effectDictinary;	//!<CEffectのインスタンスのディクショナリ。ファイルパスのハッシュ値をキーに持ち、CEffectのインスタンスを値に持つ。
};
