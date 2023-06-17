// Copyright 2023 Borys Boiko

#pragma once

#include <cstdint>
#include <functional>
#include <queue>

namespace e172 {

class PriorityProcedure {
public:
    PriorityProcedure() = default;

    PriorityProcedure(std::int64_t priority,
                      std::int64_t index,
                      const std::function<void()> &function)
        : m_priority(priority)
        , m_index(index)
        , m_function(function)
    {}

    void operator()() const;
    bool operator<(const PriorityProcedure& other) const;

    class Queue {
    public:
        Queue() {}
        void push(std::int64_t priority, const std::function<void()>& function);
        void exec();
        auto size() const { return m_queue.size(); }

    private:
        std::priority_queue<PriorityProcedure> m_queue;
        std::size_t m_index = 0;
    };

private:
    std::int64_t m_priority = 0;
    std::int64_t m_index = 0;
    std::function<void()> m_function;
};

} // namespace e172
