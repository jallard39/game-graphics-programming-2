
// Basically just returns a given --> used for multiple render targets

// Defines the input to this pixel shader
struct VertexToPixel
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
};

// Textures
Texture2D PixelColors : register(t0);

// Entry point for this pixel shader
float4 main(VertexToPixel input) : SV_TARGET
{
    return PixelColors.Load(float3(input.position.xy, 0));
}