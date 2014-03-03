#include "Precompiled.h"
#include "DepthStencilBuffer.h"
#include <D3D11.h>              // DirectX
#include "Device.h"             // device singleton

/*--------------------------------------------------------------------------
create the buffer
--------------------------------------------------------------------------*/
void DepthStencilBuffer::Create( uint32_t width, uint32_t height ) {
    // create the depth buffer texture
    D3D11_TEXTURE2D_DESC depthDesc;
    memset( &depthDesc, 0, sizeof( depthDesc ) );
    depthDesc.Width = width;
    depthDesc.Height = height;
    depthDesc.MipLevels = 1;
    depthDesc.ArraySize = 1;
    depthDesc.Format = DXGI_FORMAT_R24G8_TYPELESS; 
    depthDesc.SampleDesc.Count = 1;
    depthDesc.Usage = D3D11_USAGE_DEFAULT;
    depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
    DxVerify( g_dxDevice->CreateTexture2D( &depthDesc, NULL, &m_texture) );

    // create the depth view
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    memset( &dsvDesc, 0, sizeof( dsvDesc ) );
    dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Flags = 0;
    DxVerify( g_dxDevice->CreateDepthStencilView( m_texture, &dsvDesc, &m_depthStencilView ) );

    if( g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) {
        // create the depth view (read only)
        dsvDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
        DxVerify( g_dxDevice->CreateDepthStencilView( m_texture, &dsvDesc, &m_readOnlyDepthStencilView ) );
    }

    // create a shader resource view for the depth buffer
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    memset( &srvDesc, 0, sizeof( srvDesc ) );
    srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    DxVerify( g_dxDevice->CreateShaderResourceView( m_texture, &srvDesc, &m_depthShaderResourceView ) );

    // create a shader resource view for the stencil buffer
    srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    DxVerify( g_dxDevice->CreateShaderResourceView( m_texture, &srvDesc, &m_stencilShaderResourceView ) );
}

/*--------------------------------------------------------------------------
free the buffer
--------------------------------------------------------------------------*/
void DepthStencilBuffer::Destroy() {
    m_texture->Release();
    m_depthStencilView->Release();
    m_depthShaderResourceView->Release();
    if( g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ) {
        m_readOnlyDepthStencilView->Release();
    }
    m_stencilShaderResourceView->Release();
}