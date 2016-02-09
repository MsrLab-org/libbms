#include "BmsReferenceListDataUnit.hpp"

#include "Detail/Utilities.hpp"

namespace Bms {

    BmsReferenceListDataUnit::BmsReferenceListDataUnit(const IdType &id)
        : BmsDataUnit(id) {}

    void BmsReferenceListDataUnit::Shrink() {
        using namespace std;
        const auto size = Value.size();
        auto gcd = size;
        for (size_t i = 0; i < size; ++i) {
            cout << '|' << i << '.' << Value[i].StringValue();
        }
        cout << endl;
        for (size_t i = 1; i < size; ++i) {
            if (Value[i].UnderlyingValue() != 0) {
                cout << "GCD: " << gcd;
                gcd = _Detail::Gcd(gcd, i);
                cout << " => " << gcd << "(" << i << ")" << endl;
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
