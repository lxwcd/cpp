STL 学习笔记

# std::any_of

`std::any_of` 是一个定义在 `<algorithm>` 头文件中的算法。它的作用非常直观：**检查一个范围（如数组、向量等）内的元素，判断是否“至少有一个”（any of）元素满足给定的条件（谓词）**。

如果存在至少一个元素使得条件为真，则函数返回 `true`；如果范围内所有元素都不满足条件（或者范围为空），则返回 `false`。

它的行为类似于逻辑“或”（||）操作在整个范围上的扩展。

**头文件**：`#include <algorithm>`。

C++11 引入了 `std::any_of`，它主要有两种重载形式：

```cpp
// (1) 最常见的形式
template< class InputIt, class UnaryPredicate >
bool any_of( InputIt first, InputIt last, UnaryPredicate p );

// (2) C++17 起支持的并行执行策略形式（高级用法，暂不深入讨论）
template< class ExecutionPolicy, class ForwardIt, class UnaryPredicate >
bool any_of( ExecutionPolicy&& policy, ForwardIt first, ForwardIt last, UnaryPredicate p );
```

通常使用第一种形式。

*   **InputIt first, InputIt last**：定义了一个要检查的元素范围 `[first, last)`。
    *   `first`：指向范围第一个元素的迭代器。
    *   `last`：指向范围**最后一个元素之后**的迭代器。
*   **UnaryPredicate p**：这是一个**一元谓词**（Unary Predicate）。它可以是一个函数指针、函数对象（仿函数）、或者更常见的是一个 **Lambda 表达式**。它接受一个范围中的元素（解引用迭代器得到的类型）作为参数，并返回一个可以转换为 `bool` 类型的值。
    *   返回 `true` 表示该元素满足条件。
    *   返回 `false` 表示不满足。
*   **返回值**：`bool` 类型。
    *   `true`：如果谓词 `p` 对 `[first, last)` 中**至少一个元素**返回 `true`。
    *   `false`：如果谓词 `p` 对 `[first, last)` 中**所有元素**都返回 `false`，或者范围为空。

## 工作原理

`std::any_of` 的工作流程可以简单描述为：

1.  从 `first` 开始，遍历到 `last - 1`。
2.  对每个遍历到的元素，调用谓词 `p(*it)`。
3.  **一旦发现某个元素使得 `p(*it)` 返回 `true`，它就立即停止遍历，并返回 `true`**（短路求值）。
4.  如果遍历完所有元素，都没有发现一个满足条件的，则返回 `false`。

**关键特性：短路求值**。这意味着只要找到一个符合条件的元素，它就立刻返回，不会继续检查后面的元素。这在处理大型容器时效率很高。

短路求值（Short-circuit evaluation）是指在布尔表达式求值过程中，如果已经可以确定整个表达式的值，就不再继续计算剩余的部分。这种求值方式可以提高效率，避免不必要的计算，甚至可以避免某些错误。

## 示例 1：检查向量中是否存在任何偶数

```cpp
#include <iostream>
#include <vector>
#include <algorithm> // 必需的头文件

int main() {
    std::vector<int> numbers = {1, 3, 5, 8, 9};

    // 使用 Lambda 表达式作为谓词
    // 判断元素 n 是否是偶数: n % 2 == 0
    if (std::any_of(numbers.begin(), numbers.end(), [](int n) { return n % 2 == 0; })) {
        std::cout << "至少存在一个偶数。" << std::endl;
    } else {
        std::cout << "不存在任何偶数。" << std::endl;
    }

    return 0;
}
```

**输出：**
```
至少存在一个偶数。
```

## 示例 2：检查字符串是否包含任何大写字母

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype> // 用于 isupper

