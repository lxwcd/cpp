STL 常用算法

--------------------------------------------------
- **迭代器区间** `[first, last)`：包含 `first`，不包含 `last`。  
- **谓词 (Predicate)**：一个可调用对象，形参是元素，返回 `bool`。  
- **函数对象 (Unary/Nullary/BinaryOp)**：一个可调用对象，用来“读/改/算”元素。
--------------------------------------------------

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

# remove

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

# vector erase

```cpp
// ① 区间删除
iterator erase( iterator first, iterator last );

// ② 单元素删除
iterator erase( iterator pos );
```

--------------------------------------------------
| 参数名  | 类型       | 含义                           |
| ------- | ---------- | ------------------------------ |
| `first` | `iterator` | **待删除区间的起始**（包含）   |
| `last`  | `iterator` | **待删除区间的尾后**（不包含） |

- `[first, last)` 必须满足：`first` 和 `last` 指向同一个容器，且 `first <= last`。
- 对 vector/string/deque 来说，必须是 **随机访问迭代器**（vector::iterator 就是）。

## 返回值
- **iterator**：指向下列两种情况之一  
  1. 如果删除区间后容器仍非空 → 指向 **被删除区间之后第一个元素**。  
  2. 如果删除区间后容器为空 → 返回 `container.end()`。

## 复杂度 & 迭代器失效规则
- **vector / string / deque**  
  - 复杂度：`O(N)`，因为要搬移后续元素填补空洞。  
  - 迭代器失效：所有指向 `[first, end())` 的迭代器、引用、指针全部失效。  
- **list / forward_list**  
  - 复杂度：`O(distance(first,last))`（链表只需改指针）。  
  - 迭代器失效：仅被删除的那些迭代器失效，其余仍有效。

## 底层实现伪代码（vector 区间 erase）
```cpp
template<class T, class Alloc>
typename vector<T,Alloc>::iterator
vector<T,Alloc>::erase(iterator first, iterator last)
{
    size_type n_erase = last - first;           // 要删多少个
    if (n_erase == 0) return first;             // 空区间直接回

    iterator new_end = std::move(last, end(), first); // 把 [last,end) 搬移到 first
    destroy(new_end, end());                       // 析构多余元素
    finish_ = new_end;                             // 更新内部尾后指针
    return first;                                  // 返回新区间起点
}
```
要点：  
1. `std::move(last, end(), first)` 把 `[last,end)` 整体搬到 `first` 处，覆盖被删元素。  
2. 析构尾部多余元素。  
3. 返回 `first` 正是删除区间之后第一个有效元素。

## 单元素 erase 与区间 erase 关系
```cpp
iterator erase(iterator pos)
{
    return erase(pos, pos + 1);   // 直接复用区间版本
}
```

## 完整示例（vector）
```cpp
#include <iostream>
#include <vector>
#include <algorithm>

int main()
{
    std::vector<int> v = {0,1,2,3,4,5,6,7,8,9};

    // ① 删除区间 [3,6)  -> 3,4,5 被删
    auto it = v.erase(v.begin() + 3, v.begin() + 6);

    // ② 此时 v = {0,1,2,6,7,8,9}, it 指向 6
    std::cout << "after erase [3,6): ";
    for (int x : v) std::cout << x << ' ';
    std::cout << "\nit points to " << *it << '\n';

    // ③ 删除单个元素 8
    it = v.erase(v.begin() + 5);   // 8 在索引 5
    std::cout << "after erase 8: ";
    for (int x : v) std::cout << x << ' ';
    std::cout << "\nit points to " << (it == v.end() ? "end" : std::to_string(*it)) << '\n';
}
```

输出：
```
after erase [3,6): 0 1 2 6 7 8 9 
it points to 6
after erase 8: 0 1 2 6 7 9 
it points to end
```

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


