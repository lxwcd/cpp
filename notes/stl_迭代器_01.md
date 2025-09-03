STL 迭代器

--------------------------------------------------
- **迭代器区间** `[first, last)`：包含 `first`，不包含 `last`。  
- **谓词 (Predicate)**：一个可调用对象，形参是元素，返回 `bool`。  
- **函数对象 (Unary/Nullary/BinaryOp)**：一个可调用对象，用来“读/改/算”元素。
--------------------------------------------------

迭代器是一种抽象的概念，它提供了对容器中元素的访问方法，同时隐藏了容器的内部实现细节。迭代器允许我们以相同的方式处理不同的容器。

# 迭代器的分类

STL 迭代器分为五类，每类支持不同的操作：

1. **输入迭代器（Input Iterator）**：只能读取元素，只能向前移动
2. **输出迭代器（Output Iterator）**：只能写入元素，只能向前移动
3. **前向迭代器（Forward Iterator）**：可读写，只能向前移动
4. **双向迭代器（Bidirectional Iterator）**：可读写，可向前和向后移动
5. **随机访问迭代器（Random Access Iterator）**：可读写，支持任意位置的跳转

# 获取迭代器

每个容器都提供了获取迭代器的成员函数：

```cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    
    // 获取指向第一个元素的迭代器
    auto begin_it = vec.begin();
    
    // 获取指向最后一个元素之后位置的迭代器
    auto end_it = vec.end();
    
    // 获取常量迭代器（C++11）
    auto cbegin_it = vec.cbegin();
    auto cend_it = vec.cend();
    
    // 获取反向迭代器
    auto rbegin_it = vec.rbegin();
    auto rend_it = vec.rend();
    
    // 获取常量反向迭代器（C++11）
    auto crbegin_it = vec.crbegin();
    auto crend_it = vec.crend();
    
    return 0;
}
```

# 迭代器的基本操作

```cpp
#include <vector>
#include <iostream>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    auto it = vec.begin();
    
    // 解引用迭代器
    std::cout << *it << std::endl; // 输出: 1
    
    // 前置递增
    ++it;
    std::cout << *it << std::endl; // 输出: 2
    
    // 后置递增
    auto old_it = it++;
    std::cout << *old_it << std::endl; // 输出: 2
    std::cout << *it << std::endl;     // 输出: 3
    
    // 迭代器比较
    if (it != vec.end()) {
        std::cout << "Iterator is valid" << std::endl;
    }
    
    return 0;
}
```

# base

`base()` 是**迭代器适配器（Iterator Adapter）** 的成员函数，并不是普通迭代器的函数。最常见的迭代器适配器是 **`std::reverse_iterator`**。

**核心作用：** `base()` 函数用于获取与当前反向迭代器相对应的**底层普通（正向）迭代器的下一个位置*。

**关键关系：** 一个 `reverse_iterator` 和它通过 `base()` 得到的普通 `iterator` 之间存在着一种**偏移关系**，理解这个关系是正确使用 `base()` 的关键。

`base()` 主要在需要将反向迭代器转换为正向迭代器进行操作的场景中使用。

## base 指向位置有偏移

**`reverse_iterator(it)` 总是与 `it - 1` 引用同一个元素。**

*   `rbegin()` 逻辑上指向最后一个元素，其 `base()` 是 `end()`。对 `end()` 进行递减是安全的（只要容器非空），得到最后一个元素。
*   `rend()` 逻辑上指向第一个元素之前，其 `base()` 是 `begin()`。对 `begin()` 进行递减是**不安全**的（未定义行为）。但因为我们永远不会解引用 `rend()`，所以不会触发这个操作。

如果 `base()` 直接指向逻辑元素，那么 `rend().base()` 将不得不指向第一个元素之前，这本身就是一个无效的位置，会破坏 STL 迭代器有效范围的约定。

## 场景 1：在反向查找后执行插入或删除操作

