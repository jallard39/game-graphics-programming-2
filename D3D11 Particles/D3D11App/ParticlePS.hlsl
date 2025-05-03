
#include "ShaderStructs.hlsli"

// Texture-related resources
Texture2D ParticleTexture : register(t0);
SamplerState BasicSampler : register(s0);

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// --------------------------------------------------------
float4 main(VertexToPixel_Particle input) : SV_TARGET
{
    return ParticleTexture.Sample(BasicSampler, input.uv);
}