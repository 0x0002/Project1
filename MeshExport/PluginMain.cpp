#if 0

// Bare minimum plugin 
#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#ifdef WIN32
    #define MLL_EXPORT __declspec(dllexport)
#else
    #define MLL_EXPORT
#endif

MLL_EXPORT MStatus initializePlugin( MObject object ) {
    MFnPlugin plugin( object );
    return MS::kSuccess;
}

MLL_EXPORT MStatus uninitializePlugin( MObject object ) {
    MFnPlugin plugin( object );
    return MS::kSuccess;
}
#endif

#define MNoPluginEntry
#include <maya/MFnPlugin.h>

#ifdef WIN32
    #define MLL_EXPORT __declspec(dllexport)
#else
    #define MLL_EXPORT
#endif

MLL_EXPORT MStatus initializePlugin( MObject object ) {
    MStatus status;
    
    MFnPlugin plugin( object );

    return status;
}

MLL_EXPORT MStatus uninitializePlugin( MObject object ) {
    MStatus status;
    
    MFnPlugin plugin( object );


    return status;
}