Texture2D< float > depthTex    : register( t0 );

// constant buffer
cbuffer PerFrameCB : register( b0 ) {
	row_major float4x4 view;
	row_major float4x4 projection;
	row_major float4x4 viewProjection;
	row_major float4x4 invView;
	row_major float4x4 invViewProjection;
	float4             invScreenSize;
	float4             twoInvScreenSize;
};

// pixel shader input
struct PS_INPUT {
    float4 sv_position			: SV_POSITION;
    float4 color				: Color;
    float2 texcoord             : Texcoord;
    uint   textured             : Textured;
};

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
float4 PS( in PS_INPUT IN ) : SV_Target {
	uint3 index = uint3( IN.sv_position.xy, 0 );
	float depth = depthTex.Load( index );

    // convert depth to linear view space
    float linearDepth = projection[ 3 ][ 2 ] / ( depth + projection[ 2 ][ 2 ] );
	linearDepth -= 281.0f;
	//linearDepth /= 100.0f; 

	//linearDepth -= 281.0f;
    return float4( linearDepth.xxx, 1.0f );
}