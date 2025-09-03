C++ 学习笔记

# string 

1.  **本质**： `std::string` 是 C++ 标准库中定义的一个类，是模板类 `std::basic_string<char>` 的别名（typedef）。它专门用于处理和管理**字节字符串**（`char` 类型的序列）。
2.  **地位**： 虽然它不属于 STL 的“六大组件”之列，但它**完全具备 STL 容器的所有特性**（支持迭代器、拥有 `begin()`, `end()`, `size()` 等方法），可以看作一个专门为字符优化的“特化版 `vector<char>`”。
3.  **核心优势**： 它自动管理内存，无需担心分配和释放；提供了极其丰富和便捷的成员函数来处理字符串；是 C++ 中替代 C 风格字符串（`char*` 或 `char[]`）的首选。

## 创建和初始化 string 对象

```cpp
#include <iostream>
#include <string> // 必须包含的头文件

int main() {
    // 1. 默认初始化：空字符串
    std::string s1;

    // 2. 用 C风格字符串 初始化
    std::string s2 = "Hello, World!";
    std::string s3("Hello, C++");

    // 3. 用另一个 string 对象初始化（拷贝构造）
    std::string s4 = s2;
    std::string s5(s2);

    // 4. 用多个相同字符初始化
    std::string s6(5, 'A'); // s6 的内容是 "AAAAA"

    // 5. 用子串初始化：从另一个string的指定位置开始，取指定长度的字符
    std::string s7(s2, 7, 5); // 从s2的第7个字符开始，取5个字符。s7 = "World"

    // 6. 用移动语义初始化 (C++11) - 高效，s2的资源被“移动”给s8，s2变为未定义状态
    std::string s8 = std::move(s2);

    // 7. 用初始化列表初始化 (C++11)
    std::string s9 = {'H', 'i'}; // s9 = "Hi"

    return 0;
}
```

## 容量相关 (Capacity)

| 函数                        | 作用                                                          | 示例                      |
| :-------------------------- | :------------------------------------------------------------ | :------------------------ |
| `s.empty()`                 | 判断字符串是否为空                                            | `if (s.empty()) {...}`    |
| `s.size()` / `s.length()`   | 返回字符串的**字符个数**（长度）                              | `int len = s.size();`     |
| `s.capacity()`              | 返回当前为字符串分配的内存大小（字节数）                      | `int cap = s.capacity();` |
| `s.reserve(n)`              | **请求**改变容量，为大量字符预留空间，避免频繁重新分配        | `s.reserve(1000);`        |
| `s.shrink_to_fit()` (C++11) | **请求**释放未使用的内存，减少 `capacity()` 至 `size()`       | `s.shrink_to_fit();`      |
| `s.clear()`                 | 清空内容，变为空字符串（`size()`变为0，`capacity()`不一定变） | `s.clear();`              |

## 元素访问 (Element Access)

| 函数                          | 作用                                                                    | 注意                                   |
| :---------------------------- | :---------------------------------------------------------------------- | :------------------------------------- |
| `s[index]`                    | 访问指定位置的字符。**不检查越界**，性能高                              | `char c = s[0]; s[0] = 'h';`           |
| `s.at(index)`                 | 访问指定位置的字符。**会检查越界**，越界则抛出 `std::out_of_range` 异常 | `try { c = s.at(100); } catch(...) {}` |
| `s.front()` (C++11)           | 返回第一个字符的引用                                                    | `s.front() = 'H';`                     |
| `s.back()` (C++11)            | 返回最后一个字符的引用                                                  | `s.back() = '!';`                      |
| `s.c_str()`                   | 返回一个指向底层C风格字符串（`const char*`）的指针                      | `printf("%s", s.c_str());`             |
| `s.data()` (C++17 前为 const) | 返回指向底层字符数组的指针（在 C++17 后，`s.data()` 可写）              | `std::cout << s.data();`               |

## 字符串修改 (Modifiers)

