Texture2D< float4 > positionTex      : register( t0 );
Texture2D< float4 > normalsTex       : register( t1 );
Texture2D< float4 > randomNormalsTex : register( t2 );

static const float kRandomTexSize = 64.0f;

// samplers
SamplerState linearClamp : register( s0 );
SamplerState linearWrap  : register( s3 );

// constant buffers
cbuffer PerFrameCB : register( b0 ) {
	row_major float4x4 view;
	row_major float4x4 projection;
	row_major float4x4 viewProjection;
	row_major float4x4 invView;
	row_major float4x4 invViewProjection;
	float4             invScreenSize;
	float4             twoInvScreenSize;
};

cbuffer LightCB : register( b1 ) {
	row_major float4x4 lightView;
	row_major float4x4 lightProjection;
	row_major float4x4 lightViewProjection;
	row_major float4x4 lightInvView;
	row_major float4x4 lightInvViewProjection;
	float4             direction;
	float4             color;
};

// pixel shader input
struct PS_INPUT {
    float4 sv_position   		: SV_POSITION;
    float4 color				: Color;
    float2 texcoord             : Texcoord;
    uint   textured             : Textured;
};

static const int kIterations = 4;
static const float kRadius = 0.03f;
static const float kIntensity = 1.0f;
static const float kScale = 0.01f;
static const float kBias = 0.03f;
static const float2 offsets[ 4 ] = { float2( 1, 0 ), float2( -1, 0 ),
                                     float2( 0, 1 ), float2( 0, -1 ) };

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
float CalculateOcclusion( float2 tcoord, float2 uv, float3 p, float3 cnorm ) {
	float3 diff = mul( float4( positionTex.Sample( linearClamp, tcoord + uv ).xyz, 1.0f ), view ).xyz - p;
	const float3 v = normalize( diff );
	const float d = length( diff ) * kScale;
	return max( 0.0f, dot( cnorm, v ) - kBias ) * ( 1.0f / ( 1.0f + d ) ) * kIntensity;
}

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
float4 PS( in PS_INPUT IN ) : SV_Target {
	uint3 index = uint3( IN.sv_position.xy, 0 );

	float3 positionWS = positionTex.Load( index ).xyz;
	float3 normalWS = normalsTex.Load( index ).xyz;

	float3 positionVS = mul( float4( positionWS, 1.0f ), view ).xyz;
	float3 normalVS = normalize( mul( float4( normalWS, 0.0f ), view ).xyz );
	float2 random = normalize( randomNormalsTex.Sample( linearWrap, IN.sv_position.xy / kRandomTexSize ).xy * 2.0f - 1.0f );

	float occlusion = 0.0f;
	float radius = kRadius / positionVS.z;
	float2 uv = ( IN.sv_position.xy + 0.5f ) * invScreenSize.xy;

	for( int i = 0; i < kIterations; ++i ) {
		float2 coord1 = reflect( offsets[ i ], random ) * radius;
		float2 coord2 = float2( coord1.x - coord1.y, coord1.x + coord1.y ) * 0.707;
  
		occlusion += CalculateOcclusion( uv, coord1 * 0.25f, positionVS, normalVS );
		occlusion += CalculateOcclusion( uv, coord2 * 0.5f,  positionVS, normalVS );
		occlusion += CalculateOcclusion( uv, coord1 * 0.75f, positionVS, normalVS );
		occlusion += CalculateOcclusion( uv, coord2,         positionVS, normalVS );
	}

	return float4( 1.0f - occlusion.xxx / ( kIterations * 4 ), 1.0f );
}