int main() {
    std::string str = "hello world";

    // 检查字符串中是否有大写字母
    bool hasUpperCase = std::any_of(str.begin(), str.end(), ::isupper);
    // ::isupper 是来自 <cctype> 的标准C库函数

    std::cout << "字符串 \"" << str << "\" "
              << (hasUpperCase ? "包含" : "不包含")
              << "大写字母。" << std::endl;

    // 测试另一个字符串
    std::string str2 = "Hello World";
    bool hasUpperCase2 = std::any_of(str2.begin(), str2.end(), ::isupper);

    std::cout << "字符串 \"" << str2 << "\" "
              << (hasUpperCase2 ? "包含" : "不包含")
              << "大写字母。" << std::endl;

    return 0;
}
```

**输出：**
```
字符串 "hello world" 不包含大写字母。
字符串 "Hello World" 包含大写字母。
```

## 示例 3：检查自定义结构体向量

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Person {
    std::string name;
    int age;
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 17},
        {"Charlie", 30},
        {"David", 16}
    };

    // 检查列表中是否有任何人年龄小于18岁（未成年人）
    bool hasMinor = std::any_of(people.begin(), people.end(),
                                [](const Person& p) { return p.age < 18; });

    if (hasMinor) {
        std::cout << "名单中存在未成年人。" << std::endl;
        // 可以进一步处理...
    }

    // 检查是否有人叫 "Bob"
    bool hasBob = std::any_of(people.begin(), people.end(),
                              [](const Person& p) { return p.name == "Bob"; });

    std::cout << "名单中" << (hasBob ? "有" : "没有") << "人叫 Bob。" << std::endl;

    return 0;
}
```

**输出：**
```
名单中存在未成年人。
名单中有人叫 Bob。
```

## 示例 4：处理空范围

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> empty_vector;

    // 检查空向量中是否有大于10的数
    bool result = std::any_of(empty_vector.begin(), empty_vector.end(),
                              [](int n) { return n > 10; });

    std::cout << "结果为空范围: " << std::boolalpha << result << std::endl;
    // 对于任何空范围，any_of 都会返回 false

    return 0;
}
```

**输出：**
```
结果为空范围: false
```

## 与 `std::all_of` 和 `std::none_of` 的对比

这三个算法是相关的，常常一起使用：

| 算法               | 功能描述                   | 返回 `true` 的条件                     |
| :----------------- | :------------------------- | :------------------------------------- |
| **`std::any_of`**  | **至少有一个**元素满足条件 | 存在一个或多个元素使谓词为真           |
| **`std::all_of`**  | **所有**元素都满足条件     | 所有元素都使谓词为真（空范围也返回真） |
| **`std::none_of`** | **没有任何**元素满足条件   | 所有元素都不使谓词为真（空范围返回真） |

# std::find_if

`std::find_if` 定义在 `<algorithm>` 头文件中。它的作用是：**在一个范围内查找第一个满足特定条件（谓词）的元素，并返回指向该元素的迭代器**。

与 `std::any_of`（只告诉你“是否存在”）不同，`std::find_if` 会告诉你“它在哪”。

```cpp
template< class InputIt, class UnaryPredicate >
InputIt find_if( InputIt first, InputIt last, UnaryPredicate p );
```

*   **InputIt first, InputIt last**：定义了一个要搜索的元素范围 `[first, last)`。
*   **UnaryPredicate p**：一个**一元谓词**（Unary Predicate）。与 `std::any_of` 一样，它可以是函数指针、函数对象或 Lambda 表达式。它接受一个范围中的元素作为参数，返回一个可转换为 `bool` 的值。
    *   返回 `true` 表示找到了目标元素。
    *   返回 `false` 表示不是要找的元素。
*   **返回值**：`InputIt` 类型的迭代器。
    *   如果找到满足条件的元素，返回**指向第一个满足条件元素**的迭代器。
    *   如果未找到任何满足条件的元素，则返回 `last`（即结束迭代器）。

## 工作原理

1.  从 `first` 开始，顺序遍历到 `last - 1`。
2.  对每个遍历到的元素，调用谓词 `p(*it)`。
3.  **一旦发现某个元素使得 `p(*it)` 返回 `true`，它就立即停止遍历，并返回指向该元素的迭代器 `it`**（短路求值）。
4.  如果遍历完所有元素，都没有发现一个满足条件的，则返回 `last`。

**关键特性**：同样是**短路求值**，找到第一个匹配项就立即返回。

## 示例 1：查找向量中第一个偶数

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 3, 5, 8, 9, 10};

    // 使用 find_if 查找第一个偶数
    auto it = std::find_if(numbers.begin(), numbers.end(),
                           [](int n) { return n % 2 == 0; });

    // 最重要的步骤：检查是否找到
    if (it != numbers.end()) {
        std::cout << "找到了第一个偶数: " << *it << std::endl; // 解引用迭代器获取值
        std::cout << "它在容器中的位置(索引): " << std::distance(numbers.begin(), it) << std::endl;
    } else {
        std::cout << "没有找到任何偶数。" << std::endl;
    }

    return 0;
}
```