| 函数                                     | 作用                                              | 示例                                |
| :--------------------------------------- | :------------------------------------------------ | :---------------------------------- |
| `s1 += s2` / `s1 += "str"` / `s1 += 'c'` | **追加** 另一个string、C字符串或字符              | `s1 += " append";`                  |
| `s1.append(args)`                        | 功能同 `+=`，但参数形式更丰富（同构造函数）       | `s1.append(3, '!'); // s1 += "!!!"` |
| `s.push_back(c)`                         | 在末尾追加一个字符                                | `s.push_back('!');`                 |
| `s.insert(pos, args)`                    | 在 `pos` 位置**插入** 另一个string、C字符串等     | `s.insert(0, "Prefix ");`           |
| `s.erase(pos, count)`                    | 从 `pos` 位置开始**删除** `count` 个字符          | `s.erase(5, 2);`                    |
| `s.replace(pos, count, args)`            | **替换** 从 `pos` 开始的 `count` 个字符为新的内容 | `s.replace(7, 5, "China");`         |
| `s.pop_back()` (C++11)                   | 删除最后一个字符                                  | `s.pop_back();`                     |
| `s1.swap(s2)` / `std::swap(s1, s2)`      | 交换两个 string 对象的内容                        | `s1.swap(s2);`                      |

### replace

`std::string::replace()` 是 C++ 标准库中 `std::string` 类的成员函数，用于替换字符串中的部分内容。它有多种重载形式，提供了灵活的替换功能。

`std::string::replace()` 有多种重载形式，主要包括：

```cpp
// 基本形式：替换指定位置的字符
string& replace(size_type pos, size_type count, const string& str);

// 使用子字符串进行替换
string& replace(size_type pos, size_type count, const string& str,
                size_type pos2, size_type count2 = npos);

// 使用C风格字符串进行替换
string& replace(size_type pos, size_type count, const char* s, size_type count2);

// 使用多个相同字符进行替换
string& replace(size_type pos, size_type count, size_type count2, char c);

// 使用迭代器范围进行替换
template<class InputIt>
string& replace(const_iterator first, const_iterator last, InputIt first2, InputIt last2);
```

- `pos`: 要替换的起始位置
- `count`: 要替换的字符数量
- `str`: 用于替换的字符串
- `pos2`: 替换字符串中的起始位置（当使用子字符串时）
- `count2`: 替换字符串中使用的字符数量
- `s`: C风格字符串（以null结尾）
- `c`: 用于替换的字符
- `first`, `last`: 要替换的字符范围的迭代器
- `first2`, `last2`: 用于替换的字符范围的迭代器

#### 示例 1: 基本替换

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello World";
    
    // 替换从位置6开始的5个字符为"There"
    str.replace(6, 5, "There");
    std::cout << str << std::endl; // 输出: Hello There
    
    return 0;
}
```

#### 示例 2: 使用子字符串进行替换

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "I like apples";
    std::string replacement = "oranges and bananas";
    
    // 替换从位置7开始的6个字符为replacement的前7个字符
    str.replace(7, 6, replacement, 0, 7);
    std::cout << str << std::endl; // 输出: I like oranges
    
    return 0;
}
```

#### 示例 3: 使用C风格字符串

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "C++ is difficult";
    
    // 替换从位置4开始的10个字符为"is fun"的前6个字符
    str.replace(4, 10, "is fun", 6);
    std::cout << str << std::endl; // 输出: C++ is fun
    
    return 0;
}
```

#### 示例 4: 使用多个相同字符替换

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Password: 12345";
    
    // 替换从位置9开始的5个字符为5个'*'
    str.replace(9, 5, 5, '*');
    std::cout << str << std::endl; // 输出: Password: *****
    
    return 0;
}
```

