#include "BmsBeatmap.hpp"

#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#include "BmsDataUnit.hpp"
#include "BmsDecimalDataUnit.hpp"
#include "BmsException.hpp"
#include "BmsIntegerListDataUnit.hpp"
#include "BmsReferenceListDataUnit.hpp"
#include "_Detail/BmsBeatmapReader.hpp"
#include "_Detail/BmsBeatmapWriter.hpp"
#include "_Detail/Utilities.hpp"

namespace Bms {

    BmsBeatmap::BmsBeatmap() {}

    std::istream &operator>>(std::istream &in, BmsBeatmap &beatmap) {
        return _Detail::BmsBeatmapReader()(in, beatmap);
    }

    std::ostream &operator<<(std::ostream &out, const BmsBeatmap &beatmap) {
        return _Detail::BmsBeatmapWriter()(out, beatmap);;
    }

    void BmsBeatmap::SortMainData() {
        std::sort(MainData.begin(), MainData.end(),
            [](std::shared_ptr<BmsDataUnit> a, std::shared_ptr<BmsDataUnit> b) {
                return a->Id < b->Id;
            });
    }

    namespace _Detail {

        void ConvertMainDataToLongNoteTypeNotePair(BmsBeatmap &beatmap) {
            decltype(beatmap.MainData) newMainData;
            std::string description = std::string("in ") + LIBBMS_FUNCTION_SIGNATURE;
            if (beatmap.MainData.empty()) {
                return;
            }
            // 0~10
            std::vector<std::shared_ptr<BmsDataUnit>> frontData;
            // 11~19, 21~29 -(-11)-> 0~8, 10~18
            std::array<std::shared_ptr<BmsReferenceListDataUnit>, 19> data;
            // 30~50
            std::vector<std::shared_ptr<BmsDataUnit>> bodyData;
            // 51~59, 61~69 -(-51)-> 0~8, 10~18
            std::array<std::shared_ptr<BmsReferenceListDataUnit>, 19> lnData;
            // 70~
            std::vector<std::shared_ptr<BmsDataUnit>> backData;
            std::array<bool, 19> inLn;
            std::array<std::pair<std::shared_ptr<BmsReferenceListDataUnit>, size_t>, 19> lnRear;
            inLn.fill(false);
            lnRear.fill({nullptr, 0});
            const auto lnObject = beatmap.LongNoteFlag;
            auto bar = beatmap.MainData.back()->Id.SectionPart();
            for (auto it = beatmap.MainData.crbegin(); it != beatmap.MainData.crend(); ++it) {
                auto unit = *it;
                if (unit->Id.SectionPart() < bar || unit == beatmap.MainData.front()) {
                    for (auto jt = backData.cbegin(); jt != backData.cend(); ++jt) {
                        if (auto p = *jt) {
                            newMainData.push_back(p);
                        }
                    }
                    backData.clear();
                    for (auto jt = lnData.crbegin(); jt != lnData.crend(); ++jt) {
                        if (auto p = *jt) {
                            newMainData.push_back(p);
                        }
                    }
                    lnData.fill(nullptr);
                    for (auto jt = bodyData.cbegin(); jt != bodyData.cend(); ++jt) {
                        if (auto p = *jt) {
                            newMainData.push_back(p);
                        }
                    }
                    bodyData.clear();
                    for (auto jt = data.crbegin(); jt != data.crend(); ++jt) {
                        if (auto p = *jt) {
                            newMainData.push_back(p);
                        }
                    }
                    data.fill(nullptr);
                    for (auto jt = frontData.cbegin(); jt != frontData.cend(); ++jt) {
                        if (auto p = *jt) {
                            newMainData.push_back(p);
                        }
                    }
                    frontData.clear();
                }
                auto channel = ToUnderlyingValue(unit->Id.ChannelPart());
                if (channel < 11) {
                    frontData.push_back(unit);
                } else if (channel < 30) {
                    if (auto u = std::dynamic_pointer_cast<BmsReferenceListDataUnit>(unit)) {
                        auto newUnit = std::make_shared<BmsReferenceListDataUnit>(u->Id);
                        newUnit->Value = u->Value;
                        for (size_t ir = 0; ir < u->Value.size(); ++ir) {
                            size_t i = u->Value.size() - ir - 1;
                            auto ref = u->Value[i];
                            if (!ref.IsPlaceholder()) {
                                if (ref == lnObject) {
                                    auto &lnUnit = lnData[channel - 11];
                                    if (!lnUnit) {
                                        lnUnit = std::make_shared<BmsReferenceListDataUnit>(
                                            BmsDataUnitId(
                                                u->Id.SectionPart(),
                                                static_cast<BmsChannelId>(channel + 40)));
                                        lnUnit->Value.assign(u->Value.size(), BmsReferenceId::Placeholder());
                                    }
                                    inLn[channel - 11] = true;
                                    lnRear[channel - 11] = {lnUnit, i};
                                    newUnit->Value[i] = BmsReferenceId::Placeholder();
                                } else if (inLn[channel - 11]) {
                                    auto &lnUnit = lnData[channel - 11];
                                    if (!lnUnit) {
                                        lnUnit = std::make_shared<BmsReferenceListDataUnit>(
                                            BmsDataUnitId(
                                                u->Id.SectionPart(),
                                                static_cast<BmsChannelId>(channel + 40)));
                                        lnUnit->Value.assign(u->Value.size(), BmsReferenceId::Placeholder());
                                    }
                                    lnUnit->Value[i] = u->Value[i];
                                    auto pos = lnRear[channel - 11];
                                    pos.first->Value[pos.second] = u->Value[i];
                                    inLn[channel - 11] = false;
                                    lnRear[channel - 11] = {nullptr, 0};
                                    newUnit->Value[i] = BmsReferenceId::Placeholder();
                                }
                            }
                        }
                        data[channel - 11] = newUnit;
                    } else {
                        throw BmsException(description + ": The data type of key channel must be reference list.");
                    }
                } else if (channel < 51) {
                    bodyData.push_back(unit);
                } else if (channel < 70) {
                    throw BmsException(description + ": Mixed long note type conversion is not supported.");
                } else {
                    backData.push_back(unit);
                }
            }
            std::reverse(newMainData.begin(), newMainData.end());
            for (auto &unit : newMainData) {
                if (auto p = std::dynamic_pointer_cast<BmsReferenceListDataUnit>(unit)) {
                    p->Shrink();
                }
            }
            beatmap.MainData = newMainData;
        }

        void ConvertMainDataToLongNoteTypeFlag(BmsBeatmap &beatmap) {

        }

    }

    void BmsBeatmap::ConvertMainDataToLongNoteType(BmsLongNoteType type, bool sorted) throw(BmsException) {
        using namespace _Detail;
        if (!sorted) {
            SortMainData();
        }
        switch (type) {
        case BmsLongNoteType::NotePair:
            ConvertMainDataToLongNoteTypeNotePair(*this);
            break;
        case BmsLongNoteType::Flag:
            ConvertMainDataToLongNoteTypeFlag(*this);
            break;
        default:
            throw BmsException(std::string("in ") + LIBBMS_FUNCTION_SIGNATURE + ": Unknown LongNoteType("
                + std::to_string(ToUnderlyingValue(type)) + ")");
        }
    }

}
