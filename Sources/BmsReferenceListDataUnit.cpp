#include "BmsReferenceListDataUnit.hpp"

#include "_Detail/Utilities.hpp"

namespace Bms {

    BmsReferenceListDataUnit::BmsReferenceListDataUnit(const IdType &id)
        : BmsDataUnit(id) {}

    void BmsReferenceListDataUnit::SetValue(const std::string &value) throw(BmsException) {
        using namespace std;
        using namespace _Detail;
        if (value.size() % 2 != 0) {
            throw BmsException(string("in ") + LIBBMS_FUNCTION_SIGNATURE + ": Wrong format.");
        }
        ValueType newValue;
        for (size_t i = 0; i < value.size(); i += 2) {
            ValueType::value_type id(value.substr(i, 2));
            newValue.push_back(id);
        }
        Value = newValue;
    }

    void BmsReferenceListDataUnit::Shrink() {
        using namespace std;
        const auto size = Value.size();
        auto gcd = size;
        for (size_t i = 1; i < size; ++i) {
            if (Value[i].UnderlyingValue() != 0) {
                gcd = _Detail::Gcd(gcd, i);
            }
        }
        vector<BmsReferenceId> ids;
        for (size_t i = 0; i < size; i += gcd) {
            ids.push_back(Value[i]);
        }
        Value = move(ids);
    }

    BmsReferenceListDataUnit::~BmsReferenceListDataUnit() {}

}
