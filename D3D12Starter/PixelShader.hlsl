#include "ShaderIncludes.hlsli"

Texture2D Albedo : register(t0);
Texture2D MetalnessMap : register(t1);
Texture2D NormalMap : register(t2);
Texture2D RoughnessMap : register(t3);

SamplerState BasicSampler : register(s0);
//SamplerComparisonState ShadowSampler : register(s1);

// Struct representing data from a constant buffer
cbuffer ExternalData : register(b0)
{
    float2 uvScale;
    float2 uvOffset;
    float3 cameraPosition;
    int lightCount;
    Light lights[10];
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
    // ====== Sampling ======
    
    // Scale UVs
    float2 uv = input.uv + uvOffset;
    uv = float2(uv.x * uvScale.x, uv.y * uvScale.y);
    
    // Sample textures
    float4 surfaceColor = Albedo.Sample(BasicSampler, uv); // * colorTint;
    surfaceColor = float4(pow(surfaceColor.rgb, 2.2f), 1.0f);
    
    float roughness = RoughnessMap.Sample(BasicSampler, uv).r;
    float metalness = MetalnessMap.Sample(BasicSampler, uv).r;
    
    float3 specularColor = lerp(F0_NON_METAL, surfaceColor.rgb, metalness);
    
    
    // ====== Normals ======
    
    // Unpack normals
    float3 unpackedNormal = NormalMap.Sample(BasicSampler, uv).rgb * 2 - 1;
    unpackedNormal = normalize(unpackedNormal);
    
    // Calculate TBN rotation matrix
    float3 tangent = normalize(input.tangent - input.normal * dot(input.tangent, input.normal));
    float3 bitangent = cross(tangent, input.normal);
    float3x3 TBN = float3x3(input.tangent, bitangent, input.normal);
    input.normal = mul(unpackedNormal, TBN);
    

    // ====== Lighting ======

    // Ambient, specular, direction -  same for all
    float3 v = normalize(cameraPosition - input.worldPosition);
    
    // Calculate total lighting
    float3 totalLight = float3(0.0f, 0.0f, 0.0f);
    for (int i = 0; i < lightCount; i++)
    {
        if (lights[i].Type == 0)
        {
            float3 lightResult = DirectionalLight(input.normal, lights[i], v, roughness, surfaceColor.rgb, specularColor, metalness);
            //if (i == 0)
            //    lightResult *= shadowAmount;
            totalLight += lightResult;
        }
        else if (lights[i].Type == 1)
        {
            totalLight += PointLight(input.normal, lights[i], v, roughness, surfaceColor.rgb, specularColor, input.worldPosition, metalness);
        }
    }
    
    float4 pixelColor = float4(pow(totalLight, 1.0f / 2.2f), 1.0f);
    
    return pixelColor;
    
}