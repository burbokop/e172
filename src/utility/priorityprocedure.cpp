#include "priorityprocedure.h"

e172::PriorityProcedure::PriorityProcedure() {}

e172::PriorityProcedure::PriorityProcedure(int64_t priority, int64_t index, const std::function<void ()> &function)
    : m_priority(priority), m_index(index), m_function(function) {}

void e172::PriorityProcedure::operator()() const {
    if(m_function)
        m_function();
}

bool e172::PriorityProcedure::operator<(const PriorityProcedure &other) const {
    if(m_priority == other.m_priority) {
        return m_index > other.m_index;
    } else {
        return m_priority > other.m_priority;
    }
}

void e172::PriorityProcedure::Queue::push(int64_t priority, const std::function<void ()> &function) {
    m_queue.push(PriorityProcedure(priority, m_index++, function));
}

void e172::PriorityProcedure::Queue::exec() {
    while(!m_queue.empty()) {
        m_queue.top()();
        m_queue.pop();
    }
    m_index = 0;
}
