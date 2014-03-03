#ifndef DEVICE_H
#define DEVICE_H

#include "SingletonBase.h"      // base class
#include "DeviceAttributes.h"   // some device stats. this includes d3d11.h
#include "Lock.h"               // critical section

/*------------------------------------------------------------------------------
container for the DirectX device
------------------------------------------------------------------------------*/
class Device : public SingletonBase< Device > {
public:
    // initialize the device and swap chain
    void Initialize();

    // cleanup the device
    void Deinitialize();

    // change the back buffer size
    void Resize( Point const &resolution );

    // search through the supported display modes to find one that matches this resolution
    DXGI_MODE_DESC const* GetMatchingDisplayMode( Point const &resolution ) const;

    // returns a supported resolution
    Point GetClosestSupportedResolution( Point const &resolution ) const;
    Point GetMaxSupportedResolution() const;
    Point GetHigherResolution( Point const &resolution ) const;
    Point GetLowerResolution( Point const &resolution ) const;

    // critical section access
    inline void AcquireLock() { m_lock.Acquire(); }
    inline void ReleaseLock() { m_lock.Release(); }

    // accessors
    inline ID3D11Device*            GetDevice()         const { return m_device;         }
    inline ID3D11DeviceContext*     GetDeviceContext()  const { return m_deviceContext;  }
    inline IDXGISwapChain*          GetSwapChain()      const { return m_swapChain;      }
    inline DeviceAttributes const*  GetAttributes()     const { return m_attributes;     }
    inline ID3D11RenderTargetView*  GetBackBufferView() const { return m_backBufferView; }
    inline D3D_FEATURE_LEVEL        GetFeatureLevel()   const { return m_featureLevel;   }

private:
    // create the device and swap chain at the same time
    void CreateDeviceAndSwapChain();

    // get refresh rate, amount of video memory, and video card name
    void GetDeviceAttributes();

private:
    ID3D11Device           *m_device;         // the device
    ID3D11DeviceContext    *m_deviceContext;  // immediate device context
    IDXGISwapChain         *m_swapChain;      // swap chain associated with the device
    DeviceAttributes       *m_attributes;     // list of supported display modes
    ID3D11RenderTargetView *m_backBufferView; // view of the back buffer
    D3D_FEATURE_LEVEL       m_featureLevel;   // feature level supported by the hardware
    Lock                    m_lock;           // critical section
};

// global instance
extern Device g_device;
#define g_dxDevice        g_device.GetDevice()          
#define g_dxImmediateContext g_device.GetDeviceContext()

#endif // DEVICE_H