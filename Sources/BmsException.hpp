#pragma once
#ifndef LIBBMS_BMS_EXCEPTION_HPP_INCLUDED
#define LIBBMS_BMS_EXCEPTION_HPP_INCLUDED

#include <string>

namespace Bms {

    class BmsException {
    public:
        BmsException(const std::string &description);
    public:
        const std::string &Description() const;
    private:
        std::string _Description;
    };

}

#endif // !LIBBMS_BMS_EXCEPTION_HPP_INCLUDED
