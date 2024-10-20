#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include <cmrc/cmrc.hpp>

CMRC_DECLARE(bifrost);

constexpr auto SCREEN_WIDTH = 1620;
constexpr auto SCREEN_HEIGHT = 2160;

constexpr auto ENV_DEBUG = "BIFROST_DEBUG";

inline std::vector<uint8_t> get_resource_file(const std::string& filename)
{
    auto fs = cmrc::bifrost::get_filesystem();
    auto file = fs.open(filename);
    return {file.begin(), file.end()};
}

#endif // CONSTANTS_H