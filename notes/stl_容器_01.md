SLT 学习笔记

# STL 介绍

**STL (Standard Template Library)**，即**标准模板库**，是 C++ 语言标准库中一个强大且不可或缺的组成部分。它提供了一系列可复用的**通用模板类**和**模板函数**，主要用于实现常见的数据结构（容器）和算法。

可以将它理解为一个**强大的“工具箱”**，里面装满了预先制作好的、高度优化的“工具”（如各种容器、算法等）。当你要解决编程问题时，不需要从头造轮子，直接从这个工具箱里挑选合适的工具组合使用即可，极大地提高了开发效率和程序质量。

STL 的设计建立在两个伟大的编程范式和一個核心概念之上：

1.  **泛型编程 (Generic Programming)**：
    *   这是 STL 的灵魂。它通过**模板 (Template)** 技术实现，意味着代码不依赖于特定的数据类型。
    *   例如，一个 `sort` 算法可以用于 `int`、`double`、`string` 甚至是你自定义的类对象，只要这些类型支持比较操作。这实现了真正的代码复用。

2.  **面向对象编程 (OOP)**：
    *   虽然 STL 大量使用模板，但它也融合了OOP的思想，如封装（容器封装了底层数据结构）、继承和多态（迭代器类别之间的继承关系）。

而将上述两者完美结合起来的粘合剂，就是它的**核心设计理念**：

**“将数据结构和算法分离开，再通过迭代器使它们协同工作。”**

在STL之前，数据结构（如链表类）通常会自带其操作方法（如排序、查找）。这导致每个数据结构都有自己的方法，难以复用。STL 打破了这一模式：
*   **容器 (Containers)**： 只负责管理和存储数据。
*   **算法 (Algorithms)**： 只负责操作数据（如排序、查找），并且是**全局函数**，不依赖于任何特定容器。
*   **迭代器 (Iterators)**： 作为两者之间的桥梁。算法通过迭代器来访问和操作容器中的元素，而无需关心容器内部的具体实现。

这种“分离”的设计使得 STL 极其灵活和强大。

# STL 的六大组件 (The Six Components)

1.  **容器 (Containers)** - **“数据的家”**
    *   用来存放和管理数据的类模板。例如：`vector`（动态数组）、`list`（链表）、`map`（键值对集合）、`set`（集合）。

2.  **算法 (Algorithms)** - **“操作的工具”**
    *   用来处理容器中数据的函数模板。例如：`sort`（排序）、`find`（查找）、`copy`（复制）。**它们通过迭代器操作容器，而不依赖于容器本身**。

3.  **迭代器 (Iterators)** - **“通用的指针”**
    *   是容器和算法之间的**胶合剂 (Glue)**。它提供了一种方法来顺序或随机访问容器中的元素，其行为类似于指针（可以解引用`*iter`、移动`++iter`等）。

4.  **仿函数 (Functors)** - **“行为像函数的对象”**
    *   重载了函数调用运算符 `operator()` 的类。它的对象可以像函数一样被调用。常用于定制算法的行为策略（例如，为 `sort` 指定自定义的排序规则）。

5.  **适配器 (Adapters)** - **“转换接口的包装器”**
    *   基于其他组件修改接口，提供一种新的功能。包括：
        *   **容器适配器**： `stack`（栈）、`queue`（队列）、`priority_queue`（优先队列）。它们底层默认基于 `deque` 实现，但改变了接口。
        *   **迭代器适配器**： `reverse_iterator`（反向迭代器）。
        *   **函数适配器**： (现代C++中较少使用，通常被 `bind` 和 `lambda` 表达式替代)。

6.  **分配器 (Allocators)** - **“内存管理的幕后工作者”**
    *   负责底层内存的分配与释放。对于绝大多数应用开发来说，我们不需要直接接触它，使用默认的分配器就足够了。

# vector

`vector` 是 C++ 标准模板库（STL）中最常用的**序列式容器**之一。它可以被看作是一个**动态数组**，能够根据需要在运行时自动调整大小。

*   **核心特性**：它占用一段**连续**的内存空间，这意味着可以像使用普通数组一样通过下标（`[]`运算符）来随机访问其中的元素，效率极高（时间复杂度 O(1)）。
*   **动态扩容**：这是它相对于普通数组最大的优势。当插入新元素导致当前容量不足时，`vector` 会自动分配一块更大的内存，将原有元素复制或移动到新空间，并释放旧空间。这个过程对用户是透明的。

## 底层实现与关键机制

为了降低空间配置时的速度成本， vector 实际配置的大小可能比用户实际需求的更大，以备将来扩充。这便是容量（capacity） 的观念。

理解 `vector` 的关键在于理解它的三个核心指针（或可以理解为三个索引位置）：

1.  `start`：指向当前使用的内存空间的首元素。
2.  `finish`：指向当前最后一个元素的**下一个**位置（尾后指针）。`size()` 函数返回的就是 `finish - start`。
3.  `end_of_storage`：指向整个内存空间末尾的下一个位置。`capacity()` 函数返回的就是 `end_of_storage - start`。

