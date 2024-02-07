#ifndef COMPILER_H
#define COMPILER_H

void compiler_debug(const char* format, ...);
void compiler_error(const char* format, ...);
void compiler_warning(const char *format, ...);

#endif