/*!
*@brief	簡単なディフューズライティング+スペキュラのシェーダー。
*/


float4x4 g_worldMatrix;			//ワールド行列。
float4x4 g_viewMatrix;			//ビュー行列。
float4x4 g_projectionMatrix;	//プロジェクション行列。
float4x4 g_rotationMatrix;		//回転行列。法線を回転させるために必要になる。ライティングするなら必須。

#define DIFFUSE_LIGHT_NUM	4		//ディフューズライトの数。
float4	g_diffuseLightDirection[DIFFUSE_LIGHT_NUM];	//ディフューズライトの方向。
float4	g_diffuseLightColor[DIFFUSE_LIGHT_NUM];		//ディフューズライトのカラー。
float4	g_ambientLight;								//環境光。

float3 vEyePt;		//自分で追加視点

texture g_diffuseTexture;		//ディフューズテクスチャ。
sampler g_diffuseTextureSampler =
sampler_state
{
	Texture = <g_diffuseTexture>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

/*ShadowMap*/
bool	g_isShadowReceiver;		//シャドマップレシーバー
float4x4 	g_lightViewMatrix;		//ライトびゅぅぅぅうううぅぅぅぅううん
float4x4 	g_lightProjectionMatrix;//ライトプルジェンクジェン
texture g_shadowMapTexture;	//テクスチャ
sampler g_shadowMapTextureSampler =
sampler_state
{
	Texture = <g_shadowMapTexture>;
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

/*ToonSheder*/
texture tex0 ;  //オブジェクトのテクスチャ
sampler tex0sampler =
sampler_state
{
	Texture = <tex0>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};
texture tex1 ;  //トゥーンマップテクスチャ
sampler tex1sampler =
sampler_state
{
	Texture = <tex1>;
	MipFilter = NONE;
	MinFilter = NONE;
	MagFilter = NONE;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VS_INPUT{
	float4	pos			: POSITION	;
	float4	color		: COLOR0	;
	float3	normal		: NORMAL0	;
	float2	uv			: TEXCOORD0	;
};

struct VS_OUTPUT{
	float4	pos			 : POSITION	;
	float4	color		 : COLOR0	;
	float2	uv			 : TEXCOORD0;
	float3	normal		 : TEXCOORD1;
	float4	worldPos 	 : TEXCOORD2;	//ワールド空間での頂点座標。
	float4	lightViewPos : TEXCOORD3;	//ワールド空間->ライトビュー空間->ライト射影空間に変換された座標。
};

/*!
*@brief	頂点シェーダー。
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 pos;
	pos          = mul(In.pos, g_worldMatrix)      ;	//モデルのローカル空間からワールド空間に変換。
	Out.worldPos = pos                             ;
	pos          = mul(pos, g_viewMatrix)          ;	//ワールド空間からビュー空間に変換。
	pos          = mul(pos, g_projectionMatrix)    ;	//ビュー空間から射影空間に変換。
	Out.pos      = pos                             ;
	Out.color    = In.color                        ;
	Out.uv       = In.uv                           ;
	Out.normal   = mul(In.normal, g_rotationMatrix);	//法線を回す。

	if (g_isShadowReceiver){
		//ワールド座標をライトカメラから見た射影空間に変換する,
		Out.lightViewPos = mul(Out.worldPos        , g_lightViewMatrix		);
		Out.lightViewPos = mul(Out.lightViewPos, g_lightProjectionMatrix);
	}

	return Out;
}

// * @brief	輪郭(反転したモデル)用
VS_OUTPUT VSEdgeMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos = In.pos;		//要調整
		pos.xyz += In.normal * 0.01f;
	pos          = mul(pos, g_worldMatrix)         ;	//モデルのローカル空間からワールド空間に変換。
	Out.worldPos = pos                             ;
	pos          = mul(pos, g_viewMatrix)          ;	//ワールド空間からビュー空間に変換。
	pos          = mul(pos, g_projectionMatrix)    ;	//ビュー空間から射影空間に変換。
	Out.pos      = pos                             ;
	Out.color    = In.color                        ;
	Out.uv       = In.uv                           ;
	Out.normal   = mul(In.normal, g_rotationMatrix);	//法線を回す。

	return Out;
}

// * @brief	トゥーンシェーダ
VS_OUTPUT VSToon(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos = In.pos;
	pos = mul(pos, g_worldMatrix);
	pos = mul(pos, g_viewMatrix);
	pos = mul(pos, g_projectionMatrix);
	Out.pos = pos;
	Out.uv = In.uv;
	Out.normal = normalize(In.normal.xyz);

	return Out;
	
}

/*!
*@brief	ディフューズライトを計算。
*@param[in]	normal	法線。
*/
float3 CalcDiffuse(float3 normal)
{
	float3 diff = 0.0f;
	for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
		diff += max(0.0f, dot(normal.xyz, -g_diffuseLightDirection[i].xyz))
			* g_diffuseLightColor[i].xyz;
	}
	return diff;
}

/*!
*@brief	ピクセルシェーダー。
*/
float4 PSMain(VS_OUTPUT In) : COLOR
{

	//ここにスペキュラライトの実装を記述しなさい。
	//視点のシェーダー定数は存在しないので、自分で追加を行うように。
	//モデルのワールド頂点座標はIn.worldPos、ワールド法線はIn.normal。
	//ライトの方向はg_diffuseLightDirection[0]とする。

	//①
	float3 eye = normalize(vEyePt - In.worldPos);

	//②
	float3 R = -eye + 2.0f * dot(In.normal, eye) * In.normal;

	//③
	//float3 spec = max(0.0f, dot(R, -g_diffuseLightDirection[0]));

	//④
	//spec = pow(spec, 2.0f);

	//アンビエントライトを加算。
	float4 lig = 0.0f;
	float3 spec;//③
	{
		for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
			lig.xyz += max(0.0f, dot(In.normal.xyz, -g_diffuseLightDirection[i].xyz))
				* g_diffuseLightColor[i].xyz;
			spec = max(0.0f, dot(R, - g_diffuseLightDirection[i]));//③
		}
		lig += g_ambientLight;

		spec = pow(spec, 2.0f);//④
		lig.xyz += spec;
	}

	float4 color = tex2D(g_diffuseTextureSampler, In.uv);
		color.xyz *= lig;

	//ShadowMap
	if (g_isShadowReceiver)
	{
		/* UV座標系に変換す */

		//(-1.0~1.0)に変換す,
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;

		//(-0.5~0.5)の範囲にす,
		shadowMapUV *= float2(0.5f, -0.5f);
		
		//(0.0~1.0)のUV座標にす,
		shadowMapUV += float2(0.5f, 0.5f);

		//シャドウマ゜ッ
		float4 shadowVal = tex2D(g_shadowMapTextureSampler, shadowMapUV);

		color *= shadowVal;
	}

	return color;
}

// *	@brief	ピクセルシェーダ,
float4 PSShadowMap(VS_OUTPUT In) : COLOR
{
	//灰色のカラーを出力している,
	return float4 (0.5f, 0.5f, 0.5f, 1.0f);
}

// *	@brief	輪郭(反転したモデル)用
float4 PSEdgeMain(VS_OUTPUT In) : COLOR
{
	return float4 (0.0f, 0.0f, 0.0f, 1.0f);
}

// *	@brief	トゥーンシェーダ
float4 PSToon(VS_OUTPUT In)	: COLOR
{
	float4 Out;
	float p = 0.0f;
	//ハーフランバート拡散照明によるライティング計算
	
	p = dot(In.normal, -g_diffuseLightDirection[0].xyz);
	 
	p = p * 0.5f + 0.5f;
	p = p * p;
	/* トゥーンシェーダ処理 */
	//色情報をテクセルのＵ成分とし、トゥーンマップテクスチャーから光の反射率を取得する
	float4 Col0 = tex2D(tex1sampler, In.uv);

	//色情報を格納する
	float4 Col1 = tex2D(tex0sampler, In.uv);
	if (p < 0.2f){
		return Col0;
	}
	else if (p < 0.3f)
	{
		return Col0 / 2 + Col1 / 2;
	}

	return Col1;
}

technique SkinModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSMain();
	}
}

technique ShadowMap
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSMain();
		PixelShader = compile ps_3_0 PSShadowMap();
	}
}

technique ReversalModel
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSEdgeMain();
		PixelShader = compile ps_3_0 PSEdgeMain();
	}
}

technique ToonShader
{
	pass p0
	{
		VertexShader = compile vs_3_0 VSToon();
		PixelShader = compile ps_3_0 PSToon();
	}
}