STL 容器

# stack
stack 是先进后出的数据结构，只有一个出口。

**栈（Stack）** 是一种非常基础且重要的**数据结构**，它遵循 **LIFO（Last-In, First-Out）** 的原则，即**后进先出**。

在 C++ STL 中，`stack` 不是一个独立的容器，而是一个**容器适配器（Container Adapter）**。这意味着它是在其他序列容器（如 `deque`, `list`）之上包装而成的，提供了栈的特定接口，隐藏了底层容器的复杂功能。

**特点：**
*   所有元素的插入（压栈）和删除（弹栈）只能在容器的一端（称为栈顶）进行。
*   不支持随机访问，即你不能用 `[]` 运算符或迭代器来访问栈中间的元素。你只能访问栈顶的元素。
*   底层容器需要支持 `back()`, `push_back()`, `pop_back()` 等操作。默认情况下，`stack` 使用 `deque` 作为其底层容器，但也可以指定使用 `vector` 或 `list`。

**头文件：**
```cpp
#include <stack>
```

## 构造函数

*   `stack<T> stk;`：默认构造函数，创建一个空的栈，底层使用 `deque`。
*   `stack<T, Container> stk;`：可以显式指定底层容器类型（如 `vector<T>` 或 `list<T>`）。
*   `stack(const stack &stk);`：拷贝构造函数。

**举例：**
```cpp
#include <iostream>
#include <stack>
#include <vector>
#include <list>

int main() {
    // 1. 默认构造（使用deque）
    std::stack<int> s1;

    // 2. 指定底层容器为 vector
    std::stack<int, std::vector<int>> s2;

    // 3. 指定底层容器为 list
    std::stack<int, std::list<int>> s3;

    // 4. 拷贝构造
    std::stack<int> s4(s1); // 此时 s4 和 s1 都是空的
    return 0;
}
```

## 赋值与交换 (``=``, `swap`)

*   `stack& operator=(const stack &stk);`：重载等号操作符，进行赋值。
*   `swap(stk);`：将当前栈中的元素与另一个栈的元素互换。

```cpp
std::stack<int> s1, s2;
// ... 给 s1 添加一些元素
s2 = s1;       // s2 现在和 s1 一模一样
s1.swap(s2);   // 交换 s1 和 s2 的内容
```

## 数据存取操作

*   `push(elem);`：向栈顶**添加**一个元素（压栈）。
*   `pop();`：从栈顶**移除**一个元素（弹栈）。**注意：** 此函数**不返回**被移除的元素的值。
*   `top();`：**返回**栈顶元素的**引用**。这是你查看栈顶元素值的方式。

```cpp
#include <iostream>
#include <stack>

int main() {
    std::stack<int> s;

    // 压栈 (Push)
    s.push(10);
    s.push(20);
    s.push(30);
    // 现在栈从底到顶是：10, 20, 30

    // 访问栈顶 (Top)
    std::cout << "Top element is: " << s.top() << std::endl; // 输出 30

    // 弹栈 (Pop) - 只删除，不返回
    s.pop(); // 移除 30
    std::cout << "Top element after pop is: " << s.top() << std::endl; // 输出 20

    // 修改栈顶元素
    s.top() = 100; // 将 20 改为 100
    std::cout << "Top element after modification is: " << s.top() << std::endl; // 输出 100

    return 0;
}
```

**非常重要：** `top()` 和 `pop()` 必须分开操作。如果试图在一个空栈上调用 `top()` 或 `pop()`，会导致**未定义行为**（通常程序会崩溃）。因此，在调用它们之前，必须先检查栈是否为空。

## 容量操作

*   `empty();`：判断栈是否为空。返回 `true` 为空，`false` 为非空。
*   `size();`：返回栈中元素的个数。

```cpp
std::stack<int> s;
if (s.empty()) {
    std::cout << "Stack is empty." << std::endl;
}

s.push(1);
s.push(2);
s.push(3);

std::cout << "Stack size is: " << s.size() << std::endl; // 输出 3

// 正确的安全弹栈方式：先检查，再操作
if (!s.empty()) {
    int top_value = s.top(); // 先获取值
    s.pop();                 // 再移除
    std::cout << "Popped value: " << top_value << std::endl;
}
```

# Queue

**队列（Queue）** 是另一种基础且重要的数据结构，它遵循 **FIFO（First-In, First-Out）** 的原则，即**先进先出**。可以把它想象成现实生活中的排队，先来的人先接受服务，新来的人只能排在队尾。

和 `stack` 一样，C++ STL 中的 `queue` 也是一个**容器适配器（Container Adapter）**。它是在其他序列容器（如 `deque`, `list`）之上包装而成的，提供了队列的特定接口。

