#ifndef PRIORITYPROCEDURE_H
#define PRIORITYPROCEDURE_H

#include <functional>
#include <queue>

namespace e172 {

class PriorityProcedure {
    int64_t m_priority = 0;
    int64_t m_index = 0;
    std::function<void()> m_function;
public:
    PriorityProcedure();
    PriorityProcedure(int64_t priority, int64_t index, const std::function<void()>& function);
    void operator()() const;
    bool operator<(const PriorityProcedure& other) const;

    class Queue {
        std::priority_queue<PriorityProcedure> m_queue;
        size_t m_index = 0;
    public:
        Queue() {}
        void push(int64_t priority, const std::function<void()>& function);
        void exec();
        auto size() const { return m_queue.size(); }
    };
};

}
#endif // PRIORITYPROCEDURE_H
