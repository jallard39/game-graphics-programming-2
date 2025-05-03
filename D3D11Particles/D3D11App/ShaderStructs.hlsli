#ifndef __GGP_SHADER_STRUCTS__
#define __GPP_SHADER_STRUCTS__

// Structs for various shaders

// Basic VS input for a standard Pos/UV/Normal vertex
struct VertexShaderInput
{
	float3 localPosition	: POSITION;
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
};



// VS Output / PS Input struct for basic lighting
struct VertexToPixel
{
	float4 screenPosition	: SV_POSITION;
	float2 uv				: TEXCOORD;
	float3 normal			: NORMAL;
	float3 tangent			: TANGENT;
	float3 worldPos			: POSITION;
};


// VStoPS struct for sky box
struct VertexToPixel_Sky
{
	float4 screenPosition	: SV_POSITION;
	float3 sampleDir		: DIRECTION;
};

// VStoPS struct for particle emitters
struct VertexToPixel_Particle
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

#endif