# std::for_each   —— 遍历
> [std::for_each - cppreference.com](https://en.cppreference.com/w/cpp/algorithm/for_each.html) 

把一段区间里的每个元素都喂给某个“动作”，动作可以是函数、lambda、函数对象。  

```cpp
template< class InputIt, class UnaryFunction >
UnaryFunction for_each( InputIt first, InputIt last, UnaryFunction f );
```

| 参数名  | 含义                                           |
| ------- | ---------------------------------------------- |
| `first` | 区间起始迭代器（含）                           |
| `last`  | 区间尾后迭代器（不含）                         |
| `f`     | 一元可调用对象，形参是元素类型，**可修改元素** |

- 返回 **传入的那个可调用对象本身**（可带状态）。  
- 多数场景直接忽略，只有在 `f` 内部需要累积结果时才用。
- 复杂度：线性 `O(last - first)`，对每个元素调用一次 `f`。  
- 迭代器失效：  
  – 序列容器（vector/string/deque）：`f` **不能** 增删元素，否则迭代器全部报废。  
  – 链表/关联容器：可在 `f` 里删除当前迭代器，但要用 `erase(it++)` 技巧。

返回值  
- 传入的函数对象本身（可以带状态，但多数场景忽略）。

## 内s实现 
```cpp
for (; first != last; ++first)
    f(*first);
return f;   // 把函数对象再返回来（可以忽略）
```

## 常见用法

1. 只读打印
```cpp
std::for_each(v.begin(), v.end(),
              [](int x){ std::cout << x << ' '; });
```

2. 就地修改
```cpp
std::for_each(v.begin(), v.end(),
              [](int& x){ x *= 2; });   // 元素乘 2
```

3. 累积求和（用返回值）
```cpp
struct Sum {
    long long value = 0;
    void operator()(int x) { value += x; }
} s = std::for_each(v.begin(), v.end(), Sum{});
std::cout << s.value;   // 拿到累加结果
```

4. 成员函数做动作
```cpp
struct Widget { void draw() { /*...*/ } };
std::vector<Widget> ws;
std::for_each(ws.begin(), ws.end(),
              std::mem_fn(&Widget::draw));
```

5. 普通函数指针
```cpp
void print(int x){ std::cout << x; }
std::for_each(v.begin(), v.end(), print);
```

## 完整示例

```cpp
#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>   // accumulate 对比用

int main() {
    std::vector<int> v = {1, 2, 3, 4};

    // 1. 打印
    std::cout << "原始: ";
    std::for_each(v.begin(), v.end(), [](int x){ std::cout << x << ' '; });
    std::cout << '\n';

    // 2. 就地 +10
    std::for_each(v.begin(), v.end(), [](int& x){ x += 10; });

    // 3. 累积乘积（用返回值）
    struct Product {
        long long p = 1;
        void operator()(int x){ p *= x; }
    } prod = std::for_each(v.begin(), v.end(), Product{});
    std::cout << "乘积 = " << prod.p << '\n';
}
```

输出：
```
原始: 1 2 3 4 
乘积 = 14641
```

## 注意

| 坑           | 说明                                                    |
| ------------ | ------------------------------------------------------- |
| **增删元素** | 在 vector 里 `push_back`/`erase` 会导致迭代器失效，UB。 |
| **捕获引用** | 异步线程中 lambda 引用捕获局部变量，可能悬垂。          |
| **元素只读** | 若 lambda 参数写成 `int x`（值传递），无法修改原元素。  |


# 示例1
原始 vector： `[1 2 3 4 5 6 7 8 9 10]`

```cpp
// 步骤 1：remove_if 逻辑删除奇数
auto new_end = remove_if(begin, end, [](int n){ return n%2!=0; });
// 结果： [2 4 6 8 10 | x x x x x]
//                        ↑ new_end

// 步骤 2：erase 真删尾部垃圾
numbers.erase(new_end, numbers.end());
// 结果： [2 4 6 8 10]  size==5

// 步骤 3：transform 就地乘 2
transform(begin, end, begin,
          [](int n){ return n*2; });
// 结果： [4 8 12 16 20]

// 步骤 4：for_each 打印
for_each(begin, end,
         [](int n){ cout << n << ' '; });
// 输出：4 8 12 16 20
```