**特点：**
*   元素的插入（入队）只能在容器的尾部进行。
*   元素的删除（出队）只能在容器的头部进行。
*   不支持随机访问，即你不能用 `[]` 运算符或迭代器来访问队列中间的元素。你只能访问队首和队尾的元素。
*   底层容器需要支持 `back()`, `push_back()`, `pop_front()`, `front()` 等操作。默认情况下，`queue` 使用 `deque` 作为其底层容器，但你也可以指定使用 `list`（`vector` 不能直接使用，因为它没有高效的 `pop_front()` 操作）。

**头文件：**
```cpp
#include <queue>
```

## 构造函数

*   `queue<T> que;`：默认构造函数，创建一个空的队列，底层使用 `deque`。
*   `queue<T, Container> que;`：可以显式指定底层容器类型（通常为 `list<T>`，因为 `vector` 不支持 `pop_front()`）。
*   `queue(const queue &que);`：拷贝构造函数。

**举例：**
```cpp
#include <iostream>
#include <queue>
#include <list>

int main() {
    // 1. 默认构造（使用deque）
    std::queue<int> q1;

    // 2. 指定底层容器为 list
    std::queue<int, std::list<int>> q2;

    // 3. 拷贝构造
    std::queue<int> q3(q1); // 此时 q3 和 q1 都是空的
    return 0;
}
```

## 赋值与交换 (`=`, `swap`)

*   `queue& operator=(const queue &que);`：重载等号操作符，进行赋值。
*   `swap(que);`：将当前队列中的元素与另一个队列的元素互换。

**举例：**
```cpp
std::queue<int> q1, q2;
// ... 给 q1 添加一些元素
q2 = q1;       // q2 现在和 q1 一模一样
q1.swap(q2);   // 交换 q1 和 q2 的内容
```

## 数据存取操作

*   `push(elem);`：在队列**尾部**添加一个元素（入队）。
*   `pop();`：从队列**头部**移除一个元素（出队）。**注意：** 此函数**不返回**被移除的元素的值。
*   `front();`：**返回**队列**头部**元素的**引用**。这是你获取下一个将要被处理元素的方式。
*   `back();`：**返回**队列**尾部**元素的**引用**。这是你查看最新加入元素的方式。

**这是队列最重要的四个操作，`front()` 和 `pop()` 通常需要配合使用。**

**举例：**
```cpp
#include <iostream>
#include <queue>

int main() {
    std::queue<int> q;

    // 入队 (Push) - 在队尾添加
    q.push(10); // 队首 <- [10] <- 队尾
    q.push(20); // 队首 <- [10, 20] <- 队尾
    q.push(30); // 队首 <- [10, 20, 30] <- 队尾

    // 访问队首 (Front) 和 队尾 (Back)
    std::cout << "Front element is: " << q.front() << std::endl; // 输出 10
    std::cout << "Back element is: " << q.back() << std::endl;  // 输出 30

    // 出队 (Pop) - 从队首移除，不返回
    q.pop(); // 移除 10
    std::cout << "Front element after pop is: " << q.front() << std::endl; // 输出 20

    // 修改队首元素
    q.front() = 100;
    std::cout << "Modified front element is: " << q.front() << std::endl; // 输出 100

    // 队尾元素不变
    std::cout << "Back element is still: " << q.back() << std::endl; // 输出 30

    return 0;
}
```

**非常重要：** `front()`, `back()`, `pop()` 必须在对**非空队列**调用。如果你试图在一个空队列上调用这些函数，会导致**未定义行为**（通常程序会崩溃）。因此，在调用它们之前，必须先检查队列是否为空。

## 容量操作

*   `empty();`：判断队列是否为空。返回 `true` 为空，`false` 为非空。
*   `size();`：返回队列中元素的个数。

**举例：**
```cpp
std::queue<std::string> q;
if (q.empty()) {
    std::cout << "Queue is empty." << std::endl;
}

q.push("Alice");
q.push("Bob");
q.push("Charlie");

std::cout << "Queue size is: " << q.size() << std::endl; // 输出 3

// 正确的安全出队方式：先检查，再操作
while (!q.empty()) {           // 经典的遍历并清空队列的方法
    std::cout << "Processing: " << q.front() << std::endl; // 先获取队首值
    q.pop();                   // 再移除
}
// 循环结束后，队列变为空
```

# heap

在 C++ STL 中，**堆（Heap）不是一种独立的容器**，而是一种**组织容器元素的特殊方式**，通常表现为一棵**完整的二叉树**。STL 提供了一系列的算法（`<algorithm>` 头文件中）来在现有容器（通常是 `vector` 或 `deque`）上管理和操作堆。

**堆的核心特性：**
堆是一种满足特定性质的完全二叉树，主要有两种类型：
1.  **最大堆（Max-Heap）**：任意节点的值都**大于或等于**其子节点的值。根节点是最大的元素。
2.  **最小堆（Min-Heap）**：任意节点的值都**小于或等于**其子节点的值。根节点是最小的元素。

