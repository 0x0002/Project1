#ifndef DEPTH_STENCIL_BUFFER_H
#define DEPTH_STENCIL_BUFFER_H

// forward declarations
struct ID3D11Texture2D;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;

/*------------------------------------------------------------------------------
container for the depth stencil buffer
------------------------------------------------------------------------------*/
struct DepthStencilBuffer {
public:
    // create the buffer
    void Create( uint32_t width, uint32_t height );

    // free the buffer
    void Destroy();

public:
    ID3D11Texture2D             *m_texture;                     // depth buffer
    ID3D11DepthStencilView      *m_depthStencilView;            // for binding/unbinding the depth stencil buffer
    ID3D11DepthStencilView      *m_readOnlyDepthStencilView;    // for binding/unbinding the depth stencil buffer (read only)

    ID3D11ShaderResourceView    *m_depthShaderResourceView;     // binding depth part of depth-stencil buffer to a shader as a texture

    ID3D11ShaderResourceView    *m_stencilShaderResourceView;   // binding stencil part of depth-stencil buffer to a shader as a texture
};

#endif // DEPTH_STENCIL_BUFFER_H