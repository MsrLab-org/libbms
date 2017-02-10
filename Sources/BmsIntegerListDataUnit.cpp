#include "BmsIntegerListDataUnit.hpp"

#include "_Detail/Utilities.hpp"

namespace Bms {

    BmsIntegerListDataUnit::BmsIntegerListDataUnit(const IdType &id)
    : BmsDataUnit(id) {}

    void BmsIntegerListDataUnit::SetValue(const std::string &value) throw(BmsException) {
        using namespace std;
        using namespace _Detail;
        if (value.size() % 2 != 0 || !IsHexString(value)) {
            throw BmsException(string("in ") + LIBBMS_FUNCTION_SIGNATURE + ": Wrong format.");
        }
        ValueType newValue;
        for (size_t i = 0; i < value.size(); i += 2) {
            ValueType::value_type n;
            auto s = value.substr(i, 2);
            n = HexCharToUint8(s[0]) * 16 + HexCharToUint8(s[1]);
            newValue.push_back(n);
        }
        Value = newValue;
    }

    BmsIntegerListDataUnit::~BmsIntegerListDataUnit() {}

}
