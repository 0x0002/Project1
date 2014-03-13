#include "Precompiled.h"
#include "Device.h"
#include <D3D11.h>                  // DirectX
#include "GameWindow.h"             // handle to the window
#include "DepthStencilBuffer.h"     // depth stencil buffer wrapper

// global instance
Device g_device;

/*--------------------------------------------------------------------------
initialize the device and swap chain
--------------------------------------------------------------------------*/
void Device::Initialize() {
    m_attributes = (DeviceAttributes*)PermanentAllocate( sizeof( DeviceAttributes ) );
    CreateDeviceAndSwapChain();
}

/*--------------------------------------------------------------------------
cleanup the device
--------------------------------------------------------------------------*/
void Device::Deinitialize() {
	// return to windowed mode before freeing
    m_backBufferView->Release();

	m_deviceContext->ClearState();
    m_deviceContext->Flush();

    m_swapChain->Release();
    m_deviceContext->Release();
    m_device->Release();
}

/*--------------------------------------------------------------------------
change the back buffer size
--------------------------------------------------------------------------*/
void Device::Resize( Point const &resolution ) {
    m_backBufferView->Release();

    DxVerify( m_swapChain->ResizeBuffers( 0, resolution.x, resolution.y, DXGI_FORMAT_UNKNOWN, 0 ) );

    // get the swap chain's resized back buffer
	ID3D11Texture2D *backBuffer = NULL;
	DxVerify( g_device.GetSwapChain()->GetBuffer( 0,
									              __uuidof( ID3D11Texture2D ),
									              (void**)&backBuffer ) );

    // create a render target view using the back buffer
	DxVerify( m_device->CreateRenderTargetView( backBuffer,
									            NULL,
									            &m_backBufferView ) );
    backBuffer->Release();
}

/*--------------------------------------------------------------------------
search through the supported display modes to find one that matches this resolution
--------------------------------------------------------------------------*/
DXGI_MODE_DESC const* Device::GetMatchingDisplayMode( Point const &resolution ) const {
    uint32_t modeCount = m_attributes->m_displayModeCount;
    DXGI_MODE_DESC const *modeDesc = m_attributes->m_displayModes;
    for( uint32_t i = 0; i < modeCount; ++i, ++modeDesc ) {
        if( modeDesc->Width == (uint32_t)resolution.x && modeDesc->Height == (uint32_t)resolution.y ) return modeDesc;
    }
    return NULL;
}

/*--------------------------------------------------------------------------
returns a supported resolution
--------------------------------------------------------------------------*/
Point Device::GetClosestSupportedResolution( Point const &resolution ) const {
    uint32_t modeCount = m_attributes->m_displayModeCount;
    DXGI_MODE_DESC const *modeDesc = &m_attributes->m_displayModes[ modeCount - 1 ];
    uint32_t i = 0;
    for( ; i < modeCount; ++i, --modeDesc ) {
        if( modeDesc->Width <= (uint32_t)resolution.x && modeDesc->Height <= (uint32_t)resolution.y ) break;
    }
    if( i == modeCount ) modeDesc = &m_attributes->m_displayModes[ 0 ];
    return Point( (int16_t)modeDesc->Width, (int16_t)modeDesc->Height );
}

/*--------------------------------------------------------------------------
get the next highest supported resolution
--------------------------------------------------------------------------*/
Point Device::GetHigherResolution( Point const &resolution ) const {
    uint32_t modeCount = m_attributes->m_displayModeCount;
    DXGI_MODE_DESC const *modeDesc = GetMatchingDisplayMode( resolution );
    modeDesc = (DXGI_MODE_DESC const*)Min( modeDesc + 1, &m_attributes->m_displayModes[ modeCount - 1 ] );
    return Point( (int16_t)modeDesc->Width, (int16_t)modeDesc->Height );
}

/*--------------------------------------------------------------------------
get the next lowest supported resolution
--------------------------------------------------------------------------*/
Point Device::GetLowerResolution( Point const &resolution ) const {
    uint32_t modeCount = m_attributes->m_displayModeCount;
    DXGI_MODE_DESC const *modeDesc = GetMatchingDisplayMode( resolution );
    modeDesc = (DXGI_MODE_DESC const*)Max( modeDesc - 1, m_attributes->m_displayModes );
    return Point( (int16_t)modeDesc->Width, (int16_t)modeDesc->Height );
}

/*--------------------------------------------------------------------------
returns a supported resolution
--------------------------------------------------------------------------*/
Point Device::GetMaxSupportedResolution() const {
    DXGI_MODE_DESC const *modeDesc = &m_attributes->m_displayModes[ m_attributes->m_displayModeCount - 1 ];
    return Point( (int16_t)modeDesc->Width, (int16_t)modeDesc->Height );
}

