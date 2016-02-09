#pragma once
#ifndef LIBBMS_BMS_REFERENCE_LIST_DATA_UNIT_HPP_INCLUDED
#define LIBBMS_BMS_REFERENCE_LIST_DATA_UNIT_HPP_INCLUDED

#include <vector>

#include "BmsDataUnit.hpp"
#include "BmsReferenceId.hpp"

namespace Bms {

    // Other Channels
    class BmsReferenceListDataUnit: public BmsDataUnit {
    public:
        using SelfType = BmsReferenceListDataUnit;
        using ValueType = std::vector<BmsReferenceId>;
    public:
        BmsReferenceListDataUnit(const IdType &id);
        void Shrink();
    public:
        ~BmsReferenceListDataUnit();
    public:
        ValueType Value;
    };

}

#endif // !LIBBMS_BMS_REFERENCE_LIST_DATA_UNIT_HPP_INCLUDED
