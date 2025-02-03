
// Struct representing a single vertex worth of data
struct VertexShaderInput
{ 
	float3 localPosition	: POSITION;     // XYZ position
    float2 uv				: TEXCOORD;
    float3 normal			: NORMAL;
    float3 tangent			: TANGENT;
};

// Struct representing the data we're sending down the pipeline
struct VertexToPixel
{
	float4 screenPosition	: SV_POSITION;	// XYZW position (System Value Position)
};

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
    //output.uv = input.uv;
    //output.normal = mul((float3x3) worldInvTranspose, input.normal);
    //output.worldPosition = mul(world, float4(input.localPosition, 1.0f)).xyz;

	return output;
}