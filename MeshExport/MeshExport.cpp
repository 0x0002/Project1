#include "MeshExport.h"

#include <stdio.h>
#include <stdint.h>

#include <maya/MItDag.h>

uint32_t const kMagicNumber = ( 'M' << 24 ) | ( 'E' << 16 ) | ( 'S' << 8 ) | ( 'H' );

MString const MeshExport::m_name = "Mesh Export";

MStatus MeshExport::writer( MFileObject const &file, MString const &, FileAccessMode mode ) {
    FILE *f = nullptr;
    if( fopen_s( &f, file.fullName().asChar(), "wb" ) != 0 )
        return MS::kFailure;

    fwrite( &kMagicNumber, sizeof( kMagicNumber ), 1, f );

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