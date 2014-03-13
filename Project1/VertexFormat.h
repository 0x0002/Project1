#ifndef VERTEX_FORMAT_H
#define VERTEX_FORMAT_H

// vertex format specification
namespace VertexFormat {
    enum Type {
        kPosition       = 0x01,
        kNormal         = 0x02,
        kTexcoord       = 0x04,
        kTangent        = 0x08,
        kBlendWeight    = 0x10,
        kBlendIndex     = 0x20,

        kPositionNormal         = kPosition | kNormal,
        kPositionTexcoord       = kPosition |           kTexcoord,
        kPositionNormalTexcoord = kPosition | kNormal | kTexcoord,
        kNormalTexcoord         =             kNormal | kTexcoord,

        kUnskinned      = kPositionNormalTexcoord,
        kSkinned        = kPositionNormalTexcoord | kBlendWeight | kBlendIndex,

        kInvalid        = 0,
    };

    enum Size {
        kPositionSize     = 3 * sizeof( float ),
        kNormalSize       = 3 * sizeof( float ),
        kTexcoordSize     = 2 * sizeof( float ),
        kTangentSize      = 3 * sizeof( float ),
        kBlendWeightSize  = 4 * sizeof( float ),
        kBlendIndexSize   = 4 * sizeof( uint8_t ),

        kUnskinnedSize    = kPositionSize + kNormalSize + kTexcoordSize,
        kSkinnedSize      = kUnskinnedSize + kBlendWeightSize + kBlendIndexSize,
    };
}

#endif // VERTEX_FORMAT_H