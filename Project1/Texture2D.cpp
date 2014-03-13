#include "Precompiled.h"
#include "Texture2D.h"
#include <d3d11.h>              // DirectX
#include "Device.h"             // device singleton
#if 0
#include "DDSTextureLoader.h"   // texture loading
#include "WICTextureLoader.h"   // texture loading
#endif


/*------------------------------------------------------------------------------
Creates texture from shader resource view
------------------------------------------------------------------------------*/
void Texture2D::CreateFromSRV(ID3D11ShaderResourceView* srv) {
    m_shaderResourceView = srv;
    m_texture = NULL;
}

/*------------------------------------------------------------------------------
load from a resource file
loadFlags are ignored for dds textures
------------------------------------------------------------------------------*/
void Texture2D::Load( FilePath const &path, uint32_t loadFlags ) {
#if 0
    g_device.AcquireLock();

    // store the arguments to help when debugging
    m_path = path;
    m_loadFlags = loadFlags;

    m_texture = NULL;
    m_shaderResourceView = NULL;

    wchar_t texturePathW[ FilePath::kMaxPathLength ];
    size_t numCharsConverted;
	mbstowcs_s( &numCharsConverted, texturePathW, FilePath::kMaxPathLength, path.GetPath(), FilePath::kMaxPathLength );

	// Create Resource View
    ID3D11Resource *resource = NULL;
	if( Crc32( path.GetExtension() ) == 0xE6EAEAC8 ) { // Crc32(".dds")
		DxVerify( CreateDDSTextureFromFile( g_dxDevice, texturePathW, &resource, &m_shaderResourceView ) );
	}
	else {
        bool cpuread      = ( loadFlags & kCpuRead      ) != 0;
        bool generateMips = ( loadFlags & kGenerateMips ) != 0;
		DxVerify( CreateWICTextureFromFile( g_dxDevice, g_dxImmediateContext, texturePathW, &resource, &m_shaderResourceView, cpuread, generateMips ) );
	}

    // makes sure the resource was created as a 2d texture
    D3D11_RESOURCE_DIMENSION type;
    resource->GetType( &type );
    Assert( type == D3D11_RESOURCE_DIMENSION_TEXTURE2D, "texture isn't 2D" );

    // get the texture resource from the view
    m_texture = (ID3D11Texture2D*)resource;

    g_device.ReleaseLock();
#endif
}

/*------------------------------------------------------------------------------
unload resources
------------------------------------------------------------------------------*/
void Texture2D::Unload() {
#if 0
    g_device.AcquireLock();

    m_texture->Release();
    if( m_shaderResourceView ) m_shaderResourceView->Release(); // view isn't created for textures created with cpuread 

    g_device.ReleaseLock();
#endif
}

/*------------------------------------------------------------------------------
get the texture's size
------------------------------------------------------------------------------*/
Point Texture2D::GetSize() {
    D3D11_TEXTURE2D_DESC desc;
    m_texture->GetDesc( &desc );
    return Point( (int16_t)desc.Width, (int16_t)desc.Height );
}