STL 提供的堆算法默认创建和操作的是**最大堆**。

**底层容器：**
由于堆需要能够高效地随机访问元素（例如，通过索引快速找到一个节点的父节点或子节点），它通常基于**支持随机访问的容器**，最常见的是 `vector` 或 `deque`。

# map
`std::map` 是 C++ 标准模板库（STL）中一个非常重要的**关联容器**。它存储的元素是 **键值对（key-value pairs）**，并且根据 **键（key）** 自动进行排序。

**核心特性：**
1.  **键值对存储**：每个元素都是一个 `std::pair<const Key, T>`，包含一个唯一的 `key` 和一个对应的 `value`。
2.  **自动排序**：容器内的元素始终按照键（key）的升序进行排序（默认使用 `operator<`）。这是通过内部维护一颗**红黑树（Red-Black Tree）**（一种自平衡的二叉搜索树）来实现的。
3.  **键的唯一性**：`map` 中每个键**只能出现一次**。如果插入一个已经存在的键，新的值不会覆盖旧的值（除非你使用特定的插入方式或 `operator[]`）。
4.  **高效的查找、插入和删除**：由于其底层是平衡二叉搜索树，这些操作的时间复杂度都是 **O(log n)**。
5.  **不可修改的键**：一旦一个键被插入到 `map` 中，就不能被修改，因为它会破坏树的内部顺序。你可以修改键对应的值（value）。

**头文件：**
```cpp
#include <map>
```

**适用场景：**
*   需要根据一个键来快速查找对应的值（例如，字典、电话簿）。
*   需要数据始终保持有序（根据键排序）。
*   键是唯一的。

## 构造函数

*   `map<Key, T> m;`：创建一个空的 map。
*   `map<Key, T> m = {{key1, value1}, {key2, value2}, ...};`：使用初始化列表创建。
*   `map<Key, T> m(m2);`：拷贝构造函数。

**举例：**
```cpp
#include <iostream>
#include <map>
#include <string>

int main() {
    // 1. 默认构造
    std::map<int, std::string> studentMap;

    // 2. 初始化列表构造 (C++11)
    std::map<int, std::string> ageMap = {
        {25, "Alice"},
        {30, "Bob"},
        {22, "Charlie"}
    }; // 会自动按年龄(键)排序：22, 25, 30

    // 3. 拷贝构造
    std::map<int, std::string> copyMap(ageMap);
    return 0;
}
```

## 赋值与大小操作

*   `operator=`, `swap()`：与其他 map 交换内容。
*   `empty()`：检查 map 是否为空。
*   `size()`：返回 map 中键值对的数量。

```cpp
std::map<int, std::string> m1, m2;
m1 = {{1, "a"}, {2, "b"}};
m2 = m1; // 赋值
m1.swap(m2); // 交换
std::cout << "Size: " << m1.size() << ", Empty? " << m1.empty() << std::endl;
```

## 元素访问

### `operator[](key)` 和 `at(key)`
*   `operator[]`：访问或插入指定的键对应的值。
    *   如果 `key` 存在，返回其对应值的**引用**。
    *   如果 `key` **不存在**，则会**自动插入**该键，并使用值类型的默认构造函数初始化其值，然后返回这个新值的引用。
    *   **非常方便，但要小心意外插入**。
*   `at(key)`：访问指定的键对应的值。
    *   如果 `key` 存在，返回其对应值的**引用**。
    *   如果 `key` **不存在**，会抛出 `std::out_of_range` 异常。
    *   **更安全，可以防止意外插入**。

```cpp
std::map<std::string, int> wordCount;

// 使用 operator[] 插入新元素
wordCount["apple"] = 5; // 插入键 "apple"，值设为 5
wordCount["banana"] = 3; // 插入键 "banana"，值设为 3

// 使用 operator[] 访问和修改现有元素
wordCount["apple"] = 10; // 修改 "apple" 的值为 10
std::cout << wordCount["apple"]; // 输出 10

// 小心！以下操作会意外插入新键 "orange"，其值被默认初始化为 0
std::cout << wordCount["orange"]; // 输出 0，但现在 map 里多了 {"orange", 0}

// 使用 at() 安全访问
try {
    std::cout << wordCount.at("apple"); // 输出 10
    std::cout << wordCount.at("grape"); // 抛出 std::out_of_range 异常
} catch (const std::out_of_range& e) {
    std::cout << "Key not found!" << std::endl;
}
```

### `find(key)` 和 `count(key)`（安全查找）
*   `find(key)`：
    *   查找键为 `key` 的元素。
    *   如果找到，返回指向该元素的**迭代器**。
    *   如果没找到，返回 `end()` 迭代器。
    *   **这是检查一个键是否存在最标准、最安全的方法。**
