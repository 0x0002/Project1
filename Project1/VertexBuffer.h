#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

// forward declarations
struct ID3D11Buffer;

/*------------------------------------------------------------------------------
vertex buffer wrapper
------------------------------------------------------------------------------*/
struct VertexBuffer {
public:
    // constructor
    VertexBuffer() {}

    // create a buffer on the GPU
    void Create( void *data, uint32_t size, uint32_t sizeOfVertex, bool dynamic );

    // create a buffer on the GPU with no initial data
    void CreateUninitialized( uint32_t size, bool dynamic );

    // free the buffer
    void Destroy();

    // maps the buffer using WRITE_DISCARD, sets the data, then unmaps
    void SetData( void *data, unsigned size );

public:
    ID3D11Buffer   *m_buffer;           // buffer in vram
    int32_t         m_sizeOfVertex;     // size of a single vertex in the buffer
};

#endif // VERTEX_BUFFER_H