#ifndef MESH_EXPORT_H
#define MESH_EXPORT_H

#include <maya/MPxFileTranslator.h>

class MeshExport : public MPxFileTranslator {
public:
    static void* Creator() { return new MeshExport; }

    virtual MStatus writer( MFileObject const &file, MString const &optionsString, FileAccessMode mode ) override;
    virtual bool haveWriteMethod() const override { return true; }

    virtual MString defaultExtension() const override { return "*.mesh"; }
    virtual MString filter() const override { return defaultExtension(); }
    virtual MPxFileTranslator::MFileKind identifyFile( MFileObject const &file, char const* buffer, short size ) const override;

    static const MString m_name;
};

#endif // MESH_EXPORT_H