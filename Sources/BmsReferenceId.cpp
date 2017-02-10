#include "BmsReferenceId.hpp"

#include <algorithm>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>

#include "BmsException.hpp"
#include "_Detail/Utilities.hpp"

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
        _Value = value;
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
        _Value = 0;
        for (auto c : str) {
            _Value *= 36;
            if (c >= '0' && c <= '9') {
                _Value += c - '0';
            } else {
                _Value += c - 'A' + 10;
            }
        }
        return *this;
    }

    bool operator==(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value == rhs._Value;
    }

    bool operator!=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value != rhs._Value;
    }

    bool operator<(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value < rhs._Value;
    }

    bool operator<=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value <= rhs._Value;
    }

    bool operator>(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value > rhs._Value;
    }

    bool operator>=(const BmsReferenceId &lhs, const BmsReferenceId &rhs) {
        return lhs._Value >= rhs._Value;
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
        instance._Value = 0; // 00, Placeholder
        return instance;
    }

    BmsReferenceId BmsReferenceId::Max() {
        SelfType instance;
        instance._Value = 1295; // ZZ
        return instance;
    }

    BmsReferenceId BmsReferenceId::Placeholder() {
        SelfType instance;
        instance._Value = 0; // 00, Placeholder
        return instance;
    }

    bool BmsReferenceId::IsPlaceholder() const {
        return _Value == 0;
    }

    BmsReferenceId::UnderlyingType BmsReferenceId::UnderlyingValue() const {
        return _Value;
    }

    std::string BmsReferenceId::StringValue() const {
        char buffer[] = "00";
        buffer[0] += _Value / 36;
        buffer[1] += _Value % 36;
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
