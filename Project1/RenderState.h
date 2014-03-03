#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include <D3D11.h>              // DirectX

/*------------------------------------------------------------------------------
returns a depth stencil description with default values
------------------------------------------------------------------------------*/
inline D3D11_DEPTH_STENCIL_DESC DefaultDepthStencilDesc() {			
    D3D11_DEPTH_STENCILOP_DESC stencilopDesc;
    stencilopDesc.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    stencilopDesc.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    stencilopDesc.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    stencilopDesc.StencilFunc = D3D11_COMPARISON_ALWAYS;

    D3D11_DEPTH_STENCIL_DESC depthDesc;
    depthDesc.DepthEnable = TRUE;
    depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthDesc.StencilEnable = FALSE;
    depthDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
    depthDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
    depthDesc.FrontFace = stencilopDesc;
    depthDesc.BackFace = stencilopDesc;
    return depthDesc;
}

/*------------------------------------------------------------------------------
returns a rasterizer description with default values
------------------------------------------------------------------------------*/
inline D3D11_RASTERIZER_DESC DefaultRasterizerDesc() {
    D3D11_RASTERIZER_DESC rasterizerDesc;
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;
    rasterizerDesc.FrontCounterClockwise = FALSE;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.ScissorEnable = FALSE;
    rasterizerDesc.MultisampleEnable = FALSE;
    rasterizerDesc.AntialiasedLineEnable = FALSE;
    return rasterizerDesc;
}

/*------------------------------------------------------------------------------
returns a blend description with default values
------------------------------------------------------------------------------*/
inline D3D11_BLEND_DESC DefaultBlendDesc() {
    D3D11_BLEND_DESC blendDesc;
    blendDesc.AlphaToCoverageEnable = FALSE;
    blendDesc.IndependentBlendEnable = FALSE;

    D3D11_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc;
    renderTargetBlendDesc.BlendEnable = FALSE;
    renderTargetBlendDesc.SrcBlend = D3D11_BLEND_ONE;
    renderTargetBlendDesc.DestBlend = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
    renderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
    renderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
    renderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

    blendDesc.RenderTarget[ 0 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 1 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 2 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 3 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 4 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 5 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 6 ] = renderTargetBlendDesc;
    blendDesc.RenderTarget[ 7 ] = renderTargetBlendDesc;
    return blendDesc;
}

/*------------------------------------------------------------------------------
returns a viewport description with default values
------------------------------------------------------------------------------*/
inline D3D11_VIEWPORT DefaultViewport() {
    D3D11_VIEWPORT viewport;
    viewport.Width = 0;
    viewport.Height = 0;
    viewport.MinDepth = 0;
    viewport.MaxDepth = 1;
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    return viewport;
}

#endif // RENDER_STATE_H