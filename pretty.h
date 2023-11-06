#ifndef PRETTY_H
#define PRETTY_H

#include <stdint.h>
#include <stddef.h>

typedef int8_t  Int8;
typedef int16_t Int16;
typedef int32_t Int32;
typedef int64_t Int64;

typedef uint8_t  UInt8;
typedef uint16_t UInt16;
typedef uint32_t UInt32;
typedef uint64_t UInt64;

#define Int8_MAX  INT8_MAX
#define Int8_MIN  INT8_MIN
#define Int16_MAX INT16_MAX
#define Int16_MIN INT16_MIN
#define Int32_MAX INT32_MAX
#define Int32_MIN INT32_MIN
#define Int64_MAX INT64_MAX
#define Int64_MIN INT64_MIN

#define UInt8_MAX  UINT8_MAX
#define UInt16_MAX UINT16_MAX
#define UInt32_MAX UINT32_MAX
#define UInt64_MAX UINT64_MAX

typedef ptrdiff_t IntSize;
typedef size_t    UIntSize;

typedef float       Float32;
typedef double      Float64;
typedef long double FloatExt;

#endif