**扩容机制 (Amortized Constant Time)**
当执行 `push_back()` 而 `size() == capacity()` 时，就会触发扩容。常见的扩容策略是：
*   如果当前容量为 0，则分配 1（或另一个较小的初始值）。
*   否则，分配 `当前容量 * 2`（即倍增，GCC 和 Clang 的实现）或 `当前容量 * 1.5`（MSVC 的实现）。

虽然倍增可能会导致一定的内存浪费（平均内存使用率约在 50%-100% 之间），但它能将多次 `push_back` 操作中分摊的（Amortized）时间复杂度降至 **O(1)**。如果每次只固定增加一个单位，那么每次扩容都需要 O(N) 的时间来复制所有元素，性能会非常差。

## 基本操作与时间复杂度

| 操作              | 函数                  | 时间复杂度    | 说明                                                                                             |
| :---------------- | :-------------------- | :------------ | :----------------------------------------------------------------------------------------------- |
| **随机访问**      | `v[i]`, `v.at(i)`     | **O(1)**      | 像数组一样快。`at()` 会进行边界检查，越界则抛出异常。                                            |
| **尾部插入**      | `push_back(value)`    | **分摊 O(1)** | 最常用的高效插入操作。                                                                           |
| **尾部删除**      | `pop_back()`          | **O(1)**      | 删除最后一个元素。                                                                               |
| **头部插入/删除** | `insert()`, `erase()` | **O(N)**      | 需要移动后续所有元素，**非常昂贵**！                                                             |
| **中间插入/删除** | `insert()`, `erase()` | **O(N)**      | 需要移动插入点之后的所有元素，**昂贵**！                                                         |
| **查找**          | (无内置find)          | **O(N)**      | 需要使用 `std::find()` 算法进行线性查找。                                                        |
| **获取大小**      | `size()`              | **O(1)**      | 返回元素数量。                                                                                   |
| **获取容量**      | `capacity()`          | **O(1)**      | 返回当前分配的内存可容纳的元素数量。                                                             |
| **改变容量**      | `reserve(n)`          | **O(N)**      | **预分配内存**。如果 `n > capacity()`，则分配至少可容纳 n 个元素的空间。这是**性能优化的关键**！ |
| **调整大小**      | `resize(n)`           | **O(N)**      | 改变 `size()`。如果 `n > size()`，则会添加新元素。                                               |

## 优点：
1.  **快速随机访问**：通过索引访问元素的速度极快。
2.  **尾部操作高效**：在末尾添加和删除元素非常快（分摊常数时间）。
3.  **缓存友好**：由于数据在内存中连续存储，对 CPU 缓存非常友好，访问效率高。
4.  **使用方便**：无需手动管理内存，自动处理扩容和释放。

## 缺点：
1.  **中部/头部操作昂贵**：在中间或开头插入或删除元素需要移动大量数据，效率低下。
2.  **存在内存浪费**：`capacity() >= size()`，总会有一些未使用的预留空间。
3.  **扩容成本高**：扩容时需要重新分配内存并复制所有元素，这是一个相对昂贵的操作。


## 最重要的性能优化：使用 `reserve()`
如果提前知道 `vector` 大致会存放多少元素，使用 `reserve()` 来预分配内存。这可以避免多次扩容，极大提升性能。
```cpp
vector<int> data;
data.reserve(1000); // 预先分配至少1000个元素的空间
for (int i = 0; i < 1000; ++i) {
    data.push_back(i); // 这1000次push_back都不会触发扩容
}
```

## 迭代元素
```cpp
// 1. 下标遍历 (最常用)
for (size_t i = 0; i < v.size(); ++i) {
    cout << v[i] << endl;
}

// 2. 范围for循环 (C++11, 简洁)
for (const auto& element : v) {
    cout << element << endl;
}

// 3. 迭代器遍历
for (auto it = v.begin(); it != v.end(); ++it) {
    cout << *it << endl;
}
```

## 注意迭代器失效
在对 `vector` 进行修改操作（如 `insert`, `erase`, `push_back`（可能引起扩容））后，指向其元素的**迭代器、引用和指针可能会失效**。
*   **扩容后**：所有的迭代器、指针、引用**全部失效**。
*   **插入后**：插入点之后的所有迭代器、指针、引用**都会失效**。
*   **删除后**：删除点之后的所有迭代器、指针、引用**都会失效**。

**错误示例**：
```cpp
vector<int> v = {1, 2, 3, 4};
auto it = v.begin() + 2;
v.push_back(5); // 可能导致扩容，it 失效！
cout << *it; // 未定义行为！程序可能崩溃。
```

## 总结与适用场景

**`vector` 是 C++ 中默认首选的序列容器**，除非你有充分的理由选择其他容器（如 `list`, `deque`），否则应该优先使用 `vector`。