*   `count(key)`：
    *   返回键 `key` 在 map 中出现的次数。对于 `map`，返回值只能是 **0** 或 **1**（因为键唯一）。
    *   可以用来快速检查键是否存在。

**举例：**
```cpp
std::map<int, std::string> m = {{1, "one"}, {2, "two"}};

// 使用 find() 安全地查找元素
auto it = m.find(2);
if (it != m.end()) { // 一定要检查是否找到！
    std::cout << "Found: " << it->second << std::endl; // 输出 "Found: two"
} else {
    std::cout << "Key 2 not found." << std::endl;
}

it = m.find(3);
if (it == m.end()) {
    std::cout << "Key 3 not found." << std::endl; // 会执行这里
}

// 使用 count() 检查存在性
if (m.count(1) > 0) {
    std::cout << "Key 1 exists." << std::endl;
}
```

## 元素修改

### `insert()` 插入元素
有多种插入方式，最常用的是使用 `std::pair` 或 `make_pair`。

```cpp
std::map<int, std::string> m;

// 方法 1: 使用 make_pair
m.insert(std::make_pair(10, "ten"));

// 方法 2: 使用花括号 (C++11)
m.insert({20, "twenty"});

// 方法 3: 使用 pair 的构造方式
m.insert(std::pair<const int, std::string>(30, "thirty"));

// insert 会返回一个 pair<iterator, bool>
// iterator 指向插入的元素（或已存在的阻止插入的元素）
// bool 表示插入是否成功（key是否已存在）
auto result = m.insert({10, "TEN"}); // 尝试再次插入 key=10
if (!result.second) { // result.second 是 false
    std::cout << "Insertion failed, key 10 already exists.\n";
}
```

## `erase()` 删除元素
可以通过 key、迭代器或迭代器范围来删除。

```cpp
std::map<int, std::string> m = {{1, "a"}, {2, "b"}, {3, "c"}, {4, "d"}};

// 通过 key 删除
m.erase(2); // 删除 key=2 的元素

// 通过迭代器删除
auto it = m.find(3);
if (it != m.end()) {
    m.erase(it); // 删除找到的元素
}

// 通过迭代器范围删除
m.erase(m.begin(), m.find(4)); // 删除从开始到key=4之前的元素
```

## 迭代器（遍历）

可以使用迭代器遍历 `map`。迭代器指向的元素是 `pair<const Key, T>` 类型。

**举例：**
```cpp
std::map<std::string, int> m = {{"apple", 5}, {"banana", 3}, {"cherry", 8}};

// 1. 使用迭代器 (顺序遍历，按键升序)
std::cout << "Traditional iterator:\n";
for (auto it = m.begin(); it != m.end(); ++it) {
    // it->first 是 key, it->second 是 value
    std::cout << it->first << ": " << it->second << std::endl;
}
// 输出:
// apple: 5
// banana: 3
// cherry: 8

// 2. 基于范围的for循环 (C++11, 更简洁)
std::cout << "\nRange-based for loop:\n";
for (const auto& kv : m) { // kv 是 pair<const string, int>
    std::cout << kv.first << ": " << kv.second << std::endl;
}

// 3. 反向迭代器 (逆序遍历，按键降序)
std::cout << "\nReverse iterator:\n";
for (auto rit = m.rbegin(); rit != m.rend(); ++rit) {
    std::cout << rit->first << ": " << rit->second << std::endl;
}
// 输出:
// cherry: 8
// banana: 3
// apple: 5
```

## 查找边界

*   `lower_bound(key)`：返回指向**第一个键不小于 `key`** 的元素的迭代器。
*   `upper_bound(key)`：返回指向**第一个键大于 `key`** 的元素的迭代器。
*   `equal_range(key)`：返回一个 `pair`，包含 `lower_bound` 和 `upper_bound` 的结果。用于查找所有匹配的键（在 `multimap` 中更有用）。

```cpp
std::map<int, char> m = {{1, 'a'}, {3, 'b'}, {5, 'c'}};

auto low = m.lower_bound(2); // 指向 key=3 的元素 (3 >= 2)
auto up = m.upper_bound(4);  // 指向 key=5 的元素 (5 > 4)

// 可以用于遍历一个范围
for (auto it = low; it != up; ++it) {
    std::cout << it->first << "->" << it->second << " "; // 输出: 3->b
}
```

## key 为自定义结构体

`std::map` 要求键必须是可比较的，默认使用 `std::less<Key>`（即 `<` 运算符）。需要为你的自定义结构体提供比较方式。

### 方法一：重载 `<` 运算符（推荐）

