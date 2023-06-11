#pragma once

#include <cstdint>
#include <memory>

namespace e172 {

class Random
{
public:
    Random(std::uint64_t seed);
    ~Random();

    std::uint64_t nextU64();
    std::uint32_t nextU32();
    std::uint16_t nextU16();
    std::uint8_t nextU8();

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace e172
