#include "Precompiled.h"
#include "Shader.h"
#include "Device.h"             // device singleton
#include <d3d11.h>              // DirectX
#include <d3dCompiler.h>        // compiling effect files
#include <sstream>              // stringstream container

/*------------------------------------------------------------------------------
Load shader from a file
------------------------------------------------------------------------------*/
void Shader::Load( FilePath const &path, uint32_t shaderFlags ) {
    m_vertexShader   = NULL;
    m_pixelShader    = NULL;
    m_geometryShader = NULL;
    m_hullShader     = NULL;
    m_domainShader   = NULL;
    m_computeShader  = NULL;
    m_inputLayout    = NULL;

    char const *vsShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "vs_5_0" : "vs_4_0";
    char const *gsShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "gs_5_0" : "gs_4_0";
    char const *psShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "ps_5_0" : "ps_4_0";
    char const *hsShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "hs_5_0" : "hs_4_0";
    char const *dsShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "ds_5_0" : "ds_4_0";
    char const *csShaderModel = g_device.GetFeatureLevel() >= D3D_FEATURE_LEVEL_11_0 ? "cs_5_0" : "cs_4_0";

    if( shaderFlags & kVS ) {
        // Compile the vertex shader
        ID3D10Blob* pVSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "VS", vsShaderModel, &pVSBlob );

        // Create the vertex shader
        DxVerify( g_dxDevice->CreateVertexShader( pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &m_vertexShader ) );

        // Create input layout from Vertex Shader
        CreateInputLayoutDescFromVertexShaderSignature( pVSBlob );

        // Create reflection data
        Reflect( &m_vertexShaderReflection, pVSBlob );

        pVSBlob->Release();
    }

    if( shaderFlags & kPS ) {
        // Compile the pixel shader
        ID3D10Blob* pPSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "PS", psShaderModel, &pPSBlob );

        // Create the pixel shader
        DxVerify( g_dxDevice->CreatePixelShader( pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &m_pixelShader ) );

        // Create reflection data
        Reflect( &m_pixelShaderReflection, pPSBlob );

        pPSBlob->Release();
    }

    if( shaderFlags & kGS ) {
        // Compile the geometry shader
        ID3D10Blob* pGSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "GS", gsShaderModel, &pGSBlob );

        // Create the geometry shader
        DxVerify( g_dxDevice->CreateGeometryShader( pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &m_geometryShader ) );

        // Create reflection data
        Reflect( &m_geometryShaderReflection, pGSBlob );

        pGSBlob->Release();
    }

    if( shaderFlags & kHS ) {
        // Compile the hull shader
        ID3D10Blob* pHSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "HS", hsShaderModel, &pHSBlob );

        // Create the geometry shader
        DxVerify( g_dxDevice->CreateHullShader( pHSBlob->GetBufferPointer(), pHSBlob->GetBufferSize(), NULL, &m_hullShader ) );

        pHSBlob->Release();
    }

    if( shaderFlags & kDS ) {
        // Compile the domain shader
        ID3D10Blob* pDSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "DS", dsShaderModel, &pDSBlob );

        // Create the domain shader
        DxVerify( g_dxDevice->CreateDomainShader( pDSBlob->GetBufferPointer(), pDSBlob->GetBufferSize(), NULL, &m_domainShader ) );

        pDSBlob->Release();
    }

    if( shaderFlags & kCS ) {
        // Compile the domain shader
        ID3D10Blob* pCSBlob = NULL;
        CompileShaderFromFile( path.GetPath(), "CS",csShaderModel, &pCSBlob );

        // Create the domain shader
        DxVerify( g_dxDevice->CreateComputeShader( pCSBlob->GetBufferPointer(), pCSBlob->GetBufferSize(), NULL, &m_computeShader ) );

        pCSBlob->Release();
    }
}

/*------------------------------------------------------------------------------
Free the shader
------------------------------------------------------------------------------*/
void Shader::Unload() {
    if( m_vertexShader   ) m_vertexShader->Release();
    if( m_pixelShader    ) m_pixelShader->Release();
    if( m_geometryShader ) m_geometryShader->Release();
    if( m_hullShader     ) m_hullShader->Release();
    if( m_domainShader   ) m_domainShader->Release();
    if( m_computeShader  ) m_computeShader->Release();
    if( m_inputLayout    ) m_inputLayout->Release();
}

