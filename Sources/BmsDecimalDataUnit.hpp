#pragma once
#ifndef LIBBMS_BMS_DECIMAL_DATA_UNIT_HPP_INCLUDED
#define LIBBMS_BMS_DECIMAL_DATA_UNIT_HPP_INCLUDED

#include "BmsDataUnit.hpp"

namespace Bms {

    // Beat channel(2)
    class BmsDecimalDataUnit: public BmsDataUnit {
    public:
        using SelfType = BmsDecimalDataUnit;
        using ValueType = double;
    public:
        BmsDecimalDataUnit(const IdType &id);
    public:
        ~BmsDecimalDataUnit();
    public:
        ValueType Value;
    };

}

#endif // !LIBBMS_BMS_DECIMAL_DATA_UNIT_HPP_INCLUDED
