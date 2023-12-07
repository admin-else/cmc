#pragma once

// ANSI escape codes for text colors
#define TEXT_COLOR_BLACK "\033[0;30m"
#define TEXT_COLOR_RED "\033[0;31m"
#define TEXT_COLOR_GREEN "\033[0;32m"
#define TEXT_COLOR_YELLOW "\033[0;33m"
#define TEXT_COLOR_BLUE "\033[0;34m"
#define TEXT_COLOR_MAGENTA "\033[0;35m"
#define TEXT_COLOR_CYAN "\033[0;36m"
#define TEXT_COLOR_WHITE "\033[0;37m"

// ANSI escape codes for background colors
#define BACKGROUND_COLOR_BLACK "\033[0;40m"
#define BACKGROUND_COLOR_RED "\033[0;41m"
#define BACKGROUND_COLOR_GREEN "\033[0;42m"
#define BACKGROUND_COLOR_YELLOW "\033[0;43m"
#define BACKGROUND_COLOR_BLUE "\033[0;44m"
#define BACKGROUND_COLOR_MAGENTA "\033[0;45m"
#define BACKGROUND_COLOR_CYAN "\033[0;46m"
#define BACKGROUND_COLOR_WHITE "\033[0;47m"

// ANSI escape codes for text attributes
#define TEXT_RESET "\033[0m"     // Reset all text attributes
#define TEXT_BOLD "\033[1m"      // Bold
#define TEXT_ITALIC "\033[3m"    // Italic
#define TEXT_UNDERLINE "\033[4m" // Underline
#define TEXT_BLINK "\033[5m"     // Blink
#define TEXT_REVERSE                                                           \
  "\033[7m" // Reverse (swap foreground and background colors)

// Other nice stuff
#define NLR                                                                    \
  "\n\033[0m" // this stands for NEW LINE RESET and its VERY convenient lol