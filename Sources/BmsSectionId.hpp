#pragma once
#ifndef LIBBMS_BMS_SECTION_ID_HPP_INCLUDED
#define LIBBMS_BMS_SECTION_ID_HPP_INCLUDED

#include <cstdint>
#include <functional>
#include <string>

#include "BmsException.hpp"

namespace Bms {

    struct BmsSectionId {
    public:
        using SelfType = BmsSectionId;
        using UnderlyingType = uint16_t;
    public:
        BmsSectionId();
        BmsSectionId(const UnderlyingType &value) throw(BmsException);
        BmsSectionId(const std::string &value) throw(BmsException);
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
        static SelfType Min(); // 0, 000
        static SelfType Max(); // 999, 999
        UnderlyingType UnderlyingValue() const;
        std::string StringValue() const;
    private:
        UnderlyingType Value = 1;
    };

}

template <>
struct std::hash<Bms::BmsSectionId> {
    using argument_type = Bms::BmsSectionId;
    using result_type = size_t;
    size_t operator()(const argument_type &key) const;
};

#endif // !LIBBMS_BMS_SECTION_ID_HPP_INCLUDED
