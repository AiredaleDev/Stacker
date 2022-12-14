// TODO: Program (and maybe stack) should use a bump allocator to be more performant.

#ifndef _UTIL_H
#define _UTIL_H

#include <stddef.h>
#include <stdint.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef __ssize_t isize;
typedef intptr_t iptr;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t usize;
typedef uintptr_t uptr;

void die(const char* message);

#endif
