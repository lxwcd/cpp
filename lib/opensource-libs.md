c++ 开源库

# 字符串格式化库 fmt
> [API - {fmt}](https://fmt.dev/12.0/api/) 

fmt 是一个轻量、高性能、类型安全的 C++ 字符串格式化开源库。C++20 标准 `std::format` 就是基于 fmt 的设计思想实现。

- 核心定位：**把变量（数字、字符串、容器等）组装成格式化文本**；不负责字符串解析（字符串转回数字）。

## 版本与 C++ 标准兼容

- `fmt 9.1.0`：最低支持 **C++11**，适合锁死C++11的遗留项目；编译期格式检查宏 `FMT_STRING` 失效，仅运行期校验。
- `fmt 10.x / 11.x`：最低要求 **C++14**，新增大量特性，推荐新项目使用。

## 核心能力

✅ 数值转字符串，灵活控制小数位数、补零、对齐
✅ 多变量拼接格式化，语法简洁（类似Python格式化）
✅ 直接打印STL容器 `std::vector`、`std::array`
✅ 时间格式化、控制台彩色输出（扩展头文件）
✅ 类型安全：编译期捕获格式参数类型不匹配（新版）
✅ 性能优于 `std::ostringstream`、传统 `sprintf`

❌ **不支持字符串解析**：不能把 `"123"` 转成int，需要使用标准库 `std::stoi` / `std::stod`
❌ 不擅长复杂字符串查找、分割、替换，这类需求用 `std::string` / abseil 字符串工具库

## 头文件划分

```cpp
#include <fmt/core.h>    // 必引入，format / print，绝大多数场景够用
#include <fmt/chrono.h>  // 时间日期格式化
#include <fmt/ranges.h>  // STL容器打印（vector等）
#include <fmt/color.h>   // 控制台彩色文字
#include <fmt/std.h>     // std::pair、std::filesystem等标准库类型支持
```

## 示例

```cpp
#include <fmt/core.h>
#include <string>
#include <iostream>

int main()
{
    // 基础格式化
    std::string str = fmt::format("订单编号:{:06d}, 金额={:.2f}", 101, 170.0);
    fmt::print("输出结果：{}\n", str);

    // 补零：输出 000042
    fmt::print("编号:{:06d}\n", 42);
    return 0;
}
```

## 优缺点总结

### ✅ 优点

1. 语法直观易读，相比流和sprintf大幅减少代码量
2. 类型安全，减少运行时崩溃
3. 性能优秀，大量工业C++项目使用
4. 跨平台：Windows/Linux/macOS

### ⚠️ 缺点

1. C++11项目只能使用旧版本9.1.0，部分高级特性缺失
2. 属于第三方依赖，项目需要管理库版本
3. 只做格式化，不支持字符串解析、复杂字符串处理