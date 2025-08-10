
# 模板参数推导（Template Argument Deduction）
> [Template argument deduction - cppreference.com](https://en.cppreference.com/w/cpp/language/template_argument_deduction.html) 

在C++中，模板参数的类型是根据传入的参数自动推导的。这个过程称为**模板参数推导（Template Argument Deduction）**。

```cpp
template <typename T>
void forwardFunction(T&& arg) {
    process(std::forward<T>(arg)); // 完美转发
}
```

`std::forward` 是一个类型转换函数，用于保持参数的左值或右值属性不变。它的行为取决于模板参数 `T` 的类型：

- 如果 `T` 是左值引用类型（如 `int&`），`std::forward<T>(arg)` 会将 `arg` 转换为左值引用。
- 如果 `T` 是非引用类型（如 `int`），`std::forward<T>(arg)` 会将 `arg` 转换为右值引用。

## 引用折叠规则
C++11引入了引用折叠规则，用于处理右值引用的模板参数。引用折叠规则如下：

1. **左值引用到左值引用**：`T& &` 折叠为 `T&`。
2. **右值引用到左值引用**：`T&& &` 折叠为 `T&`。
3. **左值引用到右值引用**：`T& &&` 折叠为 `T&`。
4. **右值引用到右值引用**：`T&& &&` 折叠为 `T&&`。

## 模板参数推导与引用折叠
当调用 `forwardFunction` 时，模板参数 `T` 的类型会根据传入的参数自动推导。具体来说：

1. **传入左值**：
   - 如果传入的是左值（如变量 `x`），模板参数 `T` 会被推导为左值引用类型。
   - 例如，`forwardFunction(x)` 中，`x` 是左值，`T` 被推导为 `int&`。
   - 参数 `T&& arg` 会退化为左值引用 `int&`。

2. **传入右值**：
   - 如果传入的是右值（如字面量 `20`），模板参数 `T` 会被推导为非引用类型。
   - 例如，`forwardFunction(20)` 中，`20` 是右值，`T` 被推导为 `int`。
   - 参数 `T&& arg` 保持为右值引用 `int&&`。

## 左值引用到左值引用
```cpp
template <typename T>
void test(T& ref) {
    // do something
}

int main() {
    int x = 10;
    test(x); // T 被推导为 int，T& 是 int&
}
```
- `T` 被推导为 `int`，`T&` 是 `int&`。

## 右值引用到左值引用
```cpp
template <typename T>
void test(T& ref) {
    // do something
}

int main() {
    test(20); // T 被推导为 int，T& 是 int&
}
```
- `T` 被推导为 `int`，`T&` 是 `int&`。

## 左值引用到右值引用
```cpp
template <typename T>
void test(T&& ref) {
    // do something
}

int main() {
    int x = 10;
    test(x); // T 被推导为 int&，T&& 是 int&
}
```
- `T` 被推导为 `int&`，`T&&` 折叠为 `int&`。

## 右值引用到右值引用
```cpp
template <typename T>
void test(T&& ref) {
    // do something
}

int main() {
    test(20); // T 被推导为 int，T&& 是 int&&
}
```
- `T` 被推导为 `int`，`T&&` 是 `int&&`。

## 总结
- **模板参数推导**：模板参数 `T` 的类型是根据传入的参数自动推导的。
- **引用折叠规则**：`T&&` 的行为取决于 `T` 的类型。
  - 如果 `T` 是左值引用类型（如 `int&`），`T&&` 折叠为左值引用 `int&`。
  - 如果 `T` 是非引用类型（如 `int`），`T&&` 保持为右值引用 `int&&`。
- **`std::forward`**：用于保持参数的左值或右值属性不变。
  - 如果 `T` 是左值引用类型，`std::forward<T>(arg)` 保持左值属性。
  - 如果 `T` 是非引用类型，`std::forward<T>(arg)` 保持右值属性。

通过这些规则，`forwardFunction` 能够同时转发左值和右值，确保参数的类型和属性在转发过程中保持不变。