#### 示例 5: 使用迭代器范围

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "The quick brown fox";
    std::string new_text = "red dog";
    
    // 替换从位置10开始的5个字符为new_text的全部内容
    str.replace(str.begin() + 10, str.begin() + 15, 
                new_text.begin(), new_text.end());
    std::cout << str << std::endl; // 输出: The quick red dog fox
    
    return 0;
}
```

#### 示例 6: 替换并扩展字符串

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello";
    
    // 替换从位置2开始的2个字符为"lp"（比原内容长）
    str.replace(2, 2, "lp");
    std::cout << str << std::endl; // 输出: Help
    
    // 替换从位置1开始的1个字符为"appy birth day"（比原内容长很多）
    str.replace(1, 1, "appy birth day");
    std::cout << str << std::endl; // 输出: Happy birth day
    
    return 0;
}
```

#### 示例 7: 替换并缩短字符串

```cpp
#include <iostream>
#include <string>

int main() {
    std::string str = "Hello World";
    
    // 替换从位置5开始的6个字符为""（空字符串，相当于删除）
    str.replace(5, 6, "");
    std::cout << str << std::endl; // 输出: Hello
    
    return 0;
}
```

#### 注意事项

1. **边界检查**：
   ```cpp
   std::string str = "Hello";
   // 这会抛出std::out_of_range异常，因为位置10不存在
   // str.replace(10, 2, "World");
   ```

2. **性能考虑**：替换操作可能导致内存重新分配，特别是当替换的内容比原内容长很多时。

3. **Unicode 支持**：对于多字节字符（如UTF-8），需要特别注意字符位置和长度的计算。

4. **返回值**：所有 `replace()` 方法都返回 `*this` 的引用，允许链式调用：
   ```cpp
   std::string str = "abc";
   str.replace(0, 1, "x").replace(2, 1, "z");
   std::cout << str << std::endl; // 输出: xbz
   ```

## 字符串操作 (String Operations)

| 函数                                                | 作用                                                                    | 示例                                                   |
| :-------------------------------------------------- | :---------------------------------------------------------------------- | :----------------------------------------------------- |
| `s1.compare(s2)`                                    | 比较两个字符串。返回 `0`(相等), `负数`(s1 < s2), `正数`(s1 > s2)        | `if (s1.compare(s2) == 0) {...}`                       |
| `s.substr(pos, count)`                              | **返回一个子串**，从 `pos` 开始，取 `count` 个字符                      | `std::string sub = s.substr(0, 5);`                    |
| `s.find(args)`                                      | **查找** 子串或字符**第一次**出现的位置，找不到返回 `std::string::npos` | `size_t pos = s.find("world");`                        |
| `s.rfind(args)`                                     | 查找子串或字符**最后一次**出现的位置                                    | `pos = s.rfind('o');`                                  |
| `s.find_first_of(args)`                             | 查找**任何一个**给定字符**第一次**出现的位置                            | `pos = s.find_first_of("aeiou"); // 找元音`            |
| `s.find_first_not_of(args)`                         | 查找**第一个不在**给定字符集中的位置                                    | `pos = s.find_first_not_of("0123456789"); // 找非数字` |
| `s.find_last_of(args)` / `s.find_last_not_of(args)` | 类似上面，但是从后往前找                                                |                                                        |

### compare

`std::string::compare()` 是 C++ 标准库中用于比较两个字符串的方法，它提供了多种重载形式来满足不同的比较需求。

`compare()` 方法返回一个整数值：
- **返回 0**：两个字符串相等
- **返回负数**：调用字符串小于参数字符串
- **返回正数**：调用字符串大于参数字符串

#### 比较整个字符串

```cpp
int compare(const string& str) const noexcept;
```

**示例：**
```cpp
#include <iostream>
#include <string>

int main() {
    std::string str1 = "apple";
    std::string str2 = "banana";
    std::string str3 = "apple";
    
    int result1 = str1.compare(str2); // 负数 (apple < banana)
    int result2 = str1.compare(str3); // 0 (apple == apple)
    int result3 = str2.compare(str1); // 正数 (banana > apple)
    
    std::cout << "str1 vs str2: " << result1 << std::endl;
    std::cout << "str1 vs str3: " << result2 << std::endl;
    std::cout << "str2 vs str1: " << result3 << std::endl;
    
    return 0;
}
```