这是 `base()` 最常见的用途。你使用 `std::find` 在反向迭代器范围进行查找，找到后需要调用像 `insert` 或 `erase` 这样的成员函数，而这些函数只接受普通迭代器。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5, 4, 3, 2, 1};

    // 反向查找最后一个等于 3 的元素
    auto rit = std::find(vec.rbegin(), vec.rend(), 3);

    if (rit != vec.rend()) {
        std::cout << "找到最后一个3，其在容器中的逻辑位置（从后往前）: " 
                  << *rit << std::endl;

        // 要在找到的元素后面插入一个新元素，需要使用 base()
        // rit.base() 指向的是正向迭代器中 3 后面的那个元素
        vec.insert(rit.base(), 99); // 在最后一个3的后面插入99

        // 如果要删除找到的这个元素，需要对 base() 进行偏移
        // vec.erase(rit.base()); // 错误！这会删除下一个元素
        // vec.erase((++rit).base()); // 一种方法，但改变了 rit
        vec.erase(std::next(rit).base()); // 更清晰的方法：删除 rit 指向的元素

        std::cout << "操作后向量: ";
        for (int num : vec) std::cout << num << " ";
        std::cout << std::endl;
    }

    return 0;
}
```
**可能的输出：**
```
找到最后一个3，其在容器中的逻辑位置（从后往前）: 3
操作后向量: 1 2 3 4 5 4 99 2 1
```
*注意：上面的插入和删除操作是分开演示的，如果同时执行需要仔细考虑迭代器失效问题。*

## 场景 2：获取反向迭代器指向元素的正向索引

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<std::string> words = {"apple", "banana", "cherry", "date", "elderberry"};

    // 反向查找第一个长度大于5的字符串
    auto rit = std::find_if(words.rbegin(), words.rend(),
                            [](const std::string& s) { return s.length() > 5; });

    if (rit != words.rend()) {
        std::cout << "从后往前找，第一个长字符串是: " << *rit << std::endl;

        // 计算它在向量中的正向位置（索引）
        // 1. 获取对应的正向迭代器
        auto it = rit.base();
        // 2. 计算从开始到该迭代器的距离
        // 因为 it 指向的是 rit 的下一个元素，所以需要 -1 来得到 rit 真正指向的元素索引
        size_t index = std::distance(words.begin(), it) - 1;

        // 在C++14/17后，更推荐使用 std::prev 来避免直接算术运算
        // size_t index = std::distance(words.begin(), std::prev(rit.base()));

        std::cout << "它在向量中的索引是: " << index << std::endl;
    }

    return 0;
}
```
**输出：**
```
从后往前找，第一个长字符串是: elderberry
它在向量中的索引是: 4
```

## 场景 3：将反向查找结果用于只接受正向迭代器的算法

```cpp
#include <iostream>
#include <list>
#include <algorithm>

int main() {
    std::list<int> myList = {10, 20, 30, 40, 50, 40, 30, 20, 10};

    // 反向查找最后一个30
    auto last_30_rit = std::find(myList.rbegin(), myList.rend(), 30);

    if (last_30_rit != myList.rend()) {
        // 假设我们想从这个30开始拷贝到末尾（使用正向迭代器）
        // 获取对应的正向迭代器
        auto start_it = last_30_rit.base(); // 这指向30后面的元素

        std::cout << "从最后一个30之后开始的内容: ";
        std::for_each(start_it, myList.end(), [](int n) { std::cout << n << " "; });
        std::cout << std::endl;

        // 如果我们想包含最后一个30本身，需要使用 std::prev
        auto start_inclusive_it = std::prev(last_30_rit.base());
        std::cout << "从最后一个30开始的内容: ";
        std::for_each(start_inclusive_it, myList.end(), [](int n) { std::cout << n << " "; });
        std::cout << std::endl;
    }

    return 0;
}
```
**输出：**
```
从最后一个30之后开始的内容: 40 20 10
从最后一个30开始的内容: 30 40 20 10
```

## 注意事项

1.  **偏移是关键**：永远记住 `rit.base()` 指向的是 `*rit` 的下一个元素。在大多数修改操作中，你都需要对 `base()` 的结果进行 `std::prev()` 或 `std::next()` 调整。

2.  **插入操作**：
    *   要在 `rit` 指向的元素**之后**插入，使用 `rit.base()`。
    *   要在 `rit` 指向的元素**之前**插入，使用 `std::prev(rit.base())` 或 `(++rit).base()`。

