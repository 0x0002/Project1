#include "Precompiled.h"
#include "ResourceManager.h"
#if 0
#include "Mesh.h"               // meshes
#include "Shader.h"             // shaders
#include "Font.h"               // fonts
#include "ft2build.h"           // FreeType
#include FT_FREETYPE_H          // this needs to be include after ft2build.h
#include "DirectoryManager.h"   // file directories
#include "Sphere.h"             // bounding spheres
#endif

// global instance
ResourceManager g_resourceManager;

// free and erase all items from a map
template< typename T1, typename T2 >
void DeleteAllItems( T1 *map, void ( T2::*Release )() );

/*------------------------------------------------------------------------------
allocate maps. must be initialized after graphics
------------------------------------------------------------------------------*/
void ResourceManager::Initialize() {
#if 0
    // load the font library
    // tutorial: http://www.freetype.org/freetype2/docs/tutorial/step1.html
    FT_Error error = FT_Init_FreeType( &m_library );
    Assert( !error, "FreeType initialization failed" );

    // use placement new to initialize vtable pointers
    m_fbxMap        = new ( PermanentAllocate( sizeof( HashToFbxModelMap          ) ) ) HashToFbxModelMap();
    m_meshMap       = new ( PermanentAllocate( sizeof( HashAndFormatToMeshMap     ) ) ) HashAndFormatToMeshMap();
    m_texture2DMap  = new ( PermanentAllocate( sizeof( HashAndFlagsToTexture2DMap ) ) ) HashAndFlagsToTexture2DMap();
    m_shaderMap     = new ( PermanentAllocate( sizeof( HashToShaderMap            ) ) ) HashToShaderMap();
    m_fontMap       = new ( PermanentAllocate( sizeof( HashAndSizeToFontMap       ) ) ) HashAndSizeToFontMap();
#endif
}

/*------------------------------------------------------------------------------
free memory, check that all resources were released
------------------------------------------------------------------------------*/
void ResourceManager::Deinitialize() {
#if 0
    FreeAllResources();

    // explicit destructor calls because these were created with placement new
    m_fbxMap->~HashToFbxModelMap();
    m_meshMap->~HashAndFormatToMeshMap();
    m_texture2DMap->~HashAndFlagsToTexture2DMap();
    m_shaderMap->~HashToShaderMap();
    m_fontMap->~HashAndSizeToFontMap();

    FT_Done_FreeType( m_library );
#endif
}

/*------------------------------------------------------------------------------
get a mesh resource
---------------------------------------------------------------------------*/
Mesh* ResourceManager::GetMesh( char const *file ) {
#if 0
    FilePath path = GetPath( file );
    uint32_t pathHash = Crc32( path.GetPath() );

    // check if this Mesh is already loaded
    HashAndFormatToMeshMap::iterator meshIterator = m_meshMap->find( pathHash );
    if( meshIterator != m_meshMap->end() ) {
        // the Mesh is already loaded
        meshIterator->second->AddReference();

        return meshIterator->second;
    }

    // this entry isn't in the map
    // check if the source FbxMesh is loaded
    FbxModel *fbxModel = NULL;
    HashToFbxModelMap::iterator fbxModelIterator = m_fbxMap->find( pathHash );
    if( fbxModelIterator == m_fbxMap->end() ) {
        // FbxMesh needs to be loaded
        fbxModel = (FbxModel*)HeapAllocate( sizeof( FbxModel ) );
        ( *m_fbxMap )[ pathHash ] = fbxModel;

        fbxModel->Load( path );
    }
    else {
        // Fbx is already loaded
        fbxModel = fbxModelIterator->second;
    }

    // create the Mesh and add it to the map. placement new to initialize base class
    Mesh *mesh = new ( HeapAllocate( sizeof( Mesh ) ) ) Mesh();
    ( *m_meshMap )[ pathHash ] = mesh;

    FbxMesh *fbxMesh = fbxModel->m_mesh;
    void     *vertexData       = fbxMesh->GetVertexData();
    uint32_t  sizeOfVertexData = fbxMesh->GetVertexDataSize();
    void     *indexData        = fbxMesh->GetIndexData();
    uint32_t  sizeOfIndexData  = fbxMesh->GetIndexDataSize();
    uint32_t  indexCount       = fbxMesh->GetIndexCount();
    uint32_t  vertexFormat     = fbxMesh->GetVertexFormat();
    mesh->Create( vertexData, sizeOfVertexData, indexData, sizeOfIndexData, indexCount, vertexFormat );
    mesh->AddReference();
    return mesh;
#endif
    return NULL;
}

