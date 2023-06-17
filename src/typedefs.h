// Copyright 2023 Borys Boiko

#pragma once

#include <complex>
#include <inttypes.h>
#include <list>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace e172 {

using String = std::string;
using StringList = std::list<String>;
using StringVector = std::vector<String>;
using StringSet = std::set<String>;

class Variant;
using VariantVector = std::vector<Variant>;
using VariantList = std::list<Variant>;
using VariantMap = std::map<std::string, Variant>;

template<typename T>
using Complex = std::complex<T>;

} // namespace e172