/*------------------------------------------------------------------------------
Helper for compiling shaders with D3DX11
------------------------------------------------------------------------------*/
void Shader::CompileShaderFromFile( char const *fileName, char const *entryPoint, char const *shaderModel, ID3D10Blob **blob ) {
    DWORD hlslFlags = D3D10_SHADER_WARNINGS_ARE_ERRORS | D3D10_SHADER_ENABLE_STRICTNESS;	// don't allow deprecated syntax

#ifdef _DEBUG
    hlslFlags |= D3D10_SHADER_SKIP_OPTIMIZATION | D3D10_SHADER_DEBUG;  // no optimization and debug symbols
#else
    hlslFlags |= D3D10_SHADER_OPTIMIZATION_LEVEL3;	// maximum optimization
#endif

    ID3D10Blob* compileErrors = NULL;
    HRESULT hr = D3DCompileFromFile( ToWchar( fileName ),
                                     NULL,
                                     NULL,
                                     entryPoint,
                                     shaderModel, 
                                     hlslFlags,
                                     0,
                                     blob,
                                     &compileErrors );
    // check for errors
    if( compileErrors ) {
        PrintLine( "\n--- Compilation errors in %s ---", fileName );
        PrintLine( "%s", (const char*)compileErrors->GetBufferPointer() );

        // shader compilation error. see output window for a clickable message with details
        TriggerBreakpoint();
    }
    else if( FAILED( hr ) ) {
        // some other error. probably can't locate the file
        AssertAlways( "Failed to load shader: %s", fileName );
    }
    if( compileErrors ) compileErrors->Release();
}

/*------------------------------------------------------------------------------
create an input layout and determine input formats
based on http://takinginitiative.net/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
------------------------------------------------------------------------------*/
void Shader::CreateInputLayoutDescFromVertexShaderSignature( ID3D10Blob* pShaderBlob ) {
    // lookup table for format types
    static DXGI_FORMAT const dxgiFloatFormats[] = {
        DXGI_FORMAT_R32_FLOAT,
        DXGI_FORMAT_R32G32_FLOAT,
        DXGI_FORMAT_R32G32B32_FLOAT,
        DXGI_FORMAT_R32G32B32A32_FLOAT
    };

    static DXGI_FORMAT const dxgiUintFormats[] = {
        DXGI_FORMAT_R32_UINT,
        DXGI_FORMAT_R32G32_UINT,
        DXGI_FORMAT_R32G32B32_UINT,
        DXGI_FORMAT_R32G32B32A32_UINT,
    };

    static DXGI_FORMAT const dxgiSintFormats[] = {
        DXGI_FORMAT_R32_SINT,
        DXGI_FORMAT_R32G32_SINT,
        DXGI_FORMAT_R32G32B32_SINT,
        DXGI_FORMAT_R32G32B32A32_SINT,
    };

    // Reflect shader info
    ID3D11ShaderReflection* pVertexShaderReflection = NULL; 
    DxVerify( D3DReflect( pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pVertexShaderReflection ) );

    // Get shader info
    D3D11_SHADER_DESC shaderDesc;
    pVertexShaderReflection->GetDesc( &shaderDesc );

    // Read input layout description from shader info
    enum { kMaxInputElements = 16 };
    D3D11_INPUT_ELEMENT_DESC inputElementDescs[ kMaxInputElements ];
    uint32_t inputElementCount = 0;

    D3D11_INPUT_ELEMENT_DESC *inputElementDesc = inputElementDescs;
    for( uint32_t i = 0; i < shaderDesc.InputParameters; ++i, ++inputElementDesc ) {
        D3D11_SIGNATURE_PARAMETER_DESC paramDesc;       
        pVertexShaderReflection->GetInputParameterDesc( i, &paramDesc );

        // instanced input elements should have a semantic prefixed with 'INSTANCED_' in the shader
        bool instanced = !std::string( paramDesc.SemanticName ).find( "INSTANCED_" );

        // fill out input element desc
        inputElementDesc->SemanticName          = paramDesc.SemanticName;      
        inputElementDesc->SemanticIndex         = paramDesc.SemanticIndex;
        inputElementDesc->InputSlot             = instanced ? 1 : 0;
        inputElementDesc->AlignedByteOffset     = D3D11_APPEND_ALIGNED_ELEMENT;
        inputElementDesc->InputSlotClass        = instanced ? D3D11_INPUT_PER_INSTANCE_DATA : D3D11_INPUT_PER_VERTEX_DATA;
        inputElementDesc->InstanceDataStepRate  = instanced ? 1 : 0;
         
        // determine DXGI format
        DXGI_FORMAT formatEnum = DXGI_FORMAT_UNKNOWN;

        // Map Mask to Format Index
        size_t formatIndex = 0;
        if (paramDesc.Mask == 1) formatIndex = 0;
        else if (paramDesc.Mask <= 3 ) formatIndex = 1;
        else if (paramDesc.Mask <= 7 ) formatIndex = 2;
        else if (paramDesc.Mask <= 15) formatIndex = 3;

        switch( paramDesc.ComponentType ) {
        case D3D_REGISTER_COMPONENT_FLOAT32: formatEnum = dxgiFloatFormats[ formatIndex ]; break;
        case D3D_REGISTER_COMPONENT_UINT32:  formatEnum = dxgiUintFormats [ formatIndex ]; break;
        case D3D_REGISTER_COMPONENT_SINT32:  formatEnum = dxgiSintFormats [ formatIndex ]; break;
        }
        Assert( formatEnum != DXGI_FORMAT_UNKNOWN, "unknown DXGI format" );

        if( Crc32( std::string(paramDesc.SemanticName).c_str() ) == 0x35aed99f ) { // Crc32( "BLEND_INDEX" )
            formatEnum = DXGI_FORMAT_R8G8B8A8_UINT;
        }
	    
        inputElementDesc->Format = formatEnum;

	    //save element desc
        ++inputElementCount;
        Assert( inputElementCount < kMaxInputElements, "input element buffer overflow" );
    }       

    // Try to create Input Layout
    DxVerify( g_dxDevice->CreateInputLayout( inputElementDescs, inputElementCount, pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), &m_inputLayout ) );

    //Free allocation shader reflection memory
    pVertexShaderReflection->Release();
}

