#include "Precompiled.h"
#include "Graphics.h"
#include "Device.h"
#include "GameWindow.h"

#include <D3D11.h>                  // DirectX
#include <D3DCommon.h>              // 
#undef DrawText

#include "RenderState.h"

// global instance
Graphics g_graphics;

/*------------------------------------------------------------------------------
initialize DirectX
------------------------------------------------------------------------------*/
void Graphics::Initialize() {
    // initialize some singletons
    g_device.Initialize();
    //g_camera.Initialize();

    SetViewport();
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
    //g_camera.Deinitialize();
    g_device.Deinitialize();
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