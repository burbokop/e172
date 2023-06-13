#include "random.h"
#include <cstdlib>
#include <random>

namespace e172 {

#ifdef __unix__
struct Random::Impl
{
    Impl(uint64_t seed)
        : m_seed(static_cast<unsigned int>(seed))
    {}

    unsigned int m_seed;
};
#else
struct Random::Impl {
    Impl(uint64_t seed)
        : m_gen(static_cast<std::mt19937::result_type>(seed))
    {}

    std::mt19937 m_gen;
};
#endif

Random::Random(uint64_t seed)
    : m_impl(std::make_unique<Impl>(seed))
{}

Random::~Random() = default;

uint64_t Random::nextU64()
{
#ifdef __unix__
    return rand_r(&m_impl->m_seed);
#else
    return m_impl->m_gen();
#endif
}

uint32_t Random::nextU32()
{
#ifdef __unix__
    return rand_r(&m_impl->m_seed);
#else
    return m_impl->m_gen();
#endif
}

uint16_t Random::nextU16()
{
#ifdef __unix__
    return rand_r(&m_impl->m_seed);
#else
    return m_impl->m_gen();
#endif
}

uint8_t Random::nextU8()
{
#ifdef __unix__
    return rand_r(&m_impl->m_seed);
#else
    return m_impl->m_gen();
#endif
}

} // namespace e172
