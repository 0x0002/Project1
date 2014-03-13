#include "Precompiled.h"
#include "Graphics.h"
#include "ResourceManager.h"
#include "Device.h"
#include "Camera.h"
#include "GameWindow.h"

#include <D3D11.h>                  // DirectX
#include <D3DCommon.h>              // 
#undef DrawText
#undef near
#undef far

#include "Shader.h"
#include "RenderState.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "DepthStencilBuffer.h"
#include "RenderTarget.h"

// global instance
Graphics g_graphics;

/*------------------------------------------------------------------------------
initialize DirectX
------------------------------------------------------------------------------*/
void Graphics::Initialize() {
    // initialize some singletons
    g_device.Initialize();
    g_camera.Initialize();
    g_camera.SetView( Vector3( 0, 0, 10 ), Vector3( 0, 0, 0 ), Vector3( 0, 1, 0 ) );

    // allocate stuff
    m_staticModels = (StaticModelData*)PermanentAllocate( sizeof( StaticModelData ) * kMaxStaticModels );
    m_staticModelCount = 0;

    // depth stencil buffer
    m_depthStencilBuffer = (DepthStencilBuffer*)PermanentAllocate( sizeof( DepthStencilBuffer ) );
    m_lightDepthStencilBuffer = (DepthStencilBuffer*)PermanentAllocate( sizeof( DepthStencilBuffer ) );

    // render targets
    m_positionTarget = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );
    m_normalTarget   = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );
    m_albedoTarget   = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );

    m_lightPositionTarget = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );
    m_lightNormalTarget   = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );
    m_lightAlbedoTarget   = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );

    m_finalTarget = (RenderTarget*)PermanentAllocate( sizeof( RenderTarget ) );

    // instance buffer
    m_instanceBuffer = (VertexBuffer*)PermanentAllocate( sizeof( VertexBuffer ) );
    m_instanceBuffer->CreateUninitialized( kInstanceBufferSize, true );

    CreateDepthStencilBuffer();
    CreateRenderTargets();
    CreateConstantBuffers();
    CreateSamplers();
    CreateRenderStates();
}

/*------------------------------------------------------------------------------
initialize DirectX
------------------------------------------------------------------------------*/
void Graphics::PostInitialize() {
    LoadShaders();
}

/*------------------------------------------------------------------------------
return to windowed mode
------------------------------------------------------------------------------*/
void Graphics::PreDeinitialize() {
    // if we are in fullscreen mode, we immediately receive a WM_SIZE message
    g_device.GetSwapChain()->SetFullscreenState( false, NULL );
}