**适用场景**：
*   需要频繁**随机访问**元素。
*   主要是在容器的**末尾**进行添加和删除操作。
*   元素数量未知或会变化，但你希望内存连续以提高访问效率。
*   非常注重**缓存性能**。

**不适用场景**：
*   需要在容器的**头部或中部**频繁进行插入和删除操作（应考虑 `std::list` 或 `std::deque`）。
*   元素非常大且复制成本高（扩容时的复制代价会很大）。
*   要求插入和删除操作绝对保证迭代器不失效（应考虑 `std::list`）。


## 构造函数

### 默认构造函数
```cpp
vector<int> v1; // 创建一个空的 vector
```

### 指定大小的构造函数
```cpp
vector<int> v2(5); // 创建包含 5 个元素的 vector，默认值为 0
vector<string> v3(3, "hello"); // 创建包含 3 个元素的 vector，每个元素都是 "hello"
```

### 拷贝构造函数
```cpp
vector<int> v4(v2); // 创建 v2 的副本
```

### 范围构造函数
```cpp
int arr[] = {1, 2, 3, 4, 5};
vector<int> v5(arr, arr + 3); // 使用前3个元素创建 vector: {1, 2, 3}
```

### 初始化列表构造函数 (C++11)
```cpp
vector<int> v6 = {1, 2, 3, 4, 5}; // 使用初始化列表
```

## 赋值操作

### `operator=` 赋值
```cpp
vector<int> v7 = v6; // 拷贝赋值
v7 = {6, 7, 8}; // 使用初始化列表赋值
```

### `assign()` 方法
```cpp
v7.assign(5, 100); // 分配5个值为100的元素: {100, 100, 100, 100, 100}
v7.assign(arr, arr + 3); // 使用范围赋值: {1, 2, 3}
```

## 元素访问

### `operator[]` - 随机访问（不检查边界）
```cpp
vector<int> v = {1, 2, 3, 4, 5};
cout << v[2]; // 输出: 3
v[1] = 10; // 修改第二个元素为10
```

### `at()` - 随机访问（检查边界）
```cpp
cout << v.at(2); // 输出: 3
// v.at(10); // 如果越界，会抛出 std::out_of_range 异常
```

### `front()` 和 `back()` - 访问首尾元素
```cpp
cout << v.front(); // 输出第一个元素: 1
cout << v.back();  // 输出最后一个元素: 5
```

### `data()` - 访问底层数组 (C++11)
```cpp
int* p = v.data(); // 返回指向底层数组的指针
cout << p[2]; // 输出: 3
```

## 迭代器

### 常规迭代器
```cpp
vector<int>::iterator it;
for (it = v.begin(); it != v.end(); ++it) {
    cout << *it << " ";
}
```

### 常量迭代器
```cpp
vector<int>::const_iterator cit;
for (cit = v.cbegin(); cit != v.cend(); ++cit) {
    cout << *cit << " "; // 不能修改元素
}
```

### 反向迭代器
```cpp
for (auto rit = v.rbegin(); rit != v.rend(); ++rit) {
    cout << *rit << " "; // 反向遍历
}
```

## 容量相关

### `empty()` - 检查是否为空
```cpp
if (v.empty()) {
    cout << "Vector is empty";
}
```

### `size()` - 返回元素数量
```cpp
cout << v.size(); // 输出元素个数
```

### `capacity()` - 返回当前容量
```cpp
cout << v.capacity(); // 输出当前分配的内存可容纳的元素数量
```

**`capacity()`**：是一个**查询函数**，它返回一个值，告诉你当前 `vector` 已经分配的总空间（以元素个数计），即在不触发重新分配的情况下，最多可以容纳多少元素。

无论内存是通过构造函数、`reserve()`、`resize()` 还是通过 `push_back()` 自动扩容分配的，`capacity()` 返回的值总是当前vector底层数组的实际长度。


### `reserve()` - 预留空间
```cpp
v.reserve(100); // 预留至少100个元素的空间，避免多次重新分配
```

**`reserve()` 是一个“请求”而非“命令”**
调用 `vec.reserve(100)` 的意思是：“请确保容量**至少**为100”。实现可能会分配**恰好**100的空间，也可能会分配**多于**100的空间（例如，采用倍增策略时，可能会分配128）。所以 `capacity()` 之后可能等于也可能大于 `reserve` 的数字。
```cpp
vector<int> vec;
vec.reserve(100);
cout << vec.capacity(); // 输出可能是 100, 也可能是 128, 144 等，但保证 >= 100
```

**作用：** 请求 vector 的 **`capacity`（容量）** 至少为 `n`。

-   它只是一个**对内存的请求**，不影响 `size`，也不创建或销毁任何元素。
-   如果 `n > current_capacity`，则会分配新的、更大的内存空间（至少能容纳 `n` 个元素），并将所有现有元素**移动或复制**到新空间，然后释放旧空间。这个过程会使**所有迭代器、指针、引用失效**。
-   如果 `n <= current_capacity`，则**什么也不做**。它不会缩小容量。

