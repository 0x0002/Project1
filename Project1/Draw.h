#ifndef DRAW_H
#define DRAW_H

#if 0
#include "Circle.h"     //
#include "Sphere.h"     //
#include "OBB.h"        //
#include "AABB.h"       //
#include "Triangle.h"   //
#include "TeamTypes.h"

// forward declarations
class Mesh;
class Texture2D;
class Font;

/*------------------------------------------------------------------------------
class with static functions to put all draw calls in one place
this class is stupid
------------------------------------------------------------------------------*/
class Draw {
public:
    // models
    static void StaticModel ( Mesh const *mesh, Texture2D const *diffuseTexture, Texture2D const *specularTexture, Matrix const &transform, Team::Type teamColor );
    static void SkinnedModel( Mesh const *mesh, Texture2D const *diffuseTexture, Texture2D const *specularTexture, Matrix const &transform, Matrix const *boneMatrices, uint16_t matrixCount, Team::Type teamColor );

    // lights
    static void PointLight( Matrix const &transform,  Vector3 const &position, Vector3 const &color, float radius, float intensity );

    // shockwaves
    static void Shockwave( Matrix const &transform,  Vector3 const &position, float radius, float startTime);

    // decals
    static void ScreenSpaceDecal( uint32_t textureAlbedoIndex, uint32_t textureNormalIndex, Matrix const &transform,  Vector3 const &position, Vector3 const &scale, float rotation2D );

    // pixel
    static void Pixel( Point const &p, Vector4 const &color );
    inline static void Pixel( Point const &p ) { Pixel( p, Vector4( 1, 1, 1, 1 ) ); }

    // line
    static void Line2D( Point const &start, Point const &end, Vector4 const &color );
    static void Line3D( Vector3 const &start, Vector3 const &end, Vector4 const &color );

    inline static void Line2D( Point const &start, Point const &end )     { Line2D( start, end, Vector4( 1, 1, 1, 1 ) ); }
    inline static void Line3D( Vector3 const &start, Vector3 const &end ) { Line3D( start, end, Vector4( 1, 1, 1, 1 ) ); }

    // quad
    static void FilledRect( Point const &topLeft, Point const &bottomRight, Vector4 const &color );
    static void TexturedRect( Texture2D *texture, Point const &topLeft, Point const &bottomRight, float u1, float v1, float u2, float v2 );

    inline static void FilledRect( Point const &topLeft, Point const &bottomRight ) { FilledRect( topLeft, bottomRight, Vector4( 1, 1, 1, 1 ) ); }
    inline static void TexturedRect( Texture2D *texture, Point const &topLeft, Point const &bottomRight ) { TexturedRect( texture, topLeft, bottomRight, 0, 0, 1, 1 ); }

    // wireframe primitive
    static void WireframeCircle  ( Circle const &c,   Vector4 const &color );
    static void WireframeTriangle( Triangle const &t, Vector4 const &color );
    static void WireframeArrow   ( Triangle const &t, Vector4 const &color );
    static void WireframeSphere  ( Sphere const &s,   Vector4 const &color );
    static void WireframeAABB    ( AABB const &aabb,  Vector4 const &color );
    static void WireframeOBB     ( OBB const &obb,    Vector4 const &color );

    inline static void WireframeCircle  ( Circle const &c   ) { WireframeCircle  ( c,    Vector4( 1, 1, 1, 1 ) ); }
    inline static void WireframeTriangle( Triangle const &t ) { WireframeTriangle( t,    Vector4( 1, 1, 1, 1 ) ); }
    inline static void WireframeSphere  ( Sphere const &s   ) { WireframeSphere  ( s,    Vector4( 1, 1, 1, 1 ) ); }
    inline static void WireframeAABB    ( AABB const &aabb  ) { WireframeAABB    ( aabb, Vector4( 1, 1, 1, 1 ) ); }
    inline static void WireframeOBB     ( OBB const &obb    ) { WireframeOBB     ( obb,  Vector4( 1, 1, 1, 1 ) ); }

    // text
    static void Text2D( char const *text, Point const &position, Vector4 const &color = Vector4( 1, 1, 1, 1 ) );
    static void Text2D( Font *font, char const *text, Point const &position, Vector4 const &color = Vector4( 1, 1, 1, 1 ) );
};

#endif

#endif // DRAW_H