/*------------------------------------------------------------------------------
get a texture resource
------------------------------------------------------------------------------*/
Texture2D* ResourceManager::GetTexture2D( char const *file, uint32_t loadFlags ) {
#if 0
    FilePath path = GetPath( file );
    unsigned pathHash = Crc32( path.GetPath() );
    uint64_t key = ( ( (uint64_t)pathHash ) << 32 ) | loadFlags;

    // check if this texture is already loaded
    HashAndFlagsToTexture2DMap::iterator textureIterator = m_texture2DMap->find( key );
    if( textureIterator != m_texture2DMap->end() ) {
        // the texture is already loaded
        textureIterator->second->AddReference();

        return textureIterator->second;
    }

    // load the texture and add it to the map. placement new to initialize base class
    Texture2D *texture = new ( HeapAllocate( sizeof( Texture2D ) ) ) Texture2D();
    ( *m_texture2DMap )[ key ] = texture;

    texture->Load( path, loadFlags );
    texture->AddReference();
    return texture;
#endif
    return NULL;
}
        
/*------------------------------------------------------------------------------
get a shader resource
------------------------------------------------------------------------------*/
Shader* ResourceManager::GetShader( char const *file, uint32_t shaderFlags ) {
#if 0
    FilePath path = GetPath( file );
    uint32_t pathHash = Crc32( path.GetPath() );
    uint64_t key = ( ( (uint64_t)pathHash ) << 32 ) | shaderFlags;

    // check if this shader is already loaded
    HashToShaderMap::iterator shaderIterator = m_shaderMap->find( key );
    if( shaderIterator != m_shaderMap->end() ) {
        // the shader is already loaded
        shaderIterator->second->AddReference();

        return shaderIterator->second;
    }

    // load the shader and add it to the map. placement new to initialize base class
    Shader *shader = new ( HeapAllocate( sizeof( Shader ) ) ) Shader();
    ( *m_shaderMap )[ key ] = shader;

    Todo( "keying on path/flags is not ideal because shaders could be loaded multiple times" )
    shader->Load( path, shaderFlags );
    shader->AddReference();
    return shader;
#endif
    return NULL;
}

/*------------------------------------------------------------------------------
get a font resource
------------------------------------------------------------------------------*/
Font* ResourceManager::GetFont( char const *file, float size ) {
#if 0
    FilePath path = GetPath( file );
    uint32_t pathHash = Crc32( path.GetPath() );
    uint64_t key = ( ( (uint64_t)pathHash ) << 32 ) | *(uint32_t*)&size;

    // check if this font is already loaded
    HashAndSizeToFontMap::iterator fontIterator = m_fontMap->find( key );
    if( fontIterator != m_fontMap->end() ) {
        // the font is already loaded
        fontIterator->second->AddReference();

        return fontIterator->second;
    }

    // load the font and add it to the map. placement new to initialize base class
    Font *font = new ( HeapAllocate( sizeof( Font ) ) ) Font();
    ( *m_fontMap )[ key ] = font;

    font->Load( path, size );
    font->AddReference();
    return font;
#endif
    return NULL;
}

/*------------------------------------------------------------------------------
unload a shader resource
------------------------------------------------------------------------------*/
void ResourceManager::UnloadShader( char const *file, uint32_t shaderFlags ) {
#if 0
    FilePath path = GetPath( file );
    uint32_t pathHash = Crc32( path.GetPath() );
    uint64_t key = ( ( (uint64_t)pathHash ) << 32 ) | shaderFlags;

    // check if this shader is already loaded
    HashToShaderMap::iterator shaderIterator = m_shaderMap->find( key );
    if( shaderIterator != m_shaderMap->end() ) {
        Shader *shader = shaderIterator->second;
        shader->Unload();
        shader->~Shader();
        HeapFree( shader );
        m_shaderMap->erase( shaderIterator );
    }
#endif
}

/*------------------------------------------------------------------------------
free all loaded resources
------------------------------------------------------------------------------*/
void ResourceManager::FreeAllResources() {
#if 0
    DeleteAllItems( m_fbxMap,       &FbxModel::Unload  );
    DeleteAllItems( m_meshMap,      &Mesh::Destroy     );
    DeleteAllItems( m_texture2DMap, &Texture2D::Unload );
    DeleteAllItems( m_shaderMap,    &Shader::Unload    );
    DeleteAllItems( m_fontMap,      &Font::Unload      );
#endif
}

/*------------------------------------------------------------------------------
free and erase all items from a map
------------------------------------------------------------------------------*/
template< typename T1, typename T2 >
void DeleteAllItems( T1 *map, void ( T2::*Release )() ) {
    T1::iterator end = map->end();
    for( T1::iterator i = map->begin(); i != end; ++i ) {
        CallMemberFn( *i->second, Release )();
        // explicit destructor call because placement new was used to allocate
        i->second->~T2();
        HeapFree( i->second );
    }
}