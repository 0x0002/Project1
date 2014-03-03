#ifndef POINT_H
#define POINT_H

/*------------------------------------------------------------------------------
a pair of int16s
------------------------------------------------------------------------------*/
struct Point {
public:
    // constructors
    inline Point() {}
    inline Point( int16_t x, int16_t y ) : x( x ), y( y ) {}
    inline explicit Point( Vector2 const &v ) : x( (int16_t)v.x ), y( (int16_t)v.y ) {}

    // binary operators
    inline bool  operator==( Point const &p ) const { return *(uint32_t*)this == *(uint32_t*)&p; }
    inline bool  operator!=( Point const &p ) const { return *(uint32_t*)this != *(uint32_t*)&p; }
    inline Point operator+ ( Point const &p ) const { return Point( x + p.x, y + p.y ); }
    inline Point operator- ( Point const &p ) const { return Point( x - p.x, y - p.y ); }
    inline Point operator* ( int16_t i )      const { return Point( x * i, y * i ); }
    inline Point operator/ ( int16_t i )      const { return Point( x / i, y / i ); }

public:
	int16_t x, y;
};

#endif // POINT_H