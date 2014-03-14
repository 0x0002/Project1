#include "MeshExport.h"
#include <stdio.h>
#include <maya/MItDag.h>

MString const MeshExport::m_name = "Mesh Export";

MStatus MeshExport::writer( MFileObject const &file, MString const &, FileAccessMode mode ) {
    FILE *f = nullptr;
    if( fopen_s( &f, file.fullName().asChar(), "wb" ) != 0 )
        return MS::kFailure;

    MItDag it;

    if( mode == kExportActiveAccessMode ) {
        // export selected objects
    }
    else {
        // export all objects
    }

    fclose( f );
    return MS::kSuccess;
}

MPxFileTranslator::MFileKind MeshExport::identifyFile( MFileObject const &, char const *, short ) const {
    // don't try to import this file
    return kNotMyFileType;
}