**输出：**
```
找到了第一个偶数: 8
它在容器中的位置(索引): 3
```

**关键点**：**永远不要直接解引用 `std::find_if` 返回的迭代器**，必须先检查它是否等于容器的 `end()` 迭代器，否则如果未找到元素，解引用会导致未定义行为（通常是崩溃）。

## 示例 2：在字符串中查找第一个非字母字符

```cpp
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype> // 用于 isalpha

int main() {
    std::string filename = "myfile.txt";

    // 查找第一个不是字母的点号 '.'
    auto it = std::find_if(filename.begin(), filename.end(),
                           [](char c) { return !std::isalpha(c); });

    if (it != filename.end()) {
        std::cout << "文件扩展名起始于: '" << *it << "'" << std::endl;
        // 可以基于这个迭代器进行字符串分割
        std::string name(filename.begin(), it);   // "myfile"
        std::string ext(it, filename.end());      // ".txt"
        std::cout << "文件名: " << name << "\n扩展名: " << ext << std::endl;
    }

    return 0;
}
```

**输出：**
```
文件扩展名起始于: '.'
文件名: myfile
扩展名: .txt
```

## 示例 3：在自定义结构体向量中查找

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

struct Product {
    int id;
    std::string name;
    double price;
};

int main() {
    std::vector<Product> inventory = {
        {101, "Apple", 0.99},
        {202, "Milk", 2.49},
        {303, "Bread", 1.99},
        {404, "Eggs", 3.99}
    };

    // 查找第一个价格高于 $3.00 的商品
    auto expensiveIt = std::find_if(inventory.begin(), inventory.end(),
                                    [](const Product& p) { return p.price > 3.0; });

    if (expensiveIt != inventory.end()) {
        std::cout << "找到贵的商品: " << expensiveIt->name
                  << ", 价格: $" << expensiveIt->price << std::endl;
    }

    // 查找 ID 为 303 的商品
    auto idIt = std::find_if(inventory.begin(), inventory.end(),
                             [](const Product& p) { return p.id == 303; });

    if (idIt != inventory.end()) {
        std::cout << "ID 303 的商品是: " << idIt->name << std::endl;
    }

    return 0;
}
```

**输出：**
```
找到贵的商品: Eggs, 价格: $3.99
ID 303 的商品是: Bread
```


# std::find_if_not
`std::find_if_not` 定义在 `<algorithm>` 头文件中。它的作用与 `std::find_if` 相反：**在给定范围内查找第一个不满足特定条件（谓词）的元素，并返回指向该元素的迭代器**。

```cpp
template< class InputIt, class UnaryPredicate >
InputIt find_if_not( InputIt first, InputIt last, UnaryPredicate p );
```

*   **InputIt first, InputIt last**：定义要搜索的元素范围 `[first, last)`。
*   **UnaryPredicate p**：一个**一元谓词**，接受范围中的一个元素作为参数，返回一个可转换为 `bool` 的值。
    *   返回 `true` 表示元素**满足**条件（将被跳过）
    *   返回 `false` 表示元素**不满足**条件（这是我们要找的）
*   **返回值**：
    *   如果找到不满足条件的元素，返回指向**第一个**此类元素的迭代器
    *   如果所有元素都满足条件（或范围为空），返回 `last`

1.  从 `first` 开始遍历到 `last - 1`
2.  对每个元素调用 `p(*it)`
3.  如果 `p(*it)` 返回 `false`，立即停止遍历并返回 `it`
4.  如果所有 `p(*it)` 都返回 `true`，则返回 `last`

与 `std::find_if` 一样，它也使用**短路求值**，找到第一个匹配项就立即返回。

## 示例 1：查找第一个非正数

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 0, 4, -1, 5};

    // 使用 find_if_not 查找第一个非正数（<=0的数）
    auto it = std::find_if_not(numbers.begin(), numbers.end(),
                              [](int n) { return n > 0; });

    if (it != numbers.end()) {
        std::cout << "找到第一个非正数: " << *it 
                  << " 在位置 " << std::distance(numbers.begin(), it) 
                  << std::endl;
    } else {
        std::cout << "所有数字都是正数" << std::endl;
    }

    return 0;
}
```

