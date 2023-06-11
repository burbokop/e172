#include "priorityprocedurespec.h"

#include <src/utility/priorityprocedure.h>

#include <map>
#include <src/testing.h>

void e172::tests::PriorityProcedureSpec::execTest0()
{
    std::map<int64_t, size_t> map;

    PriorityProcedure::Queue queue;
    const auto push = [&queue, &map](int64_t proirity){
        queue.push(proirity, [&map, proirity](){ map[proirity]++; });
    };

    push(0);
    push(2);
    push(2);
    push(0);
    push(0);
    push(-100);
    push(100);
    push(0);

    e172_shouldEqual(queue.size(), 8);

    queue.exec();

    e172_shouldEqual(queue.size(), 0);

    e172_shouldEqual(map.at(0), 4);
    e172_shouldEqual(map.at(2), 2);
    e172_shouldEqual(map.at(-100), 1);
    e172_shouldEqual(map.at(100), 1);
}

void e172::tests::PriorityProcedureSpec::execTest1()
{
    std::vector<int64_t> vector;

    PriorityProcedure::Queue queue;

    const auto push = [&queue, &vector](int64_t priority){
        queue.push(priority, [&vector, priority](){ vector.push_back(priority); });
    };

    push(0);
    push(2);
    push(2);
    push(0);
    push(0);
    push(-100);
    push(100);
    push(0);

    queue.exec();

    e172_shouldEqual(vector[0], -100);
    e172_shouldEqual(vector[1], 0);
    e172_shouldEqual(vector[2], 0);
    e172_shouldEqual(vector[3], 0);
    e172_shouldEqual(vector[4], 0);
    e172_shouldEqual(vector[5], 2);
    e172_shouldEqual(vector[6], 2);
    e172_shouldEqual(vector[7], 100);
}

void e172::tests::PriorityProcedureSpec::execTest2()
{
    std::vector<int64_t> vector;

    PriorityProcedure::Queue queue;
    queue.push(0, [&vector](){ vector.push_back(0); });
    queue.push(0, [&vector](){ vector.push_back(1); });
    queue.push(0, [&vector](){ vector.push_back(2); });
    queue.push(0, [&vector](){ vector.push_back(3); });

    queue.exec();

    e172_shouldEqual(vector[0], 0);
    e172_shouldEqual(vector[1], 1);
    e172_shouldEqual(vector[2], 2);
    e172_shouldEqual(vector[3], 3);
}