### `shrink_to_fit()` - 减少容量以适应大小 (C++11)
```cpp
v.shrink_to_fit(); // 请求减少容量以适应当前大小
```

## 修改操作

### `push_back()` - 在末尾添加元素
```cpp
v.push_back(6); // 在末尾添加元素6
```

### `pop_back()` - 删除末尾元素
```cpp
v.pop_back(); // 删除最后一个元素
```

### `insert()` - 插入元素
```cpp
// 在指定位置插入元素
v.insert(v.begin() + 2, 99); // 在第三个位置插入99

// 在指定位置插入多个相同元素
v.insert(v.begin(), 3, 0); // 在开头插入3个0

// 在指定位置插入范围
vector<int> other = {7, 8, 9};
v.insert(v.end(), other.begin(), other.end()); // 在末尾插入other的所有元素
```

### `erase()` - 删除元素
```cpp
// 删除指定位置的元素
v.erase(v.begin() + 1); // 删除第二个元素

// 删除范围内的元素
v.erase(v.begin() + 2, v.begin() + 4); // 删除第3到第4个元素
```

### `clear()` - 清空所有元素
```cpp
v.clear(); // 清空vector，但容量不变
```

### `resize()` - 改变大小
```cpp
v.resize(10); // 将大小改为10，新增元素默认初始化为0
v.resize(15, 5); // 将大小改为15，新增元素初始化为5
v.resize(3); // 将大小改为3，删除多余元素
```

**作用：** 直接设置 vector 的 **`size`（元素个数）**。

-   **如果 `n > current_size`**：
    -   vector 的大小会增加至 `n`。
    -   新增的 `(n - current_size)` 个元素会被添加到末尾。
    -   这些新元素会进行**初始化**：
        -   `resize(n)`: 使用类型的默认构造函数（如 `int` 被初始化为 `0`）。
        -   `resize(n, value)`: 使用指定的 `value` 进行初始化。
    -   如果新 `size` 超过了当前 `capacity`，则会触发**重新分配**（扩容），导致所有迭代器、指针、引用失效。

-   **如果 `n < current_size`**：
    -   vector 的大小会减小至 `n`。
    -   末尾的 `(current_size - n)` 个元素会被**销毁**（调用其析构函数）。
    -   **`capacity()` 保持不变**，内存不会被释放。

-   **如果 `n == current_size`**：
    -   什么也不做。

| 特性               | `resize(n)`                                                          | `reserve(n)`                       | `capacity()`                     |
| :----------------- | :------------------------------------------------------------------- | :--------------------------------- | :------------------------------- |
| **作用对象**       | **Size (元素数量)**                                                  | **Capacity (内存容量)**            | **Capacity (内存容量)**          |
| **主要目的**       | **改变** vector 中**实际元素的数量**                                 | **预分配内存**，为添加新元素做准备 | **查询**当前已分配的内存**容量** |
| **是否改变元素**   | **是**。可能创建新元素或销毁现有元素                                 | **否**。只分配内存，不创建任何对象 | **否**。只是查询，无任何影响     |
| **新元素初始化**   | 如果增大，新元素会被**默认初始化**或按指定值初始化                   | 不创建新元素，内存是"原始"的       | 不适用                           |
| **函数类型**       | **Setter** (设置器) - 主动操作                                       | **Setter** (设置器) - 主动操作     | **Getter** (获取器) - 被动查询   |
| **对迭代器的影响** | 如果 `capacity()` 改变，则**全部失效**；否则，仅修改点之后的可能失效 | 如果重新分配发生，则**全部失效**   | 无影响                           |

### `emplace()` - 原位构造元素 (C++11)
```cpp
// 在指定位置原位构造元素，避免拷贝
v.emplace(v.begin() + 2, 42); // 在第三个位置构造值为42的元素
```

### `emplace_back()` - 在末尾原位构造元素 (C++11)
```cpp
// 在末尾原位构造元素，比push_back更高效
v.emplace_back(42);
```

### `emplace_back` 与 `push_back` 的区别与选择

两者的根本区别在于**如何将参数传递给 vector 中的新元素**：

- **`push_back(value)`**：接受一个**已经构造好的对象**（或者可以隐式转换为该对象的参数），然后将其**拷贝**或**移动**到 vector 的末尾。
- **`emplace_back(args...)`**：接受一**系列构造参数**，直接在 vector 的内存末尾**原位构造**（in-place construction）对象，避免任何不必要的拷贝或移动。

#### 1. 对于简单类型（如 `int`, `double`）

对于内置基本类型，两者性能**没有区别**，编译器优化后生成的代码通常是相同的。

```cpp
std::vector<int> vec;

vec.push_back(42);    // 可以
vec.emplace_back(42); // 也可以，效果相同
```

#### 2. 对于复杂类型（体现主要优势）

