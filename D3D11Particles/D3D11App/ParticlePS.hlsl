
#include "ShaderStructs.hlsli"

// Texture-related resources
Texture2D ParticleTexture : register(t0);
SamplerState BasicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// --------------------------------------------------------
float4 main(VertexToPixel_Particle input) : SV_TARGET
{
    // Black background pngs
    //float4 textureSample = ParticleTexture.Sample(BasicSampler, input.uv);
    //return float4(textureSample.r, textureSample.g, textureSample.b, textureSample.r) * input.colorTint;
    
    // Transparent pngs
    return ParticleTexture.Sample(BasicSampler, input.uv) * input.colorTint;

}