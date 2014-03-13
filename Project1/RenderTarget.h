#ifndef RENDER_TARGET_H
#define RENDER_TARGET_H

// forward declarations
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11UnorderedAccessView;

/*------------------------------------------------------------------------------
container for a render target
------------------------------------------------------------------------------*/
struct RenderTarget {
public:
    //RenderTarget() : m_texture( nullptr ), m_shaderResourceView( nullptr ), m_renderTargetView( nullptr ), m_renderTargetUAV( nullptr ) {}

    // create the render target
    void Create( int32_t dxgiFormat, uint32_t width, uint32_t height );

    // free the render target
    void Destroy();

public:
    ID3D11Texture2D             *m_texture;             // render target's texture
    ID3D11ShaderResourceView    *m_shaderResourceView;  // lets shaders read from this texture
    ID3D11RenderTargetView      *m_renderTargetView;    // for rendering to this texture
    ID3D11UnorderedAccessView   *m_renderTargetUAV;     // UAV - to use as output for compute shader
};

#endif // RENDER_TARGET_H