#### 比较子字符串

```cpp
int compare(size_type pos, size_type count, const string& str) const;
```

```cpp
std::string str = "hello world";
std::string sub = "hello";

// 比较 str 的前5个字符与 "hello"
int result = str.compare(0, 5, sub); // 返回 0 (相等)
```

#### 比较字符串的子串与另一个字符串的子串

```cpp
int compare(size_type pos1, size_type count1, 
           const string& str, 
           size_type pos2, size_type count2) const;
```

```cpp
std::string str1 = "hello world";
std::string str2 = "world hello";

// 比较 str1 的 "world" 与 str2 的 "world"
int result = str1.compare(6, 5, str2, 0, 5); // 返回 0 (相等)
```

## 与非成员函数

| 函数                                                             | 作用                                                                                                         |
| :--------------------------------------------------------------- | :----------------------------------------------------------------------------------------------------------- |
| `std::getline(std::cin, s)` / `std::getline(std::cin, s, delim)` | 从输入流（如命令行）中读取**一行**文本（默认以`\n`分隔），存入 `s`。这是读取带空格字符串的**唯一正确方式**。 |
| `std::stoi(s)`, `std::stol(s)`, `std::stod(s)` 等                | 将字符串转换为数字（`int`, `long`, `double` 等）                                                             |
| `std::to_string(value)`                                          | 将数字（`int`, `double` 等）转换为 `std::string`                                                             |
| `operator<<` / `operator>>`                                      | 用于标准输入输出流                                                                                           |

## 重要特性与最佳实践

1.  **自动内存管理**： `string` 内部动态分配内存，`size()` 和 `capacity()` 不同。当 `size() == capacity()` 时再添加字符，会触发重新分配（可能导致**迭代器失效**）。
2.  **`c_str()` 的使用场景**： 当你需要与接收 `const char*` 的 C 函数或旧 API 交互时（如 `fopen()`, `printf()`）。
3.  **`find` 系列函数的返回值**： 它们的返回值类型是 `size_type`（通常是 `size_t`）。如果找不到，会返回一个特殊常量 `std::string::npos`。**判断是否找到一定要用这个常量**：
    ```cpp
    if (s.find("key") != std::string::npos) {
        std::cout << "Found!";
    }
    ```
4.  **`getline` 读取输入**： 使用 `cin >> s` 会在遇到空格时停止，而 `getline(cin, s)` 会读取整行，包括空格。
5.  **现代C++：移动语义与 `noexcept`**： C++11 后，`string` 的移动构造函数和移动赋值运算符通常是 `noexcept` 的，这意味着在容器（如 `vector<string>`）中重新分配时，会使用高效的移动操作而非拷贝，性能极大提升。

## 示例

```cpp
#include <iostream>
#include <string>

int main() {
    // 读取一整行
    std::string line;
    std::cout << "Enter a line: ";
    std::getline(std::cin, line);
    std::cout << "You entered: " << line << std::endl;

    // 查找和替换
    size_t pos = line.find("stupid");
    if (pos != std::string::npos) {
        line.replace(pos, 5, "smart"); // 替换不雅词汇
        std::cout << "Filtered: " << line << std::endl;
    }

    // 获取文件扩展名
    std::string filename = "document.txt";
    size_t dot_pos = filename.rfind('.'); // 从后往前找最后一个点
    if (dot_pos != std::string::npos) {
        std::string ext = filename.substr(dot_pos + 1); // 从点后开始到结尾
        std::cout << "File extension is: " << ext << std::endl; // 输出 "txt"
    }

    // 数字字符串转换
    std::string num_str = "3.14159";
    double pi = std::stod(num_str);
    std::cout << "Pi * 2 = " << pi * 2 << std::endl;

    return 0;
}
```

