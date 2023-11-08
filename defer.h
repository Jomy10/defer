// Defer
// =====
// @desc Defer block and autofree attribute in C (or C++)
// @author: Jonas Everaert
//
// Instructions:
// ------------
// Do this:
//    #define DEFER_IMPL
// before you include this file in *one* C or C++ file to create the implementation.
//
// i.e. it should look like this:
// #include ...
// #include ...
// #include ...
// #define DEFER_IMPL
// #include "defer.h"
//
// You can #define DEFER_NO_AUTOFREE_IMPL to only include the defer implementation and
// avoid importing <stdlib.h>. `DEFERDEF void autofree_var(void* ptr);` can then be
// implemented by yourself, if you need it, which can use your own allocator.
//
// Compiler-specific Instructions:
// ------------------------------
// - Clang on non-Apple platforms:
//      The blocks runtime needs to be installed, e.g.:
//          sudo apt-get install libblocksruntime-dev
//      And cflags: -fblock -lBlocksRuntime
// - GCC:
//      Only the GCC "nested functions" and "statement expressions"
//      need to be enabled.
// - MSVC: TODO (https://stackoverflow.com/a/1828587/14874405)
// - ICC: works like GCC
// - zig cc: see clang
//
// Usage:
// -----
// ## Autofree
// ```c
// {
// 		autofree int* i = malloc(sizeof(int));
// 		printf("%d\n", i);
// 		// i freed here
// }
// ```
//
// ## Defer
// ```c
// int* create_my_int() {
//   return malloc(sizeof(int));
// }
//
// void destroy_my_int(int* i) {
//   free(i);
// }
//
// int main(void) {
//   int* i = create_my_int();
//   defer({
//     destroy_my_int(i);
//   });
//   printf("%d\n", i);
//   // defer block is executed here, freeing i
// }
// ```
//
// See end of file for examples
//

#ifndef _DEFER_H
#define _DEFER_H

#define DEFER_VERSION 1

#ifndef DEFERDEF
#ifdef DEFERDEF_STATIC
#define DEFERDEF static
#else
#define DEFERDEF extern
#endif
#endif

#define _DEFER_CONCAT_IMPL(a, b) a##b
#define _DEFER_CONCAT(a, b) _DEFER_CONCAT_IMPL(a, b)

#define autofree __attribute__((cleanup(autofree_var)))

#ifdef __cplusplus
extern "C" {
#endif

// Compiler-specific definitions
#if defined(__GNUC__) && !defined(__clang__)
    // GCC/ICC
    typedef void (*defer_block)(void);
    #define defer_block_create(body) ({ void __fn__(void) body; __fn__; })
    #define defer(body) defer_block __attribute((cleanup(do_defer))) _DEFER_CONCAT(__defer, __COUNTER__) = defer_block_create(body)
#elif defined(__clang__)
    // Clang/zig cc
    typedef void (^defer_block)(void);
    #define defer_block_create(body) ^body
    #define defer(body) defer_block __attribute__((cleanup(do_defer))) _DEFER_CONCAT(__defer, __COUNTER__) = defer_block_create(body)
#else
	#error "Compiler not compatible with defer library"
#endif

DEFERDEF inline void do_defer(defer_block* ptr);
DEFERDEF inline void autofree_var(void* ptr);

#ifdef DEFER_IMPL
// defer
DEFERDEF inline void do_defer(defer_block* ptr) {
	(*ptr)();
}

#ifndef DEFER_NO_AUTOFREE_IMPL
#include <stdlib.h>

// autofree
DEFERDEF inline void autofree_var(void* ptr) {
	free(*(void**)ptr);
}
#endif // DEFER_NO_AUTOFREE_IMPL
#endif // DEFER_IMPL

// Example //
#if 0
#include <stdio.h>

int defer_example(void) {
	autofree int* i = (int*) malloc(sizeof(int));
	*i += 1;

	defer({
		printf("Hello world!!\n");
	});

  defer({
      printf("This is executed first\n");
  });

	return *i;
}
#endif // example

#ifdef __cplusplus
}
#endif

// LICENSE:
//
// MIT License
// 
// Copyright (c) 2023 Jonas Everaert
//  
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#endif