**输出：**
```
找到第一个非正数: 0 在位置 3
```

# std::find_end
`std::find_end` 是 C++ STL 中的一个算法，用于在指定范围内查找**最后一个**与给定子序列匹配的子序列。如果找到了匹配的子序列，它返回一个迭代器对，指向匹配的子序列的开始和结束位置；如果没有找到匹配的子序列，它返回一个迭代器对，其中两个迭代器都等于范围的结束位置。

`std::find_end` 的函数原型如下：
```cpp
template <class ForwardIt1, class ForwardIt2>
ForwardIt1 find_end(ForwardIt1 first1, ForwardIt1 last1, ForwardIt2 first2, ForwardIt2 last2);
```

- **`first1`**：范围1的起始迭代器。
- **`last1`**：范围1的结束迭代器。
- **`first2`**：范围2的起始迭代器。
- **`last2`**：范围2的结束迭代器。

- 如果找到匹配的子序列，返回一个迭代器对，其中第一个迭代器指向匹配的子序列的开始位置，第二个迭代器指向匹配的子序列的结束位置。
- 如果没有找到匹配的子序列，返回一个迭代器对，其中两个迭代器都等于 `last1`。

## 示例 1：查找最后一个匹配的子序列
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 2, 3};
    std::vector<int> subsequence = {2, 3};

    auto result = std::find_end(numbers.begin(), numbers.end(), subsequence.begin(), subsequence.end());

    if (result.first != numbers.end()) {
        std::cout << "Found subsequence starting at index: " << std::distance(numbers.begin(), result.first) << std::endl;
    } else {
        std::cout << "Subsequence not found." << std::endl;
    }

    return 0;
}
```

输出结果
```
Found subsequence starting at index: 5
```

# std::count 
`std::count` 是 C++ STL 中的一个算法，用于计算范围内等于指定值的元素数量。它属于 `<algorithm>` 头文件中的标准库算法。

`std::count` 的函数原型如下：
```cpp
template <class InputIt, class T>
typename iterator_traits<InputIt>::difference_type count(InputIt first, InputIt last, const T& value);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`value`**：要计数的值。

返回范围内等于 `value` 的元素数量。返回值的类型是迭代器的差值类型（`typename iterator_traits<InputIt>::difference_type`），通常是 `std::ptrdiff_t`。

## 示例 1：计算数组中某个值的数量
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    int count2 = std::count(numbers.begin(), numbers.end(), 2);
    int count3 = std::count(numbers.begin(), numbers.end(), 3);
    int count4 = std::count(numbers.begin(), numbers.end(), 4);

    std::cout << "Count of 2: " << count2 << std::endl;
    std::cout << "Count of 3: " << count3 << std::endl;
    std::cout << "Count of 4: " << count4 << std::endl;

    return 0;
}
```

输出结果
```
Count of 2: 2
Count of 3: 3
Count of 4: 4
```

# std::count_if 
`std::count_if` 是 C++ STL 中的一个算法，用于计算范围内满足特定条件的元素数量。它属于 `<algorithm>` 头文件中的标准库算法。

`std::count_if` 的函数原型如下：
```cpp
template <class InputIt, class UnaryPredicate>
typename iterator_traits<InputIt>::difference_type count_if(InputIt first, InputIt last, UnaryPredicate p);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`p`**：一元谓词，用于检查每个元素是否满足条件。谓词是一个函数对象，接受一个参数并返回一个布尔值。

