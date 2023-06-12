#include "intergrator.h"

e172::Intergrator::Intergrator(double alpha) {
    m_alpha = alpha;
}

double e172::Intergrator::proceed(double value, size_t channel) {
    auto c = static_cast<std::vector<ChannelState>::size_type>(channel);
    if(c >= channelStateArray.size())
        channelStateArray.resize(c + 1);

    if (channelStateArray[channel].frst) {
        channelStateArray[channel].frst = false;
        return (channelStateArray[channel].lastY = value);
    }
    return (channelStateArray[channel].lastY = m_alpha * channelStateArray[channel].lastY + (1 - m_alpha) * value);
}

e172::Complex<double> e172::Intergrator::proceed(const Complex<double> &value, size_t channel)
{
    auto c = static_cast<std::vector<ChannelState>::size_type>(channel);
    if(c >= channelStateArray.size())
        channelStateArray.resize(c + 1);

    if (channelStateArray[channel].frst) {
        channelStateArray[channel].frst = false;
        return (channelStateArray[channel].lastComplexY = value);
    }

    return (channelStateArray[channel].lastComplexY = Complex<double>{m_alpha, 0}
                                                          * channelStateArray[channel].lastComplexY
                                                      + Complex<double>{1 - m_alpha, 0} * value);
}
