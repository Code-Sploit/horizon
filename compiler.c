#include "compiler.h"

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

void compiler_debug(const char* format, ...)
{
    printf("[HORIZON]: ");

    va_list ptr;
    va_start(ptr, format);

    char buffer[1024];

    int k = 0;
    int i = 0;

    for (int i = 0; format[i] != '\0'; i++)
    {
        buffer[k++] = format[i];

        if (format[i + 1] == '%' || format[i + 1] == '\0')
        {
            buffer[k] = '\0';

            k = 0;

            if (buffer[0] != '%')
            {
                fprintf(stdout, "%s", buffer);
            }
            else
            {
                int j = 1;
                char c = 0;

                while ((c = buffer[j++]) < 58) {}

                if (c == 'i' || c == 'd' || c == 'u' || c == 'h')
                {
                    fprintf(stdout, buffer, va_arg(ptr, int));
                }
                else if (c == 'c')
                {
                    fprintf(stdout, buffer, va_arg(ptr, int));
                }
                else if (c == 'f')
                {
                    fprintf(stdout, buffer, va_arg(ptr, double));
                }
                else if (c == 'l')
                {
                    char c2 = buffer[2];

                    if (c2 == 'u' || c2 == 'd' || c2 == 'i')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long));
                    }
                    else if (c2 == 'f')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, double));
                    }
                }
                else if (c == 'L')
                {
                    char c2 = buffer[2];

                    if (c2 == 'u' || c2 == 'd' || c2 == 'i')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long long));
                    }

                    else if (c2 == 'f')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long double));
                    }
                }
                else if (c == 's')
                {
                    fprintf(stdout, buffer, va_arg(ptr, char*));
                }
                else
                {
                    fprintf(stdout, "%s", buffer);
                }
            }
        }
    }

    va_end(ptr);

    printf("\n");
}

void compiler_error(const char* format, ...)
{
    printf("[ERROR]: ");

    va_list ptr;
    va_start(ptr, format);

    char buffer[1024];

    int k = 0;
    int i = 0;

    for (int i = 0; format[i] != '\0'; i++)
    {
        buffer[k++] = format[i];

        if (format[i + 1] == '%' || format[i + 1] == '\0')
        {
            buffer[k] = '\0';

            k = 0;

            if (buffer[0] != '%')
            {
                fprintf(stdout, "%s", buffer);
            }
            else
            {
                int j = 1;
                char c = 0;

                while ((c = buffer[j++]) < 58) {}

                if (c == 'i' || c == 'd' || c == 'u' || c == 'h')
                {
                    fprintf(stdout, buffer, va_arg(ptr, int));
                }
                else if (c == 'c')
                {
                    fprintf(stdout, buffer, va_arg(ptr, int));
                }
                else if (c == 'f')
                {
                    fprintf(stdout, buffer, va_arg(ptr, double));
                }
                else if (c == 'l')
                {
                    char c2 = buffer[2];

                    if (c2 == 'u' || c2 == 'd' || c2 == 'i')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long));
                    }
                    else if (c2 == 'f')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, double));
                    }
                }
                else if (c == 'L')
                {
                    char c2 = buffer[2];

                    if (c2 == 'u' || c2 == 'd' || c2 == 'i')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long long));
                    }

                    else if (c2 == 'f')
                    {
                        fprintf(stdout, buffer, va_arg(ptr, long double));
                    }
                }
                else if (c == 's')
                {
                    fprintf(stdout, buffer, va_arg(ptr, char*));
                }
                else
                {
                    fprintf(stdout, "%s", buffer);
                }
            }
        }
    }

    va_end(ptr);

    printf("\n");

    exit(1);
}