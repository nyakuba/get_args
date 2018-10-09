#ifndef _GET_OPTIONS_H_
#define _GET_OPTIONS_H_
#include <algorithm>
#include <cassert>
#include <cerrno>
#include <cstdint>
#include <string>

// routines writen by Jeff Stedfast
// Macro which define strtointX routine, used for parsing
// X bit integer from character string
#define STRTOINT(bits, max)                                                    \
  static int strtoint##bits(const char *in, char **inend,                      \
                            int##bits##_t *retval, int *err) {                 \
    register const char *inptr = in;                                           \
    int##bits##_t val = 0;                                                     \
    int digit, sign = 1;                                                       \
                                                                               \
    while (*inptr == ' ')                                                      \
      inptr++;                                                                 \
                                                                               \
    if (*inptr == '-') {                                                       \
      sign = -1;                                                               \
      inptr++;                                                                 \
    }                                                                          \
                                                                               \
    in = inptr;                                                                \
    while (*inptr >= '0' && *inptr <= '9') {                                   \
      digit = (*inptr - '0');                                                  \
      if (val > (max / 10)) {                                                  \
        *err = EOVERFLOW;                                                      \
        return -1;                                                             \
      } else if (val == (max / 10)) {                                          \
        if (digit > (max % 10) && (sign > 0 || digit > ((max % 10) + 1))) {    \
          *err = EOVERFLOW;                                                    \
          return -1;                                                           \
        }                                                                      \
                                                                               \
        if (sign < 0)                                                          \
          val = (val * sign * 10) - digit;                                     \
        else                                                                   \
          val = (val * 10) + digit;                                            \
                                                                               \
        inptr++;                                                               \
        if (*inptr >= '0' && *inptr <= '9') {                                  \
          *err = EOVERFLOW;                                                    \
          return -1;                                                           \
        }                                                                      \
                                                                               \
        goto ret;                                                              \
      } else {                                                                 \
        val = (val * 10) + digit;                                              \
      }                                                                        \
                                                                               \
      inptr++;                                                                 \
    }                                                                          \
                                                                               \
    if (inptr == in) {                                                         \
      *err = EINVAL;                                                           \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    val *= sign;                                                               \
                                                                               \
  ret:                                                                         \
    *inend = (char *)inptr;                                                    \
    *retval = val;                                                             \
                                                                               \
    return 0;                                                                  \
  }
STRTOINT(8, 127)
STRTOINT(16, 32767)
STRTOINT(32, 2147483647L)
STRTOINT(64, 9223372036854775807LL)

// Macro which define strtointX routine, used for parsing
// X bit unsigned integer from character string
#define STRTOUINT(bits, max)                                                   \
  static int strtouint##bits(const char *in, char **inend,                     \
                             uint##bits##_t *retval, int *err) {               \
    register const char *inptr = in;                                           \
    uint##bits##_t val = 0;                                                    \
    int digit;                                                                 \
                                                                               \
    while (*inptr == ' ')                                                      \
      inptr++;                                                                 \
                                                                               \
    in = inptr;                                                                \
    while (*inptr >= '0' && *inptr <= '9') {                                   \
      digit = (*inptr - '0');                                                  \
      if (val > (max / 10)) {                                                  \
        *err = EOVERFLOW;                                                      \
        return -1;                                                             \
      } else if (val == (max / 10) && digit > (max % 10)) {                    \
        *err = EOVERFLOW;                                                      \
        return -1;                                                             \
      } else {                                                                 \
        val = (val * 10) + digit;                                              \
      }                                                                        \
                                                                               \
      inptr++;                                                                 \
    }                                                                          \
                                                                               \
    if (inptr == in) {                                                         \
      *err = EINVAL;                                                           \
      return -1;                                                               \
    }                                                                          \
                                                                               \
    *inend = (char *)inptr;                                                    \
    *retval = val;                                                             \
                                                                               \
    return 0;                                                                  \
  }
