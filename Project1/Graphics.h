#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "SingletonBase.h"  // base class
#include "Texture2D.h"      //

// forward declarations
struct DepthStencilBuffer;
struct ID3D11DepthStencilState;
struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11SamplerState;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct RenderTarget;
struct VertexBuffer;
struct Mesh;
struct Shader;

enum {
    kInstanceBufferSize = 1024 * 16,
    kShadowMapSize = 512,
};

/*------------------------------------------------------------------------------
does all rendering
------------------------------------------------------------------------------*/
class Graphics : public SingletonBase< Graphics > {
public:
    // initialize DirectX
    void Initialize();
    void PostInitialize();

    // uninitialize DirectX
    void PreDeinitialize();
    void Deinitialize();

    // create/destroy stuff
    void LoadShaders();
    void ReloadShaders();

    void CreateDepthStencilBuffer();
    void DestroyDepthScentilBuffer();

    void CreateRenderTargets();
    void DestroyRenderTargets();

    void CreateConstantBuffers();
    void DestroyConstantBuffers();

    void CreateSamplers();
    void DestroySamplers();

    void CreateRenderStates();
    void DestroyRenderStates();

    // returns a supported resolution
    Point GetClosestSupportedResolution( Point const &resolution ) const;
    Point GetMaxSupportedResolution() const;
    Point GetHigherResolution( Point const &resolution ) const;
    Point GetLowerResolution( Point const &resolution ) const;

    // render the scene
    void Render( float dt );

    // resize all buffers and set the viewport
    void Resize( Point const &resolution );

private:
    // render
    void RenderInitialize();
    void RenderGBuffer();
    void RenderShadow();
    void RenderSSAO();
    void RenderFinal();

    // set the viewport using the current screen resolution
    void SetViewport();

    enum { 
        kMaxStaticModels  = 1024,      // Static models max count
    };

    struct StaticModelData {
        Mesh      const *m_mesh;               // render this mesh
        Matrix           m_transform;          // and using this transform (object space to world space)
        Vector4          m_color;              //
    };

    StaticModelData             *m_staticModels;        
    uint32_t                     m_staticModelCount;

    VertexBuffer                *m_instanceBuffer;

    DepthStencilBuffer          *m_depthStencilBuffer;
    DepthStencilBuffer          *m_lightDepthStencilBuffer;

    RenderTarget                *m_positionTarget;
    RenderTarget                *m_normalTarget;
    RenderTarget                *m_albedoTarget;
    RenderTarget                *m_lightPositionTarget;
    RenderTarget                *m_lightNormalTarget;
    RenderTarget                *m_lightAlbedoTarget;
    RenderTarget                *m_finalTarget;

    ID3D11Buffer                *m_perFrameCB;
    ID3D11Buffer                *m_lightCB;
    ID3D11Buffer                *m_shadowCB;

    ID3D11SamplerState          *m_samplerLinearClamp;
    ID3D11SamplerState          *m_samplerAnisotropicClamp;
    ID3D11SamplerState          *m_samplerPointClamp;
    ID3D11SamplerState          *m_samplerLinearWrap;

    ID3D11DepthStencilState     *m_defaultDepthStencilState;
    ID3D11DepthStencilState     *m_depthLessEqual;
    ID3D11DepthStencilState     *m_depthDisable;

    ID3D11RasterizerState       *m_defaultRasterizerState;
    ID3D11RasterizerState       *m_cullFront;
    ID3D11RasterizerState       *m_cullNone;

    ID3D11BlendState            *m_defaultBlendState;
    ID3D11BlendState            *m_additive;

    Shader                      *m_gBufferShader;
    Shader                      *m_shadowShader;
    Shader                      *m_finalShader;

    Shader                      *m_debugDepthShader;
    Shader                      *m_debugNormalsShader;
    Shader                      *m_debugDiffuseShader;
    Shader                      *m_debugLightDepthShader;

    Texture2D                    m_texture;
};

// global instance
extern Graphics g_graphics;

#endif // GRAPHICS_H