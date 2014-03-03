#include "Precompiled.h"
#include "Draw.h"

#if 0
#include "Mesh.h"
#include "Texture2D.h"
#include "Camera.h"
#include "UIRenderer.h"
#include "ResourceManager.h"

Todo( "store the rendering data in this class instead of having it split between Graphics and D3D11UIRenderer?" )
// ^^^^^^ this will still be bad. need to refactor lots of stuff

Todo( "properly encapsulate singletons. remove most of this friend class stuff that's everywhere" )

#include "Graphics.h"
#include "UIManager.h"
#include "UIRenderer.h"

/*------------------------------------------------------------------------------
render a static model
------------------------------------------------------------------------------*/
void Draw::StaticModel( Mesh const *mesh, Texture2D const *diffuseTexture, Texture2D const *specularTexture, Matrix const &transform, Team::Type teamColor ) {
    if( IsServer() ) return;

    g_graphics.AcquireLock();
    Assert( g_graphics.m_staticModelCount < Graphics::kMaxStaticModels, "static model buffer overflow" );
    Graphics::StaticModelData *data = &g_graphics.m_staticModels[ g_graphics.m_staticModelCount ];
    data->m_mesh             = mesh;
    data->m_diffuseTexture   = diffuseTexture;
    data->m_specularTexture  = specularTexture;
    data->m_transform        = transform;
    data->m_teamColor        = teamColor;

    ++g_graphics.m_staticModelCount;
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render an animated model
------------------------------------------------------------------------------*/
void Draw::SkinnedModel( Mesh const *mesh, Texture2D const *diffuseTexture, Texture2D const *specularTexture, Matrix const &transform, Matrix const *boneMatrices, uint16_t matrixCount, Team::Type teamColor ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    Assert( g_graphics.m_skinnedModelCount < Graphics::kMaxSkinnedModels, "skinned model buffer overflow" );
    Graphics::SkinnedModelData *data = &g_graphics.m_skinnedModels[ g_graphics.m_skinnedModelCount ];
    data->m_mesh             = mesh;
    data->m_diffuseTexture   = diffuseTexture;
    data->m_specularTexture  = specularTexture;
    data->m_transform        = transform;
    data->m_teamColor        = teamColor;
    memcpy( data->m_boneMatrices, boneMatrices, sizeof( Matrix ) * matrixCount );
    data->m_boneMatrixCount = matrixCount;

    ++g_graphics.m_skinnedModelCount;
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render a point light
------------------------------------------------------------------------------*/
void Draw::PointLight( Matrix const &transform,  Vector3 const &position, Vector3 const &color, float radius, float intensity ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    Assert( g_graphics.m_pointLightCount < Graphics::kMaxPointLights, "point light buffer overflow" );
    Graphics::PointLightData *data = &g_graphics.m_pointLights[ g_graphics.m_pointLightCount ];
    data->m_world = transform;
    data->m_lightPositionRcpRadius = Vector4( position, 1.0f / radius );
    data->m_lightColorIntensity = Vector4( color, intensity );

    Graphics::PointLightDataTiled *dataTiled = &g_graphics.m_pointLightsTiled[ g_graphics.m_pointLightCount ];
    dataTiled->position = Vector4(position, 1) * g_camera.GetView();
    dataTiled->radius = radius;
    dataTiled->color = color * intensity;
    dataTiled->invRadius = data->m_lightPositionRcpRadius.w;

    ++g_graphics.m_pointLightCount;
    g_graphics.ReleaseLock();
}
/*------------------------------------------------------------------------------
render a shockwave effect
------------------------------------------------------------------------------*/
void Draw::Shockwave( Matrix const &transform,  Vector3 const &position, float radius, float startTime) {
    if( IsServer() ) return;

    g_graphics.AcquireLock();
    Assert( g_graphics.m_pointLightCount < Graphics::kMaxShockWaves, "shockwave buffer overflow" );

    Graphics::ShockwaveData *data = &g_graphics.m_shockwaves[ g_graphics.m_shockwaveCount ];
    data->m_world = transform;
    data->m_positionRadius = Vector4( position, radius );
    data->m_startTime = startTime;

    ++g_graphics.m_shockwaveCount;
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render a screen space decal
------------------------------------------------------------------------------*/
void Draw::ScreenSpaceDecal( uint32_t textureAlbedoIndex, uint32_t textureNormalIndex, Matrix const &transform,  Vector3 const &position, Vector3 const &scale, float rotation2D ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    Assert( g_graphics.m_decalCount < Graphics::kMaxDecals, "decal buffer overflow" );
    Graphics::DecalData *data = &g_graphics.m_decals[ g_graphics.m_decalCount ];
    data->m_world = transform; 
    data->m_decalPosition2DRotationAlbedoTextureIndex = Vector4( position.x, position.z, rotation2D, (float)textureAlbedoIndex );
    data->m_decalScaleNormalTextureIndex = Vector4( scale, (float)textureNormalIndex );

    ++g_graphics.m_decalCount;
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
pixel
------------------------------------------------------------------------------*/
void Draw::Pixel( Point const &p, Vector4 const &color ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    FilledRect( p, p + Point( 1, 1 ), color );
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render a line
bresenham line drawing algorithm
------------------------------------------------------------------------------*/
void Draw::Line2D( Point const &start, Point const &end, Vector4 const &color ) {
    if( IsServer() ) return;
    
    int32_t x0 = start.x;
    int32_t y0 = start.y;
    int32_t x1 = end.x;
    int32_t y1 = end.y;

    int32_t dx  = abs( x1 - x0 );
    int32_t dy  = abs( y1 - y0 );
    int32_t sx  = x0 < x1 ? 1 : -1;
    int32_t sy  = y0 < y1 ? 1 : -1;
    int32_t err = dx - dy;
 
    for( ; ; ) {
        Pixel( Point( (int16_t)x0, (int16_t)y0 ), color );
        if( x0 == x1 && y0 == y1 ) return;
        int32_t e2 = 2 * err;
        if( e2 > -dy ) {
            err -= dy;
            x0  += sx;
        }
        if( e2 < dx ) {
            err += dx;
            y0  += sy;
        }
    }
}

/*------------------------------------------------------------------------------
render a line
------------------------------------------------------------------------------*/
void Draw::Line3D( Vector3 const &start, Vector3 const &end, Vector4 const &color ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    Assert( g_graphics.m_pointCount < Graphics::kMaxPoints - 1, "point buffer overflow" );
    Graphics::PointData *startPoint = &g_graphics.m_points[ g_graphics.m_pointCount     ];
    Graphics::PointData *endPoint   = &g_graphics.m_points[ g_graphics.m_pointCount + 1 ];

    startPoint->m_position = start;
    startPoint->m_color    = color;
    endPoint->m_position   = end;
    endPoint->m_color      = color;

    g_graphics.m_pointCount += 2;
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render a colored quad
------------------------------------------------------------------------------*/
void Draw::FilledRect( Point const &topLeft, Point const &bottomRight, Vector4 const &color ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    g_uiManager.GetRenderer().SetDrawColor( color );
    g_uiManager.GetRenderer().DrawFilledRect( Rect( topLeft, bottomRight ) );
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
render a textured quad
------------------------------------------------------------------------------*/
void Draw::TexturedRect( Texture2D *texture, Point const &topLeft, Point const &bottomRight, float u1, float v1, float u2, float v2 ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    g_uiManager.GetRenderer().SetDrawColor( Vector4( 1, 1, 1, 1 ) );
    g_uiManager.GetRenderer().DrawTexturedRect( texture, Rect( topLeft, bottomRight ), u1, v1, u2, v2 );
    g_graphics.ReleaseLock();
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeCircle( Circle const &circle, Vector4 const &color ) {
    if( IsServer() ) return;
    
    enum {
        kSegments = 12,
    };

    Vector3 const &c = Vector3( circle.m_center.x, 15.0f, circle.m_center.y );
    float const r    = circle.m_radius;

    float thetaStep = ( k2Pi  / kSegments );
    float theta = thetaStep;
    Vector3 start = Vector3( c.x + r, c.y, c.z );
    for( uint32_t i = 0; i < kSegments; ++i, theta += thetaStep ) {
        Vector3 end = Vector3( c.x + r * cosf( theta ), c.y, c.z + r * sinf( theta ) );
        Draw::Line3D( start, end, color );
        start = end;
    }
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeTriangle( Triangle const &t, Vector4 const &color ) {
    if( IsServer() ) return;
    
    Draw::Line3D( t.m_a, t.m_b, color );
    Draw::Line3D( t.m_b, t.m_c, color );
    Draw::Line3D( t.m_c, t.m_a, color );
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeArrow( Triangle const &t, Vector4 const &color ) {
    if( IsServer() ) return;
    
    Draw::Line3D( t.m_a, t.m_b, color );
    Draw::Line3D( t.m_a, t.m_c, color );
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeSphere( Sphere const &sphere, Vector4 const &color ) {
    if( IsServer() ) return;
    
    enum {
        kRings = 8,
        kSegmentsPerRing = 12,
    };

    float thetaStep = ( kPi  / ( kRings + 1 ) );
    float phiStep   = ( k2Pi / kSegmentsPerRing );
    float cosPhiArray[ kSegmentsPerRing ];
    float sinPhiArray[ kSegmentsPerRing ];

    // precalculate sin( phi ) and cos( phi )
    float *cosPhi = cosPhiArray;
    float *sinPhi = sinPhiArray;
    float phi = phiStep;
    for( uint32_t i = 0; i < kSegmentsPerRing; ++i, ++cosPhi, ++sinPhi, phi += phiStep ) {
        *cosPhi = cosf( phi );
        *sinPhi = sinf( phi );
    }

    Vector3 const &c = sphere.m_center;
    float const r    = sphere.m_radius;

    float theta = thetaStep;
    for( uint32_t i = 0; i < kRings; ++i, theta += thetaStep ) {
        float rSinTheta = r * sinf( theta );
        float y = c.y + r * cosf( theta );
        Vector3 start = Vector3( c.x + rSinTheta, y, c.z );

        cosPhi = cosPhiArray;
        sinPhi = sinPhiArray;
        for( uint32_t j = 0; j < kSegmentsPerRing; ++j, ++cosPhi, ++sinPhi ) {
            Vector3 end = Vector3( c.x + rSinTheta * *cosPhi, y, c.z + rSinTheta * *sinPhi );
            Draw::Line3D( start, end, color );
            start = end;
        }
    }
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeAABB( AABB const &aabb, Vector4 const &color ) {
    if( IsServer() ) return;
    
    // transform the extents
    Vector3 a = aabb.m_center + aabb.m_halfExtents;
    Vector3 b = aabb.m_center - aabb.m_halfExtents;

    Vector3 v[ 8 ];
    v[ 0 ] = Vector3( a.x, a.y, a.z );
    v[ 1 ] = Vector3( a.x, a.y, b.z );
    v[ 2 ] = Vector3( a.x, b.y, a.z );
    v[ 3 ] = Vector3( a.x, b.y, b.z );
    v[ 4 ] = Vector3( b.x, a.y, a.z );
    v[ 5 ] = Vector3( b.x, a.y, b.z );
    v[ 6 ] = Vector3( b.x, b.y, a.z );
    v[ 7 ] = Vector3( b.x, b.y, b.z );

    Draw::Line3D( v[ 0 ], v[ 1 ], color );
    Draw::Line3D( v[ 1 ], v[ 3 ], color );
    Draw::Line3D( v[ 3 ], v[ 2 ], color );
    Draw::Line3D( v[ 2 ], v[ 0 ], color );
    
    Draw::Line3D( v[ 4 ], v[ 5 ], color );
    Draw::Line3D( v[ 5 ], v[ 7 ], color );
    Draw::Line3D( v[ 7 ], v[ 6 ], color );
    Draw::Line3D( v[ 6 ], v[ 4 ], color );

    Draw::Line3D( v[ 0 ], v[ 4 ], color );
    Draw::Line3D( v[ 1 ], v[ 5 ], color );
    Draw::Line3D( v[ 2 ], v[ 6 ], color );
    Draw::Line3D( v[ 3 ], v[ 7 ], color );
}

/*------------------------------------------------------------------------------
wireframe primitive
------------------------------------------------------------------------------*/
void Draw::WireframeOBB( OBB const &obb, Vector4 const &color ) {
    if( IsServer() ) return;
    
    // transform the extents
    Vector3 const &a = obb.m_halfExtents;
    Vector3 b = -a;

    Vector3 v[ 8 ];
    v[ 0 ] = Vector3( a.x, a.y, a.z ) * obb.m_orientation + obb.m_center;
    v[ 1 ] = Vector3( a.x, a.y, b.z ) * obb.m_orientation + obb.m_center;
    v[ 2 ] = Vector3( a.x, b.y, a.z ) * obb.m_orientation + obb.m_center;
    v[ 3 ] = Vector3( a.x, b.y, b.z ) * obb.m_orientation + obb.m_center;
    v[ 4 ] = Vector3( b.x, a.y, a.z ) * obb.m_orientation + obb.m_center;
    v[ 5 ] = Vector3( b.x, a.y, b.z ) * obb.m_orientation + obb.m_center;
    v[ 6 ] = Vector3( b.x, b.y, a.z ) * obb.m_orientation + obb.m_center;
    v[ 7 ] = Vector3( b.x, b.y, b.z ) * obb.m_orientation + obb.m_center;

    Draw::Line3D( v[ 0 ], v[ 1 ], color );
    Draw::Line3D( v[ 1 ], v[ 3 ], color );
    Draw::Line3D( v[ 3 ], v[ 2 ], color );
    Draw::Line3D( v[ 2 ], v[ 0 ], color );
    
    Draw::Line3D( v[ 4 ], v[ 5 ], color );
    Draw::Line3D( v[ 5 ], v[ 7 ], color );
    Draw::Line3D( v[ 7 ], v[ 6 ], color );
    Draw::Line3D( v[ 6 ], v[ 4 ], color );

    Draw::Line3D( v[ 0 ], v[ 4 ], color );
    Draw::Line3D( v[ 1 ], v[ 5 ], color );
    Draw::Line3D( v[ 2 ], v[ 6 ], color );
    Draw::Line3D( v[ 3 ], v[ 7 ], color );
}

/*------------------------------------------------------------------------------
2D text
------------------------------------------------------------------------------*/
void Draw::Text2D( char const *text, Point const &position, Vector4 const &color ) {
    Text2D( g_resourceManager.GetFont( "Arial.ttf", 12.0f ), text, position, color );
}

/*------------------------------------------------------------------------------
2D text
------------------------------------------------------------------------------*/
void Draw::Text2D( Font *font, char const *text, Point const &position, Vector4 const &color ) {
    if( IsServer() ) return;
    
    g_graphics.AcquireLock();
    Point pos     = Point( position.x, position.y );
    Point dropPos = Point( pos.x + 1, pos.y + 1 );

    // add a black dropshadow
    g_uiManager.GetRenderer().SetDrawColor( Vector4( 0, 0, 0, 1 ) );
    g_uiManager.GetRenderer().RenderText( font, dropPos, text );

    // render the rext
    g_uiManager.GetRenderer().SetDrawColor( color );
    g_uiManager.GetRenderer().RenderText( font, pos, text );
    g_graphics.ReleaseLock();
}

#endif