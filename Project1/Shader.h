#ifndef SHADER_H
#define SHADER_H

enum ShaderFlags {
    kVS       = 0x01,
    kGS       = 0x02,       
    kPS       = 0x04,
    kHS       = 0x08,
    kDS       = 0x10,
    kCS       = 0x20,

    kVS_PS       = kVS                   | kPS,
    kVS_GS       = kVS | kGS                  , 
    kGS_PS       =       kGS             | kPS,
    kVS_GS_PS    = kVS | kGS             | kPS,
    kVS_HS_DS    = kVS |       kHS | kDS,
    kVS_HS_DS_PS = kVS |       kHS | kDS | kPS
};

// forward declarations
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11GeometryShader;
struct ID3D11HullShader;
struct ID3D11DomainShader;
struct ID3D11ComputeShader;
struct ID3D11InputLayout;
struct ID3D10Blob;

/*------------------------------------------------------------------------------
Shader wrapper
------------------------------------------------------------------------------*/
class Shader {
public:
    friend class ResourceManager;

private:
    // Load and Unload are intentionally private. all texture Loading/Unloading must go through the resource manager

    // load a shader from a file
    void Load( FilePath const &path, uint32_t shaderFlags );

    // free the shader
    void Unload();

    // Create input layout for vertex shader
    void CreateInputLayoutDescFromVertexShaderSignature( ID3D10Blob* pShaderBlob );

    // Helper for compiling shaders with D3DX11
    void CompileShaderFromFile( char const *fileName, char const *entryPoint, char const *shaderModel, ID3D10Blob **blob );

    struct ReflectionData;  // forward declaration

    // get a shader's instruction count and a formatted c-string of its assembly instructions 
    void Reflect( ReflectionData *reflectionData, ID3D10Blob *blob );

public:
    // accessors
    inline ID3D11VertexShader*    GetVertexShader()    { return m_vertexShader;   }
    inline ID3D11PixelShader*     GetPixelShader()     { return m_pixelShader;    }
    inline ID3D11GeometryShader*  GetGeometryShader()  { return m_geometryShader; }
    inline ID3D11HullShader*      GetHullShader()      { return m_hullShader;     }
    inline ID3D11DomainShader*    GetDomainShader()    { return m_domainShader;   }
    inline ID3D11ComputeShader*   GetComputeShader()   { return m_computeShader;  }
    inline ID3D11InputLayout*     GetInputLayout()     { return m_inputLayout;    }

    inline uint32_t GetVertexShaderInstructionCount()   const { return m_vertexShaderReflection.m_instructionCount;   }
    inline uint32_t GetGeometryShaderInstructionCount() const { return m_geometryShaderReflection.m_instructionCount; }
    inline uint32_t GetPixelShaderInstructionCount()    const { return m_pixelShaderReflection.m_instructionCount;    }
    inline char const* GetVertexShaderDisassembly()     const { return m_vertexShaderReflection.m_disassembly;        }
    inline char const* GetGeometryShaderDisassembly()   const { return m_geometryShaderReflection.m_disassembly;      }
    inline char const* GetPixelShaderDisassembly()      const { return m_pixelShaderReflection.m_disassembly;         }

private:
    // contains compiled shader instruction information
    struct ReflectionData {
        ReflectionData() { SetToDefaults(); }
        inline void SetToDefaults() { m_instructionCount = 0; m_disassembly[ 0 ] = '\0'; }

        enum { kDisassemblyBufferSize = 128000 };
        char        m_disassembly[ kDisassemblyBufferSize ];  // formatted c-string containing assembly instructions
        unsigned    m_instructionCount;                       // number of gpu instructions
    };

    ReflectionData         m_vertexShaderReflection;       // information about the vertex shader
    ReflectionData         m_geometryShaderReflection;     // information about the geometry shader
    ReflectionData         m_pixelShaderReflection;        // information about the pixel shader

    ID3D11VertexShader*    m_vertexShader;    // vertex shader
    ID3D11PixelShader*     m_pixelShader;     // pixel shader
    ID3D11GeometryShader*  m_geometryShader;  // geometry shader
    ID3D11HullShader*      m_hullShader;      // hull shader
    ID3D11DomainShader*    m_domainShader;    // domain shader
    ID3D11ComputeShader*   m_computeShader;   // compute shader
    ID3D11InputLayout*     m_inputLayout;     // vertex shader's input layout   
};

#endif //SHADER_H