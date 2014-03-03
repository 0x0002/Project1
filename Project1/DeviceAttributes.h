#ifndef DEVICE_ATTRIBUTES_H
#define DEVICE_ATTRIBUTES_H

#include <D3D11.h>      // DXGI_MODE_DESC

enum {
    kMaxGraphicsCardNameLength = 260,
    kMaxDisplayModes = 200
};

/*------------------------------------------------------------------------------
container to hold data related to the video card
------------------------------------------------------------------------------*/
struct DeviceAttributes {
    char            m_graphicsCardName[ kMaxGraphicsCardNameLength ];   // name of the card
    DXGI_MODE_DESC  m_displayModes[ kMaxDisplayModes ];                 // video modes supported by the graphics gard/monitor
    uint32_t        m_displayModeCount;                                 // number of elements in m_displayModes array
    uint32_t        m_videoMemory;                                      // total amount of video memory (MB) 
};

#endif // DEVICE_ATTRIBUTES_H