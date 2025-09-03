STL 学习笔记

# std::for_each
`std::for_each` 是 C++ STL 中的一个算法，用于对指定范围内的每个元素执行给定的操作。它属于 `<algorithm>` 头文件中的标准库算法。

`std::for_each` 的函数原型如下：
```cpp
template <class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`f`**：一元函数，用于对每个元素执行操作。函数对象接受一个参数（范围内的每个元素）并返回一个值。

返回传递给 `for_each` 的函数对象 `f`。这允许链式调用和进一步的操作。

## 示例 1：打印数组中的每个元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    std::for_each(numbers.begin(), numbers.end(), [](int n) {
        std::cout << n << " ";
    });
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
1 2 3 4 5
```

# std::generate
`std::generate` 是 C++ STL 中的一个算法，用于用指定的生成器函数填充容器中的元素。它属于 `<algorithm>` 头文件中的标准库算法。

`std::generate` 的函数原型如下：
```cpp
template <class ForwardIt, class Generator>
void generate(ForwardIt first, ForwardIt last, Generator gen);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`gen`**：生成器函数，用于生成值填充到容器中。这个函数对象不接受参数，返回一个值。

`std::generate` 不返回任何值，它直接在指定范围内填充生成的值。

## 示例 1：用随机数填充数组
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main() {
    std::vector<int> numbers(10);

    // 用随机数填充数组
    std::srand(std::time(0)); // 初始化随机数种子
    std::generate(numbers.begin(), numbers.end(), std::rand);

    // 打印数组
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
12345 67890 12345 67890 12345 67890 12345 67890 12345 67890
```

## 示例 2：用递增序列填充数组
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers(10);

    // 用递增序列填充数组
    int value = 1;
    std::generate(numbers.begin(), numbers.end(), [&value]() {
        return value++;
    });

    // 打印数组
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
1 2 3 4 5 6 7 8 9 10
```

## 示例 3：用特定函数生成值填充数组
假设我们有一个生成器函数，用于生成特定的值。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int generateValue() {
    static int value = 1;
    return value++;
}

int main() {
    std::vector<int> numbers(10);

    // 用特定函数生成值填充数组
    std::generate(numbers.begin(), numbers.end(), generateValue);

    // 打印数组
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
1 2 3 4 5 6 7 8 9 10
```

## 注意事项
1. **生成器函数**：
   - 生成器函数不接受参数，返回一个值。
   - 生成器函数可以是普通函数、lambda 表达式或函数对象。
   - 例如，`std::rand` 是一个普通函数，`[&value]() { return value++; }` 是一个 lambda 表达式。

2. **范围**：
   - `std::generate` 要求范围内的迭代器是正向迭代器（`ForwardIt`），这意味着它们可以多次遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是正向迭代器。

3. **性能**：
   - `std::generate` 的时间复杂度是 O(n)，其中 n 是范围内的元素数量。
   - 它逐个填充范围内的每个元素，因此对于大型数据集，性能可能会受到影响。

# std::generate_n
`std::generate_n` 是 C++ STL 中的一个算法，用于用指定的生成器函数填充容器中的前 `n` 个元素。它属于 `<algorithm>` 头文件中的标准库算法。

`std::generate_n` 的函数原型如下：
```cpp
template <class OutputIt, class Size, class Generator>
OutputIt generate_n(OutputIt first, Size n, Generator gen);
```

- **`first`**：范围的起始迭代器。
- **`n`**：要填充的元素数量。
- **`gen`**：生成器函数，用于生成值填充到容器中。这个函数对象不接受参数，返回一个值。

返回指向填充结束位置的迭代器。

## 示例 1：用随机数填充数组的前 `n` 个元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>

