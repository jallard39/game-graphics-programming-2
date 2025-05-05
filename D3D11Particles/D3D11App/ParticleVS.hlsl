
#include "ShaderStructs.hlsli"

cbuffer ExternalData : register(b0)
{
    matrix view;
    matrix projection;
    
    float4 startColor;
    float4 endColor;
    
    float currentTime;
    float3 acceleration;
    
    float startSize;
    float endSize;
    float lifetime;
    float fadeOut;
};

// Particle struct for particle emitters
struct Particle
{
    float EmitTime;
    float3 StartPosition;
    float3 StartVelocity;
};

// Buffer of particle data
StructuredBuffer<Particle> ParticleData : register(t0);

// The entry point for our vertex shader
VertexToPixel_Particle main( uint id : SV_VertexID )
{
	// Set up output
    VertexToPixel_Particle output;
    float3 pos;
    
    // Get id info
    uint particleID = id / 4; // Every 4 verts are ONE particle!
    uint cornerID = id % 4; // 0,1,2,3 = which corner of the "quad"
    
    // Grab one particle
    Particle p = ParticleData.Load(particleID);

    // Calculate age and agePercent
    float age = currentTime - p.EmitTime;
    float agePercent = age / lifetime;
    
    // Simulate position
    pos = acceleration * age * age / 2.0f + p.StartVelocity * age + p.StartPosition;
    
    // Offsets for the 4 corners of a quad - we'll only use one for each
    // vertex, but which one depends on the cornerID
    float2 offsets[4];
    offsets[0] = float2(-1.0f, +1.0f); // TL
    offsets[1] = float2(+1.0f, +1.0f); // TR
    offsets[2] = float2(+1.0f, -1.0f); // BR
    offsets[3] = float2(-1.0f, -1.0f); // BL
    
    // Size interpolation
    float size = lerp(startSize, endSize, agePercent);
    
    // Billboarding!
    // Offset the position based on the camera's right and up vectors
    pos += float3(view._11, view._12, view._13) * offsets[cornerID].x * size; // RIGHT
    pos += float3(view._21, view._22, view._23) * offsets[cornerID].y * size; // UP
    
    // Finally, calculate output position here using View and Projection matrices
    output.position = mul(mul(projection, view), float4(pos, 1.0f));
    
    // Offsets for the uvs
    float2 uvs[4];
    uvs[0] = float2(0, 0); // TL
    uvs[1] = float2(1, 0); // TR
    uvs[2] = float2(1, 1); // BR
    uvs[3] = float2(0, 1); // BL
    output.uv = uvs[cornerID];
    
    // Set output color and fade
    output.colorTint = lerp(startColor, endColor, agePercent);
    output.colorTint *= saturate((lifetime - age) / fadeOut);
    
    return output;
}