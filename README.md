# defer.h

A simple, stb-style single-header library which adds a compile-time defer block and autofree attribute to C/C++.

## Autofree

```c
{
    autofree int* i = malloc(sizeof(int));
    printf("%d\n", i);
    // i is freed here
}
```

## Defer

```c
{
    struct MyStruct* s = MyStruct_create();
    defer({
        MyStruct_destroy(s);
    });

    printf("%p\d", s);
    // s is freed here using MyStruct_destroy
}
```

## Including the header

All info can be found in the header file, but to summarize:

Do this:
```c
#define DEFER_IMPL
```
before you include this file in *one* C or C++ file to create the implementation.

i.e. it should look like this:

```c
#include ...
#include ...
#include ...
#define DEFER_IMPL
#include "defer.h"
```

## License

Licesed under MIT license, do what you want with it.

