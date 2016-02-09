#include "BmsReferenceId.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

#include "BmsException.hpp"
#include "Detail/Utilities.hpp"

namespace Bms {

    BmsReferenceId::BmsReferenceId() {}

    BmsReferenceId::BmsReferenceId(const UnderlyingType &value) throw(BmsException) {
        *this = value;
    }

    BmsReferenceId::BmsReferenceId(const std::string &value) throw(BmsException) {
        *this = value;
    }

    BmsReferenceId &BmsReferenceId::operator=(const UnderlyingType &value) throw(BmsException) {
        auto minValue = Min().UnderlyingValue();
        auto maxValue = Max().UnderlyingValue();
        if (value < minValue || value > maxValue) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": BmsReferenceId out of range. (Given " +
                std::to_string(value) + ", expect [" +
                std::to_string(minValue) + ", " +
                std::to_string(maxValue) + "])");
        }
        Value = value;
        return *this;
    }

    BmsReferenceId &BmsReferenceId::operator=(const std::string &value) throw(BmsException) {
        auto str = value;
        std::transform(str.begin(), str.end(), str.begin(), toupper);
        bool failed = str.length() != 2;
        if (!failed) {
            for (auto c : str) {
                if (!(c >= '0' && c <= '9') && !(c >= 'A' && c <= 'Z')) {
                    failed = true;
                }
            }
        }
        if (failed) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": Unexpected BmsReferenceId format. (Given " + value +
                ", expect [" + Min().StringValue() +
                ", " + Max().StringValue() + "])");
        }
        Value = 0;
        for (auto c : str) {
            Value *= 36;
            if (c >= '0' && c <= '9') {
                Value += c - '0';
            } else {
                Value += c - 'A' + 10;
            }
        }
        return *this;
    }

    bool operator==(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value == rhs.Value;
    }

    bool operator!=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value != rhs.Value;
    }

    bool operator<(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value < rhs.Value;
    }

    bool operator<=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value <= rhs.Value;
    }

    bool operator>(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value > rhs.Value;
    }

    bool operator>=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs.Value >= rhs.Value;
    }

    std::istream &operator>>(std::istream &in, BmsReferenceId &id) throw(BmsException) {
        char buffer[] = "00";
        in.get(buffer, sizeof(buffer));
        id = buffer;
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const BmsReferenceId &id) {
        return out << id.StringValue();
    }

    BmsReferenceId BmsReferenceId::Min() {
        SelfType instance;
        instance.Value = 0; // 00, Placeholder
        return std::move(instance);
    }

    BmsReferenceId BmsReferenceId::Max() {
        SelfType instance;
        instance.Value = 1295; // ZZ
        return std::move(instance);
    }

    BmsReferenceId::UnderlyingType BmsReferenceId::UnderlyingValue() const {
        return Value;
    }

    std::string BmsReferenceId::StringValue() const {
        char buffer[] = "00";
        buffer[0] += Value / 36;
        buffer[1] += Value % 36;
        for (auto *p = buffer; *p != '\0'; ++p) {
            auto &c = *p;
            if (c > '9') {
                c += 'A' - '0' - 10;
            }
        }
        return buffer;
    }

}

size_t std::hash<Bms::BmsReferenceId>::operator()(const argument_type &key) const {
    return std::hash<argument_type::UnderlyingType>()(key.UnderlyingValue());
}
