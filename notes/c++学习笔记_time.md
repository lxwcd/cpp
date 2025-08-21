C++ 学习笔记

# timestamp
获取时间戳

## 示例1
> [示例: SimpleSignal 库](https://github.com/larspensjo/SimpleSignal/blob/master/README.md)


```cpp
static uint64_t
timestamp_benchmark()
{
    auto now = std::clock();
    return 1.0e9 / CLOCKS_PER_SEC * now;
}
```

- **`static`**：表示这个函数是静态的，只能在定义它的文件内部使用。这有助于减少函数的可见性，避免与其他文件中的同名函数冲突。
- **`uint64_t`**：函数返回一个 64 位无符号整数。`uint64_t` 是 C++11 标准库中的类型，定义在 `<cstdint>` 头文件中，表示一个 64 位无符号整数。

- **`std::clock`**：这是一个 C 标准库函数，定义在 `<ctime>` 头文件中。它返回从程序开始执行到当前时间的时钟周期数（clock ticks）。
- **`auto`**：自动推导变量类型。在这里，`std::clock` 返回的类型是 `clock_t`，因此 `now` 的类型是 `clock_t`。
- **`now`**：变量名，存储当前的时钟周期数。

- **`1.0e9`**：表示 \(1 \times 10^9\)，表示 1 纳秒。
- **`CLOCKS_PER_SEC`**：一个宏，定义在 `<ctime>` 头文件中，表示每秒的时钟周期数。
- **`return`**：返回当前时间的纳秒数。

## 示例2

如果需要更精确的时间测量，可以使用 C++11 的 `std::chrono` 库。例如：

```cpp
#include <iostream>
#include <chrono>

static uint64_t
timestamp_benchmark()
{
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count();
}

int main() {
    uint64_t timestamp = timestamp_benchmark();
    std::cout << "Timestamp: " << timestamp << " ns" << std::endl;
    return 0;
}
```

这个版本使用了 `std::chrono::high_resolution_clock`，提供了更高的精度和更广泛的时间范围。