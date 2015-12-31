cbuffer ConstantBuffer : register(b0)
{
	float4x4 matFinal;
}

cbuffer CenterXConstantBuffer : register(b1)
{
	float centerX;
}

cbuffer CenterYConstantBuffer : register(b2)
{
	float centerY;
}

struct VOut
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoordIn : TEXCOORD0;
	float2 padding : TEXCOORD1;
	float2 centerIn : TEXCOORD2;
};

// Vertex Shader
VOut main(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD)
{
	// create a VOut struct
	VOut output;

	// set the output values
	output.position = mul(matFinal, position);    // transform the vertex from 3D to 2D
	output.color = color;
	output.texcoordIn = texcoord;    // set the texture coordinates, unmodified
	output.texcoordIn.x = (output.position.x + 1.0) / 2.0;
	output.texcoordIn.y = (output.position.y + 1.0) / 2.0;
	output.texcoordIn.y = 1 - output.texcoordIn.y;

	output.padding = float2(0, 0);

	float4 center = float4(centerX, centerY, 0, 1);
	float4 screenCenter = mul(matFinal, center);

	output.centerIn = float2(screenCenter.x, screenCenter.y);
	output.centerIn.x = (screenCenter.x + 1.0) / 2.0;
	output.centerIn.y = (screenCenter.y + 1.0) / 2.0;
	output.centerIn.y = 1 - output.centerIn.y;

	// return the output values
	return output;
}