#ifndef REGEX_HPP
#define REGEX_HPP

#define INTEGER_REGEX "([0-9])\\w+"
#define WORD_REGEX "([A-Z]|[a-z])\\w+"
#define NAME_REGEX "^([a-zA-Z]{2,}\\s[a-zA-z]{1,}'?-?[a-zA-Z]{2,}\\s?([a-zA-Z]{1,})?)"
#define REAL_NUMBER_REGEX "^(-|\\+)?(([1-9][0-9]*)|(0))(?:\\.[0-9]+)?$"

#endif // REGEX_HPP
