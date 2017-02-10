#pragma once
#ifndef LIBBMS_BMS_BEATMAP_HPP_INCLUDED
#define LIBBMS_BMS_BEATMAP_HPP_INCLUDED

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "BmsDataUnit.hpp"
#include "BmsReferenceId.hpp"

namespace Bms {

    enum class BmsGameMode: uint8_t {
        SinglePlay = 1,
        Cooperation = 2,
        DoublePlay = 3,
        SinglePlay9Key = 4,
        SinglePlay13Key = 5,
        SinglePlay7Key = SinglePlay
    };

    enum class BmsDifficulty: uint8_t {
        Beginner = 1,
        Normal = 2,
        Hyper = 3,
        Another = 4,
        Insane = 5
    };

    enum class BmsJudgmentDifficulty: uint8_t {
        VeryHard = 0,
        Hard = 1,
        Normal = 2,
        Easy = 3
    };

    enum class BmsLongNoteType: uint8_t {
        NotePair = 1, // Independent channel
        Flag = 2      // Short note channel
    };

    class BmsBeatmap {
    public:
        using SelfType = BmsBeatmap;
    public:
        BmsBeatmap();
    public:
        friend std::istream &operator>>(std::istream &in, SelfType &beatmap);
        friend std::ostream &operator<<(std::ostream &out, const SelfType &beatmap);
    public:
        void SortMainData();
        void ConvertMainDataToLongNoteType(BmsLongNoteType type, bool sorted = false) throw(BmsException);
    public:
        // Header field
        BmsGameMode GameMode = BmsGameMode::SinglePlay; // #PLAYER
        std::string Genre = ""; // #GENRE
        std::string Title = ""; // #TITLE
        std::string Subtitle = ""; // #SUBTITLE
        std::string Artist = ""; // #ARTIST
        std::string Subartist = ""; // #SUBARTIST
        double Bpm = 0; // #BPM
        intmax_t Level = 0; // PLAYLEVEL
        BmsJudgmentDifficulty JudgmentDifficulty = BmsJudgmentDifficulty::Easy; // #RANK
        uintmax_t TotalHp = 0; // #TOTAL
        std::string Cover = ""; // #STAGEFILE
        BmsLongNoteType LongNoteType = BmsLongNoteType::NotePair; // #LNTYPE
        BmsReferenceId LongNoteFlag = std::string("ZZ"); // #LNOBJ, Available when LongNoteType(LNTYPE) == Flag(2)
        BmsDifficulty Difficulty = BmsDifficulty::Beginner; // #DIFFICULTY
        // Defination field
        std::map<BmsReferenceId, std::string> WavMap;
        std::map<BmsReferenceId, std::string> BmpMap;
        std::map<BmsReferenceId, double> BpmMap;
        // Main data field
        std::vector<std::shared_ptr<BmsDataUnit>> MainData;
    };

}

#endif // !LIBBMS_BMS_BEATMAP_HPP_INCLUDED
