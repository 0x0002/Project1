#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

// forward declaration
struct ID3D11Buffer;

/*------------------------------------------------------------------------------
index buffer wrapper
------------------------------------------------------------------------------*/
struct IndexBuffer {
public:
    // create a buffer on the GPU
    void Create( void *data, uint32_t size, uint32_t indexCount );

    // free the buffer
    void Destroy();

public: 
    ID3D11Buffer   *m_buffer;         // buffer in vram
    uint32_t        m_indexCount;     // number of indices
};

#endif // INDEX_BUFFER_H