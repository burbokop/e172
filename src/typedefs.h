#pragma once

#include <inttypes.h>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <complex>

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
