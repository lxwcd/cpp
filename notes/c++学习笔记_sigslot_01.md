sigslot 库学习笔记

# 学习笔记
> [GitHub - palacaze/sigslot: A simple C++14 signal-slots implementation](https://github.com/palacaze/sigslot) 

# copy_on_write 类
```cpp

/**
 * A simple copy on write container that will be used to improve slot lists
 * access efficiency in a multithreaded context.
 */
template <typename T>
class copy_on_write {
    struct payload {
        payload() = default;

        template <typename... Args>
        explicit payload(Args && ...args)
            : value(std::forward<Args>(args)...)
        {}

        std::atomic<std::size_t> count{1};
        T value;
    };

public:
    using element_type = T;

    copy_on_write()
        : m_data(new payload)
    {}

    template <typename U>
    explicit copy_on_write(U && x, std::enable_if_t<!std::is_same<std::decay_t<U>,
                           copy_on_write>::value>* = nullptr)
        : m_data(new payload(std::forward<U>(x)))
    {}

    copy_on_write(const copy_on_write &x) noexcept
        : m_data(x.m_data)
    {
        ++m_data->count;
    }

    copy_on_write(copy_on_write && x) noexcept
        : m_data(x.m_data)
    {
        x.m_data = nullptr;
    }

    ~copy_on_write() {
        if (m_data && (--m_data->count == 0)) {
            delete m_data;
        }
    }

    copy_on_write& operator=(const copy_on_write &x) noexcept {
        if (&x != this) {
            *this = copy_on_write(x);
        }
        return *this;
    }

    copy_on_write& operator=(copy_on_write && x) noexcept  {
        auto tmp = std::move(x);
        swap(*this, tmp);
        return *this;
    }

    element_type& write() {
        if (!unique()) {
            *this = copy_on_write(read());
        }
        return m_data->value;
    }

    const element_type& read() const noexcept {
        return m_data->value;
    }

    friend inline void swap(copy_on_write &x, copy_on_write &y) noexcept {
        using std::swap;
        swap(x.m_data, y.m_data);
    }

private:
    bool unique() const noexcept {
        return m_data->count == 1;
    }

private:
    payload *m_data;
};
```

核心思想：写时复制(Copy-on-Write)
- **读操作**：共享同一份数据，零拷贝
- **写操作**：当数据被多个对象共享时，创建副本再修改
- **优点**：减少不必要的拷贝，提高读取效率
- **适用场景**：读多写少的共享数据

## 内部结构体 `payload`

```cpp
struct payload {
    payload() = default;  // 默认构造函数
    
    // 完美转发构造函数
    template <typename... Args>
    explicit payload(Args && ...args)
        : value(std::forward<Args>(args)...)
    {}
    
    std::atomic<std::size_t> count{1}; // 原子引用计数器
    T value;                          // 存储的实际值
};
```

- **模板构造函数**：`payload` 的模板构造函数可以接受任意数量和类型的参数，并将它们转发到 `T` 的构造函数中。
- **完美转发**：使用 `std::forward` 实现完美转发，确保参数的值类别（左值或右值）在转发过程中保持不变。
- **通用性**：`payload` 可以用于任何类型 `T`，只要 `T` 的构造函数接受相应的参数。

### 默认构造函数
```cpp
payload() = default;
```
- **`payload()`**：默认构造函数。
- **`= default;`**：这是C++11引入的语法，表示默认构造函数。编译器会自动生成一个默认构造函数，初始化所有成员变量为默认值。
- **作用**：初始化 `count` 为 1，`value` 为默认值。

### 模板构造函数
```cpp
template <typename... Args>
explicit payload(Args && ...args)
    : value(std::forward<Args>(args)...)
{}
```
- **`template <typename... Args>`**：模板参数包，表示可以接受任意数量和类型的参数。
- **`explicit`**：表示这个构造函数是显式的，不能用于隐式类型转换。
- **`Args && ...args`**：参数包扩展，表示接受一个可变数量的右值引用参数。
- **`std::forward<Args>(args)...`**：完美转发，将参数 `args` 转发到 `value` 的构造函数中。
- **作用**：使用完美转发将参数转发到 `value` 的构造函数中，初始化 `value`。

### 成员变量
```cpp
std::atomic<std::size_t> count{1};
T value;
```
- **`std::atomic<std::size_t> count{1};`**：一个原子变量，用于引用计数。初始化为 1，表示当前对象有一个引用。
- **`T value;`**：存储实际的数据，类型为 `T`。

**原子引用计数器 `count`**
   - 类型：`std::atomic<std::size_t>`
   - 初始值：`1`（创建时即有一个引用）
   - 作用：线程安全的引用计数，确保多线程环境下安全操作
   
### `payload` 的使用方法

```cpp
#include <iostream>
#include <atomic>
#include <utility>
#include <string>

template <typename T>
struct payload {
    payload() = default;

    template <typename... Args>
    explicit payload(Args && ...args)
        : value(std::forward<Args>(args)...)
    {}

    std::atomic<std::size_t> count{1};
    T value;
};

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
};

int main() {
    // 使用 int
    payload<int> p1(10);
    std::cout << "p1.value: " << p1.value << std::endl; // 输出：p1.value: 10

    // 使用 std::string
    payload<std::string> p2("Hello, World!");
    std::cout << "p2.value: " << p2.value << std::endl; // 输出：p2.value: Hello, World!

    // 使用 std::pair
    payload<std::pair<int, int>> p3(10, 20);
    std::cout << "p3.value: (" << p3.value.first << ", " << p3.value.second << ")" << std::endl; // 输出：p3.value: (10, 20)

    // 使用自定义类型 Point
    payload<Point> p4(10, 20);
    std::cout << "p4.value: (" << p4.value.x << ", " << p4.value.y << ")" << std::endl; // 输出：p4.value: (10, 20)

    return 0;
}
```

输出结果
```
p1.value: 10
p2.value: Hello, World!
p3.value: (10, 20)
p4.value: (10, 20)
```

## 默认构造函数
```cpp
copy_on_write()
    : m_data(new payload)  // 创建新的 payload 对象
{}
```
- 创建一个新的 `payload` 对象
- 引用计数器初始化为 1

## 通用值构造函数
```cpp
template <typename U>
explicit copy_on_write(U && x, 
    std::enable_if_t<!std::is_same<std::decay_t<U>, copy_on_write>::value>* = nullptr)
    : m_data(new payload(std::forward<U>(x)))
{}

```

这个构造函数的作用是：
允许用户使用任何类型 U 来初始化 copy_on_write，但排除了 copy_on_write 自身类型。
如果用户尝试使用 copy_on_write 类型来初始化另一个 copy_on_write 实例，编译器会报错，防止不必要的拷贝或移动操作。
```cpp
copy_on_write<int> cow1(42); // 正确，U 是 int
copy_on_write<std::string> cow2("Hello, World!"); // 正确，U 是 std::string
copy_on_write<copy_on_write<int>> cow3(cow1); // 错误，U 是 copy_on_write<int>
```

- **模板参数 `U`**：表示构造函数接受的参数类型。`U` 可以是任意类型，包括基本类型（如 `int`、`double`）、自定义类型等。
- **`explicit`**：用于防止隐式类型转换。当你在构造函数前加上 `explicit` 时，这个构造函数不能被用于隐式类型转换，只能用于显式构造。例如：
  ```cpp
  copy_on_write<int> c = 42; // 隐式构造，不允许
  copy_on_write<int> c(42);  // 显式构造，允许
  ```

```cpp
U && x
```
- **`U &&`**：表示参数 `x` 是一个右值引用。这里使用了模板参数 `U`，因此 `x` 可以是任意类型的右值引用。
- **`&&`**：右值引用，用于支持完美转发。

```cpp
std::enable_if_t<!std::is_same<std::decay_t<U>, copy_on_write>::value>* = nullptr
```
- **`std::enable_if_t`**：是一个模板别名，用于根据条件启用或禁用模板。
  - `std::enable_if_t<Cond, T>` 的定义如下：
    ```cpp
    template <bool Cond, typename T = void>
    using enable_if_t = typename enable_if<Cond, T>::type;
    ```
  - `Cond` 是一个布尔条件，`T` 是一个类型，默认为 `void`。
  - 如果 `Cond` 为 `true`，`std::enable_if_t<Cond, T>` 的类型为 `T`；如果 `Cond` 为 `false`，`std::enable_if_t<Cond, T>` 不存在，这会导致编译错误。

- **`!std::is_same<std::decay_t<U>, copy_on_write>::value`**：
  - `std::is_same<T, U>::value`：检查类型 `T` 和 `U` 是否相同，返回一个布尔值。
  - `std::decay_t<U>`：去除 `U` 的引用和 `const` 修饰符，返回基础类型。
  - `!std::is_same<std::decay_t<U>, copy_on_write>::value`：确保 `U` 不是 `copy_on_write` 的实例。

- **`* = nullptr`**：这是一个默认参数，类型为 `std::enable_if_t<...>*`。由于 `std::enable_if_t<...>` 的默认型为 `void`，因此这个参数的默认型为 `void*`。这个参数的默认值为 `nullptr`，用于确保编译器不会报错。

## 初始化列表
```cpp
: m_data(new payload(std::forward<U>(x)))
```
- **`m_data`**：是 `copy_on_write` 类的成员变量，类型为 `payload*`。
- **`new payload(std::forward<U>(x))`**：
  - `new payload`：动态分配一个新的 `payload` 对象。
  - `std::forward<U>(x)`：完美转发参数 `x` 到 `payload` 的构造函数中。`std::forward` 用于保持参数 `x` 的值类别（左值或右值）不变。

## 拷贝构造函数
```cpp
copy_on_write(const copy_on_write &x) noexcept
    : m_data(x.m_data)  // 共享同一份 payload
{
    ++m_data->count;    // 原子递增引用计数
}
```
- 共享同一份数据
- 原子操作递增引用计数
- `noexcept` 保证不抛出异常

## 移动构造函数
```cpp
copy_on_write(copy_on_write && x) noexcept
    : m_data(x.m_data)  // 接管数据所有权
{
    x.m_data = nullptr; // 置空原指针，防止双重释放
}
```
- 转移数据所有权
- 原对象置空，不再拥有数据
- `noexcept` 保证移动操作安全

## 析构函数

```cpp
~copy_on_write() {
    if (m_data && (--m_data->count == 0)) {
        delete m_data;
    }
}
```

1. 检查 `m_data` 是否有效
2. 原子递减引用计数
3. 当计数归零时，删除 `payload` 对象
4. 线程安全的引用计数管理

## 拷贝赋值运算符
```cpp
copy_on_write& operator=(const copy_on_write &x) noexcept {
    if (&x != this) {  // 防止自赋值
        *this = copy_on_write(x); // 拷贝构造 + 交换
    }
    return *this;
}
```
- 使用拷贝并交换惯用法（copy-and-swap idiom）
- 创建临时副本后交换
- 自动处理原有资源的释放

## 移动赋值运算符
```cpp
copy_on_write& operator=(copy_on_write && x) noexcept {
    auto tmp = std::move(x); // 移动构造临时对象
    swap(*this, tmp);       // 交换当前对象与临时对象
    return *this;
}
```
- 移动构造临时对象
- 交换当前对象内容
- 临时对象析构时释放原资源

## swap 函数
```cpp
friend inline void swap(copy_on_write &x, copy_on_write &y) noexcept {
    using std::swap;
    swap(x.m_data, y.m_data);
}
```

- **成员函数**：成员函数是类的一部分，可以访问类的私有和保护成员。成员函数的第一个参数是隐式的 `this` 指针，指向调用该函数的对象。
- **友元函数**：友元函数不是类的成员函数，但它可以访问类的私有和保护成员。友元函数没有隐式的 `this` 指针，它需要显式地传递对象作为参数。

- **成员函数**：成员函数通过对象调用，例如 `obj.memberFunction()`。
- **友元函数**：友元函数是普通的函数，通过函数名和参数调用，例如 `friendFunction(obj1, obj2)`。

- **成员函数**：成员函数是类的一部分，通常用于操作单个对象。
- **友元函数**：友元函数通常用于操作两个或多个对象，或者需要访问多个对象的私有成员。

在 `copy_on_write` 类中，`swap` 函数需要交换两个 `copy_on_write` 对象的内部数据 `m_data`。如果使用成员函数，那么交换操作只能在一个对象上调用，而另一个对象需要作为参数传递。这会导致代码的逻辑不够直观和对称。

使用成员函数的可能实现：
```cpp
void swap(copy_on_write &other) noexcept {
    using std::swap;
    swap(this->m_data, other.m_data);
}
```

调用方式：
```cpp
x.swap(y);
```

## 写操作 `write()`
```cpp
element_type& write() {
    if (!unique()) {         // 检查是否唯一持有
        *this = copy_on_write(read()); // 创建副本
    }
    return m_data->value;    // 返回可修改引用
}
```

1. 检查当前是否唯一持有数据 (`unique()`)
2. 如果不是唯一持有者：
   - 通过 `read()` 获取当前值，返回常量左值引用
   - 使用模板构造函数创建新副本，此副本的计数 count 为 1
   - 赋值给当前对象
3. 返回内部数据的可修改引用

### 注意
这个写操作内置


## 读操作 `read()`
```cpp
const element_type& read() const noexcept {
    return m_data->value; // 返回常量引用
}
```
- 返回常量引用，防止意外修改
- `noexcept` 保证不抛出异常
- 高效零拷贝访问
- 返回左值引用

## 唯一性检查
```cpp
bool unique() const noexcept {
    return m_data->count == 1; // 原子访问计数
}
```
- 检查当前是否唯一持有数据
- 用于决定是否需要执行写时复制

## 线程安全性分析

| 操作     | 线程安全保证             | 说明                   |
| -------- | ------------------------ | ---------------------- |
| 构造函数 | 安全（新对象独立）       | 创建新数据             |
| 拷贝构造 | 安全（原子递增）         | 共享数据，原子操作计数 |
| 移动构造 | 安全（转移所有权）       | 原对象置空             |
| 析构函数 | 安全（原子递减）         | 原子操作计数，最后释放 |
| 读操作   | 安全（常量访问）         | 多线程同时读取安全     |
| 写操作   | **不安全**（需外部同步） | 检查+复制非原子操作    |
| 赋值操作 | 依赖拷贝/移动构造安全性  | 间接依赖底层操作安全性 |

## 使用示例

### 基本使用
```cpp
copy_on_write<std::vector<int>> shared_data;

// 线程1：读取数据
std::thread reader([&] {
    auto& data = shared_data.read(); // 安全读取
    std::cout << "Size: " << data.size() << "\n";
});

// 线程2：修改数据
std::thread writer([&] {
    auto& mutable_data = shared_data.write(); // 必要时复制
    mutable_data.push_back(42);
});

reader.join();
writer.join();
```

### 高效赋值
```cpp
copy_on_write<std::string> str1("Hello");
auto str2 = str1; // 共享数据，无拷贝

// 修改时复制
str2.write() += " World"; 

std::cout << str1.read() << "\n"; // 输出 "Hello"
std::cout << str2.read() << "\n"; // 输出 "Hello World"
```

## 设计模式分析

### 1. 享元模式 (Flyweight)
- 共享相同数据，减少内存占用
- 只在必要时创建副本

### 2. 写时复制 (Copy-on-Write)
- 延迟复制策略，优化性能
- 读操作零开销，写操作按需复制

### 3. 引用计数
- 自动内存管理
- 原子操作保证线程安全

## 改进建议

### 1. 写操作线程安全增强
```cpp
element_type& write() {
    payload* old_data = m_data;
    
    // 检查并准备新数据
    if (!unique()) {
        auto* new_data = new payload(read());
        if (m_data->count.fetch_sub(1) != 1) {
            m_data = new_data;
        } else {
            delete new_data;
            delete old_data;
        }
    }
    
    return m_data->value;
}
```

### 2. 添加线程安全包装
```cpp
template <typename T>
class synchronized_cow {
public:
    // 加锁的写操作
    template <typename Func>
    void write(Func&& f) {
        std::lock_guard<std::mutex> lock(m_mutex);
        f(cow.write());
    }
    
    // 加锁的读操作
    template <typename Func>
    void read(Func&& f) const {
        std::lock_guard<std::mutex> lock(m_mutex);
        f(cow.read());
    }
    
private:
    copy_on_write<T> cow;
    mutable std::mutex m_mutex;
};
```

### 3. 支持自定义分配器
```cpp
template <typename T, typename Allocator = std::allocator<payload>>
class copy_on_write {
    // 使用分配器创建和删除 payload
};
```

## 性能考量

1. **读操作**：与直接访问数据几乎相同（仅多一次指针解引用）
2. **写操作**：
   - 唯一持有时：与直接修改相同
   - 共享时：一次完整拷贝 + 原子操作开销
3. **内存占用**：额外增加一个指针和原子计数（通常 8-16 字节）

## 适用场景

1. **配置数据**：多线程读取配置，偶尔更新
2. **UI 状态**：界面元素共享状态数据
3. **缓存系统**：只读缓存数据共享
4. **函数式数据结构**：持久化数据结构实现
5. **资源管理**：大型资源（如图像、模型）共享

## 总结

此 `copy_on_write` 类实现了经典的写时复制语义：

1. **高效读操作**：共享数据，零拷贝访问
2. **惰性写操作**：仅在必要时复制数据
3. **自动内存管理**：引用计数自动释放资源
4. **线程安全基础**：原子引用计数保证基本安全
5. **现代C++特性**：
   - 完美转发构造函数
   - 移动语义支持
   - `noexcept` 异常规范
   - SFINAE 约束构造函数