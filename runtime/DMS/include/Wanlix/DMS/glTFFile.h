#pragma once

#include <unordered_map>
#include <stdint.h>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <stdexcept>

#define RAPIDJSON_HAS_STDSTRING 1
#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#if defined(_MSC_VER)
#   define GLTF_PACKED
#elif defined(__GNUC__) || defined(__clang__)
#   define GLTF_PACKED __attribute__((__packed__))
#endif

#define GLTF_ENUM_STRUCT(T) \
    uint32_t value = 0; \
    constexpr T() noexcept = default; \
    constexpr T(uint32_t val) : value(val) {} \
    constexpr operator uint32_t&() { return value; } \
    constexpr operator uint32_t const&() const { return value; } \
    constexpr bool operator==(T const& rhs) const { return value == rhs.value; } \
    constexpr bool operator!=(T const& rhs) const { return !(*this == rhs); } \
    constexpr bool operator< (T const& rhs) const { return value <  rhs.value; } \
    constexpr bool operator<=(T const& rhs) const { return value <= rhs.value; } \
    constexpr bool operator> (T const& rhs) const { return value >  rhs.value; } \
    constexpr bool operator>=(T const& rhs) const { return value >= rhs.value; } \
    constexpr T operator&(T const& rhs) const { return value & rhs.value; } \
    constexpr T operator|(T const& rhs) const { return value | rhs.value; } \
    constexpr T operator^(T const& rhs) const { return value ^ rhs.value; } \
    constexpr T operator~() const { return ~value; } \
    constexpr T& operator&=(T const& rhs) { value &= rhs.value; return *this; } \
    constexpr T& operator|=(T const& rhs) { value |= rhs.value; return *this; } \
    constexpr T& operator^=(T const& rhs) { value ^= rhs.value; return *this; }

namespace Wanlix::glTF
{
    struct Object;
    struct BufferView;
    struct Accessor;

    #pragma pack(push, 1)
    struct GLBHeader
    {
        uint8_t  magic[4]; // "glTF"
        uint32_t version = 2;
        uint32_t length = 0;
    } GLTF_PACKED;

    struct GLB_Chunk
    {
        uint32_t length = 0;
        uint32_t type = 0;
    } GLTF_PACKED;
    #pragma pack(pop)

    struct ChunkType
    {
        enum {
            JSON = 0x4E4F534A,
            BIN = 0x004E4942,
        };
        GLTF_ENUM_STRUCT(ChunkType)
    };

    enum class Primitive
    {
        Undefined = 0,
        Points,
        Lines,
        LineLoop,
        LineStrip,
        Triangles,
        TriangleStrip,
        TriangleFan,
    };

    //! Values for the Accessor::componentType field
    struct ComponentType
    {
        enum {
            Undefined = 0,
            Byte = 5120,
            UnsignedByte = 5152,
            Short = 5122,
            UnsignedShort = 5123,
            Int = 5124,
            UnsignedInt = 5125,
            Float = 5126,
            Double = 5127,
        };
        GLTF_ENUM_STRUCT(ComponentType)
        static constexpr int Size(int type) noexcept;
    };

    //! Values for the BufferView::target field
    struct BufferViewTarget
    {
        enum {
            Undefined = 0,
            ArrayBuffer = 34962,
            ElementArrayBuffer = 34963,
        };
        GLTF_ENUM_STRUCT(BufferViewTarget)
    };

    //! Values for the Sampler::[mag|min]Filter field
    struct SamplerFilter
    {
        enum {
            Unset = 0,
            Nearest = 9728,
            Linear = 9729,
            Nearest_Mipmap_Nearest = 9984,
            Linear_Mipmap_Nearest = 9985,
            Nearest_Mipmap_Linear = 9986,
            Linear_Mipmap_Linear = 9987,
        };
        GLTF_ENUM_STRUCT(SamplerFilter)
    };

    //! Values for the Sampler::wrap[S|T] field
    struct SamplerWrap
    {
        enum {
            Unset = 0,
            ClampToEdge = 33071,
            MirroredRepeat = 33648,
            Repeat = 10497,
        };
        GLTF_ENUM_STRUCT(SamplerWrap)
    };

    //! Values for the Texture::format and Texture::internalFormat fields
    struct TextureFormat
    {
        enum {
            Undefined = 0,
            Alpha = 6406,
            RGB = 6407,
            RGBA = 6408,
            Luminance = 6409,
            LuminanceAlpha = 6410,
        };
        GLTF_ENUM_STRUCT(TextureFormat)
    };

    //! Values for the Texture::target field
    struct TextureTarget
    {
        enum {
            Undefined = 0,
            Texture2D = 3553,
        };
        GLTF_ENUM_STRUCT(TextureTarget)
    };

    //! Values for the Texture::type field
    struct TextureType
    {
        enum {
            Undefined = 0,
            UnsignedByte = 5152,
            UnsignedShort_5_6_5 = 33653,
            UnsignedShort_4_4_4_4 = 32819,
            UnsignedShort_5_5_5_1 = 32820,
        };
        GLTF_ENUM_STRUCT(TextureType)
    };

    //! Values for the Animation::Target::path field
    enum class AnimationPath
    {
        Undefined = 0,
        Translation,
        Rotation,
        Scale,
        Weights,
    };

    //! Values for the Animation::Sampler::interpolation field
    enum class Interpolation
    {
        Undefined = 0,
        Linear,
        Step,
        CubicSpline,
    };

    struct Blob final
    {
        void* data = nullptr;
        size_t size = 0;

        Blob() = default;
        Blob(void* data, size_t size);
        Blob(Blob const& rhs);
        Blob(Blob&& rhs);
        Blob& operator=(Blob const& rhs);
        Blob& operator=(Blob&& rhs);
        ~Blob();
    };

    using BlobPtr = std::shared_ptr<Blob>;

    struct Attribute
    {
        enum class Value {
            Scalar,
            Vec2,
            Vec3,
            Vec4,
            Mat2,
            Mat3,
            Mat4,
        };

        static Value FromString(const char* str);
        static const char* ToString(Value type);
        static int NumComponents(Value type);

    private:
        std::unordered_map<Value, int> mInfoMap;
    };

    struct Object
    {
        int localIndex = 0;
        int globalIndex = 0;
        std::string globalId;
        std::string name;
    };

    struct Indexer
    {
        friend struct Accessor;
        Accessor& accessor;
        uint8_t* data = nullptr;
        size_t elementSize = 0;
        size_t stride = 0;

        Indexer(Accessor& accessor);
        bool IsValid() const;
        template<typename T>
        T& GetValue(int index);
    };

    //! An accessor provides a typed view into a BufferView or a subset of a BufferView.
    struct Accessor : public Object
    {
        std::shared_ptr<BufferView> bufferView;
        size_t byteOffset = 0;
        ComponentType componentType = ComponentType::Undefined;
        size_t count = 0;

        uint32_t NumComponents() const;
        uint32_t BytesPerComponent() const;
        uint32_t ElementSize() const;
        uint8_t* GetPointer() const;
        Indexer  GetIndexer();
        template<typename T> 
        BlobPtr ReadData() const { return ReadData(sizeof(T)); }
        BlobPtr ReadData(size_t destElemSize) const;
        void WriteData(size_t count, const void* src, size_t stride);
    };

    //! A buffer points to binary geometry, animation, or skins.
    struct Buffer : public Object
    {
        enum class Type {
            ArrayBuffer,
            Text,
        };
        struct EncodedRegion;

        size_t byteLength = 0;
        size_t capacity = 0;
        Type type;
    };

}