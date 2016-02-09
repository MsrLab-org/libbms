#pragma once
#ifndef LIBBMS_DETAIL_UTILITIES_HPP_INCLUDED
#define LIBBMS_DETAIL_UTILITIES_HPP_INCLUDED

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

    }
}

#endif // !LIBBMS_DETAIL_UTILITIES_HPP_INCLUDED
