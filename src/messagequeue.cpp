// Copyright 2023 Borys Boiko

#include "messagequeue.h"
#include "debug.h"

void e172::MessageQueuePrivate::throwExeption(const std::string &exeption) {
    throw std::runtime_error(exeption.c_str());
}

void e172::MessageQueuePrivate::warningExeption(const std::string &exeption) {
    Debug::warning(exeption);
}
