/*!
*@brief	�ȒP�ȃf�B�t���[�Y���C�e�B���O+�X�y�L�����̃V�F�[�_�[�B
*/


float4x4 g_worldMatrix;			//���[���h�s��B
float4x4 g_viewMatrix;			//�r���[�s��B
float4x4 g_projectionMatrix;	//�v���W�F�N�V�����s��B
float4x4 g_rotationMatrix;		//��]�s��B�@������]�����邽�߂ɕK�v�ɂȂ�B���C�e�B���O����Ȃ�K�{�B

#define DIFFUSE_LIGHT_NUM	4		//�f�B�t���[�Y���C�g�̐��B
float4	g_diffuseLightDirection[DIFFUSE_LIGHT_NUM];	//�f�B�t���[�Y���C�g�̕����B
float4	g_diffuseLightColor[DIFFUSE_LIGHT_NUM];		//�f�B�t���[�Y���C�g�̃J���[�B
float4	g_ambientLight;								//�����B

float3 vEyePt;		//�����Œǉ����_

texture g_diffuseTexture;		//�f�B�t���[�Y�e�N�X�`���B
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
bool	g_isShadowReceiver;		//�V���h�}�b�v���V�[�o�[
float4x4 	g_lightViewMatrix;		//���C�g�тィ������������������������
float4x4 	g_lightProjectionMatrix;//���C�g�v���W�F���N�W�F��
texture g_shadowMapTexture;	//�e�N�X�`��
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
texture tex0 ;  //�I�u�W�F�N�g�̃e�N�X�`��
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
texture tex1 ;  //�g�D�[���}�b�v�e�N�X�`��
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
	float4	worldPos 	 : TEXCOORD2;	//���[���h��Ԃł̒��_���W�B
	float4	lightViewPos : TEXCOORD3;	//���[���h���->���C�g�r���[���->���C�g�ˉe��Ԃɕϊ����ꂽ���W�B
};

/*!
*@brief	���_�V�F�[�_�[�B
*/
VS_OUTPUT VSMain(VS_INPUT In)
{
	VS_OUTPUT Out;
	float4 pos;
	pos          = mul(In.pos, g_worldMatrix)      ;	//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	Out.worldPos = pos                             ;
	pos          = mul(pos, g_viewMatrix)          ;	//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos          = mul(pos, g_projectionMatrix)    ;	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
	Out.pos      = pos                             ;
	Out.color    = In.color                        ;
	Out.uv       = In.uv                           ;
	Out.normal   = mul(In.normal, g_rotationMatrix);	//�@�����񂷁B

	if (g_isShadowReceiver){
		//���[���h���W�����C�g�J�������猩���ˉe��Ԃɕϊ�����,
		Out.lightViewPos = mul(Out.worldPos        , g_lightViewMatrix		);
		Out.lightViewPos = mul(Out.lightViewPos, g_lightProjectionMatrix);
	}

	return Out;
}

// * @brief	�֊s(���]�������f��)�p
VS_OUTPUT VSEdgeMain(VS_INPUT In)
{
	VS_OUTPUT Out = (VS_OUTPUT)0;
	float4 pos = In.pos;		//�v����
		pos.xyz += In.normal * 0.01f;
	pos          = mul(pos, g_worldMatrix)         ;	//���f���̃��[�J����Ԃ��烏�[���h��Ԃɕϊ��B
	Out.worldPos = pos                             ;
	pos          = mul(pos, g_viewMatrix)          ;	//���[���h��Ԃ���r���[��Ԃɕϊ��B
	pos          = mul(pos, g_projectionMatrix)    ;	//�r���[��Ԃ���ˉe��Ԃɕϊ��B
	Out.pos      = pos                             ;
	Out.color    = In.color                        ;
	Out.uv       = In.uv                           ;
	Out.normal   = mul(In.normal, g_rotationMatrix);	//�@�����񂷁B

	return Out;
}

// * @brief	�g�D�[���V�F�[�_
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
*@brief	�f�B�t���[�Y���C�g���v�Z�B
*@param[in]	normal	�@���B
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
*@brief	�s�N�Z���V�F�[�_�[�B
*/
float4 PSMain(VS_OUTPUT In) : COLOR
{

	//�����ɃX�y�L�������C�g�̎������L�q���Ȃ����B
	//���_�̃V�F�[�_�[�萔�͑��݂��Ȃ��̂ŁA�����Œǉ����s���悤�ɁB
	//���f���̃��[���h���_���W��In.worldPos�A���[���h�@����In.normal�B
	//���C�g�̕�����g_diffuseLightDirection[0]�Ƃ���B

	//�@
	float3 eye = normalize(vEyePt - In.worldPos);

	//�A
	float3 R = -eye + 2.0f * dot(In.normal, eye) * In.normal;

	//�B
	//float3 spec = max(0.0f, dot(R, -g_diffuseLightDirection[0]));

	//�C
	//spec = pow(spec, 2.0f);

	//�A���r�G���g���C�g�����Z�B
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

	//ShadowMap
	if (g_isShadowReceiver)
	{
		/* UV���W�n�ɕϊ��� */

		//(-1.0~1.0)�ɕϊ���,
		float2 shadowMapUV = In.lightViewPos.xy / In.lightViewPos.w;

		//(-0.5~0.5)�͈̔͂ɂ�,
		shadowMapUV *= float2(0.5f, -0.5f);
		
		//(0.0~1.0)��UV���W�ɂ�,
		shadowMapUV += float2(0.5f, 0.5f);

		//�V���h�E�}�K�b
		float4 shadowVal = tex2D(g_shadowMapTextureSampler, shadowMapUV);

		color *= shadowVal;
	}

	return color;
}

// *	@brief	�s�N�Z���V�F�[�_,
float4 PSShadowMap(VS_OUTPUT In) : COLOR
{
	//�D�F�̃J���[���o�͂��Ă���,
	return float4 (0.5f, 0.5f, 0.5f, 1.0f);
}

// *	@brief	�֊s(���]�������f��)�p
float4 PSEdgeMain(VS_OUTPUT In) : COLOR
{
	return float4 (0.0f, 0.0f, 0.0f, 1.0f);
}

// *	@brief	�g�D�[���V�F�[�_
float4 PSToon(VS_OUTPUT In)	: COLOR
{
	float4 Out;
	float p = 0.0f;
	//�n�[�t�����o�[�g�g�U�Ɩ��ɂ�郉�C�e�B���O�v�Z
	
	p = dot(In.normal, -g_diffuseLightDirection[0].xyz);
	 
	p = p * 0.5f + 0.5f;
	p = p * p;
	/* �g�D�[���V�F�[�_���� */
	//�F�����e�N�Z���̂t�����Ƃ��A�g�D�[���}�b�v�e�N�X�`���[������̔��˗����擾����
	float4 Col0 = tex2D(tex1sampler, In.uv);

	//�F�����i�[����
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