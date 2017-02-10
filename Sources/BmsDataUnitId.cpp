#include "BmsDataUnitId.hpp"

#include <cstdint>
#include <functional>
#include <iostream>
#include <limits>
#include <string>

#include "BmsException.hpp"
#include "_Detail/Utilities.hpp"

namespace Bms {

    BmsDataUnitId::BmsDataUnitId() {};

    BmsDataUnitId::BmsDataUnitId(const BmsSectionId &section, const BmsChannelId &channel) {
        Section = section;
        Channel = channel;
    }

    BmsDataUnitId::BmsDataUnitId(const std::pair<BmsSectionId, BmsChannelId> &value) {
        *this = value;
    }

    BmsDataUnitId::BmsDataUnitId(const std::string &value) throw(BmsException) {
        *this = value;
    }

    BmsDataUnitId &BmsDataUnitId::operator=(const std::pair<BmsSectionId, BmsChannelId> &value) {
        Section = value.first;
        Channel = value.second;
        return *this;
    }

    BmsDataUnitId &BmsDataUnitId::operator=(const std::string &value) throw(BmsException) {
        if (value.length() != 5) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": Unexpected BmsDataUnitId format. (Given " + value +
                ", expect {{BmsSectionId(3)}{BmsChannelId(2)}}(5)");
        }
        Section = BmsSectionId(value.substr(0, 3));
        Channel = ConvertToBmsChannelId(value.substr(3, 2));
        return *this;
    }

    bool operator==(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section == rhs.Section && lhs.Channel == rhs.Channel;
    }

    bool operator!=(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section != rhs.Section || lhs.Channel != rhs.Channel;
    }

    bool operator<(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section < rhs.Section || (lhs.Section == rhs.Section && lhs.Channel < rhs.Channel);
    }

    bool operator<=(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section <= rhs.Section || (lhs.Section == rhs.Section && lhs.Channel <= rhs.Channel);
    }

    bool operator>(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section > rhs.Section || (lhs.Section == rhs.Section && lhs.Channel > rhs.Channel);
    }

    bool operator>=(const BmsDataUnitId &lhs, const BmsDataUnitId &rhs) {
        return lhs.Section >= rhs.Section || (lhs.Section == rhs.Section && lhs.Channel >= rhs.Channel);
    }

    std::istream &operator>>(std::istream &in, BmsDataUnitId &id) throw(BmsException) {
        in >> id.Section >> id.Channel;
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const BmsDataUnitId &id) {
        out << id.Section << id.Channel;
        return out;
    }

    BmsSectionId BmsDataUnitId::SectionPart() const {
        return Section;
    }

    BmsChannelId BmsDataUnitId::ChannelPart() const {
        return Channel;
    }

    std::string BmsDataUnitId::StringValue() const {
        return Section.StringValue() + ConvertToStringValue(Channel);
    }

}

size_t std::hash<Bms::BmsDataUnitId>::operator()(const argument_type &key) const {
    uint32_t value = key.SectionPart().UnderlyingValue();
    value <<= std::numeric_limits<Bms::BmsChannelId>::digits;
    value += static_cast<uint32_t>(key.ChannelPart());
    return std::hash<uint32_t>()(value);
}
