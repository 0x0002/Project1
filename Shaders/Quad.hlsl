// this is used by the UI renderer

Texture2D tex0             : register( t0 );
SamplerState SamplerLinear : register( s0 );

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

// vertex shader and geometry shader input
struct GS_INPUT {
	float  top                  : TOP;
    float  left                 : LEFT;
    float  bottom               : BOTTOM;
    float  right                : RIGHT;
    float  topTexcoord          : TOP_TEXCOORD;
    float  leftTexcoord         : LEFT_TEXCOORD;
    float  bottomTexcoord       : BOTTOM_TEXCOORD;
    float  rightTexcoord        : RIGHT_TEXCOORD;
    float4 color				: COLOR;
    uint   textured             : TEXTURED;
};

// pixel shader input
struct PS_INPUT {
    float4 sv_position   		: SV_POSITION;
    float4 color				: Color;
    float2 texcoord             : Texcoord;
    uint   textured             : Textured;
};

/*------------------------------------------------------------------------------
vertex shader
------------------------------------------------------------------------------*/
void VS( in GS_INPUT IN, out GS_INPUT OUT ) {
    OUT = IN;
}

/*------------------------------------------------------------------------------
geometry shader
------------------------------------------------------------------------------*/
[ maxvertexcount( 4 ) ]
void GS( point GS_INPUT IN[ 1 ], inout TriangleStream< PS_INPUT > OUT ) {
    float2 scale = twoInvScreenSize.xy;
    float top    = -( IN[ 0 ].top    * scale.y - 1 );
    float left   =    IN[ 0 ].left   * scale.x - 1;
    float bottom = -( IN[ 0 ].bottom * scale.y - 1 );
    float right  =    IN[ 0 ].right  * scale.x - 1;

    PS_INPUT topLeft;
    topLeft.sv_position  = float4( left, top, 0.0f, 1.0f );
    topLeft.texcoord     = float2( IN[ 0 ].leftTexcoord, IN[ 0 ].topTexcoord );
    topLeft.color        = IN[ 0 ].color;
    topLeft.textured     = IN[ 0 ].textured;

    PS_INPUT topRight;
    topRight.sv_position = float4( right, top, 0.0f, 1.0f );
    topRight.texcoord    = float2( IN[ 0 ].rightTexcoord, IN[ 0 ].topTexcoord );
    topRight.color       = IN[ 0 ].color;
    topRight.textured    = IN[ 0 ].textured;

    PS_INPUT bottomLeft;
    bottomLeft.sv_position = float4( left, bottom, 0.0f, 1.0f );
    bottomLeft.texcoord    = float2( IN[ 0 ].leftTexcoord, IN[ 0 ].bottomTexcoord );
    bottomLeft.color       = IN[ 0 ].color;
    bottomLeft.textured    = IN[ 0 ].textured;

    PS_INPUT bottomRight;
    bottomRight.sv_position = float4( right, bottom, 0.0f, 1.0f );
    bottomRight.texcoord    = float2( IN[ 0 ].rightTexcoord, IN[ 0 ].bottomTexcoord );
    bottomRight.color       = IN[ 0 ].color;
    bottomRight.textured    = IN[ 0 ].textured;

    OUT.Append( topLeft );
    OUT.Append( topRight );
    OUT.Append( bottomLeft );    
    OUT.Append( bottomRight );  
}

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
float4 PS( in PS_INPUT IN ) : SV_Target {
    return IN.textured ? ( tex0.Sample( SamplerLinear, IN.texcoord ) * IN.color ) : IN.color;
}