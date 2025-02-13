#include "ShaderIncludes.hlsli"

// Struct representing data from a constant buffer
cbuffer ExternalData : register(b0)
{
    matrix world;
    matrix view;
    matrix projection;
    matrix worldInvTranspose;
    matrix lightView;
    matrix lightProjection;
}

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	VertexToPixel output;
	
    matrix wvp = mul(projection, mul(view, world));
    output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
    output.uv = input.uv;
    output.normal = mul((float3x3) worldInvTranspose, input.normal);
    output.tangent = mul((float3x3) world, input.tangent);
    output.worldPosition = mul(world, float4(input.localPosition, 1.0f)).xyz;

	return output;
}