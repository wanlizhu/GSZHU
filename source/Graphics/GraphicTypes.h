#pragma once

namespace GSZHU {
    template<typename E>
    struct EnableBitsOperators {
        static const bool enable = false;
    };

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type
    operator|(E lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        return static_cast<E>(static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs));
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type
    operator&(E lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        return static_cast<E>(static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs));
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type
    operator^(E lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        return static_cast<E>(static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs));
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type
    operator~(E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        return static_cast<E>(~static_cast<Underlying>(rhs));
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type&
    operator|=(E& lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        lhs = static_cast<Underlying>(lhs) | static_cast<Underlying>(rhs);
        return lhs;
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type&
    operator&=(E& lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        lhs = static_cast<Underlying>(lhs) & static_cast<Underlying>(rhs);
        return lhs;
    }

    template<typename E>
    typename std::enable_if<EnableBitsOperators<E>::enable, E>::type&
    operator^=(E& lhs, E rhs) {
        using Underlying = typename std::underlying_type<E>::type;
        lhs = static_cast<Underlying>(lhs) ^ static_cast<Underlying>(rhs);
        return lhs;
    }

#define ENABLE_BITS_OPERATORS(Enum) \
    template<> \
    struct EnableBitsOperators<Enum> {\
        static const bool enable = true;\
    };

    enum class EBindMask : unsigned int {
        None = 0x0L, 
        VertexBuffer = 0x1L,
        IndexBuffer = 0x2L,
        UniformBuffer = 0x4L,
        ShaderResource = 0x8L,
        StreamOutput = 0x10L,
        RenderTarget = 0x20L,
        DepthStencil = 0x40L,
        UAV = 0x80L,
        IndirectDrawBuffer = 0x100L
    };
    ENABLE_BITS_OPERATORS(EBindMask)

    enum class EAccess : unsigned char {
        Unknown = 0,
        None = 1,
        Read = 2,
        Write = 4,
        ReadWrite = (2 + 4),
    };

    enum class EUsage : unsigned char {
        Static = 0,
        Default,
        Dynamic,
        CPUCanRead,
        CPUCanWrite,
    };

    enum class EBufferMode : unsigned char {
        Undefined = 0,
        Formatted, //In this mode, ElementByteStride member of buffer desc defines the buffer element size.
        Structured, // In this mode, ElementByteStride member of buffer desc defines the structure stride.
        Raw
    };

    enum class EScalar {
        Undefined = 0,
        Typeless,
        Int8,
        Int16,
        Int32,
        UInt8,
        UInt16,
        UInt32,
        SNorm16,
        SNorm32,
        Norm16,
        Norm32,
        Float16,
        Float32,
        Float64
    };

    enum class EBufferViewType : unsigned char {
        Undefined = 0,
        SRV, // Shader resource view
        UAV  // Unordered access view
    };

    enum class ETextureViewType : unsigned char {
        Undefined = 0,
        SRV,
        UAV,
        RenderTarget,
        DepthStencil,
    };

    enum class ETextureDimension : unsigned char {
        Undefined,
        _1D,
        _2D,
        _3D,
        Cube,
        _1DArray,
        _2DArray,
        CubeArray
    };

    enum class EPixelFormat : unsigned int {
        Unknown = 0,
    };

    enum class EFilter : unsigned char {
        Unknown = 0,
        Point,
        Linear,
        Anisotropic,
        ComparisonPoint,
        ComparisonLinear,
        ComparisonAnisotropic,
        MinPoint,
        MinLinear,
        MinAnisotropic,
        MaxPoint,
        MaxLinear,
        MaxAnisotropic
    };

    enum class EAddressMode : unsigned char {
        Unknown = 0,
        Wrap,
        Mirror,
        Clamp,
        Border,
        MirrorOnce
    };
}