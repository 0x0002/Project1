#ifdef WIN32
    #define MLL_EXPORT __declspec(dllexport)
#else
    #define MLL_EXPORT
#endif

#define MNoPluginEntry
#include <maya/MFnPlugin.h>
#include <maya/MStatus.h>

#include "MeshExport.h"

MLL_EXPORT MStatus initializePlugin( MObject object ) {
    MStatus status;
    
    MFnPlugin plugin( object );

    status = plugin.registerFileTranslator( MeshExport::m_name, NULL, MeshExport::Creator );
    CHECK_MSTATUS_AND_RETURN( status, status );

    return status;
}

MLL_EXPORT MStatus uninitializePlugin( MObject object ) {
    MStatus status;
    
    MFnPlugin plugin( object );

    status = plugin.deregisterFileTranslator( MeshExport::m_name );
    CHECK_MSTATUS_AND_RETURN( status, status );

    return status;
}