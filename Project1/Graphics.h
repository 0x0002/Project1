#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SingletonBase.h"              // base class
#include "Lock.h"                       // 

// forward declarations
struct DepthStencilBuffer;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;

/*------------------------------------------------------------------------------
does all rendering
------------------------------------------------------------------------------*/
class Graphics : public SingletonBase< Graphics > {
public:
    // initialize DirectX
    void Initialize();

    // uninitialize DirectX
    void PreDeinitialize();
    void Deinitialize();

    // returns a supported resolution
    Point GetClosestSupportedResolution( Point const &resolution ) const;
    Point GetMaxSupportedResolution() const;
    Point GetHigherResolution( Point const &resolution ) const;
    Point GetLowerResolution( Point const &resolution ) const;

    // render the scene
    void Render( float dt );

    // resize all buffers and set the viewport
    void Resize( Point const &resolution );

    // critical section access
    inline void AcquireLock() { m_lock.Acquire(); }
    inline void ReleaseLock() { m_lock.Release(); }

private:
    // set the viewport using the current screen resolution
    void SetViewport();

    Lock m_lock;                // must acquire this before drawing
};

// global instance
extern Graphics g_graphics;

#endif // GRAPHICS_H