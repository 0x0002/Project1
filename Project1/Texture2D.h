#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

// forward declarations
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
class UIRenderer;

enum LoadFlags { 
    kCpuRead      = 0x01,
    kGenerateMips = 0x02,
        
    kLoadDefault = kGenerateMips
};

/*------------------------------------------------------------------------------
contains texture resource data
------------------------------------------------------------------------------*/
class Texture2D {
public:
    friend class ResourceManager;
    friend class Font;

    // Creates texture from shader resource view
    void CreateFromSRV( ID3D11ShaderResourceView *srv );

    // accessors
    inline ID3D11Texture2D* GetTexture() const { return m_texture; }
    inline ID3D11ShaderResourceView* GetShaderResourceView() const { return m_shaderResourceView; }
    Point GetSize();

private:
    // Load and Unload are intentionally private. all texture Loading/Unloading must go through the resource manager

    // load from a resource file
    // loadFlags are ignored for dds textures
    void Load( FilePath const &path, uint32_t loadFlags );

    // unload resources
    void Unload();

private:
    ID3D11Texture2D             *m_texture;             // the texture resource
    ID3D11ShaderResourceView    *m_shaderResourceView;  // texture view

#ifdef _DEBUG
    FilePath                     m_path;
    uint32_t                     m_loadFlags;
#endif
};

#endif // TEXTURE_2D_H