3.  **删除操作**：
    *   要删除 `rit` 指向的元素，使用 `std::next(rit).base()` 或 `(++rit).base()`（但要小心迭代器失效）。
    *   **`vec.erase(rit.base())` 是错误的**，这会删除 `rit` 指向的下一个元素。

4.  **C++11/14 后的更安全用法**：使用 `std::prev()` 和 `std::next()` 等标准函数来进行偏移，而不是直接进行算术运算，这样代码意图更清晰，也更安全。

5.  **迭代器失效**：对容器进行修改操作（如 `insert`, `erase`) 后，所有指向该容器的迭代器（包括反向迭代器及其 `base()`）都可能失效，需要重新获取。

# back_inserter
> [std::back_inserter - cppreference.com](https://en.cppreference.com/w/cpp/iterator/back_inserter.html) 

`std::back_inserter` 是 **“尾部自动扩张”** 的法宝：  
它**不是一个真正的迭代器**，而是一个 **“插入迭代器适配器”**。  
把它交给算法（如 `transform`/`copy`），算法每次往它“赋值”时，就自动调用容器的 `push_back / push_back / append`，把元素插到尾部——  
因此**不需要提前为容器分配空间**。

```cpp
#include <iterator>          // 必须
auto inserter = std::back_inserter(container);
```
- `container` 必须是支持 `push_back(value)` 的序列容器：  
  `std::vector / std::deque / std::string / std::list / std::vector<bool>` 等。

## 工作原理（伪代码）

当写：

```cpp
*inserter = value;   // 看起来像给迭代器赋值
```

内部实际执行：

```cpp
container.push_back(value);   // 把 value 追加到末尾
++inserter;                   // 无意义的空操作，保持迭代器语法一致
```

因此它**没有越界风险**，也不怕容量不足；容器会自动增长。

--------------------------------------------------
## 三种插入迭代器对比
--------------------------------------------------
| 名称                | 调用方法              | 适用容器                 | 插入位置       |
| ------------------- | --------------------- | ------------------------ | -------------- |
| `back_inserter(c)`  | `c.push_back(value)`  | vector/string/deque/list | **尾部**       |
| `front_inserter(c)` | `c.push_front(value)` | deque/list               | **头部**       |
| `inserter(c,it)`    | `c.insert(it,value)`  | 任意序列/关联容器        | **指定位置前** |

## 示例

```cpp
#include <algorithm>
#include <iterator>
#include <vector>
#include <iostream>

int main() {
    std::vector<int> src  = {1, 2, 3};
    std::vector<int> dst;                       // 空，不提前 reserve

    // 用 back_inserter 把 src 每个元素×2 追加到 dst
    std::transform(src.begin(), src.end(),
                   std::back_inserter(dst),
                   [](int x) { return x * 2; });

    // dst 现在 {2,4,6}
    for (int v : dst) std::cout << v << ' ';    // 2 4 6
}
```

## 常见用法

1. **string 转大写**
   ```cpp
   std::string s = "hello";
   std::string upper;
   std::transform(s.begin(), s.end(),
                  std::back_inserter(upper),
                  [](unsigned char c){ return std::toupper(c); });
   ```

2. **vector 追加拷贝**  
   ```cpp
   std::vector<int> a = {1,2,3}, b = {4,5,6};
   std::copy(b.begin(), b.end(), std::back_inserter(a));
   // a = {1,2,3,4,5,6}
   ```

3. **list 追加**  
   ```cpp
   std::list<double> lst = {1.1};
   std::back_insert_iterator<std::list<double>> it = std::back_inserter(lst);
   *it = 2.2;   // lst 现在是 {1.1,2.2}
   ```

## 注意

- **容器必须支持 `push_back`**，否则编译报错（如 `std::array` / 关联容器）。  
- `std::string` 的 `back_inserter` 实际上是 `std::back_insert_iterator<std::string>`，  
  每次插入都触发 **字符追加**，复杂度摊还 O(1)。  
- 高频插入时最好先 `reserve()` 以减少重新分配。