当 vector 存储的是需要资源管理的复杂对象（如 `std::string`, 自定义类）时，`emplace_back` 的优势就显现出来了。

```cpp
class Person {
public:
    Person(const std::string& name, int age) 
        : name_(name), age_(age) {
        std::cout << "Constructed: " << name_ << std::endl;
    }
    
    Person(const Person& other) 
        : name_(other.name_), age_(other.age_) {
        std::cout << "Copied: " << name_ << std::endl;
    }
    
    Person(Person&& other) noexcept 
        : name_(std::move(other.name_)), age_(other.age_) {
        std::cout << "Moved: " << name_ << std::endl;
    }
    
private:
    std::string name_;
    int age_;
};
```

**使用 `push_back`：**

```cpp
std::vector<Person> people;

// 方式1：传递临时对象（触发移动构造）
people.push_back(Person("Alice", 30));
// 输出：
// Constructed: Alice (临时对象)
// Moved: Alice     (移动到vector中)

// 方式2：传递构造参数（需要创建临时对象，可能触发拷贝）
people.push_back({"Bob", 25}); // 创建一个临时Person对象，然后移动
```

**使用 `emplace_back`：**

```cpp
std::vector<Person> people;

// 直接传递构造参数，原地构造！
people.emplace_back("Charlie", 40);
// 输出：
// Constructed: Charlie (直接在vector的内存中构造)
```

可以看到，`emplace_back` **完全避免了**创建临时对象和移动/拷贝的操作，性能更高。

#### 推荐使用 `emplace_back` 的情况：

1.  **构造参数昂贵时**：当对象的构造函数需要执行大量操作时。
2.  **避免不必要的拷贝**：当对象不可拷贝或拷贝代价很高时。
3.  **需要直接传递多个构造参数时**：
    ```cpp
    // 更简洁
    my_vec.emplace_back(arg1, arg2, arg3);
    
    // 相对于
    my_vec.push_back(MyClass(arg1, arg2, arg3));
    ```

#### 可能需要使用 `push_back` 的情况：

1.  **需要显式创建临时对象时**：
    ```cpp
    auto obj = get_object();
    vec.push_back(obj); // 明确要拷贝一个已存在的对象
    ```

2.  **使用初始化列表时**（语法更清晰）：
    ```cpp
    std::vector<std::vector<int>> vec;
    
    // 更清晰
    vec.push_back({1, 2, 3});
    
    // 也可以，但稍显冗长
    vec.emplace_back(std::initializer_list<int>{1, 2, 3});
    ```

3.  **与 `std::make_unique` 等工厂函数配合时**：
    ```cpp
    vec.push_back(std::make_unique<MyClass>(args...));
    ```

#### 重要注意事项

##### 1. 异常安全性
`emplace_back` 提供了与 `push_back` 相同的强异常保证。如果原位构造过程中抛出异常，vector 的状态不会改变。

##### 2. 显式构造函数
`emplace_back` 可能会绕过 `explicit` 构造函数，这可能是期望的行为，也可能不是，需要小心：
```cpp
class ExplicitClass {
public:
    explicit ExplicitClass(int x) {}
};

std::vector<ExplicitClass> vec;

// vec.push_back(42); // 错误！不能隐式转换
vec.push_back(ExplicitClass(42)); // 正确，但需要显式构造

vec.emplace_back(42); // 正确！直接调用构造函数，绕过explicit检查
```

##### 3. 性能并非总是更好
对于简单的、可移动的类型，或者在编译器能够进行返回值优化（RVO/NRVO）的情况下，两者性能差异可能微乎其微。

## 交换操作

### `swap()` - 交换两个vector的内容
```cpp
vector<int> v1 = {1, 2, 3};
vector<int> v2 = {4, 5, 6};
v1.swap(v2); // 交换v1和v2的内容

// 也可以使用全局swap函数
swap(v1, v2);
```

## 比较操作

`vector` 支持所有比较运算符：`==`, `!=`, `<`, `<=`, `>`, `>=`
```cpp
vector<int> a = {1, 2, 3};
vector<int> b = {1, 2, 3};
if (a == b) {
    cout << "Vectors are equal";
}
```

