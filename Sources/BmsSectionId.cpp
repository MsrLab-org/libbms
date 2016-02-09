#include "BmsSectionId.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

#include "BmsException.hpp"
#include "Detail/Utilities.hpp"

namespace Bms {

    BmsSectionId::BmsSectionId() {}

    BmsSectionId::BmsSectionId(const UnderlyingType &value) throw(BmsException) {
        *this = value;
    }

    BmsSectionId::BmsSectionId(const std::string &value) throw(BmsException) {
        *this = value;
    }

    BmsSectionId &BmsSectionId::operator=(const UnderlyingType &value) throw(BmsException) {
        auto minValue = Min().UnderlyingValue();
        auto maxValue = Max().UnderlyingValue();
        if (value < minValue || value > maxValue) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": BmsSectionId out of range. (Given " +
                std::to_string(value) + ", expect [" +
                std::to_string(minValue) + ", " +
                std::to_string(maxValue) + "])");
        }
        Value = value;
        return *this;
    }

    BmsSectionId &BmsSectionId::operator=(const std::string &value) throw(BmsException) {
        bool failed = value.length() != 3;
        if (!failed) {
            for (auto c : value) {
                if (!(c >= '0' && c <= '9')) {
                    failed = true;
                }
            }
        }
        if (failed) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": Unexpected BmsSectionId format. (Given " + value +
                ", expect [" + Min().StringValue() +
                ", " + Max().StringValue() + "])");
        }
        Value = 0;
        for (auto c : value) {
            Value *= 10;
            Value += c - '0';
        }
        return *this;
    }

    bool operator==(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value == rhs.Value;
    }

    bool operator!=(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value != rhs.Value;
    }

    bool operator<(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value < rhs.Value;
    }

    bool operator<=(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value <= rhs.Value;
    }

    bool operator>(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value > rhs.Value;
    }

    bool operator>=(const BmsSectionId &lhs, const BmsSectionId &rhs) {
        return lhs.Value >= rhs.Value;
    }

    std::istream &operator>>(std::istream &in, BmsSectionId &id) throw(BmsException) {
        char buffer[] = "000";
        in.get(buffer, sizeof(buffer));
        id = buffer;
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const BmsSectionId &id) {
        out << id.StringValue();
        return out;
    }

    BmsSectionId BmsSectionId::Min() {
        SelfType instance;
        instance.Value = 0; // 000
        return std::move(instance);
    }

    BmsSectionId BmsSectionId::Max() {
        SelfType instance;
        instance.Value = 999; // 999
        return std::move(instance);
    }

    BmsSectionId::UnderlyingType BmsSectionId::UnderlyingValue() const {
        return Value;
    }

    std::string BmsSectionId::StringValue() const {
        char buffer[] = "000";
        buffer[0] += Value / 100;
        buffer[1] += (Value / 10) % 10;
        buffer[2] += Value % 10;
        return buffer;
    }

}

size_t std::hash<Bms::BmsSectionId>::operator()(const argument_type &key) const {
    return std::hash<argument_type::UnderlyingType>()(key.UnderlyingValue());
}
