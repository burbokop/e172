// Copyright 2023 Borys Boiko

#pragma once

#include <functional>

namespace e172 {

class SharedContainer {
public:
    struct BaseHandle
    {
        virtual ~BaseHandle() = default;
    };

    template<typename T>
    struct Handle : public BaseHandle
    {
        T c;

        Handle(const T &v)
            : c(v)
        {}
        Handle() = default;
    };

    using Ptr = const void *;
    using DataPtr = const BaseHandle *;
    using VoidHandle = Handle<void *>;
    using Destructor = std::function<void(DataPtr)>;

    template<typename T>
    static Handle<T> *castHandle(const BaseHandle *ptr)
    {
        return const_cast<Handle<T> *>(dynamic_cast<const Handle<T> *>(ptr));
    }

protected:
    template<typename T>
    Handle<T> *handleAs() const
    {
        return castHandle<T>(m_data);
    }

    Ptr provider() const { return m_provider; }

    template<typename T>
    static T createSharedContainer(DataPtr data, Ptr provider, Destructor destructor)
    {
        T i;
        i.m_refCountPtr = new int(1);
        i.m_data = data;
        i.m_provider = provider;
        i.m_destructor = destructor;
        return i;
    }
    DataPtr data() const { return m_data; }
    void setData(const DataPtr &data) { m_data = data; }

public:
    SharedContainer() = default;
    SharedContainer(const SharedContainer &obj);
    void operator=(const SharedContainer &obj);
    ~SharedContainer() { detach(); }

    bool isValid() const { return m_data != nullptr; }
    bool isNull() const { return m_data == nullptr; }

private:
    DataPtr m_data = nullptr;
    Ptr m_provider = nullptr;
    Destructor m_destructor;
    int *m_refCountPtr = nullptr;

    void detach();
};

} // namespace e172
