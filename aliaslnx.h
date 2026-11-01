#ifndef ALIASLNX_H
#define ALIASLNX_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <uchar.h>
#include <wchar.h>
#include <complex.h>
#include <sys/types.h>
#include <time.h>


typedef bool             b8;//       0 .. 1
typedef char             c8;
typedef int8_t           i8;//    -128 .. 127
typedef uint8_t          u8;//       0 .. 255

typedef char16_t         c16;
typedef int16_t          i16;// -32.7k .. 32.7k
typedef uint16_t         u16;//      0 .. 65.5k

typedef char32_t         c32;
typedef int32_t          e32;//enum
typedef float            f32;
typedef int32_t          i32;//  -2.1B .. 2.1B
typedef uint32_t         u32;//      0 .. 4.2B
typedef wchar_t          w32;

typedef float _Complex   c64;
typedef double           f64;
typedef int64_t          i64;//  -9.2Q .. 9.2Q
typedef time_t           t64;
typedef uint64_t         u64;//      0 .. 18.4Q
typedef size_t           usize;
typedef ssize_t          isize;
typedef ptrdiff_t        ptrd;
typedef intptr_t         iptr;
typedef uintptr_t        uptr;
typedef void *           ptr;
//typedef long long ll;   
//typedef unsigned long long ull;   

//typedef double _Complex c128;
//typedef long double f80;
//typedef __float128 f128;
//typedef __int128 i128;
//typedef unsigned __int128 u128;

//typedef long double _Complex c256; 


#endif