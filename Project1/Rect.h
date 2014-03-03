#ifndef RECT_H
#define RECT_H

/*------------------------------------------------------------------------------
four int16s
------------------------------------------------------------------------------*/
struct Rect {
    Rect() {}
    Rect( int16_t x, int16_t y, int16_t w, int16_t h ) :
        x( x ), y( y ), w( w ), h( h ) {}
    Rect( Point const &topLeft, Point const &bottomRight ) :
        x( topLeft.x ), y( topLeft.y ), w( bottomRight.x - topLeft.x ), h( bottomRight.y - topLeft.y ) {}

    int16_t x;
    int16_t y;
    int16_t w;
    int16_t h;
};

#endif // RECT_H