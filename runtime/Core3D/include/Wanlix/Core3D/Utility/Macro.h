#pragma once

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


#define LOOP_OP(lhs, rhs, beg, end, op) for (int i = beg; i < end; i++) {  lhs[i] op rhs[i]; }

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