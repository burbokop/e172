// Copyright 2023 Borys Boiko

#include "intergrator.h"

double e172::Intergrator::proceed(double value, size_t channel)
{
    auto c = static_cast<std::vector<ChannelState>::size_type>(channel);
    if (c >= m_channelStateArray.size())
        m_channelStateArray.resize(c + 1);

    if (m_channelStateArray[channel].first) {
        m_channelStateArray[channel].first = false;
        return (m_channelStateArray[channel].lastY = value);
    }
    return (m_channelStateArray[channel].lastY = m_alpha * m_channelStateArray[channel].lastY
                                                 + (1 - m_alpha) * value);
}

e172::Complex<double> e172::Intergrator::proceed(const Complex<double> &value, size_t channel)
{
    auto c = static_cast<std::vector<ChannelState>::size_type>(channel);
    if (c >= m_channelStateArray.size())
        m_channelStateArray.resize(c + 1);

    if (m_channelStateArray[channel].first) {
        m_channelStateArray[channel].first = false;
        return (m_channelStateArray[channel].lastComplexY = value);
    }

    return (
        m_channelStateArray[channel].lastComplexY = Complex<double>{m_alpha, 0}
                                                        * m_channelStateArray[channel].lastComplexY
                                                    + Complex<double>{1 - m_alpha, 0} * value);
}
