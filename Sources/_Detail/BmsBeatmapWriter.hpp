#pragma once
#ifndef LIBBMS_BMS_BEATMAP_WRITER_HPP_INCLUDED
#define LIBBMS_BMS_BEATMAP_WRITER_HPP_INCLUDED

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../BmsBeatmap.hpp"
#include "../BmsDataUnit.hpp"
#include "../BmsDecimalDataUnit.hpp"
#include "../BmsIntegerListDataUnit.hpp"
#include "../BmsReferenceListDataUnit.hpp"
#include "Utilities.hpp"

namespace Bms {
    namespace _Detail {

        struct BmsBeatmapWriter {
        private:
        public:
            std::ostream &operator()(std::ostream &out, const BmsBeatmap &beatmap) throw(BmsException) {
                using namespace std;
                out << "**********[HEADER FIELD]**********\n" << endl;
                out << "#PLAYER " << PromoteToPrintableIntegerType(ToUnderlyingValue(beatmap.GameMode)) << endl;
                out << "#GENRE " << beatmap.Genre << endl;
                out << "#TITLE " << beatmap.Title << endl;
                out << "#SUBTITLE " << beatmap.Subtitle << endl;
                out << "#ARTIST " << beatmap.Artist << endl;
                out << "#SUBARTIST " << beatmap.Subartist << endl;
                out << "#BPM " << beatmap.Bpm << endl;
                out << "#PLAYLEVEL " << PromoteToPrintableIntegerType(beatmap.Level) << endl;
                out << "#RANK " << PromoteToPrintableIntegerType(ToUnderlyingValue(beatmap.JudgmentDifficulty)) << endl;
                out << "#TOTAL " << beatmap.TotalHp << endl;
                out << "#STAGEFILE " << beatmap.Cover << endl;
                out << "#LNTYPE " << PromoteToPrintableIntegerType(ToUnderlyingValue(beatmap.LongNoteType)) << endl;
                out << "#LNOBJ " << beatmap.LongNoteFlag.StringValue() << endl;
                out << "#DIFFICULTY " << PromoteToPrintableIntegerType(ToUnderlyingValue(beatmap.Difficulty)) << endl;
                out << "\n********[DEFINATION FIELD]********\n" << endl;
                for (const auto &e : beatmap.WavMap) {
                    out << "#WAV" << e.first.StringValue() << " " << e.second << endl;
                }
                out << endl;
                for (const auto &e : beatmap.BmpMap) {
                    out << "#BMP" << e.first.StringValue() << ':' << e.second << endl;
                }
                out << endl;
                out << setprecision(15);
                for (const auto &e : beatmap.BpmMap) {
                    out << "#BPM" << e.first.StringValue() << " " << e.second << endl;
                }
                out << "\n*********[MAINDATA FIELD]*********\n\n";
                for (const auto &unit : beatmap.MainData) {
                    out << "#" << unit->Id.StringValue() << ':';
                    if (auto p = dynamic_pointer_cast<BmsDecimalDataUnit>(unit)) {
                        out << p->Value;
                    } else if (auto p = dynamic_pointer_cast<BmsIntegerListDataUnit>(unit)) {
                        out << hex << uppercase;
                        for (const auto &i : p->Value) {
                            out << i;
                        }
                    } else if (auto p = dynamic_pointer_cast<BmsReferenceListDataUnit>(unit)) {
                        for (const auto &id : p->Value) {
                            out << id;
                        }
                    }
                    out << endl;
                }
                return out;
            }
        };

    }
}

#endif // !LIBBMS_BMS_BEATMAP_WRITER_HPP_INCLUDED