```cpp
#include <iostream>
#include <map>
#include <string>

struct Person {
    std::string name;
    int age;
    
    // 重载 < 运算符
    bool operator<(const Person& other) const {
        // 先按姓名比较，如果姓名相同再按年龄比较
        if (name == other.name) {
            return age < other.age;
        }
        return name < other.name;
    }
};

int main() {
    std::map<Person, std::string> personMap;
    
    personMap[{"Alice", 25}] = "Engineer";
    personMap[{"Bob", 30}] = "Designer";
    personMap[{"Alice", 30}] = "Manager"; // 与上面的Alice不同
    
    for (const auto& pair : personMap) {
        std::cout << pair.first.name << " (" << pair.first.age << "): " 
                  << pair.second << std::endl;
    }
    
    return 0;
}
```

### 方法二：提供自定义比较函数

```cpp
#include <iostream>
#include <map>
#include <string>

struct Person {
    std::string name;
    int age;
};

// 自定义比较函数子
struct PersonCompare {
    bool operator()(const Person& a, const Person& b) const {
        if (a.name == b.name) {
            return a.age < b.age;
        }
        return a.name < b.name;
    }
};

int main() {
    // 在模板参数中指定比较器
    std::map<Person, std::string, PersonCompare> personMap;
    
    personMap[{"Alice", 25}] = "Engineer";
    personMap[{"Bob", 30}] = "Designer";
    
    for (const auto& pair : personMap) {
        std::cout << pair.first.name << " (" << pair.first.age << "): " 
                  << pair.second << std::endl;
    }
    
    return 0;
}
```

## 总结与相关容器

| 接口                     | 功能描述       | 注意事项                           |
| :----------------------- | :------------- | :--------------------------------- |
| `m[key]`                 | 访问或插入元素 | **小心意外插入**                   |
| `m.at(key)`              | 安全访问元素   | 键不存在会抛出异常                 |
| `m.find(key)`            | 查找元素       | 返回迭代器，需检查是否为 `m.end()` |
| `m.insert({key, value})` | 插入元素       | 返回一个 `pair<iterator, bool>`    |
| `m.erase(key)`           | 删除元素       | 可按 key、迭代器或范围删除         |
| `m.begin()`, `m.end()`   | 获取迭代器     | 用于遍历                           |
| `m.size()`, `m.empty()`  | 容量查询       |                                    |

**相关容器：**
*   `std::multimap`：允许键重复的 `map`。
*   `std::unordered_map`：使用哈希表实现的 `map`，元素不排序，但平均查找性能更快（O(1)）。

**选择指南：**
*   需要**有序存储**和**按顺序遍历** → 用 `map`
*   需要**最快的查找速度**且不关心顺序 → 用 `unordered_map`
*   允许**重复的键** → 用 `multimap` 或 `multimap`

# unordered_map

`std::unordered_map` 是 C++ 标准模板库（STL）中的一个关联容器，它存储的是键值对（key-value pairs）。

它的核心特性基于 **哈希表（Hash Table）** 实现，因此具有非常高效的查找、插入和删除操作的平均时间复杂度（平均情况下为 O(1)）。

与 `std::map` 的有序性不同，`std::unordered_map` 中的元素是 **不按任何特定顺序排序** 的。元素的顺序取决于哈希函数和内部结构，对于使用者来说是未定义的。如果你需要元素按键排序，应该使用 `std::map`。

**关键特性：**
*   **键的唯一性**：容器内的每个键都是唯一的。尝试插入相同键的元素会被忽略（除非使用特殊接口）。
*   **高效的访问**：通过键直接访问元素的速度非常快。
*   **无序性**：元素在内部的位置由键的哈希值决定，遍历时不会得到任何有意义的顺序。

```cpp
#include <unordered_map>
```

## 构造函数和初始化

```cpp
// 1. 默认构造函数：创建一个空的 unordered_map
std::unordered_map<std::string, int> ageMap;

// 2. 初始化列表初始化（C++11）
std::unordered_map<std::string, int> ageMap = {
    {"Alice", 25},
    {"Bob", 30},
    {"Charlie", 28}
};

// 3. 拷贝构造函数
std::unordered_map<std::string, int> anotherMap(ageMap);
```

## 元素访问

### `operator[]`
这是最常用的访问方式。
*   **如果键存在**：返回对应值的引用。
*   **如果键不存在**：会自动插入这个键，并对其进行值初始化（对于 `int` 是 0，对于类类型是调用默认构造函数），然后返回这个新值的引用。

```cpp
ageMap["Alice"] = 26; // 修改已存在的键的值
std::cout << ageMap["Alice"]; // 输出：26

int bobAge = ageMap["Bob"]; // 获取"Bob"的值
int daveAge = ageMap["Dave"]; // "Dave"不存在，会自动插入{"Dave", 0}，并返回0
```

### `at()`
*   **如果键存在**：返回对应值的引用。
*   **如果键不存在**：抛出 `std::out_of_range` 异常。**这比 `operator[]` 更安全**，可以避免意外插入新元素。

