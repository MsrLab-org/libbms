#include "BmsBeatmap.hpp"

#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>

#include "BmsDataUnit.hpp"
#include "BmsDecimalDataUnit.hpp"
#include "BmsIntegerListDataUnit.hpp"
#include "BmsReferenceListDataUnit.hpp"
#include "Detail/Utilities.hpp"

namespace Bms {

    BmsBeatmap::BmsBeatmap() {}

    std::string BmsBeatmap::StringValue() const {
        using namespace std;
        using namespace _Detail;
        stringstream ss;
        ss << "**********[HEADER FIELD]**********\n" << endl;
        ss << "#PLAYER " << PromoteToPrintableIntegerType(ToUnderlyingValue(GameMode)) << endl;
        ss << "#GENRE " << Genre << endl;
        ss << "#TITLE " << Title << endl;
        ss << "#SUBTITLE " << Subtitle << endl;
        ss << "#ARTIST " << Artist << endl;
        ss << "#BPM " << Bpm << endl;
        ss << "#PLAYLEVEL " << PromoteToPrintableIntegerType(Level) << endl;
        ss << "#RANK " << PromoteToPrintableIntegerType(ToUnderlyingValue(JudgmentDifficulty)) << endl;
        ss << "#TOTAL " << TotalHp << endl;
        ss << "#STAGEFILE " << Cover << endl;
        ss << "#LNTYPE " << PromoteToPrintableIntegerType(ToUnderlyingValue(LongNoteType)) << endl;
        if (LongNoteType == BmsLongNoteType::Flag) {
            ss << "#LNOBJ " << LongNoteFlag.StringValue() << endl;
        }
        ss << "#DIFFICULTY " << PromoteToPrintableIntegerType(ToUnderlyingValue(Difficulty)) << endl;
        ss << "\n********[DEFINATION FIELD]********\n" << endl;
        for (const auto &e : WavMap) {
            ss << "#WAV" << e.first.StringValue() << " " << e.second << endl;
        }
        ss << endl;
        for (const auto &e : BmpMap) {
            ss << "#BMP" << e.first.StringValue() << ':' << e.second << endl;
        }
        ss << endl;
        ss << setprecision(15);
        for (const auto &e : BpmMap) {
            ss << "#BPM" << e.first.StringValue() << " " << e.second << endl;
        }
        ss << "\n*********[MAINDATA FIELD]*********\n\n";
        for (const auto &unit : MainData) {
            ss << "#" << unit->Id.StringValue() << ':';
            if (auto p = dynamic_pointer_cast<BmsDecimalDataUnit>(unit)) {
                ss << p->Value;
            } else if (auto p = dynamic_pointer_cast<BmsIntegerListDataUnit>(unit)) {
                ss << hex << uppercase;
                for (const auto &i : p->Value) {
                    ss << i;
                }
            } else if (auto p = dynamic_pointer_cast<BmsReferenceListDataUnit>(unit)) {
                for (const auto &id : p->Value) {
                    ss << id;
                }
            }
            ss << endl;
        }
        return ss.str();
    }

}
