C++ 学习笔记

# assert

`assert` 是一个在程序调试中广泛使用的宏。`assert` 用于在运行时检查假设条件是否成立，如果条件不成立，则终止程序并输出错误信息。

```c
#include <assert.h>  // C 语言
// 或
#include <cassert>   // C++ 语言

void assert(int expression);
```

## 工作原理

1. 当程序执行到 `assert(expression)` 时，会计算表达式 `expression` 的值
2. 如果表达式为真（非零），程序继续正常执行
3. 如果表达式为假（零），`assert` 会：
   - 输出错误信息（包含表达式文本、文件名和行号）
   - 调用 `abort()` 函数终止程序

## 使用示例

```c
#include <stdio.h>
#include <assert.h>

int divide(int a, int b) {
    assert(b != 0);  // 确保除数不为零
    return a / b;
}

int main() {
    int result = divide(10, 2);  // 正常执行
    printf("Result: %d\n", result);
    
    result = divide(10, 0);  // 触发 assert 失败
    printf("This line won't be executed\n");
    
    return 0;
}
```

```cpp
#include <iostream>
#include <cassert>

class Vector {
private:
    int* data;
    int size;
public:
    Vector(int n) : size(n) {
        assert(n > 0);  // 确保大小为正数
        data = new int[n];
    }
    
    int& operator[](int index) {
        assert(index >= 0 && index < size);  // 确保索引有效
        return data[index];
    }
    
    ~Vector() {
        delete[] data;
    }
};

int main() {
    Vector vec(5);
    vec[2] = 10;  // 正常访问
    
    // vec[-1] = 5;  // 这会触发 assert 失败
    // vec[10] = 5;  // 这会触发 assert 失败
    
    return 0;
}
```

## 禁用 assert

在发布版本中，通常不希望包含断言检查（出于性能考虑）。可以通过定义 `NDEBUG` 宏来禁用所有 `assert` 调用：

```c
#define NDEBUG  // 在所有 #include <assert.h> 之前定义
#include <assert.h>

// 现在所有 assert 调用都不会产生任何代码
```

或者在编译时使用标志：
```bash
gcc -DNDEBUG program.c -o program
```

## 注意事项

1. **不要在有副作用的表达式中使用 assert**：
   ```c
   // 错误示例 - 发布版本中函数不会被调用
   assert(some_function() != NULL);
   
   // 正确做法
   int* result = some_function();
   assert(result != NULL);
   ```

2. **assert 适用于调试，不应用于处理预期的运行时错误**
   - 对于文件不存在、网络连接失败等可预期的错误，应使用适当的错误处理机制
   - assert 应用于检查"不可能"发生的情况，即程序逻辑错误

3. **assert 与静态断言的区别**
   - `assert` 是运行时检查
   - `static_assert`（C11/C++11）是编译时检查

## 自定义错误处理

如果需要更复杂的错误处理，可以考虑实现自己的断言宏：

```c
#ifndef NDEBUG
    #define MY_ASSERT(expr) \
        if (!(expr)) { \
            fprintf(stderr, "Assertion failed: %s, file %s, line %d\n", \
                   #expr, __FILE__, __LINE__); \
            /* 自定义错误处理 */ \
            exit(EXIT_FAILURE); \
        }
#else
    #define MY_ASSERT(expr) ((void)0)
#endif
```

## 总结

- `assert` 是 C 和 C++ 中都存在的调试工具
- 它用于在运行时检查程序假设
- 可以通过定义 `NDEBUG` 宏来禁用
- 适用于检测程序逻辑错误，不应用于处理预期的运行时错误
- 在生产代码中通常被禁用，以提升性能

正确使用 `assert` 可以大大提高代码的健壮性和可维护性，帮助开发者快速定位和修复程序中的逻辑错误。

# static_assert

`static_assert` 是 C++11 引入的编译时断言机制，与运行时断言 `assert` 有根本性的区别。

`static_assert` 是在**编译时**进行断言检查的特性，如果断言条件不满足，编译器会报错并停止编译。

```cpp
static_assert(常量表达式, 错误消息);
```

- **常量表达式**：必须在编译时就能确定结果的表达式
- **错误消息**：字符串字面量，当断言失败时显示

## 使用示例

### 1. 检查类型大小