```cpp
try {
    int charlieAge = ageMap.at("Charlie"); // 获取成功
    int eveAge = ageMap.at("Eve"); // 抛出 std::out_of_range 异常
} catch (const std::out_of_range& e) {
    std::cout << "Key not found: " << e.what() << std::endl;
}
```

## 容量查询

```cpp
std::unordered_map<std::string, int> map = {{"A", 1}, {"B", 2}};

// 检查map是否为空
bool isEmpty = map.empty();

// 获取元素个数
size_t numElements = map.size();

// 获取桶（bucket）的数量（高级操作，通常用于性能调优）
size_t bucketCount = map.bucket_count();
```

## 元素修改

### `insert()`
插入一个或多个元素。如果键已存在，则插入失败。

```cpp
// 插入单个键值对
auto result = map.insert({"C", 3});
// result 是一个 std::pair<iterator, bool>
// result.first 是指向元素的迭代器
// result.second 是bool，表示插入是否成功（true成功，false因键已存在而失败）

// 插入多个元素（使用初始化列表）
map.insert({{"D", 4}, {"E", 5}});
```

### `emplace()`
比 `insert()` 更高效，它直接在容器内部构造元素，避免了临时对象的创建和拷贝。

```cpp
// 在原地构造一个 std::pair<const std::string, int>
auto result = map.emplace("F", 6);
```

**`erase()`**
删除元素。

```cpp
// 1. 通过迭代器删除
auto it = map.find("B");
if (it != map.end()) {
    map.erase(it);
}

// 2. 通过键删除
size_t numRemoved = map.erase("C"); // 返回被删除的元素个数（0或1）

// 3. 删除一个范围内的元素（不常用，因为无序）
// map.erase(itStart, itEnd);
```

### `clear()`
清空所有元素。

```cpp
map.clear();
```

## 查找

### `find()`
最重要的查找函数。根据键查找元素。
*   如果找到，返回指向该元素的迭代器。
*   如果没找到，返回 `end()` 迭代器。

```cpp
auto it = map.find("Alice");
if (it != map.end()) {
    // it->first 是键 (const)
    // it->second 是值
    std::cout << "Found: " << it->first << " -> " << it->second << std::endl;
} else {
    std::cout << "Key not found." << std::endl;
}
```

### `count()`
统计某个键出现的次数。由于键是唯一的，返回值只能是 0 或 1。可以用来检查键是否存在。

```cpp
if (map.count("Alice") > 0) {
    std::cout << "Alice exists." << std::endl;
}
```

## 迭代器

可以使用迭代器遍历 `unordered_map`，但遍历顺序是未定义的。

```cpp
// 使用范围for循环（C++11）
for (const auto& pair : ageMap) {
    std::cout << pair.first << ": " << pair.second << std::endl;
}

// 使用迭代器
for (auto it = ageMap.begin(); it != ageMap.end(); ++it) {
    std::cout << it->first << ": " << it->second << std::endl;
}

// C++17 结构化绑定（更清晰）
for (const auto& [key, value] : ageMap) {
    std::cout << key << ": " << value << std::endl;
}
```

## key 为自定义结构体

`std::unordered_map` 要求键必须是可哈希的且可相等比较的。。

### 方法一：特化 std::hash 和提供相等比较

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

struct Person {
    std::string name;
    int age;
    
    // 重载 == 运算符
    bool operator==(const Person& other) const {
        return name == other.name && age == other.age;
    }
};

// 特化 std::hash 模板
namespace std {
    template<>
    struct hash<Person> {
        size_t operator()(const Person& p) const {
            // 组合 name 和 age 的哈希值
            return hash<string>()(p.name) ^ (hash<int>()(p.age) << 1);
        }
    };
}

int main() {
    std::unordered_map<Person, std::string> personMap;
    
    personMap[{"Alice", 25}] = "Engineer";
    personMap[{"Bob", 30}] = "Designer";
    personMap[{"Alice", 30}] = "Manager";
    
    for (const auto& pair : personMap) {
        std::cout << pair.first.name << " (" << pair.first.age << "): " 
                  << pair.second << std::endl;
    }
    
    return 0;
}
```

### 方法二：提供自定义哈希和比较函数

```cpp
#include <iostream>
#include <unordered_map>
#include <string>

struct Person {
    std::string name;
    int age;
};

// 自定义哈希函数子
struct PersonHash {
    size_t operator()(const Person& p) const {
        return std::hash<std::string>()(p.name) ^ (std::hash<int>()(p.age) << 1);
    }
};

// 自定义相等比较函数子
struct PersonEqual {
    bool operator()(const Person& a, const Person& b) const {
        return a.name == b.name && a.age == b.age;
    }
};

