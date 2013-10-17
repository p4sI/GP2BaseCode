float4x4 matWorld:WORLD;
float4x4 matView:VIEW;
float4x4 matProjection:PROJECTION;
float4 objectColour
<
	string UIName = "ObjectColour";
	string UIWidget = "Color";
> = {1,1,1,1};



struct VS_INPUT					// input structure for vertex shader
{
	float4 pos:POSITION;
 	float2 texCoord:TEXCOORD0;
};

struct PS_INPUT					// input structure for pixel shader
{
	float4 pos:SV_POSITION;
	float2 texCoord:TEXCOORD0;
};

// Adding vertex and pixel shader
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	float4x4 matViewProjection = mul(matView, matProjection);
	float4x4 matWorldViewProjection = mul(matWorld, matViewProjection);
	
	output.pos = mul(input.pos, matWorldViewProjection);
	output.texCoord = input.texCoord;
	
	return output;
	
}

Texture2D diffuseMap;
SamplerState WrapPointSampler
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

float4 PS(PS_INPUT input):SV_TARGET
{
	return diffuseMap.Sample(WrapPointSampler, input.texCoord);
}


RasterizerState DisableCulling
{
	CullMode = NONE;	
};

technique10 Render
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_4_0, VS() ) );
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS() ) );
		SetRasterizerState(DisableCulling);
	}
}

