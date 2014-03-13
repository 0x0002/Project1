// constant buffer
cbuffer LightCB : register( b1 ) {
	row_major float4x4 lightView;
	row_major float4x4 lightProjection;
	row_major float4x4 lightViewProjection;
	row_major float4x4 lightInvView;
	row_major float4x4 lightInvViewProjection;
	float4             direction;
	float4             color;
};

// vertex shader input
struct VS_INPUT {
	float3 position                         : POSITION;                     // object-space vertex position
    float3 normal                           : NORMAL;                       // object-space vertex normal
	float2 texCoord                         : TEXCOORD;                     //
    row_major float4x4 world                : INSTANCED_WORLD;              // world
    row_major float4x4 transpInvWorld       : INSTANCED_TRANSP_INV_WORLD;   // transpose( inverse( world ) )
    float4 color                            : INSTANCED_COLOR;				//
};

// pixel shader input
struct PS_INPUT {
    float4 sv_position          : SV_POSITION;
    float4 positionWS           : POSITION0;
    float4 normalWS             : POSITION1;
    float4 color                : POSITION2;
};

struct PS_OUTPUT {
    float4 positionWS           : SV_Target0;
    float4 normalWS             : SV_Target1;
    float4 color                : SV_Target2;
};

/*------------------------------------------------------------------------------
vertex shader
------------------------------------------------------------------------------*/
void VS( in VS_INPUT IN, out PS_INPUT OUT ) {
    float4x4 positionTransform = mul( IN.world, lightView );
    positionTransform = mul( positionTransform, lightProjection );

    OUT.sv_position = mul( float4( IN.position, 1.0f ), positionTransform );
    OUT.positionWS = mul( float4( IN.position, 1.0f ), IN.world );
    OUT.normalWS = mul( float4( IN.normal, 0.0f ), IN.transpInvWorld );
    OUT.color = IN.color;
}

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
PS_OUTPUT PS( in PS_INPUT IN ) {
    PS_OUTPUT OUT; 
    OUT.positionWS = float4( IN.positionWS.xyz, 1 );
    OUT.normalWS = float4( normalize( IN.normalWS.xyz ), 1 );
    OUT.color = IN.color;
    return OUT;
}