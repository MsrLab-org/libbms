#pragma once
#ifndef LIBBMS_BMS_CHANNEL_ID_HPP_INCLUDED
#define LIBBMS_BMS_CHANNEL_ID_HPP_INCLUDED

#include <cstdint>
#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

#include "BmsException.hpp"

namespace Bms {

    enum class BmsChannelId: uint8_t {
        Bgm = 1,
        Beat = 2, // Beat provided by decimal number, Double, >0
        Bpm1 = 3, // BPM provided by raw number, Int, Hex, 0~FF
        Bga = 4,
        MissHint = 6,
        Layer = 7,
        Bpm2 = 8, // BPM provided by reference, Int, Base36, 0~ZZ
        Pause = 9,
        Key1_1P = 11,
        Key2_1P = 12,
        Key3_1P = 13,
        Key4_1P = 14,
        Key5_1P = 15,
        Key6_1P = 18,
        Key7_1P = 19,
        Scratch_1P = 16,
        Key1_2P = 21,
        Key2_2P = 22,
        Key3_2P = 23,
        Key4_2P = 24,
        Key5_2P = 25,
        Key6_2P = 28,
        Key7_2P = 29,
        Scratch_2P = 26,
        Key1Long_1P = 51, // Available when LongNoteType(LNTYPE) == NotePair(1)
        Key2Long_1P = 52,
        Key3Long_1P = 53,
        Key4Long_1P = 54,
        Key5Long_1P = 55,
        Key6Long_1P = 56,
        Key7Long_1P = 57,
        ScratchLong_1P = 58,
        Key1Long_2P = 61,
        Key2Long_2P = 62,
        Key3Long_2P = 63,
        Key4Long_2P = 64,
        Key5Long_2P = 65,
        Key6Long_2P = 66,
        Key7Long_2P = 67,
        ScratchLong_2P = 69,
        Key1 = Key1_1P,
        Key2 = Key2_1P,
        Key3 = Key3_1P,
        Key4 = Key4_1P,
        Key5 = Key5_1P,
        Key6 = Key6_1P,
        Key7 = Key7_1P,
        Scratch = Scratch_1P,
        Key1Long = Key1Long_1P,
        Key2Long = Key2Long_1P,
        Key3Long = Key3Long_1P,
        Key4Long = Key4Long_1P,
        Key5Long = Key5Long_1P,
        Key6Long = Key6Long_1P,
        Key7Long = Key7Long_1P,
        ScratchLong = ScratchLong_2P
    };

    std::string ConvertToStringValue(BmsChannelId);
    BmsChannelId ConvertToBmsChannelId(std::string value) throw(BmsException);

    const type_info &DataUnitTypeOfChannel(BmsChannelId id);
    std::vector<BmsChannelId> GetBmsChannels(uint8_t keyCount, bool longNote, bool player1 = true, bool o2Style = false);
    bool IsShortNoteKeyChannel(BmsChannelId id);
    bool IsLongNoteKeyChannel(BmsChannelId id);
    bool IsKeyChannel(BmsChannelId id);

    std::istream &operator>>(std::istream &in, BmsChannelId &id) throw(BmsException);
    std::ostream &operator<<(std::ostream &out, BmsChannelId id);

}

#endif // !LIBBMS_BMS_CHANNEL_ID_HPP_INCLUDED