返回范围内满足条件的元素数量。返回值的类型是迭代器的差值类型（`typename iterator_traits<InputIt>::difference_type`），通常是 `std::ptrdiff_t`。

## 示例 1：计算数组中偶数的数量
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int countEven = std::count_if(numbers.begin(), numbers.end(), [](int n) {
        return n % 2 == 0;
    });

    std::cout << "Count of even numbers: " << countEven << std::endl;

    return 0;
}
```

输出结果
```
Count of even numbers: 5
```

# std::distance

`std::distance` 是一个定义在 `<iterator>` 头文件中的函数模板。它的核心作用是：**计算两个迭代器之间的元素个数**。

```cpp
template< class InputIt >
typename std::iterator_traits<InputIt>::difference_type
    distance( InputIt first, InputIt last );
```

*   **参数**：
    *   `first`：指向范围起始位置的迭代器
    *   `last`：指向范围结束位置的迭代器（或最后一个元素之后的位置）
*   **返回值**：
    *   返回从 `first` 到 `last` 需要移动的次数（即元素个数）
    *   返回类型通常是 `std::ptrdiff_t`（一个有符号整数类型）

## 基本用法

```cpp
#include <iostream>
#include <vector>
#include <iterator> // 必需的头文件
#include <list>

int main() {
    std::vector<int> vec = {10, 20, 30, 40, 50};
    
    // 计算整个容器的元素数量（与 size() 效果相同）
    auto total = std::distance(vec.begin(), vec.end());
    std::cout << "Vector size: " << total << std::endl; // 输出 5
    
    // 计算两个迭代器之间的元素数量
    auto start = vec.begin();
    auto mid = start + 2; // 指向30
    auto count = std::distance(start, mid);
    std::cout << "Elements between begin() and mid: " << count << std::endl; // 输出 2
    
    return 0;
}
```

# std::max_element

`std::max_element` 的函数原型如下：
```cpp
template <class ForwardIt>
ForwardIt max_element(ForwardIt first, ForwardIt last);

template <class ForwardIt, class Compare>
ForwardIt max_element(ForwardIt first, ForwardIt last, Compare comp);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`comp`**（可选）：比较函数，用于定义比较逻辑。默认情况下，使用 `operator<`。

返回一个迭代器，指向范围内的最大值元素。如果有多个最大值元素，返回第一个找到的迭代器。

## 示例 1：查找数组中的最大值
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    auto max_it = std::max_element(numbers.begin(), numbers.end());

    std::cout << "The maximum element is: " << *max_it << std::endl;

    return 0;
}
```

输出结果
```
The maximum element is: 9
```

## 示例 2：使用自定义比较函数

比较函数 comp 应该返回 true，如果第一个参数小于第二个参数。
std::max_element 会返回一个迭代器，指向范围内第一个满足 comp(*max_it, *it) 为 false 的元素。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& n, int a) : name(n), age(a) {}
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35},
        {"Diana", 20}
    };

    auto max_it = std::max_element(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age; // 比较年龄
    });

    std::cout << "The oldest person is: " << max_it->name << " (" << max_it->age << ")" << std::endl;

    return 0;
}
```

# std::min_element

`std::min_element` 的函数原型如下：
```cpp
template <class ForwardIt>
ForwardIt min_element(ForwardIt first, ForwardIt last);

template <class ForwardIt, class Compare>
ForwardIt min_element(ForwardIt first, ForwardIt last, Compare comp);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`comp`**（可选）：比较函数，用于定义比较逻辑。默认情况下，使用 `operator<`。

返回一个迭代器，指向范围内的最小值元素。如果有多个最小值元素，返回第一个找到的迭代器。

## 示例 1：查找数组中的最小值
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    auto min_it = std::min_element(numbers.begin(), numbers.end());

    std::cout << "The minimum element is: " << *min_it << std::endl;

    return 0;
}
```

