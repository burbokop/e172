// Copyright 2023 Borys Boiko

#include "smartenum.h"

#include <algorithm>
#include <limits>

namespace e172::smartenum {

namespace {

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

} // namespace

Meta Meta::fromVaArgs(const std::string &str)
{
    auto vec = split(str, ',');
    std::map<UnderlyingType, std::string> result;
    for (size_t i = 0,
                count = std::min(vec.size(),
                                 static_cast<std::size_t>(
                                     std::numeric_limits<UnderlyingType>::max()));
         i < count;
         ++i) {
        result.insert({static_cast<std::uint32_t>(i), trim(vec[i])});
    }
    return Meta(result);
}

} // namespace e172::smartenum
