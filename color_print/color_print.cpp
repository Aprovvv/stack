/** @file */
#include <stdio.h>
#ifndef _WIN32
#include <unistd.h>
#endif
#include <stdarg.h>
#include <assert.h>
#include "color_print.h"

static const char* switch_color_code(console_text_colors color);

int vfprintf_color(FILE* output,
                   console_text_colors color,
                   const char* str,
                   va_list ap)
{
    int n = 0;
#if defined _WIN32
#warning К сожалению, цветной вывод не поддерживается в вашей ОС.
    n = vfprintf(output, str, ap);
    return n;
#else
    if (isatty(fileno(output)))
    {
        fprintf(output,"%s",
                switch_color_code(color));
        n = vfprintf(output, str, ap);
        fprintf(output, "%s",
                switch_color_code(CONSOLE_TEXT_STANDART));
    }
    else
    {
        n = vfprintf(output, str, ap);
    }
    return n;
#endif
}

int printf_color(console_text_colors color,
                 const char* str, ...)
{
    int n = 0;
    va_list ap;
    va_start(ap, str);
    n = vfprintf_color(stdout, color, str, ap);
    return n;
}

int fprintf_color(FILE* output,
                  console_text_colors color,
                  const char* str, ...)
{
    int n = 0;
    va_list ap;
    va_start(ap, str);
    n = vfprintf_color(output, color, str, ap);
    return n;
}

void assert_but_better(int line,
                       const char* file,
                       const char* text_condition,
                       int condition,
                       const char* format, ...)
{
    if (condition)
        return;
    va_list ap;
    va_start(ap, format);
    fprintf_color(stderr, CONSOLE_TEXT_RED,
                  "%s:%d ", file, line);
    fprintf_color(stderr, CONSOLE_TEXT_RED,
                  "Не выполнилось условие %s ",
                  text_condition);
    vfprintf_color(stderr, CONSOLE_TEXT_RED, format, ap);
    abort();
}

static const char* switch_color_code(console_text_colors color)
{
    switch(color)
    {
    case CONSOLE_TEXT_STANDART:
        return "\033[39m";
    case CONSOLE_TEXT_BLACK:
        return "\033[30m";
    case CONSOLE_TEXT_DARK_RED:
        return "\033[31m";
    case CONSOLE_TEXT_DARK_GREEN:
        return "\033[32m";
    case CONSOLE_TEXT_DARK_YELLOW:
        return "\033[33m";
    case CONSOLE_TEXT_DARK_BLUE:
        return "\033[34m";
    case CONSOLE_TEXT_DARK_MAGENTA:
        return "\033[35m";
    case CONSOLE_TEXT_DARK_CYAN:
        return "\033[36m";
    case CONSOLE_TEXT_GREY:
        return "\033[37m";
    case CONSOLE_TEXT_DARK_GREY:
        return "\033[90m";
    case CONSOLE_TEXT_RED:
        return "\033[91m";
    case CONSOLE_TEXT_GREEN:
        return "\033[92m";
    case CONSOLE_TEXT_ORANGE:
        return "\033[93m";
    case CONSOLE_TEXT_BLUE:
        return "\033[94m";
    case CONSOLE_TEXT_MAGENTA:
        return "\033[95m";
    case CONSOLE_TEXT_CYAN:
        return "\033[96m";
    case CONSOLE_TEXT_WHITE:
        return "\033[97m";
    default:
        assert(0 && "Ошибка: неизвестный код цвета.");
        return "";
    }
}