## 示例

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    // 创建并初始化vector
    vector<int> nums = {1, 2, 3, 4, 5};
    
    // 访问元素
    cout << "First element: " << nums.front() << endl;
    cout << "Last element: " << nums.back() << endl;
    cout << "Element at index 2: " << nums[2] << endl;
    
    // 修改元素
    nums[1] = 20;
    cout << "After modification: ";
    for (int num : nums) {
        cout << num << " ";
    }
    cout << endl;
    
    // 添加元素
    nums.push_back(6);
    nums.insert(nums.begin() + 3, 99);
    cout << "After adding elements: ";
    for (int num : nums) {
        cout << num << " ";
    }
    cout << endl;
    
    // 删除元素
    nums.pop_back();
    nums.erase(nums.begin() + 3);
    cout << "After removing elements: ";
    for (int num : nums) {
        cout << num << " ";
    }
    cout << endl;
    
    // 容量信息
    cout << "Size: " << nums.size() << endl;
    cout << "Capacity: " << nums.capacity() << endl;
    
    // 预留空间
    nums.reserve(20);
    cout << "Capacity after reserve: " << nums.capacity() << endl;
    
    return 0;
}
```

# list

`list` 是 C++ 标准模板库（STL）提供的一个**环状双向链表**容器。与 `vector` 的连续内存布局完全不同，`list` 由一系列在内存中非连续存储的节点组成，每个节点包含数据和指向前后节点的指针。

1.  **双向链表结构**：每个节点包含指向前驱和后继节点的指针，支持双向遍历。
2.  **非连续内存**：元素存储在随机的内存位置，通过指针链接。
3.  **高效的插入删除**：在任何位置插入和删除元素的时间复杂度都是 **O(1)**。
4.  **迭代器稳定性**：插入和删除操作**不会使其他元素的迭代器、指针、引用失效**（除非指向被删除的元素本身）。

## 与 `vector` 的对比

| 特性             | `std::list`                          | `std::vector`                |
| :--------------- | :----------------------------------- | :--------------------------- |
| **内存布局**     | 非连续（链表）                       | 连续（动态数组）             |
| **随机访问**     | **不支持**，时间复杂度 O(N)          | **支持**，时间复杂度 O(1)    |
| **头部插入删除** | **高效**，O(1)                       | **低效**，O(N)               |
| **中部插入删除** | **高效**，O(1)                       | **低效**，O(N)               |
| **迭代器类型**   | 双向迭代器                           | 随机访问迭代器               |
| **迭代器失效**   | 只影响被操作的元素                   | 插入删除可能使所有迭代器失效 |
| **内存开销**     | 较大（每个元素需要额外存储两个指针） | 较小（只有少量额外开销）     |
| **缓存友好性**   | 差（内存不连续）                     | 好（内存连续）               |

## 构造函数

```cpp
#include <list>
#include <iostream>

std::list<int> list1;                // 空链表
std::list<int> list2(5, 100);        // 包含5个值为100的元素
std::list<int> list3 = {1, 2, 3, 4}; // 初始化列表 (C++11)
std::list<int> list4(list3);         // 拷贝构造
std::list<int> list5(list4.begin(), list4.end()); // 范围构造
```

## 元素访问

**注意：`list` 不支持随机访问（如 `operator[]` 和 `at()`）**

```cpp
std::list<int> mylist = {10, 20, 30, 40};

// 访问首尾元素
std::cout << "Front: " << mylist.front() << std::endl; // 10
std::cout << "Back: " << mylist.back() << std::endl;   // 40

// 错误示例：不能使用下标访问
// std::cout << mylist[2]; // 编译错误！
```

## 迭代器

```cpp
// 正向迭代
for (auto it = mylist.begin(); it != mylist.end(); ++it) {
    std::cout << *it << " ";
}

// 反向迭代
for (auto rit = mylist.rbegin(); rit != mylist.rend(); ++rit) {
    std::cout << *rit << " ";
}

// 范围for循环 (C++11)
for (const auto& elem : mylist) {
    std::cout << elem << " ";
}
```

## 容量操作

```cpp
std::list<int> list;

std::cout << "Size: " << list.size() << std::endl;
std::cout << "Empty: " << list.empty() << std::endl;

// list 没有 capacity() 和 reserve() 的概念！
// list.capacity(); // 错误：没有这个成员函数
```

## 修改操作（核心优势）

```cpp
std::list<int> list = {1, 2, 3};

// 头部操作
list.push_front(0);    // {0, 1, 2, 3}
list.pop_front();      // {1, 2, 3}

// 尾部操作
list.push_back(4);     // {1, 2, 3, 4}
list.pop_back();       // {1, 2, 3}

// 任意位置插入 (高效！)
auto it = list.begin();
std::advance(it, 1);   // 将迭代器移动到第二个位置
list.insert(it, 99);   // {1, 99, 2, 3}

// 任意位置删除 (高效！)
it = list.begin();
std::advance(it, 2);   // 移动到第三个位置
list.erase(it);        // {1, 99, 3}

// 清空链表
list.clear();
```

## 特殊操作（`list` 的独特优势）

这些操作是 `list` 特有的，并且非常高效（通常为 O(1) 或 O(N)）：

```cpp
std::list<int> list1 = {1, 3, 5};
std::list<int> list2 = {2, 4, 6};

// 1. splice - 移动元素从一个list到另一个 (O(1) 或 O(N))
auto it = list1.begin();
std::advance(it, 1); // 指向3
list1.splice(it, list2); // list1: {1, 2, 4, 6, 3, 5}, list2: 空

// 2. merge - 合并两个已排序的链表 (O(N))
list1 = {1, 3, 5};
list2 = {2, 4, 6};
list1.merge(list2); // list1: {1, 2, 3, 4, 5, 6}, list2: 空

