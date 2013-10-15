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
};

struct PS_INPUT					// input structure for pixel shader
{
	float4 pos:SV_POSITION;
};

// Adding vertex and pixel shader
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	float4x4 matViewProjection = mul(matView, matProjection);
	float4x4 matWorldViewProjection = mul(matWorld, matViewProjection);
	
	output.pos = mul(input.pos, matWorldViewProjection);
	
	return output;
	
}


float4 PS(PS_INPUT input):SV_TARGET
{
	return objectColour;		// return color of the pixel fragment
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

