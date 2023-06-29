// Copyright 2023 Borys Boiko

#pragma once

#include "../../src/testing.h"

namespace e172::tests {

class PriorityProcedureSpec {
    static void execTest0() e172_test(PriorityProcedureSpec, execTest0);
    static void execTest1() e172_test(PriorityProcedureSpec, execTest1);
    static void execTest2() e172_test(PriorityProcedureSpec, execTest2);
};

} // namespace e172::tests
