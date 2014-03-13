Texture2D< float4 > lightTex   : register( t0 );

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
	uint3 index = uint3( IN.sv_position.xy, 0 );
	float3 diffuse = lightTex.Load( index ).xyz;
    return float4( diffuse, 1.0f );
}