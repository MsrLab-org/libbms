#pragma once
#ifndef LIBBMS_BMS_INTEGER_LIST_DATA_UNIT_HPP_INCLUDED
#define LIBBMS_BMS_INTEGER_LIST_DATA_UNIT_HPP_INCLUDED

#include <cstdint>
#include <vector>

#include "BmsDataUnit.hpp"

namespace Bms {

    // Bpm1 channel(3)
    class BmsIntegerListDataUnit: public BmsDataUnit {
    public:
        using SelfType = BmsIntegerListDataUnit;
        using ValueType = std::vector<uint8_t>;
    public:
        BmsIntegerListDataUnit(const IdType &id);
    public:
        ~BmsIntegerListDataUnit();
    public:
        ValueType Value;
    };

}

#endif // !LIBBMS_BMS_INTEGER_LIST_DATA_UNIT_HPP_INCLUDED
