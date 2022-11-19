// cpu-texture.h
#pragma once
#include "cpu-base.h"

namespace gfx
{
using namespace Slang;

namespace cpu
{

struct CPUTextureBaseShapeInfo
{
    int32_t rank;
    int32_t baseCoordCount;
    int32_t implicitArrayElementCount;
};

static const CPUTextureBaseShapeInfo kCPUTextureBaseShapeInfos[(int)ITextureResource::Type::_Count] =
{
    /* Unknown */       { 0, 0, 0 },
    /* Buffer */        { 1, 1, 1 },
    /* Texture1D */     { 1, 1, 1 },
    /* Texture2D */     { 2, 2, 1 },
    /* Texture3D */     { 3, 3, 1 },
    /* TextureCube */   { 2, 3, 6 },
};

static CPUTextureBaseShapeInfo const* _getBaseShapeInfo(ITextureResource::Type baseShape);

typedef void (*CPUTextureUnpackFunc)(void const* texelData, void* outData, size_t outSize);

struct CPUTextureFormatInfo
{
    CPUTextureUnpackFunc unpackFunc;
};

template<int N>
void _unpackFloatTexel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackFloat16Texel(void const* texelData, void* outData, size_t outSize);

static inline float _unpackUnorm8Value(uint8_t value);

template<int N>
void _unpackUnorm8Texel(void const* texelData, void* outData, size_t outSize);

// template<int N>
// void _unpackSRGBUnorm8Texel(void const* texelData, void* outData, size_t outSize);

void _unpackUnormBGRA8Texel(void const* texelData, void* outData, size_t outSize);

void _unpackSRGBUnormBGRA8Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackUInt8Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackUInt16Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackUInt32Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackInt8Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackInt16Texel(void const* texelData, void* outData, size_t outSize);

template<int N>
void _unpackInt32Texel(void const* texelData, void* outData, size_t outSize);

struct CPUFormatInfoMap
{
    CPUFormatInfoMap()
    {
        memset(m_infos, 0, sizeof(m_infos));

        // TODO: Finish providing remaining _unpack implementations
        set(Format::R32G32B32A32_TYPELESS, &_unpackUInt32Texel<4>);
        set(Format::R32G32B32_TYPELESS, &_unpackUInt32Texel<3>);
        set(Format::R32G32_TYPELESS, &_unpackUInt32Texel<2>);
        set(Format::R32_TYPELESS, &_unpackUInt32Texel<1>);

        set(Format::R16G16B16A16_TYPELESS, &_unpackUInt16Texel<4>);
        set(Format::R16G16_TYPELESS, &_unpackUInt16Texel<2>);
        set(Format::R16_TYPELESS, &_unpackUInt16Texel<1>);

        set(Format::R8G8B8A8_TYPELESS, &_unpackUInt8Texel<4>);
        set(Format::R8G8_TYPELESS, &_unpackUInt8Texel<2>);
        set(Format::R8_TYPELESS, &_unpackUInt8Texel<1>);
        set(Format::B8G8R8A8_TYPELESS, &_unpackUInt8Texel<4>);

        set(Format::R32G32B32A32_FLOAT, &_unpackFloatTexel<4>);
        set(Format::R32G32B32_FLOAT, &_unpackFloatTexel<3>);
        set(Format::R32G32_FLOAT, &_unpackFloatTexel<2>);
        set(Format::R32_FLOAT, &_unpackFloatTexel<1>);

        set(Format::R16G16B16A16_FLOAT, &_unpackFloat16Texel<4>);
        set(Format::R16G16_FLOAT, &_unpackFloat16Texel<2>);
        set(Format::R16_FLOAT, &_unpackFloat16Texel<1>);

        set(Format::R32G32B32A32_UINT, &_unpackUInt32Texel<4>);
        set(Format::R32G32B32_UINT, &_unpackUInt32Texel<3>);
        set(Format::R32G32_UINT, &_unpackUInt32Texel<2>);
        set(Format::R32_UINT, &_unpackUInt32Texel<1>);

        set(Format::R16G16B16A16_UINT, &_unpackUInt16Texel<4>);
        set(Format::R16G16_UINT, &_unpackUInt16Texel<2>);
        set(Format::R16_UINT, &_unpackUInt16Texel<1>);

        set(Format::R8G8B8A8_UINT, &_unpackUInt8Texel<4>);
        set(Format::R8G8_UINT, &_unpackUInt8Texel<2>);
        set(Format::R8_UINT, &_unpackUInt8Texel<1>);

        set(Format::R32G32B32A32_SINT, &_unpackInt32Texel<4>);
        set(Format::R32G32B32_SINT, &_unpackInt32Texel<3>);
        set(Format::R32G32_SINT, &_unpackInt32Texel<2>);
        set(Format::R32_SINT, &_unpackInt32Texel<1>);

        set(Format::R16G16B16A16_SINT, &_unpackInt16Texel<4>);
        set(Format::R16G16_SINT, &_unpackInt16Texel<2>);
        set(Format::R16_SINT, &_unpackInt16Texel<1>);

        set(Format::R8G8B8A8_SINT, &_unpackInt8Texel<4>);
        set(Format::R8G8_SINT, &_unpackInt8Texel<2>);
        set(Format::R8_SINT, &_unpackInt8Texel<1>);

//         set(Format::R16G16B16A16_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::R16G16_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 2);
//         set(Format::R16_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 1);

        set(Format::R8G8B8A8_UNORM, &_unpackUnorm8Texel<4>);
/*        set(Format::R8G8B8A8_UNORM_SRGB, &_unpackSRGBUnorm8Texel<4>);*/
        set(Format::R8G8_UNORM, &_unpackUnorm8Texel<2>);
        set(Format::R8_UNORM, &_unpackUnorm8Texel<1>);
        set(Format::B8G8R8A8_UNORM, &_unpackUnormBGRA8Texel);
        set(Format::B8G8R8A8_UNORM_SRGB, &_unpackSRGBUnormBGRA8Texel);
        set(Format::B8G8R8X8_UNORM, &_unpackUnorm8Texel<4>);
        set(Format::B8G8R8X8_UNORM_SRGB, &_unpackSRGBUnormBGRA8Texel);

//         set(Format::R16G16B16A16_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::R16G16_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 2);
//         set(Format::R16_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 1);
// 
//         set(Format::R8G8B8A8_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::R8G8_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 2);
//         set(Format::R8_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 1);

        set(Format::D32_FLOAT, &_unpackFloatTexel<1>);
//         set(Format::D16_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 1);

//         set(Format::B4G4R4A4_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::B5G6R5_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 3);
//         set(Format::B5G5R5A1_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
// 
//         set(Format::R9G9B9E5_SHAREDEXP, SLANG_SCALAR_TYPE_FLOAT32, 3);
//         set(Format::R10G10B10A2_TYPELESS, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::R10G10B10A2_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4);
//         set(Format::R10G10B10A2_UINT, SLANG_SCALAR_TYPE_UINT32, 4);
//         set(Format::R11G11B10_FLOAT, SLANG_SCALAR_TYPE_FLOAT32, 3);
// 
//         set(Format::BC1_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC1_UNORM_SRGB, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC2_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC2_UNORM_SRGB, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC3_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC3_UNORM_SRGB, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC4_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 1, 4, 4);
//         set(Format::BC4_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 1, 4, 4);
//         set(Format::BC5_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 2, 4, 4);
//         set(Format::BC5_SNORM, SLANG_SCALAR_TYPE_FLOAT32, 2, 4, 4);
//         set(Format::BC6H_UF16, SLANG_SCALAR_TYPE_FLOAT32, 3, 4, 4);
//         set(Format::BC6H_SF16, SLANG_SCALAR_TYPE_FLOAT32, 3, 4, 4);
//         set(Format::BC7_UNORM, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
//         set(Format::BC7_UNORM_SRGB, SLANG_SCALAR_TYPE_FLOAT32, 4, 4, 4);
    }

