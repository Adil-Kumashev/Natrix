#include <string_view>


#pragma once


namespace ANSI {
    static constexpr std::string_view Green = "\e[1;32m";
    static constexpr std::string_view Red = "\e[1;31m";

    static constexpr std::string_view End = "\e[0m";
}