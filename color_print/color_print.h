/** @file */
#ifndef FPRINT_COLOR
#define FPRINT_COLOR
#include <stdio.h>
#include <stdlib.h>

enum console_text_colors
{
    CONSOLE_TEXT_STANDART,
    CONSOLE_TEXT_BLACK,
    CONSOLE_TEXT_DARK_RED,
    CONSOLE_TEXT_DARK_GREEN,
    CONSOLE_TEXT_DARK_YELLOW,
    CONSOLE_TEXT_DARK_BLUE,
    CONSOLE_TEXT_DARK_MAGENTA,
    CONSOLE_TEXT_DARK_CYAN,
    CONSOLE_TEXT_GREY,
    CONSOLE_TEXT_DARK_GREY,
    CONSOLE_TEXT_RED,
    CONSOLE_TEXT_GREEN,
    CONSOLE_TEXT_ORANGE,
    CONSOLE_TEXT_BLUE,
    CONSOLE_TEXT_MAGENTA,
    CONSOLE_TEXT_CYAN,
    CONSOLE_TEXT_WHITE
};
/**
 * Функция цветной печати (если печатаем в файл, то печатает обычным цветом).
 * Способ вывода аналогичен fprintf
 * \param output {Указатель на файл}
 * \param color {Цвет. Должен быть членом enum console_text_colors}
 * \param str {Строка форматирования}
 * \param ... {Другие аргументы, которые надо вывести}
 */
int fprintf_color(FILE* output,
                  console_text_colors color,
                  const char* str, ...);

/**
 * Функция цветной печати (если печатаем в файл, то печатает обычным цветом).
 * Способ вывода аналогичен printf
 * \param color {Цвет. Должен быть членом enum console_text_colors}
 * \param str {Строка форматирования}
 * \param ... {Другие аргументы, которые надо вывести}
 */
int printf_color(console_text_colors color,
                 const char* str, ...);

/**
 * Как assert, только лучше. Вызывается с помощью макроса ASSERT
 * \param line {строка, в которой произошла ошибка. ВВОДИТЬ НЕ НАДО, ЭТО СДЕЛАЕТ МАКРОС}
 * \param file {имя файла, в котором произошла ошибка. ВВОДИТЬ НЕ НАДО, ЭТО СДЕЛАЕТ МАКРОС}
 * \param condition {Условие, при невыполнении которого выводится ошибка}
 * \param format {Строка форматирования}
 * \param ... {Другие аргументы, которые надо вывести}
 */
void assert_but_better(int line,
                       const char* file,
                       const char* text_condition,
                       int condition,
                       const char* format, ...);
/**
 * Аналог функции vfprintf, но с цветом.
 * \param output {Указатель на файл вывода}
 * \param color {цвет}
 * \param str {строка форматирования}
 * \param ap {va_list с остальными аргументами}
 */
int vfprintf_color(FILE* output,
                   console_text_colors color,
                   const char* str,
                   va_list ap);

#ifndef NDEBUG
/**
 * Используется для удобного вызова my_own_assert.
 * \param condition {Условие, при невыполнении которого выводится ошибка}
 * \param format {Строка форматирования}
 * \param ... {Другие аргументы, которые надо вывести}
 */
#define ASSERT_BUT_BETTER(x, s, ...) assert_but_better(__LINE__, __FILE__, #x, x, s, __VA_ARGS__)
#else
#define ASSERT_BUT_BETTER
#endif
#endif
