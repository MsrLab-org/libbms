#pragma once
#ifndef LIBBMS__DETAIL_UTILITIES_HPP_INCLUDED
#define LIBBMS__DETAIL_UTILITIES_HPP_INCLUDED

#include <algorithm>
#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>

namespace Bms {
    namespace _Detail {

        inline void FunctionNameHelper() {

#if defined(__GNUC__) || defined(__ICC) && (__ICC >= 600)
#   define LIBBMS_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#   define LIBBMS_FUNCTION_SIGNATURE __FUNCSIG__
#else
#   define LIBBMS_FUNCTION_SIGNATURE "(Unsupported)"
#endif

        }

        template <class T>
        inline std::underlying_type_t<T> ToUnderlyingValue(const T &value) {
            return static_cast<std::underlying_type_t<T>>(value);
        }

        // see https://isocpp.org/wiki/faq/input-output#print-char-or-ptr-as-number
        template <class T>
        inline auto PromoteToPrintableIntegerType(const T &i) -> decltype(+i) {
            return +i;
        }

        template <class M, class N>
        std::common_type_t<M, N> Gcd(M m, N n) {
            if (m == 0 || n == 0) {
                return 0;
            }
            if (m < n) {
                std::swap(m, n);
            }
            if (m % n == 0) {
                return n;
            } else {
                return Gcd(n, m % n);
            }
        }

        template <class T>
        T To(const std::string &str) {
            std::stringstream ss;
            ss << str;
            T ret;
            ss >> ret;
            return ret;
        }

        inline intmax_t ToIntMax(const std::string &str) {
            return To<intmax_t>(str);
        }

        inline uintmax_t ToUIntMax(const std::string &str) {
            return To<uintmax_t>(str);
        }

#define LIBBMS_DEFINE_STRING_TO_INTEGER_CONVERTION_METHOD(size)   \
        template <>                                               \
        inline int##size##_t To<int##size##_t>(const std::string &str) { \
            return static_cast<int##size##_t>(ToIntMax(str));     \
        }                                                         \
                                                                  \
        inline int##size##_t ToInt##size(const std::string &str) {       \
            return To<int##size##_t>(str);                        \
        }

        LIBBMS_DEFINE_STRING_TO_INTEGER_CONVERTION_METHOD(8);
        LIBBMS_DEFINE_STRING_TO_INTEGER_CONVERTION_METHOD(16);
        LIBBMS_DEFINE_STRING_TO_INTEGER_CONVERTION_METHOD(32);

#define LIBBMS_DEFINE_STRING_TO_UNSIGNED_INTEGER_CONVERTION_METHOD(size) \
        template <>                                                      \
        inline uint##size##_t To<uint##size##_t>(const std::string &str) {      \
            return static_cast<uint##size##_t>(ToUIntMax(str));          \
        }                                                                \
                                                                         \
        inline uint##size##_t ToUInt##size(const std::string &str) {            \
            return To<uint##size##_t>(str);                              \
        }

        LIBBMS_DEFINE_STRING_TO_UNSIGNED_INTEGER_CONVERTION_METHOD(8);
        LIBBMS_DEFINE_STRING_TO_UNSIGNED_INTEGER_CONVERTION_METHOD(16);
        LIBBMS_DEFINE_STRING_TO_UNSIGNED_INTEGER_CONVERTION_METHOD(32);

        template <class T>
        inline T ToEnum(const std::string &str) {
            return static_cast<T>(To<std::underlying_type_t<T>>(str));
        }

        inline double ToDouble(const std::string &str) {
            return To<double>(str);
        }

        inline double ToFloat(const std::string &str) {
            return To<float>(str);
        }

        inline bool ToBool(const std::string &str) {
            return ToUIntMax(str) != 0;
        }

        inline bool IsHexString(const std::string &str) {
            return std::all_of(str.begin(), str.end(),
                [](const char &c) {
                    return (c >= '0' && c <= '9')
                        || (c >= 'A' && c <= 'F')
                        || (c >= 'a' && c <= 'f');
                });
        }

        inline uint8_t HexCharToUint8(const char &c) {
            if (c >= '0' && c <= '9') {
                return c - '0';
            } else if (c >= 'A' && c <= 'F') {
                return c - 'A' + 10;
            } else if (c >= 'a' && c <= 'f') {
                return c - 'a' + 10;
            }
            return 0;
        }

    }
}

#endif // !LIBBMS__DETAIL_UTILITIES_HPP_INCLUDED
