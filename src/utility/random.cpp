#include "random.h"
#include <cstdlib>

namespace e172 {

struct Random::Impl
{
    Impl(unsigned int seed)
        : m_seed(seed)
    {}

    unsigned int m_seed;
};

Random::Random(uint64_t seed)
    : m_impl(std::make_unique<Impl>(seed))
{}

Random::~Random() = default;

uint64_t Random::nextU64()
{
    return rand_r(&m_impl->m_seed);
}

uint32_t Random::nextU32()
{
    return rand_r(&m_impl->m_seed);
}

uint16_t Random::nextU16()
{
    return rand_r(&m_impl->m_seed);
}

uint8_t Random::nextU8()
{
    return rand_r(&m_impl->m_seed);
}

} // namespace e172
