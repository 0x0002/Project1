#include "Precompiled.h"
#include "IndexBuffer.h"
#include <D3D11.h>                  // DirectX
#include "Device.h"                 // device wrapper

/*--------------------------------------------------------------------------
create a buffer on the GPU
--------------------------------------------------------------------------*/
void IndexBuffer::Create( void *data, uint32_t size, uint32_t indexCount ) { 
    m_indexCount = indexCount;

	// create the index buffer
	D3D11_BUFFER_DESC bufferDesc;
    memset( &bufferDesc, 0, sizeof( bufferDesc ) );
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = size;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = data;
	g_dxDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer);

    #ifdef _DEBUG
        // Only works if device is created with the D3D10 or D3D11 debug layer, or when attached to PIX for Windows
        const char c_szName[] = "index buffer";
        m_buffer->SetPrivateData( WKPDID_D3DDebugObjectName, sizeof( c_szName ) - 1, c_szName );
    #endif
}

/*--------------------------------------------------------------------------
free the buffer
--------------------------------------------------------------------------*/
void IndexBuffer::Destroy() {
    m_buffer->Release();
}