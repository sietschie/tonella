#include "nulllogger.h"

void NullLogger::init() {}

void NullLogger::print(LogLevel level, const char *str) {}

void NullLogger::println(LogLevel level, const char *str) {}

void NullLogger::print(LogLevel level, int32_t n) {}

void NullLogger::println(LogLevel level) {}
