float4x4 matWorld:WORLD<string UIWidget="None";>;
float4x4 matView:VIEW<string UIWidget="None";>;
float4x4 matProjection:PROJECTION<string UIWidget="None";>;
float4 ambientMaterial
<
	string UIName = "Ambient Material";
	string UIWidget = "Color";
> = {0.5, 0.5, 0.5, 0.5};
float4 ambientLightColour
<
	string UIName = "Ambient Light Colour";
	string UIWidget = "Color";
> = {1.0, 1.0, 1.0, 1.0};
/*float4 objectColour
<
	string UIName = "ObjectColour";
	string UIWidget = "Color";
> = {1,1,1,1};
*/
float4 diffuseMaterial 
<
	string UIName = "Diffuse Material";
	string UIWidget = "Color";
> = {0.8, 0.8, 0.8, 1.0};
float3 lightDirection:DIRECTION<
	string Object = "DirectionalLight3";
	string UIWidget = "None";
>;
float4 diffuseLightColour:COLOR
<
	string UIName = "Diffuse Light Colour";
	string UIWidget = "None";
	string Object = "DirectionalLight3";
> = {1.0, 1.0, 1.0, 1.0};

struct VS_INPUT					// input structure for vertex shader
{
	float4 pos:POSITION; 
	float3 normal:NORMAL;
};

struct PS_INPUT					// input structure for pixel shader
{
	float4 pos:SV_POSITION;
	float3 normal:NORMAL;
};

// Adding vertex and pixel shader
PS_INPUT VS(VS_INPUT input)
{
	PS_INPUT output = (PS_INPUT)0;
	
	float4x4 matViewProjection = mul(matView, matProjection);
	float4x4 matWorldViewProjection = mul(matWorld, matViewProjection);
	
	output.pos = mul(input.pos, matWorldViewProjection);
	output.normal = mul(input.normal, matWorld);
	
	return output;
	
}


float4 PS(PS_INPUT input):SV_TARGET
{
	float3 normal = normalize(input.normal);
	float3 lightDir = normalize(lightDirection);
	float diffuseHighlight = saturate(dot(normal, lightDir));
	
	//return objectColour;		// return color of the pixel fragment
	return (ambientMaterial*ambientLightColour) + 
			(diffuseMaterial * diffuseLightColour * diffuseHighlight);
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

