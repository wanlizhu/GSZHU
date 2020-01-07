#pragma once

#define SafeDelete(a) { if (a != nullptr) { delete a; a = nullptr; } }
#define SafeDeleteArray(a) { if (a != nullptr) { delete[] a; a = nullptr; } }
#define Stringize(a) #a
#define ConcatStrings(a, b) a##b
#define EnumToInt(T, var) static_cast<std::underlying_type_t<T>>(var)


#define ENUM_CLASS_OPERATORS(T) \
inline constexpr T  operator& (T  a, T b) { return static_cast<T>(EnumToInt(T, a) & EnumToInt(T, b)); } \
inline constexpr T  operator| (T  a, T b) { return static_cast<T>(EnumToInt(T, a) | EnumToInt(T, b)); } \
inline constexpr T& operator|=(T& a, T b) { a = a | b; return a; }; \
inline constexpr T& operator&=(T& a, T b) { a = a & b; return a; }; \
inline constexpr T  operator~ (T a) { return static_cast<T>(~EnumToInt(T, a)); } \
inline constexpr bool HasBits(T val, T flag) { return (val & flag) != (T)0; }


#define CLASSINFO(type) \
private: \
    using __class = type;\
    static constexpr const char* __className = #type;


#define FUNCTION_AND_LINE (std::string(__FUNCTION__) + ":" + std::to_string(__LINE__)).c_str()


#define DEFINE_BIT_OPS(type, target) \
inline bool Has(type bit) const { return target & bit; }\
inline void Set(type bit) { target |= bit; }\
inline void Unset(type bit) { target &= ~bit; }\
inline void Toggle(type bit) { target = Has(bit) ? (target & ~bit) : (target | bit); }


#define DEFINE_CMP_OPS(type, member) \
inline bool operator==(const type& rhs) const { return member == rhs.member; }\
inline bool operator!=(const type& rhs) const { return member != rhs.member; }\
inline bool operator< (const type& rhs) const { return member <  rhs.member; }\
inline bool operator<=(const type& rhs) const { return member <= rhs.member; }\
inline bool operator> (const type& rhs) const { return member >  rhs.member; }\
inline bool operator>=(const type& rhs) const { return member >= rhs.member; }


#define DEFINE_VEC_NUMERIC_OPS(type, N, data) \
inline type  operator+(const type& rhs) const { return type(*this) += rhs; }\
inline type  operator-(const type& rhs) const { return type(*this) -= rhs; }\
inline type  operator*(const type& rhs) const { return type(*this) *= rhs; }\
inline type  operator/(const type& rhs) const { return type(*this) /= rhs; }\
inline type& operator+=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, +=); return *this; }\
inline type& operator-=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, -=); return *this; }\
inline type& operator*=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, *=); return *this; }\
inline type& operator/=(const type& rhs) { LOOP_OP((*this).data, rhs.data, 0, N, /=); return *this; }\
inline type  operator-() const { type tmp; LOOP_OP(tmp.data, (*this).data, 0, N, =-); return tmp; }


#define INHERIT_SHARED_FROM_THIS(basetype) \
public: \
std::shared_ptr<__class> shared_from_this()\
{\
    basetype* base = static_cast<basetype*>(this);\
    std::shared_ptr<basetype> shared = base->shared_from_this();\
    return std::static_pointer_cast<__class>(shared);\
}\
std::shared_ptr<const __class> shared_from_this() const\
{\
    const basetype* base = static_cast<const basetype*>(this);\
    std::shared_ptr<const basetype> shared = base->shared_from_this();\
    return std::static_pointer_cast<const __class>(shared);\
}