    void set(Format format, CPUTextureUnpackFunc func)
    {
        auto& info = m_infos[Index(format)];
        info.unpackFunc = func;
    }
    SLANG_FORCE_INLINE const CPUTextureFormatInfo& get(Format format) const { return m_infos[Index(format)]; }

    CPUTextureFormatInfo m_infos[Index(Format::_Count)];
};

static const CPUFormatInfoMap g_formatInfoMap;

static CPUTextureFormatInfo const* _getFormatInfo(Format format)
{
    const CPUTextureFormatInfo& info = g_formatInfoMap.get(format);
    return info.unpackFunc ? &info : nullptr;
}

class TextureResourceImpl : public TextureResource
{
    enum { kMaxRank = 3 };

public:
    TextureResourceImpl(const TextureResource::Desc& desc)
        : TextureResource(desc)
    {}
    ~TextureResourceImpl();

    Result init(ITextureResource::SubresourceData const* initData);

    Desc const& _getDesc() { return m_desc; }
    Format getFormat() { return m_desc.format; }
    int32_t getRank() { return m_baseShape->rank; }

    CPUTextureBaseShapeInfo const* m_baseShape;
    CPUTextureFormatInfo const* m_formatInfo;
    int32_t m_effectiveArrayElementCount = 0;
    uint32_t m_texelSize = 0;

    struct MipLevel
    {
        int32_t extents[kMaxRank];
        int64_t strides[kMaxRank+1];
        int64_t offset;
    };
    List<MipLevel>  m_mipLevels;
    void*           m_data = nullptr;
};

} // namespace cpu
} // namespace gfx
