# __attribute__ 添加属性
> [示例: SimpleSignal 库](https://github.com/larspensjo/SimpleSignal/blob/master/README.md)

```cpp
#ifndef _MSC_VER
#include <sys/time.h>
#include <stdio.h>
static std::string string_printf(const char* format, ...) __attribute__((__format__(__printf__, 1, 2)));
#endif

static std::string
string_printf(const char* format, ...)
{
    std::string result;
    char str[1000];
    va_list args;
    va_start(args, format);
    if (vsnprintf(str, sizeof str, format, args) >= 0)
        result = str;
    va_end(args);
    return result;
}
```

`__attribute__((__format__(__printf__, 1, 2)))` 是一个 GCC 特定的属性，用于指定函数的参数格式。这个属性可以帮助编译器在编译时检查格式化字符串是否与参数匹配，从而减少运行时错误。

## **`__attribute__`**
`__attribute__` 是 GCC 提供的一个扩展语法，用于为变量、函数或类型添加特定的属性。这些属性可以影响编译器对代码的处理方式，例如优化、警告检查等。

## **`__format__`**
`__format__` 是 `__attribute__` 的一个特定属性，用于指定函数的参数格式。它通常用于与标准的格式化函数（如 `printf`、`scanf`、`strftime` 等）兼容的函数。

## **`__printf__`**
`__printf__` 是 `__format__` 属性的一个参数，表示该函数的参数格式与 `printf` 相同。这意味着编译器会按照 `printf` 的规则检查格式化字符串和参数。

## **参数位置**
`__format__(__printf__, 1, 2)` 中的 `1` 和 `2` 是参数位置的索引，表示格式化字符串和可变参数列表在函数参数列表中的位置：
- **`1`**：表示格式化字符串是函数的第一个参数。
- **`2`**：表示可变参数列表从函数的第二个参数开始。

## **作用**
使用这个属性的好处是，编译器可以在编译时检查格式化字符串是否与参数匹配。如果格式化字符串中的占位符与实际传递的参数类型不匹配，编译器会发出警告或错误，从而帮助开发者在早期发现潜在的错误。

## **用法**

**正确用法**
```cpp
std::string message = string_printf("Hello, %s!", "World");
```
编译器会检查格式化字符串 `"Hello, %s!"` 和参数 `"World"` 是否匹配。

```cpp
std::string message = string_printf("Hello, my name is %s and I am %d years old.", "John", 30);
```
在这个调用中：
- **`"Hello, my name is %s and I am %d years old."`**：这是格式化字符串，对应函数的第一个参数，位置索引是 `1`。
- **`"John"`**：这是第一个可变参数，对应格式化字符串中的 `%s`。
- **`30`**：这是第二个可变参数，对应格式化字符串中的 `%d`。

--- 

**错误用法**
```cpp
std::string message = string_printf("Hello, %d!", "World");
```
编译器会发出警告，因为格式化字符串中的 `%d` 期望一个整数参数，但实际传递的是一个字符串。

## **跨平台兼容性**
需要注意的是，`__attribute__` 是 GCC 和 Clang 的扩展语法，不是标准 C++ 的一部分。如果代码需要在其他编译器（如 MSVC）上编译，可以使用预处理器宏来兼容：
```cpp
#if defined(__GNUC__) || defined(__clang__)
#define FORMAT_PRINTF(format_idx, first_to_check) __attribute__((__format__(__printf__, format_idx, first_to_check)))
#else
#define FORMAT_PRINTF(format_idx, first_to_check)
#endif

static std::string string_printf(const char* format, ...) FORMAT_PRINTF(1, 2);
```

这样，代码在 GCC 或 Clang 下会启用格式检查，在其他编译器下则忽略该属性。