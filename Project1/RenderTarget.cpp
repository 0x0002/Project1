#include "Precompiled.h"
#include "RenderTarget.h"
#include <D3D11.h>              // DirectX
#include "Device.h"             // device container

/*------------------------------------------------------------------------------
create the render target
------------------------------------------------------------------------------*/
void RenderTarget::Create( int32_t dxgiFormat, uint32_t width, uint32_t height ) {
    // create the texture
    D3D11_TEXTURE2D_DESC textureDesc;
    memset( &textureDesc, 0, sizeof( textureDesc ) );
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.CPUAccessFlags = 0;
    if( g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 )
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
    else
        textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.Format = (DXGI_FORMAT)dxgiFormat;

    DxVerify( g_dxDevice->CreateTexture2D( &textureDesc, NULL, &m_texture ) );

    // create the texture view
    D3D11_SHADER_RESOURCE_VIEW_DESC textureViewDesc;
    memset( &textureViewDesc, 0, sizeof( textureViewDesc ) );
    textureViewDesc.Format = textureDesc.Format;
    textureViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    textureViewDesc.Texture2D.MipLevels = 1;

    DxVerify( g_dxDevice->CreateShaderResourceView( m_texture, &textureViewDesc, &m_shaderResourceView ) );

    // create the render target view
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
    memset( &renderTargetDesc, 0, sizeof( renderTargetDesc ) );
    renderTargetDesc.Format = textureDesc.Format;
    renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

    DxVerify( g_dxDevice->CreateRenderTargetView( m_texture, &renderTargetDesc, &m_renderTargetView ) );

    if( g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) {
        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        memset( &uavDesc, 0, sizeof( uavDesc ) );
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
        uavDesc.Format = textureDesc.Format;      // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
        uavDesc.Texture2D.MipSlice = 0;
        DxVerify( g_dxDevice->CreateUnorderedAccessView( m_texture, &uavDesc, &m_renderTargetUAV ) );
    }
}

/*------------------------------------------------------------------------------
free the render target
------------------------------------------------------------------------------*/
void RenderTarget::Destroy() {
    m_texture->Release();
    m_shaderResourceView->Release();
    m_renderTargetView->Release();
    if( g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) m_renderTargetUAV->Release();
}