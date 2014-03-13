Texture2D< float4 > tex : register( t0 );

// pixel shader input
struct PS_INPUT {
    float4 sv_position   		: SV_POSITION;
    float4 color				: Color;
    float2 texcoord             : Texcoord;
    uint   textured             : Textured;
};

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
float4 PS( in PS_INPUT IN ) : SV_Target {
	float3 sum = float3( 0.0f, 0.0f, 0.0f );
	for( int i = -2; i <= 2; ++i ) {
		for( int j = -2; j <= 2; ++j ) {
			sum += tex.Load( uint3( IN.sv_position.x + i, IN.sv_position.y + j, 0 ) ).xyz;
		}
	}
	return float4( sum / 25, 1.0f );
}