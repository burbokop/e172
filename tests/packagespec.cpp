#include "packagespec.h"

#include <src/utility/package.h>

namespace e172::tests {

class TestWrite : public Write
{
    // Write interface
public:
    std::size_t write(const uint8_t *src, std::size_t size) override
    {
        m_data.insert(m_data.end(), src, src + size);
        return size;
    }

    const Bytes &data() const { return m_data; }

private:
    Bytes m_data;
};

class TestRead : public Read
{
public:
    TestRead(const Bytes &data)
        : m_data(data)
    {}

    // Read interface
public:
    std::size_t bufferize() override
    {
        if (m_data.size() > 0) {
            m_bufferizedData = m_data;
            m_data = {};
            return m_bufferizedData.size();
        } else {
            return 0;
        }
    }

    std::size_t bytesAvailable() const override { return m_bufferizedData.size() - m_pos; }

    std::size_t read(Byte *dst, std::size_t size) override
    {
        const auto s = std::min(bytesAvailable(), size);
        if (s > 0) {
            for (std::size_t i = 0; i < s; ++i) {
                dst[i] = m_bufferizedData[m_pos + i];
            }
            m_pos += s;
            return s;
        } else {
            return 0;
        }
    }

    std::size_t peek(Byte *dst, std::size_t size) const override
    {
        const auto s = std::min(bytesAvailable(), size);
        if (s > 0) {
            for (std::size_t i = 0; i < s; ++i) {
                dst[i] = m_bufferizedData[m_pos + i];
            }
            return s;
        } else {
            return 0;
        }
    }

private:
    std::size_t m_pos = 0;
    Bytes m_bufferizedData;
    Bytes m_data;
};

void PackageSpec::writePackageTest()
{
    TestWrite w;

    const auto size = WritePackage::pack(w, 1, [](WritePackage p) {
        p.write<std::uint8_t>(2);
        p.write<std::uint16_t>(4);
        p.write<std::uint32_t>(8);
    });
    e172_shouldEqual(size, 4 + 2 + 1 + 2 + 4);
    e172_shouldEqual(w.data(), e172_initializerList(Bytes, 0, 0, 0, 7, 0, 1, 2, 0, 4, 0, 0, 0, 8));
}

void PackageSpec::readPackageTest()
{
    TestRead r(e172_initializerList(Bytes, 0, 0, 0, 7, 0, 1, 2, 0, 4, 0, 0, 0, 8));

    const auto ok = ReadPackage::pull(r, [](ReadPackage p) {
        e172_shouldEqual(p.type(), 1);
        e172_shouldEqual(p.bytesAvailable(), 7);
        e172_shouldEqual(p.read<std::uint8_t>().value(), 2);
        e172_shouldEqual(p.bytesAvailable(), 6);
        e172_shouldEqual(p.read<std::uint16_t>().value(), 4);
        e172_shouldEqual(p.bytesAvailable(), 4);
        e172_shouldEqual(p.read<std::uint32_t>().value(), 8);
        e172_shouldEqual(p.bytesAvailable(), 0);
    });
    e172_shouldEqual(ok, true);
}

void PackageSpec::readPackageTestFail()
{
    TestRead r(e172_initializerList(Bytes, 0, 0, 0, 7, 0, 1, 2, 0, 4, 0, 0, 0));

    bool closureCalled = false;
    const auto ok = ReadPackage::pull(r, [&closureCalled](ReadPackage p) { closureCalled = true; });

    e172_shouldEqual(ok, false);
    e172_shouldEqual(closureCalled, false);
}

void PackageSpec::readWritePackageTest()
{
    TestWrite w;

    (void) WritePackage::pack(w, 1, [](WritePackage p) {
        p.write<std::uint8_t>(2);
        p.write<std::uint16_t>(4);
        p.write<std::uint32_t>(8);
    });

    TestRead r(w.data());

    const auto ok = ReadPackage::pull(r, [](ReadPackage p) {
        e172_shouldEqual(p.type(), 1);
        e172_shouldEqual(p.bytesAvailable(), 7);
        e172_shouldEqual(p.read<std::uint8_t>().value(), 2);
        e172_shouldEqual(p.bytesAvailable(), 6);
        e172_shouldEqual(p.read<std::uint16_t>().value(), 4);
        e172_shouldEqual(p.bytesAvailable(), 4);
        e172_shouldEqual(p.read<std::uint32_t>().value(), 8);
        e172_shouldEqual(p.bytesAvailable(), 0);
    });
    e172_shouldEqual(ok, true);
}

} // namespace e172::tests
