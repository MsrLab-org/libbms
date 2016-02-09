#pragma once
#ifndef LIBBMS_BMS_DATA_UNIT_HPP_INCLUDED
#define LIBBMS_BMS_DATA_UNIT_HPP_INCLUDED

#include "BmsDataUnitId.hpp"

namespace Bms {

    class BmsDataUnit {
    public:
        using SelfType = BmsDataUnit;
        using IdType = BmsDataUnitId;
    public:
        BmsDataUnit(const IdType &id);
    public:
        virtual ~BmsDataUnit() = 0;
    public:
        IdType Id;
    };

}

#endif // !LIBBMS_BMS_DATA_UNIT_HPP_INCLUDED
