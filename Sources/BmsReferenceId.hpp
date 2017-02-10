#pragma once
#ifndef LIBBMS_BMS_REFERENCE_ID_HPP_INCLUDED
#define LIBBMS_BMS_REFERENCE_ID_HPP_INCLUDED

#include <cstdint>
#include <functional>
#include <string>

#include "BmsException.hpp"

namespace Bms {

    struct BmsReferenceId {
    public:
        using SelfType = BmsReferenceId;
        using UnderlyingType = uint16_t;
    public:
        BmsReferenceId();
        BmsReferenceId(const UnderlyingType &value) throw(BmsException);
        BmsReferenceId(const std::string &value) throw(BmsException);
    public:
        SelfType &operator=(const UnderlyingType &value) throw(BmsException);
        SelfType &operator=(const std::string &value) throw(BmsException);
        friend bool operator==(const SelfType &lhs, const SelfType &rhs);
        friend bool operator!=(const SelfType &lhs, const SelfType &rhs);
        friend bool operator<(const SelfType &lhs, const SelfType &rhs);
        friend bool operator<=(const SelfType &lhs, const SelfType &rhs);
        friend bool operator>(const SelfType &lhs, const SelfType &rhs);
        friend bool operator>=(const SelfType &lhs, const SelfType &rhs);
        friend std::istream &operator>>(std::istream &in, SelfType &id) throw(BmsException);
        friend std::ostream &operator<<(std::ostream &out, const SelfType &id);
    public:
        static SelfType Min(); // 0, 00, Placeholder
        static SelfType Max(); // 1295, ZZ
        static SelfType Placeholder(); // 0, 00, Placeholder
        bool IsPlaceholder() const;
        UnderlyingType UnderlyingValue() const;
        std::string StringValue() const;
    private:
        UnderlyingType _Value = 1;
    };

}

template <>
struct std::hash<Bms::BmsReferenceId> {
    using argument_type = Bms::BmsReferenceId;
    using result_type = size_t;
    size_t operator()(const argument_type &key) const;
};

#endif // !LIBBMS_BMS_REFERENCE_ID_HPP_INCLUDED
