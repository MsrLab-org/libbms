#pragma once
#ifndef LIBBMS_BMS_BEATMAP_READER_HPP_INCLUDED
#define LIBBMS_BMS_BEATMAP_READER_HPP_INCLUDED

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../BmsException.hpp"
#include "Utilities.hpp"

namespace Bms {
    namespace _Detail {

        struct BmsBeatmapReader {
        private:
            // Given string shuold be valid
            std::pair<std::string, std::string> ParseLine(const std::string &line) {
                size_t colon_index = line.find_first_of(':');
                size_t space_index = line.find_first_of(' ');
                size_t index = std::min(colon_index, space_index);
                auto key = line.substr(1, index - 1);
                ++index;
                auto value = line.substr(index);
                return std::make_pair(key, value);
            }
            bool IsValid(const std::string &line) {
                return line.size() >= 2 && line[0] == '#';
            }
            bool IsDataUnitId(const std::string &key) {
                return key.size() == 5 &&
                    std::all_of(key.begin(), key.end(),
                        [](const char &c) { return c >= '0' && c <= '9'; });
            }
        public:
            std::istream &operator()(std::istream &in, BmsBeatmap &beatmap) throw(BmsException) {
                using namespace std;
                beatmap = BmsBeatmap();
                bool success = true;
                string description = string("in ") + LIBBMS_FUNCTION_SIGNATURE + ": ";
                string line;
                while (getline(in, line)) {
                    if (!IsValid(line)) {
                        continue;
                    }
                    const auto kv = ParseLine(line);
                    const string &key = kv.first;
                    const string &value = kv.second;
                    if (key == "PLAYER") {
                        beatmap.GameMode = ToEnum<BmsGameMode>(value);
                    } else if (key == "GENRE") {
                        beatmap.Genre = value;
                    } else if (key == "TITLE") {
                        beatmap.Title = value;
                    } else if (key == "SUBTITLE") {
                        beatmap.Subtitle = value;
                    } else if (key == "ARTIST") {
                        beatmap.Artist = value;
                    } else if (key == "SUBARTIST") {
                        beatmap.Subartist = value;
                    } else if (key == "BPM") {
                        beatmap.Bpm = To<decltype(beatmap.Bpm)>(value);
                    } else if (key == "PLAYLEVEL") {
                        beatmap.Level = To<decltype(beatmap.Level)>(value);
                    } else if (key == "RANK") {
                        beatmap.JudgmentDifficulty = ToEnum<BmsJudgmentDifficulty>(value);
                    } else if (key == "TOTAL") {
                        beatmap.TotalHp = To<decltype(beatmap.TotalHp)>(value);
                    } else if (key == "STAGEFILE") {
                        beatmap.Cover = value;
                    } else if (key == "LNTYPE") {
                        beatmap.LongNoteType = ToEnum<BmsLongNoteType>(value);
                    } else if (key == "LNOBJ") {
                        beatmap.LongNoteFlag = value;
                    } else if (key == "DIFFICULTY") {
                        beatmap.Difficulty = ToEnum<BmsDifficulty>(value);
                    } else if (key.size() == 5 && key.substr(0, 3) == "WAV") {
                        BmsReferenceId id(key.substr(3, 2));
                        beatmap.WavMap[id] = value;
                    } else if (key.size() == 5 && key.substr(0, 3) == "BMP") {
                        BmsReferenceId id(key.substr(3, 2));
                        beatmap.BmpMap[id] = value;
                    } else if (key.size() == 5 && key.substr(0, 3) == "BPM") {
                        BmsReferenceId id(key.substr(3, 2));
                        beatmap.BpmMap[id] = To<decltype(beatmap.BpmMap)::mapped_type>(value);
                    } else if (IsDataUnitId(key)) {
                        BmsDataUnitId id(key);
                        if (id.ChannelPart() == BmsChannelId::Beat) {
                            auto unit = std::make_shared<BmsDecimalDataUnit>(id);
                            unit->Value = To<decltype(unit->Value)>(value);
                            beatmap.MainData.push_back(unit);
                        } else if (id.ChannelPart() == BmsChannelId::Bpm1) {
                            auto unit = std::make_shared<BmsIntegerListDataUnit>(id);
                            unit->SetValue(value);
                            beatmap.MainData.push_back(unit);
                        } else {
                            auto unit = std::make_shared<BmsReferenceListDataUnit>(id);
                            unit->SetValue(value);
                            beatmap.MainData.push_back(unit);
                        }
                    } else {
                        success = false;
                        description += "Unknown Key type: " + key;
                        break;
                    }
                }
                if (!success) {
                    throw BmsException(description);
                }
                return in;
            }
        };

    }
}

#endif // !LIBBMS_BMS_BEATMAP_READER_HPP_INCLUDED