/*------------------------------------------------------------------------------
get a shader's instruction count and a formatted c-string of its assembly instructions
------------------------------------------------------------------------------*/
void Shader::Reflect( ReflectionData *reflectionData,  ID3DBlob *blob ) {
    
    // Create readable disassembly
    ID3DBlob *disassembly = NULL;
    D3DDisassemble( blob->GetBufferPointer(), blob->GetBufferSize(), 0, NULL, &disassembly );

    // extract the assembly instructions
    std::stringstream formattedStream;
    if( disassembly ) {
        // not a null shader
        std::stringstream shaderStream = std::stringstream( (const char*)disassembly->GetBufferPointer() );
        std::string shaderLine;

        std::getline( shaderStream, shaderLine );
        while( shaderStream.good() ) {
            // remove comments and indent each line
            if( shaderLine.find( "//", 0, 2 ) ) {
                formattedStream << "\t" << shaderLine << "\n";
            }
            std::getline( shaderStream, shaderLine );
        }
    }

    // store the formatted stream
    if( formattedStream.str().size() > 0 ) {
        // remove the last newline character
        std::string formattedString = formattedStream.str();
        Assert( formattedString.size() < ReflectionData::kDisassemblyBufferSize, "need to increase kDisassemblyBufferSize" );
        formattedString[ formattedString.rfind( '\n' ) ] = '\0';
        sprintf_s( reflectionData->m_disassembly, ReflectionData::kDisassemblyBufferSize, formattedString.c_str() );
    }
    else {
        sprintf_s( reflectionData->m_disassembly, ReflectionData::kDisassemblyBufferSize, "\t%s", "no instructions" );
    }

    ID3D11ShaderReflection* pShaderReflection; 
    D3DReflect( blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &pShaderReflection );
    // Get shader info
    D3D11_SHADER_DESC shaderDesc;
    pShaderReflection->GetDesc( &shaderDesc );

    reflectionData->m_instructionCount = shaderDesc.InstructionCount;

    pShaderReflection->Release();
}