STRTOUINT(8, 255)
STRTOUINT(16, 65535)
STRTOUINT(32, 4294967295UL)
STRTOUINT(64, 18446744073709551615ULL)

/**
 @param character string to parse
 @param pointer to store result
 @return 0 if success, -1 if failure
 */
template <typename T> int parse_arg(char *str, T *val);

template <> int parse_arg<int8_t>(char *str, int8_t *val) {
  char *end;
  int err;
  return strtoint8(str, &end, val, &err);
}

template <> int parse_arg<uint8_t>(char *str, uint8_t *val) {
  char *end;
  int err;
  return strtouint8(str, &end, val, &err);
}

template <> int parse_arg<int16_t>(char *str, int16_t *val) {
  char *end;
  int err;
  return strtoint16(str, &end, val, &err);
}

template <> int parse_arg<uint16_t>(char *str, uint16_t *val) {
  char *end;
  int err;
  return strtouint16(str, &end, val, &err);
}

template <> int parse_arg<int32_t>(char *str, int32_t *val) {
  char *end;
  int err;
  return strtoint32(str, &end, val, &err);
}

template <> int parse_arg<uint32_t>(char *str, uint32_t *val) {
  char *end;
  int err;
  return strtouint32(str, &end, val, &err);
}

template <> int parse_arg<int64_t>(char *str, int64_t *val) {
  char *end;
  int err;
  return strtoint64(str, &end, val, &err);
}

template <> int parse_arg<uint64_t>(char *str, uint64_t *val) {
  char *end;
  int err;
  return strtouint64(str, &end, val, &err);
}

template <> int parse_arg<float>(char *str, float *val) {
  errno = 0;
  char *end;
  *val = float(std::strtod(str, &end));
  if (*end != '\0' || errno != 0) {
    // error, we didn't consume the entire string
    // error, overflow or underflow
    return -1;
  }
  return 0;
}

template <> int parse_arg<double>(char *str, double *val) {
  errno = 0;
  char *end;
  *val = std::strtod(str, &end);
  if (*end != '\0' || errno != 0) {
    // error, we didn't consume the entire string
    // error, overflow or underflow
    return -1;
  }
  return 0;
}

template <> int parse_arg<long double>(char *str, long double *val) {
  errno = 0;
  char *end;
  *val = std::strtold(str, &end);
  if (*end != '\0' || errno != 0) {
    // error, we didn't consume the entire string
    // error, overflow or underflow
    return -1;
  }
  return 0;
}

template <> int parse_arg<std::string>(char *str, std::string *val) {
  *val = str;
  return 0;
}

template <> int parse_arg<char *>(char *str, char **val) {
  *val = str;
  return 0;
}

/**
 @param array with arguments
 @param length of argv
 @param option string
 @param pointer on last processed element of argv
 @return 0 if success, -1 if failure
 */
bool get_flag(char **argv, int argc, const std::string &opt,
              char ***pend = nullptr) {
  char **e = std::find(argv, argv + argc, opt);
  bool ret = true;
  if (e == (argv + argc))
    ret = false;

  if (pend != nullptr)
    *pend = e;

  return ret;
}

/** 
 @param array with arguments
 @param length of argv
 @param option string
 @param iterator pointing on variable, where the result will be stored
 @param number of arguments to parse
 @param pointer on last processed element of argv
 @return 0 if success, -1 if failure
 */
template <typename It>
bool get_args(char **argv, int argc, const std::string &opt, It begin,
              unsigned N = 1, char ***pend = nullptr) {
  char **e = std::find(argv, argv + argc, opt);
  bool ret = true;
  if (e == (argv + argc)) {
    ret = false;
  } else {
    for (unsigned i = 0; i < N; ++i, ++begin) {
      ret = !parse_arg(*(++e), &(*begin));
      if (!ret)
        break;
    }
  }

  if (pend != nullptr)
    *pend = e;

  return ret;
}

#endif // _GET_OPTIONS_H_
