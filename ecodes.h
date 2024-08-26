#ifndef ECODES_H
#define ECODES_H

#include <stdio.h>

// Enable or disable colors depending on the platform
#if defined(_WIN32) || defined(_WIN64)
#define ENABLE_COLORS 0
#else
#define ENABLE_COLORS 1
#endif

// Color codes
#if ENABLE_COLORS

#define RESET        "\033[0m"
#define BLACK        "\033[30m"
#define RED          "\033[31m"
#define GREEN        "\033[32m"
#define YELLOW       "\033[33m"
#define BLUE         "\033[34m"
#define MAGENTA      "\033[35m"
#define CYAN         "\033[36m"
#define WHITE        "\033[37m"
#define BG_BLACK     "\033[40m"
#define BG_RED       "\033[41m"
#define BG_GREEN     "\033[42m"
#define BG_YELLOW    "\033[43m"
#define BG_BLUE      "\033[44m"
#define BG_MAGENTA   "\033[45m"
#define BG_CYAN      "\033[46m"
#define BG_WHITE     "\033[47m"
#define BOLD_BLACK   "\033[1;30m"
#define BOLD_RED     "\033[1;31m"
#define BOLD_GREEN   "\033[1;32m"
#define BOLD_YELLOW  "\033[1;33m"
#define BOLD_BLUE    "\033[1;34m"
#define BOLD_MAGENTA "\033[1;35m"
#define BOLD_CYAN    "\033[1;36m"
#define BOLD_WHITE   "\033[1;37m"

#endif // ENABLE_COLORS

#ifdef ECODES_IMPLEMENTATION
void eprint(const char* color, const char* text) {
    if (ENABLE_COLORS) {
        printf("%s%s%s\n", color, text, RESET);
    } else {
        printf("%s NO_COLORS\n", text); // Print without color if colors are disabled
    }
}
#endif // ECODES_IMPLEMENTATION

#endif // ECODES_H