// 3. sort - 链表排序 (O(N log N))
std::list<int> list3 = {3, 1, 4, 1, 5};
list3.sort(); // {1, 1, 3, 4, 5}

// 4. unique - 去除连续重复元素 (O(N)) 只移除相邻重复元素
list3.unique(); // {1, 3, 4, 5} (需要先排序才能去除所有重复)

// 5. remove - 删除所有匹配值的元素 (O(N))
list3.remove(3); // {1, 4, 5}

// 6. remove_if - 条件删除 (O(N))
list3.remove_if([](int n) { return n % 2 == 0; }); // 删除所有偶数: {1, 5}

// 7. reverse - 反转链表 (O(N))
list3.reverse(); // {5, 1}
```

## 示例

```cpp
#include <iostream>
#include <list>
#include <algorithm> // 用于 std::find

int main() {
    std::list<std::string> names = {"Alice", "Bob", "Charlie"};
    
    // 在中间插入
    auto it = names.begin();
    std::advance(it, 2);
    names.insert(it, "David"); // {"Alice", "Bob", "David", "Charlie"}
    
    // 删除特定元素
    names.remove("Bob"); // {"Alice", "David", "Charlie"}
    
    // 查找并删除（需要算法库）
    it = std::find(names.begin(), names.end(), "David");
    if (it != names.end()) {
        names.erase(it); // {"Alice", "Charlie"}
    }
    
    // 排序
    names.sort(); // {"Alice", "Charlie"}
    
    // 输出结果
    std::cout << "Final list: ";
    for (const auto& name : names) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
```

## 总结

优点：
1.  **任意位置高效插入删除**（O(1)）
2.  **迭代器稳定性**（插入删除不会使其他迭代器失效）
3.  **不需要预分配内存**
4.  **内置高效的特殊操作**（`splice`, `merge`, `sort`等）

缺点：
1.  **不支持随机访问**（访问元素需要遍历，O(N)）
2.  **内存开销大**（每个元素需要两个指针的开销）
3.  **缓存不友好**（内存不连续，访问速度可能较慢）
4.  **比 `vector` 占用更多内存**

使用 `std::list` 当需求是：
-   需要频繁在**任意位置**插入和删除元素
-   插入删除操作后需要**保持其他迭代器有效**
-   需要大量使用 `splice`、`merge` 等链表特有操作
-   不关心随机访问性能

不使用 `std::list` 当你的需求是：
-   需要频繁**随机访问**元素
-   关注**内存使用效率**和**缓存性能**
-   元素主要是**在尾部添加**（此时 `vector` 更合适）

# deque

`deque`（双端队列，全称 double-ended queue）是 C++ 标准模板库（STL）中的一个容器适配器，它结合了 `vector` 和 `list` 的某些优点，提供了在两端高效插入和删除操作的能力。

1.  **双端操作**：在队列的**前端和后端**进行插入和删除操作都非常高效（时间复杂度 O(1)）。
2.  **分段连续存储**：`deque` 由多个固定大小的数组块组成，这些块通过一个中央映射结构（通常是一个指针数组）管理。
3.  **伪随机访问**：支持通过索引访问元素，但性能略低于 `vector`（仍然是 O(1) 时间复杂度，但常数因子更大）。
4.  **动态扩容**：像 `vector` 一样可以自动增长，但在两端都能高效扩容。

vector 是单向开口的连续线性空间，deque 是双向开口的连续线性空间。
deque 允许头部插入或移除元素。效率比 vector 高。
deque 没用 capacity 概念，它动态的以分段连续空间组合而成，随时可以增加一段新的空间并链接起来。

`deque` 通常由一个**中央映射表**（指针数组）和多个**固定大小的存储块**组成：
- 映射表指向各个存储块
- 每个存储块可以容纳固定数量的元素
- 当需要在任一端添加元素时，只需分配新的存储块并更新映射表

这种结构使得在两端添加元素时不需要移动大量现有元素，这是它与 `vector` 的关键区别。

## 与 `vector` 和 `list` 的对比

| 特性             | `std::deque` | `std::vector` | `std::list` |
| :--------------- | :----------- | :------------ | :---------- |
| **内存布局**     | 分段连续     | 完全连续      | 完全不连续  |
| **头部插入删除** | **O(1)**     | O(N)          | **O(1)**    |
| **尾部插入删除** | **O(1)**     | **分摊 O(1)** | **O(1)**    |
| **中部插入删除** | O(N)         | O(N)          | **O(1)**    |
| **随机访问**     | **O(1)**     | **O(1)**      | O(N)        |
| **迭代器类型**   | 随机访问     | 随机访问      | 双向        |
| **内存开销**     | 中等         | 小            | 大          |
| **缓存友好性**   | 中等         | 好            | 差          |

## 构造函数

```cpp
#include <deque>

std::deque<int> dq1;                 // 空 deque
std::deque<int> dq2(5, 100);         // 包含5个值为100的元素
std::deque<int> dq3 = {1, 2, 3, 4};  // 初始化列表 (C++11)
std::deque<int> dq4(dq3);            // 拷贝构造
std::deque<int> dq5(dq4.begin(), dq4.end()); // 范围构造
```

## 元素访问

`deque` 支持与 `vector` 相同的随机访问方式：

```cpp
std::deque<int> dq = {10, 20, 30, 40, 50};

// 下标访问（不检查边界）
std::cout << dq[2] << std::endl;    // 30

// 带边界检查的访问
std::cout << dq.at(3) << std::endl; // 40

// 访问首尾元素
std::cout << dq.front() << std::endl; // 10
std::cout << dq.back() << std::endl;  // 50
```

## 迭代器

```cpp
// 正向迭代
for (auto it = dq.begin(); it != dq.end(); ++it) {
    std::cout << *it << " ";
}

// 反向迭代
for (auto rit = dq.rbegin(); rit != dq.rend(); ++rit) {
    std::cout << *rit << " ";
}

// 范围for循环
for (const auto& elem : dq) {
    std::cout << elem << " ";
}
```

## 容量操作

```cpp
std::deque<int> dq;

std::cout << "Size: " << dq.size() << std::endl;
std::cout << "Empty: " << dq.empty() << std::endl;

// deque 没有 capacity() 概念，但可以预分配内存
// dq.capacity(); // 错误：没有这个成员函数

// 改变大小
dq.resize(10);     // 将大小改为10，新增元素默认初始化为0
dq.resize(15, 5);  // 将大小改为15，新增元素初始化为5
```

## 修改操作

```cpp
std::deque<int> dq = {2, 3, 4};

// 前端操作（vector 做不到高效！）
dq.push_front(1);    // {1, 2, 3, 4} - O(1)
dq.pop_front();      // {2, 3, 4}    - O(1)

// 后端操作
dq.push_back(5);     // {2, 3, 4, 5} - O(1)
dq.pop_back();       // {2, 3, 4}    - O(1)

// 任意位置插入（不如 list 高效）
auto it = dq.begin() + 1;
dq.insert(it, 99);   // {2, 99, 3, 4} - O(N)

// 任意位置删除（不如 list 高效）
it = dq.begin() + 2;
dq.erase(it);        // {2, 99, 4} - O(N)

// 清空
dq.clear();
```

## 特殊操作

```cpp
std::deque<int> dq1 = {1, 2, 3};
std::deque<int> dq2 = {4, 5, 6};

// 交换内容
dq1.swap(dq2); // dq1: {4, 5, 6}, dq2: {1, 2, 3}

// 原位构造 (C++11)
dq1.emplace_front(0);    // 前端原位构造: {0, 4, 5, 6}
dq1.emplace_back(7);     // 后端原位构造: {0, 4, 5, 6, 7}
dq1.emplace(dq1.begin() + 2, 99); // 中间原位构造: {0, 4, 99, 5, 6, 7}
```

## 迭代器失效规则

`deque` 的迭代器失效规则比较复杂：
- **前端插入**：可能使所有迭代器失效，但引用和指针通常保持有效
- **后端插入**：通常使所有迭代器失效，但引用和指针保持有效
- **中间插入**：使所有迭代器失效
- **删除操作**：被删除元素之后的迭代器失效

## 优点：
1.  **两端高效操作**：前端和后端的插入删除都是 O(1)
2.  **随机访问支持**：像数组一样通过索引访问元素
3.  **内存效率较高**：比 `list` 的内存开销小
4.  **不需要连续内存**：可以分配多个内存块，避免大内存分配问题

## 缺点：
1.  **中间操作效率低**：在中间位置插入删除需要 O(N) 时间
2.  **随机访问性能**：比 `vector` 稍慢（常数因子更大）
3.  **迭代器失效规则复杂**：比 `vector` 和 `list` 更复杂
4.  **内存局部性**：不如 `vector` 那样缓存友好

## 适用场景

- 需要**频繁在两端插入删除**元素（如实现队列、双端队列、滑动窗口）
- 同时需要**随机访问**能力
- 元素数量可能很大，担心 `vector` 重新分配的成本

不使用 `std::deque` 当你的需求是：
- 需要**最高性能的随机访问**（选择 `vector`）
- 需要**频繁在中间位置插入删除**（选择 `list`）
- 需要**绝对连续的内存**（如与C API交互，选择 `vector`）

## 典型应用场景

1.  **消息队列系统**：新消息从前端加入，旧消息从后端处理
2.  **撤销操作栈**：支持在两端操作历史记录
3.  **滑动窗口算法**：需要在两端高效添加和移除元素
4.  ** steal-work 算法**：多线程任务调度

`deque` 是一个在特定场景下非常有用的容器，它填补了 `vector` 和 `list` 之间的空白，提供了两者优点的折中方案。