int main() {
    // 在模板参数中指定哈希器和比较器
    std::unordered_map<Person, std::string, PersonHash, PersonEqual> personMap;
    
    personMap[{"Alice", 25}] = "Engineer";
    personMap[{"Bob", 30}] = "Designer";
    
    for (const auto& pair : personMap) {
        std::cout << pair.first.name << " (" << pair.first.age << "): " 
                  << pair.second << std::endl;
    }
    
    return 0;
}
```

## 注意事项

1.  **自定义键类型**：
    如果键是自定义的类或结构体，需要提供两个东西：
    *   **哈希函数**：告诉 `unordered_map` 如何计算你的键的哈希值。
    *   **相等比较函数**：告诉 `unordered_map` 如何判断两个键是否相等。

    可以通过特化 `std::hash` 模板或在自己的函数对象中实现它们，并在模板参数中传入。

    ```cpp
    struct MyKey {
        std::string name;
        int id;
    };

    // 1. 定义哈希函数（方式一：特化 std::hash）
    namespace std {
        template<> struct hash<MyKey> {
            size_t operator()(const MyKey& k) const {
                return hash<string>()(k.name) ^ (hash<int>()(k.id) << 1);
            }
        };
    }

    // 2. 定义相等比较（方式二：提供函数对象）
    struct MyKeyEqual {
        bool operator()(const MyKey& lhs, const MyKey& rhs) const {
            return lhs.name == rhs.name && lhs.id == rhs.id;
        }
    };

    // 使用
    std::unordered_map<MyKey, std::string, std::hash<MyKey>, MyKeyEqual> myMap;
    // 如果特化了 std::hash，可以省略第三个模板参数
    ```

2.  **性能**：
    *   **负载因子（Load Factor）**：是 `size() / bucket_count()` 的比值。当负载因子超过 `max_load_factor()`（默认约为 1.0）时，容器会自动增加桶的数量并进行 **重哈希（Rehash）**，这是一个昂贵的操作。
    *   你可以使用 `reserve(size_t n)` 来预先分配足够的桶，以避免插入大量元素时多次重哈希，提升性能。

3.  **键是 const**：
    在 `unordered_map` 中，键是 **常量** (`const`)。一旦插入，就不能修改键的值，因为这可能会破坏哈希表的结构（键的哈希值会变）。你可以修改值，但不能修改键。

## 总结

| 操作     | 接口                              | 说明                                                    |
| :------- | :-------------------------------- | :------------------------------------------------------ |
| **插入** | `insert`, `emplace`, `operator[]` | `emplace` 效率最高，`operator[]` 最方便但可能插入意外值 |
| **访问** | `operator[]`, `at`                | `at` 更安全，会进行边界检查                             |
| **查找** | `find`                            | 核心查找函数，返回迭代器                                |
| **删除** | `erase`                           | 通过迭代器或键删除                                      |
| **遍历** | 迭代器, 范围for                   | 顺序未定义                                              |
| **容量** | `size`, `empty`                   | 查询状态                                                |
| **调优** | `reserve`                         | 预分配桶，优化性能                                      |

`std::unordered_map` 是处理需要快速通过键查找值的场景的绝佳选择，在大多数情况下都应该优先于 `std::map` 使用，除非你需要元素保持有序。

# set

`std::set` 是 C++ 标准模板库（STL）中的一个关联容器，它存储唯一元素（不允许重复），并且元素会自动按照特定顺序排列。

- **唯一性**：容器内的每个元素都是唯一的
- **有序性**：元素总是按照指定的排序准则自动排序
- **基于红黑树**：通常使用平衡二叉搜索树（红黑树）实现
- **不可修改元素**：元素的值不能被直接修改（因为可能破坏排序）
- **高效操作**：查找、插入和删除操作的时间复杂度为 O(log n)

```cpp
#include <set>
```

## 构造函数和初始化

```cpp
// 1. 默认构造函数
std::set<int> s1;

// 2. 初始化列表初始化（C++11）
std::set<int> s2 = {3, 1, 4, 1, 5, 9}; // 结果为 {1, 3, 4, 5, 9}

// 3. 拷贝构造函数
std::set<int> s3(s2);

// 4. 使用迭代器范围构造
int arr[] = {5, 2, 8, 2, 1};
std::set<int> s4(arr, arr + 5); // 结果为 {1, 2, 5, 8}

// 5. 自定义比较函数
struct Compare {
    bool operator()(const int& a, const int& b) const {
        return a > b; // 降序排列
    }
};
std::set<int, Compare> s5 = {3, 1, 4, 2}; // 结果为 {4, 3, 2, 1}
```

## 插入元素

```cpp
std::set<int> s;

// 插入单个值
auto result = s.insert(10);
// result 是 std::pair<iterator, bool>
// result.first 是指向插入元素的迭代器
// result.second 表示是否插入成功（true表示成功，false表示元素已存在）

