#include "BmsException.hpp"

namespace Bms {

    BmsException::BmsException(const std::string &description)
    : _Description(description) {}

    const std::string &BmsException::Description() const {
        return _Description;
    }

}
