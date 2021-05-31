#include "priorityproceduretest.h"

#include <src/utility/priorityprocedure.h>

#include <map>
#include <src/testing.h>


void e172::PriorityProcedureTest::testAll() {
    execTest0();
    execTest1();
    execTest2();
}



void e172::PriorityProcedureTest::execTest0() {
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

    shouldEqual(queue.size(), 8);

    queue.exec();

    shouldEqual(queue.size(), 0);

    shouldEqual(map.at(0), 4);
    shouldEqual(map.at(2), 2);
    shouldEqual(map.at(-100), 1);
    shouldEqual(map.at(100), 1);
}


void e172::PriorityProcedureTest::execTest1() {
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

    shouldEqual(vector[0], -100);
    shouldEqual(vector[1], 0);
    shouldEqual(vector[2], 0);
    shouldEqual(vector[3], 0);
    shouldEqual(vector[4], 0);
    shouldEqual(vector[5], 2);
    shouldEqual(vector[6], 2);
    shouldEqual(vector[7], 100);
}

void e172::PriorityProcedureTest::execTest2() {
    std::vector<int64_t> vector;

    PriorityProcedure::Queue queue;
    queue.push(0, [&vector](){ vector.push_back(0); });
    queue.push(0, [&vector](){ vector.push_back(1); });
    queue.push(0, [&vector](){ vector.push_back(2); });
    queue.push(0, [&vector](){ vector.push_back(3); });

    queue.exec();

    shouldEqual(vector[0], 0);
    shouldEqual(vector[1], 1);
    shouldEqual(vector[2], 2);
    shouldEqual(vector[3], 3);
}