/*--------------------------------------------------------------------------
create the device and swap chain at the same time
--------------------------------------------------------------------------*/
void Device::CreateDeviceAndSwapChain() {

    GetDeviceAttributes();

    UINT createDeviceFlags = 0;
	#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	// swap chain description
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
    memset( &swapChainDesc, 0, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount            = 2;
    swapChainDesc.BufferDesc.Width       = 1;
    swapChainDesc.BufferDesc.Height      = 1;
	swapChainDesc.BufferDesc.Format      = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate = m_attributes->m_displayModes[ m_attributes->m_displayModeCount - 1 ].RefreshRate;
	swapChainDesc.BufferUsage            = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow           = g_gameWindow.GetWindowHandle();
	swapChainDesc.SampleDesc.Count       = 1;
	swapChainDesc.SampleDesc.Quality     = 0;
	swapChainDesc.Windowed               = true;
	swapChainDesc.Flags                  = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.SwapEffect             = DXGI_SWAP_EFFECT_DISCARD;

	// create the device and swap chain at the same time
	DxVerify( D3D11CreateDeviceAndSwapChain( NULL,
											 D3D_DRIVER_TYPE_HARDWARE,
											 NULL,
											 createDeviceFlags,
                                             NULL,
                                             NULL,
											 D3D11_SDK_VERSION,
											 &swapChainDesc,
                                             &m_swapChain,
                                             &m_device,
                                             &m_featureLevel,
                                             &m_deviceContext ) ); 

	// disable alt+enter fullscreen
	IDXGIFactory *factory;
    DxVerify( m_swapChain->GetParent( __uuidof( IDXGIFactory ), (void**)&factory ) );
	DxVerify( factory->MakeWindowAssociation( g_gameWindow.GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER ) );
	factory->Release();

    // get the swap chain's back buffer
	ID3D11Texture2D *backBuffer = NULL;
	DxVerify( m_swapChain->GetBuffer( 0,
									  __uuidof( ID3D11Texture2D ),
									  (void**)&backBuffer ) );

    // create a render target view using the back buffer
	DxVerify( m_device->CreateRenderTargetView( backBuffer,
												NULL,
												&m_backBufferView ) );
    backBuffer->Release();
}

/*--------------------------------------------------------------------------
get refresh rate, amount of video memory, and video card name
--------------------------------------------------------------------------*/
void Device::GetDeviceAttributes() {
	// create a factory
	IDXGIFactory* factory = NULL;
	DxVerify( CreateDXGIFactory( __uuidof( IDXGIFactory ),
								 (void**)&factory ) );

	// get an adapter for the primary device
	IDXGIAdapter* adapter = NULL;
	DxVerify( factory->EnumAdapters( 0, &adapter ) );
		
	// make sure the device supports directx10
	LARGE_INTEGER version;
	DxVerify( adapter->CheckInterfaceSupport( __uuidof( ID3D10Device ), &version) );

	// get the adapter output, which is used to get a list
	// of supported display modes
	IDXGIOutput* adapterOutput = NULL;
	DxVerify( adapter->EnumOutputs( 0, &adapterOutput ) );

	// get the number of display modes that work with the
	// DXGI_FORMAT_R8G8B8A8_UNORM format
	uint32_t tempNumModes;
	DxVerify( adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM,
												 DXGI_ENUM_MODES_INTERLACED,
												 &tempNumModes,
												 NULL ) );

    Assert( tempNumModes <= kMaxDisplayModes, "display mode buffer overflow" );

	// actually get the display mode list
    DXGI_MODE_DESC tempDisplayModes[ kMaxDisplayModes ];
	DxVerify( adapterOutput->GetDisplayModeList( DXGI_FORMAT_R8G8B8A8_UNORM,
												 DXGI_ENUM_MODES_INTERLACED,
												 &tempNumModes,
												 tempDisplayModes ) );

    // copy display modes that are using DXGI_MODE_SCALING_UNSPECIFIED into the attributes struct
    // and copy only the display mode with the highest refresh rate for any given resolution if there are duplicates
    uint32_t finalNumModes = 0;
    DXGI_MODE_DESC *previouslyAddedMode = NULL;
    DXGI_MODE_DESC *displayMode = tempDisplayModes;
    for( uint32_t i = 0; i < tempNumModes; ++i, ++displayMode ) {
        if( displayMode->Scaling == DXGI_MODE_SCALING_UNSPECIFIED ) { // some modes will uave DXGI_MODE_SCALING_CENTERED
            if( previouslyAddedMode &&                                // don't add duplicate entries for a resolution
                previouslyAddedMode->Width  == displayMode->Width &&
                previouslyAddedMode->Height == displayMode->Height ) {
                // we're overwriting the duplicate mode with this new one, which has a higher refresh rate
                --finalNumModes; 
            }

            previouslyAddedMode = displayMode;
            m_attributes->m_displayModes[ finalNumModes++ ] = *displayMode;
        }
    }
    m_attributes->m_displayModeCount = finalNumModes;
		
	// get the adapter attributes
	DXGI_ADAPTER_DESC adapterDesc;
	DxVerify( adapter->GetDesc( &adapterDesc ) );
		
	// amount of available video memory
    m_attributes->m_videoMemory = (uint32_t)( adapterDesc.DedicatedVideoMemory / ( 1024 * 1024 ) );
		
	// name of the video card
	size_t numCharsConverted;
    wcstombs_s( &numCharsConverted, m_attributes->m_graphicsCardName, kMaxGraphicsCardNameLength, adapterDesc.Description, kMaxGraphicsCardNameLength );

    // cleanup
    adapterOutput->Release();
    adapter->Release();
    factory->Release();
}