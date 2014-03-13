#include "Precompiled.h"
#include "VertexBuffer.h"
#include <D3D11.h>                  // DirectX
#include "Device.h"                 // device wrapper

/*--------------------------------------------------------------------------
create a buffer on the GPU
--------------------------------------------------------------------------*/
void VertexBuffer::Create( void *data, uint32_t size, uint32_t sizeOfVertex, bool dynamic ) {
    m_sizeOfVertex = sizeOfVertex;

    // create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
    memset( &bufferDesc, 0, sizeof( bufferDesc ) );
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	g_dxDevice->CreateBuffer( &bufferDesc, &initData, &m_buffer );

    #ifdef _DEBUG
        // Only works if device is created with the D3D10 or D3D11 debug layer, or when attached to PIX for Windows
        const char c_szName[] = "vertex buffer";
        m_buffer->SetPrivateData( WKPDID_D3DDebugObjectName, sizeof( c_szName ) - 1, c_szName );
    #endif
}

/*--------------------------------------------------------------------------
create a buffer on the GPU with no initial data
--------------------------------------------------------------------------*/
void VertexBuffer::CreateUninitialized( uint32_t size, bool dynamic ) {
    // create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
    memset( &bufferDesc, 0, sizeof( bufferDesc ) );
	bufferDesc.Usage = dynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = dynamic ? D3D11_CPU_ACCESS_WRITE : 0;
	bufferDesc.MiscFlags = 0;
	g_dxDevice->CreateBuffer( &bufferDesc, NULL, &m_buffer );
}

/*--------------------------------------------------------------------------
free the buffer
--------------------------------------------------------------------------*/
void VertexBuffer::Destroy() {
   m_buffer->Release();
}

/*------------------------------------------------------------------------------
maps the buffer using WRITE_DISCARD, sets the data, then unmaps
------------------------------------------------------------------------------*/
void VertexBuffer::SetData( void *data, unsigned size ) {
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    g_dxImmediateContext->Map( m_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
    memcpy( mappedResource.pData, data, size );
    g_dxImmediateContext->Unmap( m_buffer, 0 );
}