/*------------------------------------------------------------------------------
deinitialize DirectX
------------------------------------------------------------------------------*/
void Graphics::Deinitialize() {
    DestroyDepthScentilBuffer();
    DestroyRenderTargets();
    DestroyConstantBuffers();
    DestroySamplers();
    DestroyRenderStates();

    m_instanceBuffer->Destroy();

    //g_camera.Deinitialize();
    g_device.Deinitialize();
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::LoadShaders() {
    m_gBufferShader = g_resourceManager.GetShader( "GBuffer.hlsl",          kVS_PS );
    m_shadowShader  = g_resourceManager.GetShader( "Shadow.hlsl",           kVS_PS );
    m_finalShader   = g_resourceManager.GetShader( "DirectionalLight.hlsl", kPS );

    m_debugDepthShader      = g_resourceManager.GetShader( "DebugDepth.hlsl",      kPS );
    m_debugNormalsShader    = g_resourceManager.GetShader( "DebugNormals.hlsl",    kPS );
    m_debugDiffuseShader    = g_resourceManager.GetShader( "DebugDiffuse.hlsl",    kPS );
    m_debugLightDepthShader = g_resourceManager.GetShader( "DebugLightDepth.hlsl", kPS );
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::ReloadShaders() {
    g_resourceManager.UnloadShader( "GBuffer.hlsl",          kVS_PS );
    g_resourceManager.UnloadShader( "Shadow.hlsl",           kVS_PS );
    g_resourceManager.UnloadShader( "DirectionalLight.hlsl", kPS );

    g_resourceManager.UnloadShader( "DebugDepth.hlsl",      kPS );
    g_resourceManager.UnloadShader( "DebugNormals.hlsl",    kPS );
    g_resourceManager.UnloadShader( "DebugDiffuse.hlsl",    kPS );
    g_resourceManager.UnloadShader( "DebugLightDepth.hlsl", kPS );
    LoadShaders();
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::CreateDepthStencilBuffer() {
    Point resolution = g_gameWindow.GetResolution();
    m_depthStencilBuffer->Create( resolution.x, resolution.y );

    m_lightDepthStencilBuffer->Create( kShadowMapSize, kShadowMapSize );
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::DestroyDepthScentilBuffer() {
    m_depthStencilBuffer->Destroy();
    m_lightDepthStencilBuffer->Destroy();
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::CreateRenderTargets() {
    Point resolution = g_gameWindow.GetResolution();
    m_positionTarget->Create( DXGI_FORMAT_R16G16B16A16_FLOAT, resolution.x, resolution.y );
    m_normalTarget->Create  ( DXGI_FORMAT_R16G16B16A16_FLOAT, resolution.x, resolution.y );
    m_albedoTarget->Create  ( DXGI_FORMAT_R16G16B16A16_FLOAT, resolution.x, resolution.y );
    m_lightPositionTarget->Create( DXGI_FORMAT_R16G16B16A16_FLOAT, kShadowMapSize, kShadowMapSize );
    m_lightNormalTarget->Create  ( DXGI_FORMAT_R16G16B16A16_FLOAT, kShadowMapSize, kShadowMapSize );
    m_lightAlbedoTarget->Create  ( DXGI_FORMAT_R16G16B16A16_FLOAT, kShadowMapSize, kShadowMapSize );
    m_finalTarget->Create( DXGI_FORMAT_R16G16B16A16_FLOAT, resolution.x, resolution.y );
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::DestroyRenderTargets() {
    m_positionTarget->Destroy();
    m_normalTarget->Destroy();
    m_albedoTarget->Destroy();
    m_lightPositionTarget->Destroy();
    m_lightNormalTarget->Destroy();
    m_lightAlbedoTarget->Destroy();
    m_finalTarget->Destroy();
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::CreateConstantBuffers() {
    D3D11_BUFFER_DESC bufferDesc;
    memset( &bufferDesc, 0, sizeof( bufferDesc ) );
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;

#if 0
    bufferDesc.ByteWidth = sizeof( PerFrameCB );
    g_dxDevice->CreateBuffer( &bufferDesc, NULL, &m_perFrameCB );

    bufferDesc.ByteWidth = sizeof( LightCB );
    g_dxDevice->CreateBuffer( &bufferDesc, NULL, &m_lightCB );

    bufferDesc.ByteWidth = sizeof( ShadowCB );
    g_dxDevice->CreateBuffer( &bufferDesc, NULL, &m_shadowCB );
#endif
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::DestroyConstantBuffers() {
#if 0
    m_perFrameCB->Release();
    m_lightCB->Release();
    m_shadowCB->Release();
#endif
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::CreateSamplers() {
    D3D11_SAMPLER_DESC samplerLinearClampDesc;
    memset( &samplerLinearClampDesc, 0, sizeof( samplerLinearClampDesc ) );
    samplerLinearClampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerLinearClampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerLinearClampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerLinearClampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerLinearClampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerLinearClampDesc.MinLOD = 0;
    samplerLinearClampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    g_dxDevice->CreateSamplerState( &samplerLinearClampDesc, &m_samplerLinearClamp );

    D3D11_SAMPLER_DESC samplerAnisotropicClampDesc;
    memset( &samplerAnisotropicClampDesc, 0, sizeof( samplerAnisotropicClampDesc ) );
    samplerAnisotropicClampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerAnisotropicClampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerAnisotropicClampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerAnisotropicClampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerAnisotropicClampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerAnisotropicClampDesc.MinLOD = 0;
    samplerAnisotropicClampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    g_dxDevice->CreateSamplerState( &samplerAnisotropicClampDesc, &m_samplerAnisotropicClamp );

    D3D11_SAMPLER_DESC samplerPointClampDesc;
    memset( &samplerPointClampDesc, 0, sizeof( samplerPointClampDesc ) );
    samplerPointClampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
    samplerPointClampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerPointClampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerPointClampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samplerPointClampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerPointClampDesc.MinLOD = 0;
    samplerPointClampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    g_dxDevice->CreateSamplerState( &samplerPointClampDesc, &m_samplerPointClamp );

    D3D11_SAMPLER_DESC samplerLinearWrapDesc;
    memset( &samplerLinearWrapDesc, 0, sizeof( samplerLinearWrapDesc ) );
    samplerLinearWrapDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerLinearWrapDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerLinearWrapDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerLinearWrapDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerLinearWrapDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerLinearWrapDesc.MinLOD = 0;
    samplerLinearWrapDesc.MaxLOD = D3D11_FLOAT32_MAX;
    g_dxDevice->CreateSamplerState( &samplerLinearWrapDesc, &m_samplerLinearWrap );
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::DestroySamplers() {
    m_samplerLinearClamp->Release();
    m_samplerAnisotropicClamp->Release();
    m_samplerPointClamp->Release();
    m_samplerLinearWrap->Release();
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::CreateRenderStates() {
    // depth stencil states
    D3D11_DEPTH_STENCIL_DESC defaultDepthStencilDesc = DefaultDepthStencilDesc();
    g_dxDevice->CreateDepthStencilState( &defaultDepthStencilDesc, &m_defaultDepthStencilState );

    D3D11_DEPTH_STENCIL_DESC depthLessEqualDesc = DefaultDepthStencilDesc();
    depthLessEqualDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
    depthLessEqualDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
    g_dxDevice->CreateDepthStencilState( &depthLessEqualDesc, &m_depthLessEqual );

    D3D11_DEPTH_STENCIL_DESC depthDisableDesc = DefaultDepthStencilDesc();
    depthDisableDesc.DepthEnable = FALSE;                                  // depth test disabled
    depthDisableDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;         // depth write disabled
    DxVerify( g_dxDevice->CreateDepthStencilState( &depthDisableDesc, &m_depthDisable ) );

    // rasterizer states
    D3D11_RASTERIZER_DESC defaultRasterizerDesc = DefaultRasterizerDesc();
    g_dxDevice->CreateRasterizerState( &defaultRasterizerDesc, &m_defaultRasterizerState );

    D3D11_RASTERIZER_DESC cullFrontDesc = DefaultRasterizerDesc();
    cullFrontDesc.CullMode = D3D11_CULL_FRONT;
    DxVerify( g_dxDevice->CreateRasterizerState( &cullFrontDesc, &m_cullFront ) );

    D3D11_RASTERIZER_DESC cullNoneDesc = DefaultRasterizerDesc();
    cullNoneDesc.CullMode = D3D11_CULL_NONE;
    DxVerify( g_dxDevice->CreateRasterizerState( &cullNoneDesc, &m_cullNone ) );

    // blend states
    D3D11_BLEND_DESC defaultBlendDesc = DefaultBlendDesc();
    g_dxDevice->CreateBlendState( &defaultBlendDesc, &m_defaultBlendState );

    D3D11_BLEND_DESC additiveDesc = DefaultBlendDesc();
    additiveDesc.RenderTarget[ 0 ].BlendEnable = TRUE;
    additiveDesc.RenderTarget[ 0 ].DestBlend = D3D11_BLEND_ONE;
    additiveDesc.RenderTarget[ 0 ].DestBlendAlpha = D3D11_BLEND_ONE;
    DxVerify( g_dxDevice->CreateBlendState( &additiveDesc, &m_additive ) );
}

/*------------------------------------------------------------------------------

------------------------------------------------------------------------------*/
void Graphics::DestroyRenderStates() {
    // depth stencil states
    m_defaultDepthStencilState->Release();
    m_depthLessEqual->Release();
    m_depthDisable->Release();

    // rasterizer states
    m_defaultRasterizerState->Release();
    m_cullFront->Release();
    m_cullNone->Release();

    // blend states
    m_defaultBlendState->Release();
    m_additive->Release();
}

/*------------------------------------------------------------------------------
returns a supported resolution
------------------------------------------------------------------------------*/
Point Graphics::GetClosestSupportedResolution( Point const &resolution ) const {
    return g_device.GetClosestSupportedResolution( resolution );
}

/*------------------------------------------------------------------------------
returns a supported resolution
------------------------------------------------------------------------------*/
Point Graphics::GetMaxSupportedResolution() const {
    return g_device.GetMaxSupportedResolution();
}

/*------------------------------------------------------------------------------
returns a supported resolution
------------------------------------------------------------------------------*/
Point Graphics::GetHigherResolution( Point const &resolution ) const {
    return g_device.GetHigherResolution( resolution );
}

/*------------------------------------------------------------------------------
returns a supported resolution
------------------------------------------------------------------------------*/
Point Graphics::GetLowerResolution( Point const &resolution ) const {
    return g_device.GetLowerResolution( resolution );
}

/*------------------------------------------------------------------------------
render the scene
------------------------------------------------------------------------------*/
void Graphics::Render( float dt ) {
#if 0
    m_globalTime += dt;

    enum { kNumFrames = 120 };
    static float    dtHistory[ kNumFrames ];
    static uint32_t dtHistoryCount = 0;

    char text[ kTempTextLength ];

    if( m_drawFramerate ) {
        // draw fps -- do this before acquiring the lock
        float averageDt = 0;
        if( dtHistoryCount < kNumFrames ) dtHistory[ dtHistoryCount++ ] = dt;
        else {
            // shift values to the left
            for( uint32_t i = 1; i < kNumFrames; ++i ) dtHistory[ i - 1 ] = dtHistory[ i ];
            dtHistory[ kNumFrames - 1 ] = dt;
        }
        for( uint32_t i = 0; i < dtHistoryCount; ++i ) averageDt += dtHistory[ i ];
        averageDt /= dtHistoryCount;

        sprintf_s( text, "FPS: %03.0f", 1.0f / averageDt );
        Draw::Text2D( text, Point( 10, 10 ) );
        sprintf_s( text, "%06.2f ms", averageDt * 1000.0f );
        Draw::Text2D( text, Point( 10, 25 ) );

        // resolution
        Point resolution = g_gameWindow.GetResolution();
        sprintf_s( text, "resolution: %ix%i", resolution.x, resolution.y );
        //Draw::Text2D( text, Point( 10, 45 ) );
    }

    AcquireLock();

    g_device.AcquireLock();
    static float const clearColor [ 4 ] = { 100.0f / 256.0f, 149.0f / 256.0f, 237.0f / 256.0f, 1.0f };
    g_dxImmediateContext->ClearRenderTargetView( g_device.GetBackBufferView(), clearColor );
    g_dxImmediateContext->ClearDepthStencilView( m_depthStencilBuffer->m_depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0 ); 

    // reset all render states to default values
    g_dxImmediateContext->OMSetDepthStencilState( m_defaultDepthStencilState, 0 );
    g_dxImmediateContext->RSSetState( m_defaultRasterizerState );
    g_dxImmediateContext->OMSetBlendState( m_defaultBlendState, NULL, 0xffffffff );

    // render the scene
    g_deferredRenderer.Render(dt);

    // render UI
    ID3D11RenderTargetView* renderTargetViews[ 1 ] = { g_device.GetBackBufferView() };
    g_dxImmediateContext->OMSetRenderTargets( 1, renderTargetViews, m_depthStencilBuffer->m_depthStencilView );
    g_uiManager.Render();

    // reset buffers for the next frame
    m_staticModelCount  = 0;
    m_skinnedModelCount = 0;
    m_pointCount        = 0;
    m_pointLightCount   = 0;
    m_shockwaveCount    = 0;
    m_decalCount        = 0;

	// present
    static bool const vSync = false;
	g_device.GetSwapChain()->Present( vSync ? 1 : 0, 0 );
    g_device.ReleaseLock();

    ReleaseLock();
#endif
}

/*------------------------------------------------------------------------------
resize all buffers and set the viewport
------------------------------------------------------------------------------*/
void Graphics::Resize( Point const &resolution ) {
#if 0
    m_depthStencilBuffer->Destroy();
    m_depthStencilBuffer->Create( resolution.x, resolution.y );


    uint16_t width      = Max( (int16_t)4, g_gameWindow.GetResolution().x );
    uint16_t height     = Max( (int16_t)4, g_gameWindow.GetResolution().y );
    m_captureFrameRenderTarget->Destroy();
    m_captureFrameRenderTarget->Create(DXGI_FORMAT_R8G8B8A8_UNORM, width / 4, height / 4);

    g_device.Resize( resolution );
    g_deferredRenderer.Resize();
    SetViewport();
#endif
}

/*------------------------------------------------------------------------------
set the viewport using the current screen resolution
------------------------------------------------------------------------------*/
void Graphics::SetViewport() {
    Point resolution = g_gameWindow.GetResolution();
	D3D11_VIEWPORT viewport = DefaultViewport();
    viewport.Width  = (float)resolution.x;
	viewport.Height = (float)resolution.y;
	g_dxImmediateContext->RSSetViewports( 1, &viewport );
}