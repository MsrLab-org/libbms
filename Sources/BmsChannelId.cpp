#include "BmsChannelId.hpp"

#include <cstdint>
#include <iostream>
#include <set>
#include <string>

#include "BmsException.hpp"
#include "BmsDataUnit.hpp"
#include "BmsDecimalDataUnit.hpp"
#include "BmsIntegerListDataUnit.hpp"
#include "BmsReferenceListDataUnit.hpp"
#include "_Detail/Utilities.hpp"

namespace Bms {

    std::string ConvertToStringValue(BmsChannelId id) {
        char buffer[] = "00";
        buffer[0] += static_cast<uint8_t>(id) / 10;
        buffer[1] += static_cast<uint8_t>(id) % 10;
        return buffer;
    }

    BmsChannelId ConvertToBmsChannelId(std::string value) throw(BmsException) {
        bool failed = value.length() != 2;
        if (!failed) {
            for (auto c : value) {
                if (!(c >= '0' && c <= '9')) {
                    failed = true;
                }
            }
        }
        if (failed) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE
                + ": Unexpected BmsChannelId format.");
        }
        static const std::set<uint8_t> ids = {
            1, 2, 3, 4, 6, 7, 8, 9, 11, 12,
            13, 14, 15, 16, 18, 19, 21, 22,
            23, 24, 25, 26, 28, 29, 51, 52,
            53, 54, 55, 56, 57, 61, 62, 63,
            64, 65, 66, 67
        };
        uint8_t ret = 0;
        for (auto c : value) {
            ret *= 10;
            ret += c - '0';
        }
        if (ids.find(ret) == ids.end()) {
            throw BmsException(
                std::string("in ") + LIBBMS_FUNCTION_SIGNATURE +
                ": Unknown BmsChannelId.");
        }
        return static_cast<BmsChannelId>(ret);
    }

    const type_info &DataUnitTypeOfChannel(BmsChannelId id) {
        if (id == BmsChannelId::Beat) {
            return typeid(BmsDecimalDataUnit);
        } else if (id == BmsChannelId::Bpm1) {
            return typeid(BmsIntegerListDataUnit);
        } else {
            return typeid(BmsReferenceListDataUnit);
        }
    }

    std::vector<BmsChannelId> GetBmsChannels(uint8_t keyCount, bool longNote, bool player1, bool o2Style) {
#define ID(n) static_cast<BmsChannelId>(n)
#define VEC(...) std::vector<BmsChannelId>({__VA_ARGS__})
        return keyCount == 7 ?
            (player1 ?
                (longNote ?
                    (o2Style ?
                        VEC(ID(56), ID(51), ID(52), ID(53), ID(54), ID(55), ID(58)) :
                        VEC(ID(51), ID(52), ID(53), ID(54), ID(55), ID(58), ID(59))) :
                    (o2Style ?
                        VEC(ID(16), ID(11), ID(12), ID(13), ID(14), ID(15), ID(18)) :
                        VEC(ID(11), ID(12), ID(13), ID(14), ID(15), ID(18), ID(19)))) :
                (longNote ?
                    (o2Style ?
                        VEC(ID(61), ID(62), ID(63), ID(64), ID(65), ID(68), ID(69)) :
                        VEC(ID(61), ID(62), ID(63), ID(64), ID(65), ID(68), ID(69))) :
                    (o2Style ?
                        VEC(ID(21), ID(22), ID(23), ID(24), ID(25), ID(28), ID(29)) :
                        VEC(ID(21), ID(22), ID(23), ID(24), ID(25), ID(28), ID(29))))) :
            VEC();
#undef VEC
#undef ID
    }

    bool IsShortNoteKeyChannel(BmsChannelId id) {
        auto value = _Detail::ToUnderlyingValue(id);
        return value > 10 && value < 30 && value != 20;
    }

    bool IsLongNoteKeyChannel(BmsChannelId id) {
        auto value = _Detail::ToUnderlyingValue(id);
        return value > 50 && value < 70 && value != 60;
    }

    bool IsKeyChannel(BmsChannelId id) {
        return IsShortNoteKeyChannel(id) || IsLongNoteKeyChannel(id);
    }

    std::istream &operator>>(std::istream &in, BmsChannelId &id) throw(BmsException) {
        char buffer[] = "00";
        in.get(buffer, sizeof(buffer));
        id = ConvertToBmsChannelId(buffer);
        return in;
    }

    std::ostream &operator<<(std::ostream &out, BmsChannelId id) {
        out << ConvertToStringValue(id);
        return out;
    }

}
