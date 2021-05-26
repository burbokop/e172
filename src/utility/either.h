#ifndef EITHER_H
#define EITHER_H

#include <variant>




template<typename T>
struct Either {
    class Error {

    };
private:
    std::variant<Error, T> m_data;
public:
    Either(const T &value) {
        m_data = value;
    }
    Either(const Error &value) {
        m_data = value;
    }
};

#endif // EITHER_H
