SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 

# 声明信号
```cpp
Simple::Signal<char (float, int, std::string)> sig;
```

## 模板参数解析
```
char(float, int, std::string)
```
这不是一个函数调用，而是一个**函数类型**的表示法，包含：
- **返回类型**: `char`
- **参数类型**: `float`, `int`, `std::string`

## 等效的普通函数声明
这个模板参数对应于这样一个函数签名：
```cpp
char someFunction(float f, int i, std::string s);
```

## 完整的类型推导
编译器看到这个声明时，会进行如下推导：
```cpp
// 模板实例化过程：
Simple::Signal<
    char(float, int, std::string),                    // SignalSignature
    Simple::Lib::CollectorDefault<char>               // Collector (默认)
> sig;
```

## 在库内部的类型映射

### ProtoSignal 的模板特化
```cpp
// 根据声明，会实例化这个特化版本
template<class Collector, class R, class... Args>
class ProtoSignal<R(Args...), Collector> {
  // 在这里：
  // R = char
  // Args... = float, int, std::string
  
  using CbFunction = std::function<char(float, int, std::string)>;
  // ...
};
```

### 生成的具体类型
- **回调函数类型**: `std::function<char(float, int, std::string)>`
- **emit方法签名**: `char emit(float, int, std::string)`
- **connect方法**: `size_t connect(std::function<char(float, int, std::string)>)`

## 与std::function保持一致
```cpp
// std::function使用相同的语法
std::function<char(float, int, std::string)> callback;

// 因此Signal的声明方式很自然
Simple::Signal<char(float, int, std::string)> signal;
```

## 更好的类型安全性
编译器可以完整验证函数签名的正确性：
```cpp
// 正确的使用
signal.connect([](float f, int i, std::string s) -> char {
    return 'A';
});

// 编译错误：参数类型不匹配
signal.connect([](int wrong, float types) -> char {
    return 'B';  // 错误！
});
```

## 使用示例

### 声明和连接
```cpp
// 声明信号
Simple::Signal<char(float, int, std::string)> signal;

// 连接匹配签名的lambda
signal.connect([](float f, int i, std::string s) -> char {
    std::cout << "Received: " << f << ", " << i << ", " << s << std::endl;
    return 'S'; // 返回char
});

// 连接自由函数
char free_func(float f, int i, std::string s) {
    return 'F';
}
signal.connect(free_func);
```

### 发射信号
```cpp
// 调用emit，必须提供正确类型和数量的参数
char result = signal.emit(3.14f, 42, "hello");

// 编译错误：参数类型或数量不匹配
signal.emit(3.14);                          // 参数太少
signal.emit(3.14f, 42, "hello", "extra");   // 参数太多
signal.emit("string", 42, 3.14f);           // 类型错误
```

## 模板元编程技巧

### 类型分解技术
库内部使用模板特化来分解函数类型：

```cpp
// 主模板（未定义）
template<typename> struct function_traits;

// 特化：分解R(Args...)格式
template<typename R, typename... Args>
struct function_traits<R(Args...)> {
    using return_type = R;
    using argument_types = std::tuple<Args...>;
    static constexpr size_t arity = sizeof...(Args);
};

// 使用示例：
using Sig = char(float, int, std::string);
function_traits<Sig>::return_type;         // char
function_traits<Sig>::arity;               // 3
std::tuple_element<1, function_traits<Sig>::argument_types>::type;  // int
```