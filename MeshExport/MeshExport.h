#ifndef MESH_EXPORT_H
#define MESH_EXPORT_H

#include <maya/MPxFileTranslator.h>

class MeshExport : public MPxFileTranslator {
public:
    static void* Creator() { return new MeshExport; }

    virtual MStatus writer( MFileObject const &file, MString const &optionsString, FileAccessMode mode ) override;
    virtual bool haveWriteMethod() const override { return true; }

    virtual MString defaultExtension() const override { return "*.mesh"; }

    static const MString m_name;
};

#endif // MESH_EXPORT_H