输出结果
```
The minimum element is: 1
```

## 示例 2：使用自定义比较函数
比较函数 comp 应该返回 true，如果第一个参数小于第二个参数。
std::min_element 会返回一个迭代器，指向范围内第一个满足 comp(*min_it, *it) 为 true 的元素。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& n, int a) : name(n), age(a) {}
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35},
        {"Diana", 20}
    };

    auto min_it = std::min_element(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age; // 比较年龄
    });

    std::cout << "The youngest person is: " << min_it->name << " (" << min_it->age << ")" << std::endl;

    return 0;
}
```

## 注意事项
1. **迭代器类型**：
   - `std::max_element` 和 `std::min_element` 要求范围内的迭代器是正向迭代器（`ForwardIt`），这意味着它们可以多次遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是正向迭代器。

2. **比较函数**：
   - 比较函数可以是普通函数、lambda 表达式或函数对象，只要它接受两个参数并返回一个布尔值即可。
   - 例如，`[](int a, int b) { return a > b; }` 是一个 lambda 表达式。

3. **性能**：
   - `std::max_element` 和 `std::min_element` 的时间复杂度是 O(n)，其中 n 是范围内的元素数量。
   - 它们逐个检查范围内的每个元素，因此对于大型数据集，性能可能会受到影响。

4. **返回值**：
   - 如果范围为空，返回的迭代器等于 `last`。因此，在使用返回的迭代器之前，应该检查它是否等于 `last`。

# std::lower_bound

`std::lower_bound` 的函数原型如下：
```cpp
template <class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value);

template <class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`value`**：要查找的值。
- **`comp`**（可选）：比较函数，用于定义比较逻辑。默认情况下，使用 `operator<`。

返回一个迭代器，指向范围 `[first, last)` 中第一个不小于 `value` 的元素。如果所有元素都小于 `value`，返回 `last`。

## 示例 1：查找第一个不小于指定值的元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 5, 4, 5, 6, 7};

    auto it = std::lower_bound(numbers.begin(), numbers.end(), 4);

    if (it != numbers.end()) {
        std::cout << "First element not less than 4 is: " << *it << std::endl;
    } else {
        std::cout << "No element not less than 4 found." << std::endl;
    }

    return 0;
}
```

输出结果
```
First element not less than 4 is: 5
```

# std::upper_bound

`std::upper_bound` 的函数原型如下：
```cpp
template <class ForwardIt, class T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value);

template <class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`value`**：要查找的值。
- **`comp`**（可选）：比较函数，用于定义比较逻辑。默认情况下，使用 `operator<`。

返回一个迭代器，指向范围 `[first, last)` 中第一个大于 `value` 的元素。如果所有元素都小于或等于 `value`，返回 `last`。

## 示例 1：查找第一个大于指定值的元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 4, 4, 5, 6, 7};

    auto it = std::upper_bound(numbers.begin(), numbers.end(), 4);

    if (it != numbers.end()) {
        std::cout << "First element greater than 4 is: " << *it << std::endl;
    } else {
        std::cout << "No element greater than 4 found." << std::endl;
    }

    return 0;
}
```

输出结果
```
First element greater than 4 is: 5
```

## 注意事项
1. **范围必须有序**：
   - `std::lower_bound` 和 `std::upper_bound` 要求范围 `[first, last)` 必须是有序的。
   - 如果范围不是有序的，结果是未定义的。

2. **迭代器类型**：
   - 这两个算法要求范围内的迭代器是正向迭代器（`ForwardIt`），这意味着它们可以多次遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是正向迭代器。

3. **性能**：
   - `std::lower_bound` 和 `std::upper_bound` 的时间复杂度是 O(log n)，其中 n 是范围内的元素数量。
   - 它们使用二分查找算法，因此对于大型数据集，性能非常高效。

4. **自定义比较函数**：
   - 比较函数可以是普通函数、lambda 表达式或函数对象，只要它接受两个参数并返回一个布尔值即可。
   - 例如，`[](const Person& a, int age) { return a.age < age; }` 是一个 lambda 表达式。

# std::unique
`std::unique` 是 C++ STL 中的一个算法，用于移除有序范围内重复的元素。它属于 `<algorithm>` 头文件中的标准库算法。`std::unique` 会将重复的元素移到范围的末尾，并返回一个迭代器，指向“新范围”的结束位置。

`std::unique` 的函数原型如下：
```cpp
template <class ForwardIt>
ForwardIt unique(ForwardIt first, ForwardIt last);

