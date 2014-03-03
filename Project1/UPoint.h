#ifndef UPOINT_H
#define UPOINT_H

/*------------------------------------------------------------------------------
a pair of uints16s
------------------------------------------------------------------------------*/
struct UPoint {
public:
    // constructors
    inline UPoint() : x( 0 ), y( 0 ) {}
    inline UPoint( uint16_t x, uint16_t y ) : x( x ), y( y ) {}
    inline explicit UPoint( Vector2 const &v ) : x( v.x >= 0 ? (uint16_t)v.x : 0 ), y( v.y >= 0 ? (uint16_t)v.y : 0 ) {}

    // binary operators
    inline bool   operator==( UPoint const &p ) const { return *(uint32_t*)this == *(uint32_t*)&p; }
    inline bool   operator!=( UPoint const &p ) const { return *(uint32_t*)this != *(uint32_t*)&p; }
    inline UPoint operator+ ( UPoint const &p ) const { return UPoint( x + p.x, y + p.y ); }
    inline UPoint operator- ( UPoint const &p ) const { return UPoint( x - p.x, y - p.y ); }
    inline UPoint operator* ( uint16_t i )      const { return UPoint( x * i, y * i ); }
    inline UPoint operator/ ( uint16_t i )      const { return UPoint( x / i, y / i ); }

public:
	uint16_t x, y;
};

#endif // UPOINT_H