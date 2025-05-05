
#include "ShaderStructs.hlsli"
#include "Lighting.hlsli"

cbuffer ExternalData : register(b0)
{
	// Camera data
    float3 cameraPosition;
    
    // Material data
    float2 uvScale;
    float2 uvOffset;
    float3 colorTint;
    
    // Refraction data
    float screenWidth;
    float screenHeight;
    float refractionScale;
}

// Texture related resources
Texture2D NormalMap : register(t0);

// Environment map for reflections
TextureCube EnvironmentMap : register(t1);

// Refraction resources
Texture2D ScreenPixels : register(t2);
Texture2D RefractionSilhouette : register(t3);

// Samplers
SamplerState BasicSampler : register(s0);
SamplerState ClampSampler : register(s1);  

// Fresnel term - Schlick approximation
float SimpleFresnel(float3 n, float3 v, float f0)
{
    float NdotV = saturate(dot(n, v));
    return f0 + (1 - f0) * pow(1 - NdotV, 5);
}


float4 main(VertexToPixel input) : SV_TARGET
{
    // Clean up un-normalized normals
    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);

	// Adjust uv scaling
    input.uv = input.uv * uvScale + uvOffset;
    
    // The actual screen UV of this pixel
    float2 screenUV = input.screenPosition.xy / float2(screenWidth, screenHeight);
    
    // Use object’s normal map as an offset
    float2 offsetUV = NormalMap.Sample(BasicSampler, input.uv).xy * 2 - 1;
    offsetUV.y *= -1; // UV's are upside down compared to world space
    
    // Distort the screen UV by the offset, scaling as necessary
    float2 refractedUV = screenUV + offsetUV * refractionScale;
    
    // Verify that depth offset is valid
    float silhouette = RefractionSilhouette.Sample(ClampSampler, refractedUV).r;
    if (silhouette < 1)
        refractedUV = screenUV;
    
    // Get the color at the offset UV (now confirmed valid)
    float3 sceneColor = pow(ScreenPixels.Sample(ClampSampler, refractedUV).rgb, 2.2f) * colorTint;
    
    // Get reflections
    float3 viewToCam = normalize(cameraPosition - input.worldPos);
    float3 viewRefl = normalize(reflect(-viewToCam, input.normal));
    float3 envSample = EnvironmentMap.Sample(BasicSampler, viewRefl).rgb;
    
    // Determine the reflectivity based on viewing angle using the Schlick approximation of the Fresnel term
    float fresnel = SimpleFresnel(input.normal, viewToCam, F0_NON_METAL);
    return float4(pow(lerp(sceneColor, envSample * colorTint, fresnel), 1.0f / 2.2f), 1);
}