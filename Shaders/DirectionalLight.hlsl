Texture2D< float >  depthTex         : register( t0 );
Texture2D< float4 > positionTex      : register( t1 );
Texture2D< float4 > normalsTex       : register( t2 );
Texture2D< float4 > diffuseTex       : register( t3 );
Texture2D< float >  lightDepthTex    : register( t4 );

static const float3 kAmbientUp = float3( 0.2f, 0.2f, 0.2f );
static const float3 kAmbientDown = float3( 0.1f, 0.1f, 0.1f );
static const float kBias = 0.002f;

// samplers
SamplerState linearClamp : register( s0 );

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

cbuffer ShadowCB : register( b2 ) {
	int4   cascadeCount;
	float4 cascadeDepth[ 2 ];
	float4 cascadeOffset[ 8 ];
	float4 cascadeScale[ 8 ];
};

// pixel shader input
struct PS_INPUT {
    float4 sv_position   		: SV_POSITION;
    float4 color				: Color;
    float2 texcoord             : Texcoord;
    uint   textured             : Textured;
};

static const float4 cascadeColor[ 8 ] =  {
    float4 ( 1.5f, 0.0f, 0.0f, 1.0f ),
    float4 ( 0.0f, 1.5f, 0.0f, 1.0f ),
    float4 ( 0.0f, 0.0f, 5.5f, 1.0f ),
    float4 ( 1.5f, 0.0f, 5.5f, 1.0f ),
    float4 ( 1.5f, 1.5f, 0.0f, 1.0f ),
    float4 ( 1.0f, 1.0f, 1.0f, 1.0f ),
    float4 ( 0.0f, 1.0f, 5.5f, 1.0f ),
    float4 ( 0.5f, 3.5f, 0.75f, 1.0f )
};

/*------------------------------------------------------------------------------
p.xy = uv coordinates to lookup in shadow map
p.z = projection space depth (z/w, w == 1 because orthographic) using one of the cascade projections
------------------------------------------------------------------------------*/
float4 PositionLightVSToCascade( int cascadeIndex, float4 positionLightVS ) {
	float4 p = positionLightVS;
	p *= cascadeScale[ cascadeIndex ];
	p += cascadeOffset[ cascadeIndex ];
	p.x = ( p.x + (float)cascadeIndex ) / (float)8;
	return p;
}

/*------------------------------------------------------------------------------
pixel shader
------------------------------------------------------------------------------*/
float4 PS( in PS_INPUT IN ) : SV_Target {
	uint3 index = uint3( IN.sv_position.xy, 0 );

	float3 position = positionTex.Load( index ).xyz;
	float3 normal = normalsTex.Load( index ).xyz;
	float3 diffuse = diffuseTex.Load( index ).xyz;

#if 0
	float4 pixelShadowPS = mul( float4( position, 1.0f ), lightViewProjection );
	float pixelDepth = pixelShadowPS.z / pixelShadowPS.w;
	float2 uv = float2( pixelShadowPS.x, -pixelShadowPS.y ) / pixelShadowPS.w * 0.5f + 0.5f;
	float lightDepth = lightDepthTex.Sample( linearClamp, uv ).x;

	//return float4( uv, 0, 1 );

	if( uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f ) lightDepth = 0.0f;

	float s = dot( normal, float3( 0, 1, 0 ) ) * 0.5f + 0.5f;
	float3 ambient = lerp( kAmbientDown, kAmbientUp, s );

	//return float4( 1, 1, 1, 1 );

	if( pixelDepth <= lightDepth + kBias ) {
		float nDotL = dot( normal, -direction.xyz );
		return float4( diffuse * nDotL + diffuse * ambient, 1.0f );
	}
		
	return float4( diffuse * ambient, 1.0f );
#endif

	float4 positionVS = mul( float4( position, 1.0f ), view );
	float4 depthVS = positionVS.zzzz;

	//return float4( -positionVS.zzz, 1 );

    float4 comparison  = ( depthVS < cascadeDepth[ 0 ] );
    float4 comparison2 = ( depthVS < cascadeDepth[ 1 ] );
    float cascadeIndexFloat = dot( float4( cascadeCount.x > 0,
                                           cascadeCount.x > 1, 
                                           cascadeCount.x > 2, 
                                           cascadeCount.x > 3 ), comparison ) +
                              dot( float4( cascadeCount.x > 4,
                                           cascadeCount.x > 5,
                                           cascadeCount.x > 6,
                                           cascadeCount.x > 7 ), comparison2 );      
	cascadeIndexFloat = min( cascadeIndexFloat, cascadeCount.x - 1 ); 
    int cascadeIndex = (int)cascadeIndexFloat;

	float f = dot( float4( cascadeCount.x > 0,
                                           cascadeCount.x > 1, 
                                           cascadeCount.x > 2, 
                                           cascadeCount.x > 3 ), comparison );
	//return float4( f.xxx, 1 );
  
#if 0
    // Repeat text coord calculations for the next cascade. 
    // The next cascade index is used for blurring between maps.
    iNextCascadeIndex = min( CASCADE_COUNT_FLAG - 1, iCurrentCascadeIndex + 1 ); 

    float fBlendBetweenCascadesAmount = 1.0f;
    float fCurrentPixelsBlendBandLocation = 1.0f;

    CalculateBlendAmountForInterval( iCurrentCascadeIndex,
									 fCurrentPixelDepth,
									 fCurrentPixelsBlendBandLocation,
									 fBlendBetweenCascadesAmount );
#endif

	//if( cascadeCount.x == 1 ) return float4( 0, 1, 1, 1 );

	float4 positionLightVS = mul( float4( position, 1.0f ), lightView );
	float4 cascade = PositionLightVSToCascade( cascadeIndex, positionLightVS );
	float pixelDepth = cascade.z;
	float lightDepth = lightDepthTex.Sample( linearClamp, cascade.xy ).x;

	float s = dot( normal, float3( 0, 1, 0 ) ) * 0.5f + 0.5f;
	float3 ambient = lerp( kAmbientDown, kAmbientUp, s );

	//return cascadeColor[ cascadeIndex ];
	//return float4( lightDepth.xxx, 1 );

	float4 color;
	if( pixelDepth <= lightDepth + kBias ) {
		float nDotL = dot( normal, -direction.xyz );
		color = float4( diffuse * ( nDotL + ambient ), 1.0f );
	}
	else color = float4( diffuse * ambient, 1.0f );
		
	//return color * cascadeColor[ cascadeIndex ];
	return color;
			                                 
#if 0
    if( fCurrentPixelsBlendBandLocation < m_fCascadeBlendArea) {
		// the current pixel is within the blend band.
            
		ComputeCoordinatesTransform( iNextCascadeIndex,
									 Input.vInterpPos,
									 vShadowMapTextureCoord_blend,
									 vShadowMapTextureCoordViewSpace );  

		CalculatePCFPercentLit( vShadowMapTextureCoord_blend,
								fRightTextDepthWeight_blend, 
								fUpTextDepthWeight_blend,
								fBlurRowSize,
								fPercentLit_blend );

		// Blend the two calculated shadows by the blend amount.
		fPercentLit = lerp( fPercentLit_blend, fPercentLit, fBlendBetweenCascadesAmount ); 	
    }   
#endif

}