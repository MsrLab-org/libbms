#include "BmsException.hpp"

namespace Bms {

    BmsException::BmsException(const std::string &description)
    : Desc(description) {}

    const std::string &BmsException::Description() {
        return Desc;
    }

}