// 插入多个值（C++11）
s.insert({5, 15, 20, 5}); // 只会插入 5, 15, 20

// 使用迭代器提示插入位置（可能提高效率）
auto it = s.begin();
s.insert(it, 7); // 提示插入位置，但set仍会保持有序

// 使用emplace原地构造（C++11，避免拷贝）
s.emplace(25);
```

## 删除元素

```cpp
std::set<int> s = {1, 2, 3, 4, 5};

// 通过迭代器删除
auto it = s.find(3);
if (it != s.end()) {
    s.erase(it);
}

// 通过值删除
size_t count = s.erase(2); // 返回删除的元素个数（0或1）

// 删除一个范围内的元素
s.erase(s.find(4), s.end()); // 删除4及之后的元素

// 清空所有元素
s.clear();
```

## 查找操作

```cpp
std::set<int> s = {10, 20, 30, 40, 50};

// find() - 查找元素，返回迭代器
auto it = s.find(30);
if (it != s.end()) {
    std::cout << "Found: " << *it << std::endl;
}

// count() - 检查元素是否存在（返回0或1）
if (s.count(25) > 0) {
    std::cout << "25 exists" << std::endl;
}

// lower_bound() - 返回第一个不小于给定值的元素迭代器
auto lb = s.lower_bound(25); // 指向30

// upper_bound() - 返回第一个大于给定值的元素迭代器
auto ub = s.upper_bound(25); // 指向30

// equal_range() - 返回匹配元素的范围（对于set，范围长度为0或1）
auto range = s.equal_range(30);
if (range.first != range.second) {
    std::cout << "Element found" << std::endl;
}
```

## 容量查询

```cpp
std::set<int> s = {1, 2, 3};

// 检查set是否为空
bool empty = s.empty();

// 获取元素个数
size_t size = s.size();

// 获取可能的最大元素个数
size_t max_size = s.max_size();
```

## 迭代器

```cpp
std::set<int> s = {30, 10, 20, 40};

// 正向迭代器（元素按升序排列）
for (auto it = s.begin(); it != s.end(); ++it) {
    std::cout << *it << " "; // 输出: 10 20 30 40
}

// 反向迭代器
for (auto rit = s.rbegin(); rit != s.rend(); ++rit) {
    std::cout << *rit << " "; // 输出: 40 30 20 10
}

// 常量迭代器（防止修改元素）
for (auto cit = s.cbegin(); cit != s.cend(); ++cit) {
    // *cit = 5; // 错误：不能修改set中的元素
    std::cout << *cit << " ";
}

// 范围for循环（C++11）
for (const auto& elem : s) {
    std::cout << elem << " ";
}
```

## 其他操作

```cpp
std::set<int> s1 = {1, 2, 3};
std::set<int> s2 = {3, 4, 5};

// 交换两个set的内容
s1.swap(s2);

// 获取比较函数对象
auto comp = s1.value_comp();
bool result = comp(1, 2); // true，因为1 < 2
```

## 自定义类型作为set元素

当使用自定义类型作为set元素时，需要提供比较函数：

```cpp
#include <iostream>
#include <set>
#include <string>

struct Person {
    std::string name;
    int age;
    
    // 方法一：重载 < 运算符
    bool operator<(const Person& other) const {
        if (name == other.name) {
            return age < other.age;
        }
        return name < other.name;
    }
};

// 方法二：提供自定义比较函数
struct PersonCompare {
    bool operator()(const Person& a, const Person& b) const {
        if (a.name == b.name) {
            return a.age < b.age;
        }
        return a.name < b.name;
    }
};

int main() {
    // 使用方法一（重载<运算符）
    std::set<Person> s1;
    s1.insert({"Alice", 25});
    s1.insert({"Bob", 30});
    
    // 使用方法二（提供比较函数子）
    std::set<Person, PersonCompare> s2;
    s2.insert({"Charlie", 28});
    
    return 0;
}
```

## 应用场景

1. **去重**：自动去除重复元素
2. **有序存储**：需要元素按特定顺序排列
3. **快速查找**：需要频繁查找元素是否存在
4. **范围查询**：需要查找某个范围内的所有元素

## 与其它容器的比较

- **vs std::unordered_set**：`set` 有序但稍慢，`unordered_set` 无序但更快（平均O(1)查找）
- **vs std::multiset**：`set` 元素唯一，`multiset` 允许重复元素
- **vs std::vector**：`set` 自动排序且去重，但内存使用不如vector紧凑

## 性能特点

- **插入/删除/查找**：O(log n) 时间复杂度
- **内存使用**：由于基于树结构，需要额外指针空间
- **迭代效率**：遍历顺序即排序顺序，遍历时间复杂度为 O(n)

`std::set` 是一个功能强大的容器，特别适合需要维护唯一且有序元素的场景。
