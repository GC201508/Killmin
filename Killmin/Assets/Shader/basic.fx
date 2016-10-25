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

//自分で追加視点
float3 vEyePt;

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

struct VS_INPUT{
	float4	pos			: POSITION;
	float4	color		: COLOR0;
	float3	normal		: NORMAL0;
	float2	uv			: TEXCOORD0;
};

struct VS_OUTPUT{
	float4	pos			: POSITION;
	float4	color		: COLOR0;
	float2	uv			: TEXCOORD0;
	float3	normal		: TEXCOORD1;
	float4	worldPos 	: TEXCOORD2;		//ワールド空間での頂点座標。
};

/*!
*@brief	頂点シェーダー。
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 pos;
	pos = mul(In.pos, g_worldMatrix);		//モデルのローカル空間からワールド空間に変換。
	Out.worldPos = pos;
	pos = mul(pos, g_viewMatrix);			//ワールド空間からビュー空間に変換。
	pos = mul(pos, g_projectionMatrix);	//ビュー空間から射影空間に変換。
	Out.pos = pos;
	Out.color = In.color;
	Out.uv = In.uv;
	Out.normal = mul(In.normal, g_rotationMatrix);	//法線を回す。
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

	//�@
	float3 eye = normalize(vEyePt - In.worldPos);

	//�A
	float3 R = -eye + 2.0f * dot(In.normal, eye) * In.normal;

	//�B
	//float3 spec = max(0.0f, dot(R, -g_diffuseLightDirection[0]));

	//�C
	//spec = pow(spec, 2.0f);

	//アンビエントライトを加算。
	float4 lig = 0.0f;
	float3 spec;//�B
	{
		for (int i = 0; i < DIFFUSE_LIGHT_NUM; i++){
			lig.xyz += max(0.0f, dot(In.normal.xyz, -g_diffuseLightDirection[i].xyz))
				* g_diffuseLightColor[i].xyz;
			spec = max(0.0f, dot(R, - g_diffuseLightDirection[i]));//�B
		}
		lig += g_ambientLight;

		spec = pow(spec, 2.0f);//�C
		lig.xyz += spec;
	}

	float4 color = tex2D(g_diffuseTextureSampler, In.uv);
		color.xyz *= lig;
	return color;
}

// *	@brief	ピクセルシェーダ,
float4 PSShadowMap(VS_OUTPUT In) : COLOR
{
	//灰色のカラーを出力している,
	return float4 (0.5f, 0.5f, 0.5f, 1.0f);
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
};