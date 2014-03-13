#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "VertexFormat.h"       // format enum
#include <map>                  // map container
#include "Texture2D.h"          // load flags

// forward declarations
class Mesh;
class Shader;
class Font;
struct FT_LibraryRec_;

/*------------------------------------------------------------------------------
storage container for all reference counted resources
------------------------------------------------------------------------------*/
class ResourceManager {
public:
    friend class Font;
    friend class Graphics;

    // allocate maps. must be initialized after graphics
    void Initialize();

    // free memory, check that all resources were released
    void Deinitialize();

    // get resources
    Mesh*      GetMesh       ( char const *file );
    Texture2D* GetTexture2D  ( char const *file, uint32_t loadFlags = kLoadDefault );
    Shader*    GetShader     ( char const *file, uint32_t shaderFlags );
    Font*      GetFont       ( char const *file, float size );

private:
    // FreeType font library
    inline FT_LibraryRec_* GetFontLibrary() { return m_library; }

    // unload a shader resource
    void UnloadShader( char const *file, uint32_t shaderFlags );

    // free all loaded resources
    void FreeAllResources();

private:
    typedef std::map< uint32_t, Mesh* > HashAndFormatToMeshMap;             // key is path Crc32
    HashAndFormatToMeshMap       *m_meshMap;                                // all loaded meshes

    typedef std::map< uint64_t, Texture2D* > HashAndFlagsToTexture2DMap;    // key is path Crc32 in the upper 32 bits, load flags in the lower 32 bits
    HashAndFlagsToTexture2DMap   *m_texture2DMap;                           // all loaded textures

    typedef std::map< uint64_t, Shader* > HashToShaderMap;                  // key is path Crc32 in the upper 32 bits, load flags in the lower 32 bits
    HashToShaderMap              *m_shaderMap;                              // all loaded shaders

    typedef std::map< uint64_t, Font* > HashAndSizeToFontMap;               // key is path Crc32 in upper 32 bits, size (float reinterpreted as uint32_t) in lower 32 bits
    HashAndSizeToFontMap         *m_fontMap;                                // all loaded fonts

    FT_LibraryRec_               *m_library;                                // handle to FreeType library 
};

// global instance
extern ResourceManager g_resourceManager;

#endif // RESOURCE_DATABASE_H