```cpp
#include <cstdint>

// 确保 int 是 4 字节
static_assert(sizeof(int) == 4, "int must be 4 bytes");

// 确保平台是 64 位
static_assert(sizeof(void*) == 8, "This code requires 64-bit platform");

struct MyStruct {
    int a;
    char b;
    double c;
};

// 检查结构体大小
static_assert(sizeof(MyStruct) == 16, "MyStruct size should be 16 bytes");
```

### 2. 模板元编程中的应用

```cpp
template<typename T>
class Container {
    // 确保模板参数是指针类型
    static_assert(std::is_pointer<T>::value, "T must be a pointer type");
    // ...
};

template<typename T>
void process() {
    // 确保类型有默认构造函数
    static_assert(std::is_default_constructible<T>::value, 
                 "T must be default constructible");
    // ...
}
```

### 3. 检查编译时常量

```cpp
constexpr int MAX_SIZE = 100;

// 检查常量值
static_assert(MAX_SIZE > 0, "MAX_SIZE must be positive");
static_assert(MAX_SIZE <= 1000, "MAX_SIZE is too large");

// 使用 constexpr 函数
constexpr int calculate_size(int base) {
    return base * 2 + 10;
}

static_assert(calculate_size(5) == 20, "Calculation error");
```

## `static_assert` 与 `assert` 的区别

| 特性             | `static_assert`        | `assert`               |
| ---------------- | ---------------------- | ---------------------- |
| **检查时机**     | 编译时                 | 运行时                 |
| **错误发现时间** | 编译阶段               | 程序运行阶段           |
| **性能影响**     | 无运行时开销           | 有运行时开销           |
| **可禁用性**     | 不可禁用               | 可通过 `NDEBUG` 宏禁用 |
| **表达式要求**   | 必须是编译时常量表达式 | 可以是任何运行时表达式 |
| **错误信息**     | 可自定义详细错误消息   | 固定格式的错误信息     |
| **标准版本**     | C++11/C11              | C89/C++98              |
| **主要用途**     | 类型检查、编译期约束   | 调试、运行时错误检测   |

## 实际应用场景

### 1. 平台特定代码检查

```cpp
// 检查字节序
static_assert(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__, 
             "This code only works on little-endian systems");

// 检查编译器特性
static_assert(__cplusplus >= 201703L, "C++17 or later required");
```

### 2. 确保算法正确性

```cpp
template<typename T>
constexpr T factorial(T n) {
    return (n <= 1) ? 1 : (n * factorial(n - 1));
}

// 验证编译期计算
static_assert(factorial(5) == 120, "Factorial calculation error");
```

### 3. 接口约束

```cpp
template<typename Iterator>
void sort_range(Iterator begin, Iterator end) {
    using value_type = typename std::iterator_traits<Iterator>::value_type;
    
    // 确保元素类型可比较
    static_assert(std::is_arithmetic<value_type>::value,
                 "Sorting requires arithmetic types");
    
    std::sort(begin, end);
}
```

## 高级用法

### 1. 结合类型特征

```cpp
template<typename T>
class SafeVector {
    // 确保类型不是 void
    static_assert(!std::is_void<T>::value, "Cannot create vector of void");
    
    // 确保类型不是引用
    static_assert(!std::is_reference<T>::value, "Cannot create vector of references");
    
    // 确保类型是可析构的
    static_assert(std::is_destructible<T>::value, 
                 "Type must be destructible");
    // ...
};
```

### 2. 自定义错误消息（C++17）

C++17 允许省略错误消息，但建议始终提供：

```cpp
// C++17 可以这样写（但不推荐）
static_assert(sizeof(int) == 4);

// 推荐写法（提供有意义的错误信息）
static_assert(sizeof(int) == 4, "Platform compatibility requires 4-byte integers");
```

## 最佳实践

1. **提供有意义的错误消息**：帮助其他开发者理解断言失败的原因
2. **用于接口约束**：在模板代码中使用 `static_assert` 提供清晰的错误信息
3. **验证编译期计算**：确保 `constexpr` 函数和常量的正确性
4. **替代宏检查**：相比 `#if` + `#error`，`static_assert` 提供更好的类型安全和错误信息

## 总结

`static_assert` 是一个强大的编译时检查工具，与运行时断言 `assert` 形成互补：

- 使用 `static_assert` 进行**编译时检查**：类型约束、平台要求、编译期计算验证
- 使用 `assert` 进行**运行时检查**：参数验证、状态检查、调试假设

正确使用这两种断言可以大大提高代码的健壮性，在最早的可能时间（编译时或运行时）发现和诊断问题。