int main() {
    std::vector<int> numbers(10);

    // 用随机数填充数组的前 5 个元素
    std::srand(std::time(0)); // 初始化随机数种子
    std::generate_n(numbers.begin(), 5, std::rand);

    // 打印数组
    for (int n : numbers) {
        std::cout << n << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
12345 67890 12345 67890 12345 0 0 0 0 0
```

# std::remove_if  —— 逻辑删除  

```cpp
template< class ForwardIt, class UnaryPred >
ForwardIt remove_if( ForwardIt first, ForwardIt last, UnaryPred p );
```

| 参数    | 含义                                        |
| ------- | ------------------------------------------- |
| `first` | 区间起始迭代器                              |
| `last`  | 区间尾后迭代器                              |
| `p`     | 谓词，`p(*it)` 返回 true 的元素被“逻辑删除” |

内部做的事（伪代码）  
```cpp
auto new_end = first;
for (; first != last; ++first)
    if (!p(*first))          // 如果 **不用** 删除
        *new_end++ = *first; // 把元素搬到前面
return new_end;              // 指向新区间末尾
```

返回值  
- `ForwardIt new_end`：新区间的“逻辑尾后”迭代器。  
- `[new_end, last)` 这一段里的元素是**垃圾值**，**容器大小没变**。

常见坑  
- 忘了 `erase` 真删 → 垃圾值还在，size 不对。  
- 谓词里不能改元素（算法可能搬移两次）。  

初始状态（举例）  
走廊箱子： `[1 2 3 4 5 6 7 8 9 10]`  
假设要删除所有奇数。

```text
索引: 0 1 2 3 4 5 6 7 8 9
数据: 1 2 3 4 5 6 7 8 9 10
      ↑first↑new_end  都从0出发
```

当 `first` 跑到 `last`（扫描结束）时：

```text
索引: 0 1 2 3 4 5 6 7 8 9
数据: 2 4 6 8 10 ? ? ? ? ?
                ↑new_end
```

- 索引 0~4 是要保留的偶数。  
- 索引 5~9 的内容是“垃圾值”，但**物理上还在**。  
- `new_end` 指向索引 5，即“新区间尾后”迭代器。

`return new_end;` 把上面这个“垃圾区起点”交给调用者，后续由 `erase(new_end, vec.end())` 把尾巴真正砍掉。
残余值要删除可以用 erase()；
array 不适合用 remove() 和 remove_if()，因为 array 无法缩小尺寸。可以用 remove_copy() 和 remove_copy_if()。

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

    // 移除所有年龄大于 30 的人
    auto new_end = std::remove_if(people.begin(), people.end(), [](const Person& p) {
        return p.age > 30;
    });

    // 删除范围 [new_end, people.end()) 中的元素
    people.erase(new_end, people.end());

    // 输出修改后的数组
    for (const auto& person : people) {
        std::cout << person.name << " (" << person.age << ") ";
    }
    std::cout << std::endl;

    return 0;
}
```

# std::replace
`std::replace` 是 C++ STL 中的一个算法，用于在指定范围内将所有等于某个值的元素替换为另一个值。它属于 `<algorithm>` 头文件中的标准库算法。

`std::replace` 的函数原型如下：
```cpp
template <class ForwardIt, class T>
void replace(ForwardIt first, ForwardIt last, const T& old_value, const T& new_value);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`old_value`**：要被替换的值。
- **`new_value`**：用于替换的新值。

`std::replace` 不返回任何值，它直接在指定范围内进行替换操作。

## 示例 1：替换数组中的特定值
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 2, 4, 2, 5};

    // 将所有值为 2 的元素替换为 10
    std::replace(numbers.begin(), numbers.end(), 2, 10);

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
1 10 3 10 4 10 5
```

## 示例 2：替换自定义对象中的特定值
假设有一个自定义类 `Person`，我们想将所有年龄为 30 的人替换为一个新的 `Person` 对象。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

class Person {
public:
    std::string name;
    int age;

    Person(const std::string& n, int a) : name(n), age(a) {}

    bool operator==(const Person& other) const {
        return age == other.age;
    }
};

int main() {
    std::vector<Person> people = {
        {"Alice", 25},
        {"Bob", 30},
        {"Charlie", 35},
        {"Diana", 30}
    };

    Person new_person{"Newbie", 20};

    // 将所有年龄为 30 的人替换为 new_person
    std::replace(people.begin(), people.end(), Person{"", 30}, new_person);

    // 输出修改后的数组
    for (const auto& person : people) {
        std::cout << person.name << " (" << person.age << ") ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
Alice (25) Newbie (20) Charlie (35) Newbie (20)
```

## 注意事项
1. **迭代器类型**：
   - `std::replace` 要求范围内的迭代器是正向迭代器（`ForwardIt`），这意味着它们可以多次遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是正向迭代器。

2. **性能**：
   - `std::replace` 的时间复杂度是 O(n)，其中 n 是范围内的元素数量。
   - 它逐个检查范围内的每个元素，因此对于大型数据集，性能可能会受到影响。

3. **自定义对象**：
   - 如果要替换自定义对象，需要确保自定义对象支持 `operator==`，以便 `std::replace` 可以正确比较元素。

# std::reverse
`std::reverse` 是 C++ STL 中的一个算法，用于反转指定范围内元素的顺序。它属于 `<algorithm>` 头文件中的标准库算法。

`std::reverse` 的函数原型如下：
```cpp
template <class BidirectionalIt>
void reverse(BidirectionalIt first, BidirectionalIt last);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。

`std::reverse` 不返回任何值，它直接在指定范围内反转元素的顺序。

## 示例 1：反转数组中的元素
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {1, 2, 3, 4, 5};

    // 反转数组中的元素
    std::reverse(numbers.begin(), numbers.end());

    // 输出反转后的数组
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
5 4 3 2 1
```

## 示例 2：反转自定义对象中的元素
假设有一个自定义类 `Person`，我们想反转一个 `Person` 对象的数组。

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
        {"Bob", 30},
        {"Charlie", 35},
        {"Diana", 20}
    };

    // 反转数组中的元素
    std::reverse(people.begin(), people.end());

    // 输出反转后的数组
    for (const auto& person : people) {
        std::cout << person << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
Diana (20) Charlie (35) Bob (30) Alice (25)
```

## 注意事项
1. **迭代器类型**：
   - `std::reverse` 要求范围内的迭代器是双向迭代器（`BidirectionalIt`），这意味着它们可以双向遍历范围。
   - 例如，`std::vector::iterator` 和 `std::list::iterator` 都是双向迭代器。

2. **性能**：
   - `std::reverse` 的时间复杂度是 O(n)，其中 n 是范围内的元素数量。
   - 它逐个交换范围内的每个元素，因此对于大型数据集，性能可能会受到影响。

3. **自定义对象**：
   - 如果要反转自定义对象，需要确保自定义对象支持拷贝或移动操作。


# std::transform   —— 映射转换  
> [std::transform - cppreference.com](https://en.cppreference.com/w/cpp/algorithm/transform.html) 

`std::transform` 是 `<algorithm>` 头文件里的“映射”算法：  
把「一段区间」里的每个元素，经过某个可调用对象处理后，写到「另一段区间」。  
它有两种重载：一元版本（1 个输入序列）和二元版本（2 个输入序列）。  

## 一元版本（Unary Transform）

```cpp
template< class InputIt, class OutputIt, class UnaryOp >
OutputIt transform( InputIt first1, InputIt last1,
                    OutputIt d_first, UnaryOp unary_op );
```

最后一个参数仿函数 unary_op 作用于 [first, last] 中的每一个元素身上，并以其结果产生一个新序列。

1. 形参  
   | 名字            | 作用                                                      |
   | --------------- | --------------------------------------------------------- |
   | `first1, last1` | 输入区间 `[first1, last1)`                                |
   | `d_first`       | **目标区间的起始迭代器**（可以 = `first1`，实现就地修改） |
   | `unary_op`      | 一元可调用对象：`auto r = unary_op(*it);`                 |

2. 返回值  
   `OutputIt`：指向目标区间「最后一个写入元素的下一位置」。

3. 复杂度  
   线性 `O(last1 - first1)`，对每个元素恰好调用一次 `unary_op`。

```cpp
std::vector<int> v{1,2,3,4};
std::transform(v.begin(), v.end(), v.begin(),
               [](int x){ return x * x; });
// v == {1,4,9,16}
```

```cpp
std::string s = "hello";
std::string upper;
std::transform(s.begin(), s.end(), std::back_inserter(upper),
               [](unsigned char c){ return std::toupper(c); });
// upper == "HELLO"
```

## 二元版本（Binary Transform）

```cpp
template< class InputIt1, class InputIt2,
          class OutputIt, class BinaryOp >
OutputIt transform(
    InputIt1  first1,   // ① 第一组数据起点
    InputIt1  last1,    // ② 第一组数据终点（不含）
    InputIt2  first2,   // ③ 第二组数据起点
    OutputIt  d_first,  // ④ 结果写到哪里
    BinaryOp  binary_op // ⑤ 二元函数 f(a,b)
);
```

最后一个参数仿函数 binary_op 作用于一双元素身上，其中一个元素来自[first1, last] ，另一个元素来自从 first2 开始的序列，并以其结果产生一个新序列。

1. `first1` / `last1`  
   描述**第一组数据** `[first1, last1)` 区间长度。  
   例子：`std::vector<int> a = {1,2,3};`  
   那么 `first1 = a.begin(); last1 = a.end();`

2. `first2`  
   **第二组数据**的起点，**没有 last2**！  
   算法假设第二组数据**至少跟第一组一样长**。  
   例子：`std::vector<int> b = {10,20,30};`  
   那么 `first2 = b.begin();`

3. `d_first`  
   结果写到哪里。  
   可以：  
   - 预先分配好空间的另一个容器  
   - 就地写回（覆盖第一组或第二组）  
   - `std::back_inserter` 自动增长

4. `binary_op`  
   一个可调用对象，接受两个参数，返回结果。  
   可以是：  
   - lambda `[](int x, int y){ return x + y; }`  
   - 普通函数 `int add(int x, int y){ return x + y; }`  
   - 函数对象 `std::plus<int>{}`  
   - 带状态的仿函数

- **返回值**：指向“写完最后一个元素”的**下一个位置**。

## 算法内部伪代码

```cpp
while (first1 != last1) {
    *d_first = binary_op(*first1, *first2);  // 计算并写入
    ++first1;  // 第一组指针右移
    ++first2;  // 第二组指针右移
    ++d_first; // 结果指针右移
}
return d_first;
```

- **循环次数 = 第一组区间元素个数**。  
- 第二组只给起点，算法自己 `++first2` 向后走，所以第二组必须足够长！

## 示例

```cpp
#include <algorithm>   // transform
#include <iostream>
#include <vector>
#include <iterator>    // back_inserter

int main() {
    // 1. 准备两组数据
    std::vector<int> a = {1, 2, 3};
    std::vector<int> b = {10, 20, 30};

    // 2. 结果容器，预先分配空间
    std::vector<int> c(a.size());      // 长度必须 >= a.size()

    // 3. 调用二元 transform
    auto dest_end = std::transform(
        a.begin(), a.end(),            // 第一组区间 [begin, end)
        b.begin(),                     // 第二组起点
        c.begin(),                     // 结果起点
        [](int x, int y) {             // 二元 lambda：逐元素相加
            return x + y;
        });

    // 4. 打印结果
    std::cout << "c = ";
    for (int n : c) std::cout << n << ' ';   // 11 22 33
    std::cout << "\n返回的 dest_end 指向索引 " << (dest_end - c.begin()) << '\n';
}
```

运行结果：
```
c = 11 22 33 
返回的 dest_end 指向索引 3
```

逐字符解释：
- `a.begin(), a.end()` 告诉算法：要走 3 步。  
- `b.begin()` 是第二组起点，算法自己会 `b.begin()+1`、`+2`…  
- `c.begin()` 是目的地，第 0 位放 `1+10`，第 1 位放 `2+20`…  
- 返回值 `dest_end` 指向 `c.begin()+3`，即最后一个写完的元素的下一个位置。

## 注意

1. **为什么第二组没有 last2？**  
   算法只按第一组长度循环，第二组默认“足够长”。如果给短了，就是未定义行为。

2. **长度不一样怎么办？**  
   必须保证第二组长度 ≥ 第一组，或只取共同长度：
   ```cpp
   auto min_len = std::min(a.size(), b.size());
   std::transform(a.begin(), a.begin() + min_len,
                  b.begin(),
                  std::back_inserter(result), op);
   ```

3. **可以就地写回吗？**  
   可以，只要源和目的不重叠，或重叠但向前搬移安全：
   ```cpp
   std::transform(a.begin(), a.end(),   // 读
                  b.begin(),            // 读
                  a.begin(),            // 就地写回 a
                  [](int x, int y){ return x + y; });
   ```

4. **用普通函数或函数对象也行？**  
   ```cpp
   int add(int x, int y) { return x + y; }
   std::transform(a.begin(), a.end(), b.begin(), c.begin(), add);

   std::transform(a.begin(), a.end(), b.begin(), c.begin(),
                  std::plus<int>{});   // <functional> 里的标准加法器
   ```

   ```cpp
   int negate_fn(int x) { return -x; }
   struct Times {
内部做的事  
```cpp
for (; first != last; ++first)
    f(*first);
return f;   // 把函数对象再返回来（可以忽略）
```

返回值  
- 传入的函数对象本身（可以带状态，但多数场景忽略）。

       int factor;
       int operator()(int x) const { return x * factor; }
   };

   std::vector<int> v{1,2,3}, out(v.size());
   std::transform(v.begin(), v.end(), out.begin(), negate_fn);      // 函数指针
   std::transform(out.begin(), out.end(), out.begin(), Times{5});   // 仿函数
   ```

# std::sort
`std::sort` 是 C++ STL 中的一个非常强大的算法，用于对指定范围内的元素进行排序。它属于 `<algorithm>` 头文件中的标准库算法。`std::sort` 使用高效的排序算法（通常是快速排序或introsort）来对元素进行排序。

`std::sort` 的函数原型如下：
```cpp
template <class RandomIt>
void sort(RandomIt first, RandomIt last);

template <class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare comp);
```

- **`first`**：范围的起始迭代器。
- **`last`**：范围的结束迭代器。
- **`comp`**（可选）：比较函数，用于定义排序的顺序。默认情况下，使用 `operator<`。

`std::sort` 不返回任何值，它直接在指定范围内对元素进行排序。

## 示例 1：对数组进行排序
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    // 对数组进行排序
    std::sort(numbers.begin(), numbers.end());

    // 输出排序后的数组
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
1 1 2 3 3 4 5 5 6 9
```

## 示例 2：使用自定义比较函数
假设想按降序对数组进行排序。

```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::vector<int> numbers = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};

    // 使用自定义比较函数按降序排序
    std::sort(numbers.begin(), numbers.end(), [](int a, int b) {
        return a > b;
    });

    // 输出排序后的数组
    for (int num : numbers) {
        std::cout << num << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
9 6 5 5 4 3 3 2 1 1
```

## 示例 3：对自定义对象进行排序
假设有一个自定义类 `Person`，我们想根据年龄对 `Person` 对象的数组进行排序。

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
        {"Bob", 30},
        {"Charlie", 35},
        {"Diana", 20}
    };

    // 根据年龄对 Person 对象的数组进行排序
    std::sort(people.begin(), people.end(), [](const Person& a, const Person& b) {
        return a.age < b.age;
    });

    // 输出排序后的数组
    for (const auto& person : people) {
        std::cout << person << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

输出结果
```
Diana (20) Alice (25) Bob (30) Charlie (35)
```

## 注意事项
1. **迭代器类型**：
   - `std::sort` 要求范围内的迭代器是随机访问迭代器（`RandomIt`），这意味着它们可以随机访问范围内的任何元素。
   - 例如，`std::vector::iterator` 和 `std::deque::iterator` 都是随机访问迭代器。

2. **性能**：
   - `std::sort` 的时间复杂度是 O(n log n)，其中 n 是范围内的元素数量。
   - 它使用高效的排序算法（通常是快速排序或 introsort），因此对于大型数据集，性能非常高效。

3. **自定义比较函数**：
   - 比较函数可以是普通函数、lambda 表达式或函数对象，只要它接受两个参数并返回一个布尔值即可。
   - 例如，`[](int a, int b) { return a > b; }` 是一个 lambda 表达式。

4. **稳定性**：
   - `std::sort` 不保证排序的稳定性，即相等的元素可能不会保持原始顺序。
   - 如果需要稳定的排序，可以使用 `std::stable_sort`。
