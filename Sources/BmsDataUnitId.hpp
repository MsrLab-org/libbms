#pragma once
#ifndef LIBBMS_BMS_DATA_UNIT_ID_HPP_INCLUDED
#define LIBBMS_BMS_DATA_UNIT_ID_HPP_INCLUDED

#include <functional>
#include <string>
#include <tuple>

#include "BmsChannelId.hpp"
#include "BmsException.hpp"
#include "BmsSectionId.hpp"

namespace Bms {

    struct BmsDataUnitId {
    public:
        using SelfType = BmsDataUnitId;
    public:
        BmsDataUnitId();
        BmsDataUnitId(const BmsSectionId &section, const BmsChannelId &channel);
        BmsDataUnitId(const std::pair<BmsSectionId, BmsChannelId> &value);
        BmsDataUnitId(const std::string &value) throw(BmsException);
    public:
        SelfType &operator=(const std::pair<BmsSectionId, BmsChannelId> &value);
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
        BmsSectionId SectionPart() const;
        BmsChannelId ChannelPart() const;
        std::string StringValue() const;
    private:
        BmsSectionId Section;
        BmsChannelId Channel;
    };

}

template <>
struct std::hash<Bms::BmsDataUnitId> {
    using argument_type = Bms::BmsDataUnitId;
    using result_type = size_t;
    size_t operator()(const argument_type &key) const;
};

#endif // !LIBBMS_BMS_DATA_UNIT_ID_HPP_INCLUDED