template <class ForwardIt, class BinaryPredicate>
ForwardIt unique(ForwardIt first, ForwardIt last, BinaryPredicate p);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`p`**（可选）：二元谓词，用于定义两个元素是否相等。默认情况下，使用 `operator==`。

返回一个迭代器，指向“新范围”的结束位置。这个算法不会真正删除元素，而是将重复的元素移到范围的末尾。实际删除操作需要结合容器的 `erase` 方法。

## 示例 1：移除有序数组中的重复元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 2, 3, 3, 3, 4, 4, 4, 4};

    // 移除有序数组中的重复元素
    auto new_end = std::unique(numbers.begin(), numbers.end());

    // 删除范围 [new_end, numbers.end()) 中的元素
    numbers.erase(new_end, numbers.end());

    // 输出修改后的数组
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
1 2 3 4
```

## 示例 2：移除有序字符串中的重复字符
```cpp
#include <iostream>
#include <string>
#include <algorithm>

int main() {
    std::string str = "aaabbbcccd";

    // 移除有序字符串中的重复字符
    auto new_end = std::unique(str.begin(), str.end());

    // 删除范围 [new_end, str.end()) 中的字符
    str.erase(new_end, str.end());

    // 输出修改后的字符串
    std::cout << str << std::endl;

    return 0;
}
```

输出结果
```
abcd
```

## 示例 3：使用自定义二元谓词
假设有一个自定义类 `Person`，我们想移除名字相同的重复对象。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& n, int a) : name(n), age(a) {}

    friend std::ostream& operator<<(std::ostream& os, const Person& p) {
        os << p.name << " (" << p.age << ")";
        return os;
    }
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Alice", 30},
        {"Bob", 35},
        {"Charlie", 20},
        {"Charlie", 25}
    };

    // 移除名字相同的重复对象
    auto new_end = std::unique(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.name == b.name;
    });

    // 删除范围 [new_end, people.end()) 中的元素
    people.erase(new_end, people.end());

    // 输出修改后的数组
    for (const auto& person : people) {
        std::cout << person << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
Alice (25) Bob (35) Charlie (20)
```

## 注意事项
1. **范围必须有序**：
   - `std::unique` 要求范围 `[first, last)` 必须是有序的。如果范围不是有序的，结果是未定义的。
   - 如果需要对无序范围进行去重，可以先使用 `std::sort` 对范围进行排序。

2. **迭代器类型**：
   - `std::unique` 要求范围内的迭代器是正向迭代器（`ForwardIt`），这意味着它们可以多次遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是正向迭代器。

3. **性能**：
   - `std::unique` 的时间复杂度是 O(n)，其中 n 是范围内的元素数量。
   - 它逐个检查范围内的每个元素，因此对于大型数据集，性能可能会受到影响。

4. **实际删除操作**：
   - `std::unique` 不会真正删除元素，而是将重复的元素移到范围的末尾，并返回新的结束位置。
   - 实际删除操作需要结合容器的 `erase` 方法。

5. **自定义比较函数**：
   - 比较函数可以是普通函数、lambda 表达式或函数对象，只要它接受两个参数并返回一个布尔值即可。
   - 例如，`[](const Person& a, const Person& b) { return a.name == b.name; }` 是一个 lambda 表达式。
