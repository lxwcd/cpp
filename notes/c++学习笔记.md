C++ 学习笔记

# 资源
## 书籍
> C++ Primer

## 视频
> [侯捷 c++ 视频](https://pan.baidu.com/s/1WR3y_Qfw7CssYlEC2tmISQ?pwd=jj2w)

## 网站
> [cplusplus](https://cplusplus.com/doc/tutorial/)
> [cppreference](https://en.cppreference.com/w/)

# C++ 基础语法

## 从源代码变成可执行文件的过程
在C++中，程序从源代码转变为可执行文件的过程涉及多个步骤，这些步骤通常由集成的开发环境(IDE)或者命令行工具自动完成。下面是这一编译过程的基本步骤：

1. **预处理（Preprocessing）**：
   - 在编译代码之前，预处理器采取源代码文件，执行以井号(#)开始的预处理指令。这包括处理宏定义、条件编译指令，以及包含头文件等操作。预处理器还会移除所有的注释。
   - 预处理完成后，生成一个预处理过的源代码文件，其中包含了所有通过#include指令包含进来的文件内容，宏展开的结果等。

2. **编译（Compilation）**：
   - 将预处理过的源代码转换成汇编语言。这个过程由编译器完成，它分析代码并生成与平台相关的汇编代码。
   - 在编译过程中，编译器也会检查语法错误，并可能对代码进行优化。

3. **汇编（Assembly）**：
   - 将编译器生成的汇编代码转换为机器语言指令，生成所谓的对象文件。这个过程由汇编器完成。

4. **链接（Linking）**：
   - 链接器将一个或多个对象文件以及所有需要的库文件链接在一起生成最终的可执行文件。
   - 在这个过程中，链接器解析并合并多个对象文件及库之间的符号引用，地址计算等，并生成可执行程序。
   - 有静态链接，它会把所有代码和库合并成一个文件；也有动态链接，它在运行时加载外部库文件。

5. **加载（Loading）**（通常不包括在编译过程，但为了完整理解包含在内）：
   - 当你运行一个程序时，加载步骤涉及操作系统将可执行文件加载到内存中，并开始执行程序。

这些步骤涉及到各种不同的文件类型和程序，例如源文件(`.cpp`), 头文件(`.h`), 对象文件(`.o` 或 `.obj`), 可执行文件(`.exe` 或在Unix系统中无拓展名)等。

这个过程不同系统和编译器可能会有所不同，但大体流程是一致的。同时现代的编译器可能会在这个过程中做很多优化操作。

### 预处理
C++编译过程中的预处理阶段涉及几个关键操作，预处理器会对源代码文件进行一系列文本替换和宏展开，准备好输入供编译器进一步处理。这些操作主要包括：

1. **宏定义的扩展**：
   - 将所有的宏定义(`#define`)扩展到相应的宏调用位置。宏可以是简单的文本替换或带有参数的复杂表达式。

2. **条件编译指令的处理**：
   - 根据预设的条件决定代码块是否编译，使用的指令包括`#if`, `#ifdef`, `#ifndef`, `#else`, `#elif`, 和`#endif`。

3. **文件包含指令的处理**：
   - 处理`#include`指令，将指定的文件内容插入到当前位置。标准库头文件通常用尖括号`< >`包围，而用户定义的头文件通常用双引号`" "`包围。

4. **预定义宏的替换**：
   - 一些宏是由编译器预定义的，如`__LINE__`（当前行号），`__FILE__`（当前文件名），`__DATE__`（编译日期），`__TIME__`（编译时间）等。

5. **注释的移除**：
   - 移除源代码中的所有注释，包括单行注释（`//`）和多行注释（`/* ... */`）。

6. **生成行控制信息**：
   - 预处理器会添加特殊的行控制标识，以帮助编译器追踪行号和文件名，这对于错误调试十分重要。

## translation phase
> [Phases of translation](https://en.cppreference.com/w/cpp/language/translation_phases#Phase_4)

C++ 的翻译过程是由一系列阶段组成的，每一个阶段都将执行一个特定的操作。这些操作都是按照顺序执行的，后一阶段的输入是前一阶段的输出。翻译的阶段包括以下步骤：

1. **阶段1：字符映射（Phase 1: Character Mapping）**  
此阶段会将程序中每一个文件的源字符集，即源代码文件中的所有字符，转换为基本源字符集。例如，一些特定的Unicode字符或者转义字符会被映射到相应的字符。

2. **阶段2：行拼接（Phase 2: Line Splicing）**  
在这个阶段，预处理器将把程序中所有以反斜杠（\）结束的行与下一行合并，生成物理行。

3. **阶段3：记号生成（Phase 3: Tokenization）**  
源文件会被分解为预处理记号和空白符序列，这包括空格、换行符、制表符等。在这个阶段，注释也会被替换为一个空格。

4. **阶段4：预处理（Phase 4: Preprocessing）**  
在这个阶段，预处理器将执行所有预处理指令，即以 `#` 开头的指令。例如，处理 `#include` 和 `#define` 等预处理指令。# 

5. **阶段5：字符串字面量与字符字面量的连接（Phase 5: String and Character Literals Concatenation）**  
在这个阶段，相邻的字符串字面量会被连接为单个字符串，并处理所有的转义序列。

6. **阶段6：翻译（Phase 6: Translation）**  
在这个阶段，源代码会被翻译成目标代码。如果有模板实例，它会在此阶段进行。

7. **阶段7：实例化（Phase 7: Instantiation）**  
对于包含模板的程序，它会在这个阶段根据具体的模板参数创建模板的实例。

8. **阶段8：链接（Phase 8: Linking）**  
编译器将所有的翻译单元，包括源代码文件和包含在程序中的所有头文件，以及链接到程序中的库，都链接在一起生成最终的可执行文件。

这些阶段构成了C++程序从源代码到最终可执行文件的统一过程。虽然在实践中，这些阶段可能会在一些复杂的方式下执行（例如，模板实例化可能会在编译或链接阶段进行），但是这个模型为C++的编译模型提供了一个概念框架。

"Phases of Translation"（翻译阶段）和从编译到连接的几个过程（预处理、编译、汇编、链接）虽然描述的都是C++源代码转换为可执行文件的过程，但它们所着重的层面和细节有所不同。

"Phases of Translation"更多地描述的是C++源代码从文本文件到可执行文件的转换过程中的逻辑层面和抽象过程。根据C++标准，这个过程被定义为8个阶段，它们详细描述了源代码是如何被处理和转换的。这些阶段包括字符映射、行拼接、记号生成、预处理、字符串字面量和字符字面量连接、翻译、实例化以及链接。这个分类着重于语言标准层面的规范描述，是对编译过程的严格按照C++标准的解释。

## 翻译阶段和从编译到链接阶段的区别
- **重点不同**："Phases of Translation"侧重于语言标准和源代码处理的逻辑阶段，而从编译到连接的过程则侧重于具体实现和操作细节。
- **细节层面不同**："Phases of Translation"提供了一个更为抽象和宏观的视角，注重于标准和规范，而编译到连接的过程更注重技术实现的细节和每个步骤的具体操作。
- **目的不同**："Phases of Translation"旨在为编译器的实现提供标准化的模板，而编译到连接的过程更多地被用于解释和教育，以帮助人们理解源代码是如何被转换为可执行文件的。

## 删除注释的阶段
删除注释虽然在 phase 3，而预处理在 phase 4，但因为在实际的编译器实现中，预处理的活动通常被认为是一个整体，包括宏替换、包含处理和注释去除等操作。但如果按照C++标准中定义的翻译阶段来考虑，这些活动实际是分布在几个不同的逻辑阶段。

尽管在传统的讨论和编译器的实际执行过程中，通常将移除注释视为预处理的一部分，但根据C++的翻译阶段来看，移除注释实际上是在预处理动作（第4阶段）之前，作为记号生成过程（第3阶段）的一部分完成的。

这种分区可能在理论上有其意义，以确保语言的规范性和各实现之间的一致性，但在日常使用和讨论中，往往会简单地将所有这些活动总称为预处理。

## 预处理指令
预处理指令是C++中一部分，在编译过程的最开始被处理。它们指导编译器在实际编译之前对源代码进行一些预处理。

### `#include`

这个指令告诉预处理器包含一个文件的内容。这通常用于包含库头文件或其他源文件。

```cpp
#include <iostream> // 包含标准输入输出流库
#include "myheader.h" // 包含用户定义的头文件
```

### 宏
> [Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Macros.html) 

在 C++ 中，`#define` 指令是预处理器的一部分，用于定义宏。这意味着在编译之前，预处理器会将所有宏名称替换为其定义。`#define` 可以用来定义各种类型的常量，如整型、字符、字符串等。

#### Object-like Macros
> [Object-like Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Object-like-Macros.html) 

- 整型
```cpp
#define NUM 14
```

- 浮点数
```cpp
#define PI 3.14
```
这里定义了一个名为 `PI` 的宏，其值为 `3.14`。这个宏在预处理阶段会被直接替换为 `3.14`，因此它几乎不占用任何内存空间。

- 字符
```cpp
#define NEWLINE '\n'
```
在这个例子中，定义了一个名为 `NEWLINE` 的宏，用来代表换行符。和之前一样，这个宏在编译期被替换成对应的值，内存占用微乎其微。

- 字符串
```cpp
#define GREETING "Hello, World!"
```
这里，定义了一个包含字符串的宏 `GREETING`。在编译时，所有的 `GREETING` 实例将被替换为 `"Hello, World!"`。字符串常量通常被存储在程序的只读数据段中。

#### 多行宏书写
```cpp
#define NUMBERS 1, \
                2, \
                3
int x[] = { NUMBERS };
     → int x[] = { 1, 2, 3 };
```

#### 宏定义生效的作用域
> The C preprocessor scans your program sequentially. Macro definitions take effect at the place you write them. 

```cpp
foo = X; // X is not defined yet
#define X 4
bar = X;
```

#### Function-like Macros
> [Function-like Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Function-like-Macros.html) 
> To define a function-like macro, you use the same ‘#define’ directive, but you put a pair of parentheses **immediately** after the macro name. 

```cpp
#define SQUARE(x) ((x) * (x))
```
这个宏 `SQUARE` 接受一个参数，并计算其平方。它展示了如何将宏用作函数。但是需要注意的是，宏没有作用域的概念，也不进行类型检查，因此它们在复杂表达式中可能产生意想不到的副作用。

##### 宏参数
> [Macro Arguments (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Macro-Arguments.html) 
> Leading and trailing whitespace in each argument is dropped, and all whitespace between the tokens of an argument is reduced to a single space. Parentheses within each argument must balance; a comma within such parentheses does not end the argument. 
> Whitespace is not a preprocessing token, so if a macro foo takes one argument, foo () and foo ( ) both supply it an empty argument. Previous GNU preprocessor implementations and documentation were incorrect on this point, insisting that a function-like macro that takes a single argument be passed a space if an empty argument was required.

宏参数中的空白字符会被忽略。

#### 多层嵌套宏的扩展
> [Macro Arguments (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Macro-Arguments.html) 
> All arguments to a macro are completely macro-expanded before they are substituted into the macro body. After substitution, the complete text is scanned again for macros to expand, including the arguments.

#### `#` Stringizing 字符串化
> [Stringizing (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html) 

在宏中，`#` 操作符会把跟随它的宏参数转换成一个字符串字面量：
```cpp
#define TO_STRING(x) #x
```
调用 `TO_STRING(123)` 会产生 `"123"`。

#### `##` 连接操作符
> [Concatenation (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html) 
> It is often useful to merge two tokens into one while expanding macros. This is called token pasting or token concatenation. 
> Keep in mind that the C preprocessor converts comments to whitespace before macros are even considered. Therefore, you cannot create a comment by concatenating ‘/’ and ‘*’. 
> You can put as much whitespace between ‘##’ and its operands as you like, including comments, and you can put comments in arguments that will be concatenated. 

- 在宏定义中，`##` 操作符可以用来连接两个宏参数或宏参数与其他文本。它会删除连接符两边的空白，并将它们拼接成一个单一的标识符。
- 不能通过宏创建注释，因为注释在宏展开前就被处理了。
- `##` 的前后空白会被忽略。

##### 宏连接生成不同的函数
在软件开发中，宏可以用于实现元编程，即在编译时生成代码。这在处理大量相似但有细微差别的代码时非常有用，例如在创建大量具有相似功能的类或函数时。

**CONCAT** 宏通过 `##` 运算符连接两个参数，生成一个新的标识符。这在需要动态生成代码时非常有用，尤其是在模板元编程中。

假设需要开发一个需要支持多种类型的资源加载器的应用程序，每种资源类型都有其特定的加载函数。可以使用 **CONCAT** 宏来动态生成这些加载函数的名称。

```cpp
#include <iostream>
#include <string>

// 定义 CONCAT 宏来连接资源类型和函数后缀
#define CONCAT(name, suffix) name##suffix

// 定义资源加载函数模板
#define DEFINE_LOADER(type) \
    void CONCAT(load_, type)(const std::string& file_path) { \
        std::cout << "Loading " << #type << " from " << file_path << std::endl; \
        // 加载资源的实现细节 \
    }

// 根据资源类型定义加载函数
DEFINE_LOADER(image)
DEFINE_LOADER(model)
DEFINE_LOADER(sound)

int main() {
    load_image("path/to/image.png");
    load_model("path/to/model.obj");
    load_sound("path/to/sound.wav");

    return 0;
}
```

作用：
- **减少重复代码**：通过宏，避免了为每种资源类型编写重复的加载函数定义。
- **提高代码的可维护性**：当需要添加新的资源类型时，只需添加新的 `DEFINE_LOADER` 宏调用即可。

在这个示例中，我们使用 **DEFINE_LOADER** 宏来定义不同资源类型的加载函数。这个宏内部使用了 **CONCAT** 宏来生成具体的函数名，如 `load_image`、`load_model` 和 `load_sound`。这样，就可以在 `main` 函数中直接调用这些动态生成的加载函数，而不需要为每种资源类型手动编写加载函数。

##### 宏连接生成枚举变量和结构体成员变量
> [Concatenation (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Concatenation.html) 

```cpp
struct command
{
  char *name;
  void (*function) (void);
};

struct command commands[] =
{
  { "quit", quit_command },
  { "help", help_command },
  …
};
```

```cpp
#define COMMAND(NAME)  { #NAME, NAME ## _command }

struct command
{
  char *name;
  void (*function) (void);
};

struct command commands[] =
{
  COMMAND (quit),
  COMMAND (help),
  …
};
```

##### 注意事项
在使用 `##` 连接操作符时，需要注意以下几个方面：

1. **操作数必须是预处理器令牌**：`##` 操作符两边的操作数必须是有效的预处理器令牌。这意味着你不能使用它来连接不形成有效C++令牌的字符。

2. **生成的令牌必须是有效的**：通过 `##` 操作符连接生成的新令牌必须是编译器可以识别的有效令牌。比如，连接后形成的不是一个有效的标识符（变量名、函数名等），程序则会编译失败。

3. **使用中的空格处理**：`##` 操作符对两边的空白字符不敏感。无论你是否在 `##` 的两侧添加空格，结果都将是相同的。但是在编写宏时，良好的空格使用习惯有助于提高代码的可读性。

4. **防止宏展开错误**：在一些情况下，如果 `##` 的两边是宏参数，而这些参数自身也需要宏展开，可能会出现预处理结果不如预期的情况。为了确保宏参数在连接之前被正确地展开，你可能需要使用间接宏展开的技巧。

##### 间接宏展开
如果我们想要根据不同的条件，动态地生成变量名或者函数名。这时，我们就需要使用宏的展开特性了。但是，有时候我们直接使用宏，会发现它并没有像预期那样工作，尤其是在宏参数需要先展开成其他内容再进行连接的场合。这时，间接展开就显得非常有用。

首先定义两个宏，一个用于间接展开，一个是我们实际想要使用的宏：

```cpp
// 定义一个辅助宏，用于实现间接展开
#define CONCAT_HELPER(a, b) a##b

// 定义另一个宏，它利用上面的辅助宏来实现最终的功能
#define CONCAT(a, b) CONCAT_HELPER(a, b)
```

```cpp
#define FIRST_HALF Hello
#define SECOND_HALF World

// 假设我们想要定义一个变量名为 HelloWorld
```

如果我们直接使用 `##` 来连接 `FIRST_HALF` 和 `SECOND_HALF`，可能会因为宏参数没有正确地展开而失败。但是使用间接展开技术，则可以正确完成这个操作：

```cpp
// 使用间接展开宏
CONCAT(FIRST_HALF, SECOND_HALF)
```

在上面的代码中：

1. 调用 `CONCAT(FIRST_HALF, SECOND_HALF)` 意图是生成 `HelloWorld`。
2. 实际上，首先调用的是 `CONCAT_HELPER(FIRST_HALF, SECOND_HALF)`。
3. 在 `CONCAT_HELPER` 里，`a` 和 `b` 使用 `##` 连接之前，`FIRST_HALF` 和 `SECOND_HALF` 两个参数会首先被展开为 `Hello` 和 `World`。
4. 最后，`CONCAT_HELPER` 会把 `Hello` 和 `World` 连接成 `HelloWorld`。

这是一个非常有用的技巧，尤其是当宏的参数本身也是宏时。通过这种间接的方式，我们可以确保任何参数宏都会在连接之前被完全展开。这样不仅避免了预处理器相关错误，还提高了宏定义的灵活性和可用性。

#### X Macro
> [Serializing C++ with X-Macros](https://kewarken.medium.com/serializing-c-with-x-macros-f9cb84725b50) 
> [X macro - Wikipedia](https://en.wikipedia.org/wiki/X_macro) 

```cpp
#include <iostream>

// 定义枚举的 X Macro
#define ENUM_LIST \
    X(RED,    0) \
    X(GREEN,  1) \
    X(BLUE,   2)

// 根据 X Macro 生成枚举
enum Color {
    #define X(name, value) name = value,
    ENUM_LIST
    #undef X
};

// 定义结构体成员的 X Macro
#define STRUCT_MEMBERS \
    X(int,    age) \
    X(std::string, name) \
    X(double, salary)

// 根据 X Macro 生成结构体
struct Person {
    #define X(type, name) type name;
    STRUCT_MEMBERS
    #undef X
};

int main() {
    // 使用枚举
    Color c = GREEN;
    std::cout << "Selected color: " << c << std::endl;

    // 使用结构体
    Person p;
    p.age = 25;
    p.name = "John Doe";
    p.salary = 50000.0;

    std::cout << "Person's age: " << p.age << ", name: " << p.name << ", salary: " << p.salary << std::endl;

    return 0;
}
```
```cpp
#include <iostream>

// 定义水果的 X Macro
#define FRUITS(X) \
    X(APPLE)     \
    X(BANANA)   \
    X(ORANGE)   \
    X(GRAPE)

// 用于生成枚举值的宏
#define EXPAND_ENUM_FRUIT(name) name,
// 用于生成 switch 语句中每个 case 分支的宏
#define EXPAND_STRING_FRUIT(fruit) \
  case fruit:  \
    return #fruit;

// 定义水果枚举类型
enum fruits {
  FRUITS(EXPAND_ENUM_FRUIT)
};

// 将水果转换为字符串的函数
char* fruitToString(enum fruits value)
{
    switch (value) {
      FRUITS(EXPAND_STRING_FRUIT)
    default:
        return "Unknown Fruit";
    }
}

// 定义结构体成员的 X Macro
#define STRUCT_MEMBERS(X) \
    X(int,    quantity) \
    X(double, price)

#define EXPAND_STRUCT_FRUIT(type, name) type name;
// 定义结构体
struct FruitInfo {
  STRUCT_MEMBERS(EXPAND_STRUCT_FRUIT)
};


int main() {
    enum fruits f = APPLE;
    std::cout << fruitToString(f) << std::endl;

    FruitInfo info;
    info.quantity = 10;
    info.price = 5.5;

    std::cout << "Quantity: " << info.quantity << ", Price: " << info.price << std::endl;

    return 0;
}
```

X Macro 的主要优点是：
1. 减少代码重复：如果需要修改枚举或结构体成员的信息，只需在 X Macro 定义处进行更改，而无需在多个地方修改。
2. 提高代码的可维护性：使得代码结构更加清晰和易于管理。

然而，X Macro 也有一些潜在的缺点：
1. 可读性可能会受到一定影响。
2. 预处理器的使用可能会导致一些复杂的错误，并且调试起来相对困难。

#### 可变参数宏
> [Variadic Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Variadic-Macros.html) 

可变参数宏（Variadic Macros）是C++预处理器的一个特性，它允许宏接受可变数量的参数。这是在C++11及其之后的标准中支持的特性，使得宏能够更灵活地处理不同数量的输入参数。

使用 `...` 来指示宏定义中的可变部分，并且可以使用 `__VA_ARGS__` 宏来表示所有的可变参数列表。这使得开发人员可以书写接受任意数量参数的宏。

一个简单的日志宏，它接收可变数量的参数并打印：
```cpp
#include <iostream>

// 定义一个可变参数的宏，用来打印日志信息。
#define LOG(format, ...) printf(format, __VA_ARGS__)

int main() {
    // 使用宏来打印不同数量的参数
    LOG("This is a log message: %s\n", "Hello, World!");
    LOG("This is a log message with two parameters: %s %d\n", "Number", 1);

    return 0;
}
```

在这个例子中，定义了一个 `LOG` 宏，它可以接受一个字符串格式，然后是一个可变数量的参数，类似于 `printf` 的功能。`LOG` 宏首先接收一个格式字符串，然后是使用 `...` 表示的可变参数，它们在宏内部通过 `__VA_ARGS__` 引用。

##### 处理不同数据类型
可变参数宏通过使用 `...` 和 `__VA_ARGS__` 来处理不同类型的输入。在宏中，`...` 表示宏可以接受任意数量的参数，而 `__VA_ARGS__` 是一个特殊的宏，它表示所有传递给可变参数宏的参数列表。

要处理不同类型的输入，通常会结合使用格式化字符串（就像在 `printf` 函数中一样）与 `__VA_ARGS__`。这样做允许创建类似于标准C库函数 `printf` 的宏，它能够将不同类型的变量插入到字符串中去。

```cpp
#include <cstdio>

// 定义一个 LOG 宏，接收一个格式化字符串和可变参数
#define LOG(format, ...) printf(format, __VA_ARGS__)

int main() {
    int num = 10;
    float pi = 3.14f;
    const char* str = "example";

    // 使用宏来打印不同类型的参数
    LOG("Integer: %d, Float: %.2f, String: %s\n", num, pi, str);

    return 0;
}
```

上面的 `LOG` 宏可以接收不同类型的输入参数，比如 `int`、`float` 和 `const char*`，并且能够将它们格式化成字符串输出。

在实际使用时，应该要保证提供给宏的格式化字符串与传递的参数类型相匹配，因为预处理器不会进行类型检查，错误的匹配可能会导致未定义的行为。

#### 条件编译宏
条件编译在C/C++中是一个非常重要的功能，它允许根据不同的编译器、平台或配置来编译不同的代码。在C和C++混合编程中，`#if defined(__cplusplus)` 条件编译宏尤为重要。

##### __cplusplus
```cpp
#if defined(__cplusplus)
```

在C++代码中，编译器会自动定义 `__cplusplus` 宏，因此可以使用这个宏来区分C和C++环境。在头文件中，这通常用于确保函数声明在C和C++编译器下都正确处理。

1. **兼容C和C++编译器**:
   - 在头文件中使用 `extern "C"` 包装函数声明，可以防止C++编译器对这些函数进行名称修饰（name mangling），从而使得C++编译后的库能够与C语言代码进行链接。

2. **提供特定于C++的代码**:
   - 使用条件编译宏，可以在头文件中添加C++特有的功能或接口，而不影响纯C代码的编译。

```c
#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

void c_function(int a); // 兼容C和C++编译器的函数声明

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// 仅用于C++的接口
void cpp_function(int a);
#endif

#endif // EXAMPLE_H
```

```cpp
#include "example.h"
#include <iostream>

// C风格函数实现
extern "C" void c_function(int a) {
    std::cout << "C Function: " << a << std::endl;
}

// C++风格函数实现
void cpp_function(int a) {
    std::cout << "C++ Function: " << a << std::endl;
}
```

```c
#include "example.h"

int main() {
    c_function(10);
    // cpp_function(20); // 无法在纯C代码中调用
    return 0;
}
```

1. **头文件 `example.h`**:
   - **`#ifdef __cplusplus` 和 `extern "C"`**: 
     - `#ifdef __cplusplus` 检查是否在C++编译器环境下。
     - `extern "C"` 指示编译器按照C语言方式链接这些函数，防止C++名称修饰。
     - `extern "C"` 仅在C++环境下有效，因此需要使用条件编译来封装。

   - **仅用于C++的功能**:
     - 利用 `#ifdef __cplusplus` 包含C++特有的函数声明，使它们仅在C++编译器环境下可见。
     - 在C环境下，这些C++特性会被忽略，从而确保头文件在纯C编译器中依然能够正确工作。

2. **源文件 `example.cpp`**:
   - **C风格函数实现**:
     - 使用 `extern "C"` 实现 `c_function`，确保其名称在链接阶段不会被修饰。
   - **C++风格函数实现**:
     - 直接实现 `cpp_function`，因为它只会在C++环境下使用，没有必要使用任何条件编译。

3. **主程序文件 `main.c`**:
   - **调用C风格函数**:
     - 在纯C环境下调用 `c_function`，示例展示了如何兼容C和C++。
   - **无法调用C++函数**:
     - `cpp_function` 无法在纯C代码中调用，确保了纯C代码的纯粹性。

#### predefined macros
> [Predefined Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Predefined-Macros.html) 

##### standard predefined macros
> [Standard Predefined Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Standard-Predefined-Macros.html) 

##### common predefined macros
> [Common Predefined Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html) 

##### system-specific predefined macros
> [System-specific Predefined Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/System-specific-Predefined-Macros.html) 

##### c++ named operators
> [C++ Named Operators (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/C_002b_002b-Named-Operators.html) 

#### undefining and redefining macros
> [Undefining and Redefining Macros (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Undefining-and-Redefining-Macros.html) 

- #define
- #undef

#### `#if`, `#elif`, `#else`, `#endif`
这些指令用于条件编译。根据指定条件是否为真，可能会包括或排除代码部分。
```cpp
#define DEBUG 1

#if DEBUG
    std::cout << "Debug information";
#endif
```

#### `#ifdef` 和 `#ifndef`
这些指令检查一个宏是否被定义了，如果是(`#ifdef`)或不是(`#ifndef`)，则编译接下来的代码。
```cpp
#ifndef PI
    #define PI 3.14
#endif
```

#### `#error` 和 `#warning`
如果遇到这些指令，预处理器将分别显示错误或警告信息。
```cpp
#error This is an error message
```

#### `#pragma`
特定于编译器的指令，用于控制编译器的特定功能。
```cpp
#pragma once // 防止头文件被包含多次
```

#### `#line`
这个指令可以改变编译器认为的当前行号和文件名。
```cpp
#line 100 "newfilename.cpp"
```

#### macro pitfalls
> [Macro Pitfalls (The C Preprocessor)](https://gcc.gnu.org/onlinedocs/cpp/Macro-Pitfalls.html) 

#### 宏的局限性
虽然宏功能强大，但也有一些缺点。宏只进行文本替换，没有类型检查，也不尊重作用域。过多使用宏可能会导致代码难以阅读和调试。

1. **类型不安全**：宏进行的是文本替换，不会进行类型检查，这可能导致类型错误或不一致，而在编译时不会报错。

2. **调试困难**：宏展开后的代码在编译时插入到调用的地方，如果宏中存在错误，定位问题非常困难，因为编译器错误信息会指向展开后的代码，而不是宏定义本身。

3. **作用域问题**：宏没有作用域的概念，它在预处理阶段进行文本替换，而函数有清晰的作用域定义。

4. **可读性差**：宏函数可以进行复杂的文本替换，但这样的代码通常难以阅读和理解，特别是对于那些不熟悉宏定义的程序员。

5. **维护难度**：随着项目的增长，过多的宏定义会使代码库变得难以维护，特别是当宏进行复杂的操作，或者在多个文件中被重复定义时。

6. **侵入性强**：宏定义散布在代码中多个地方时，可能会无意中覆盖其他重要的代码片段，因为宏不考虑变量和函数的命名冲突。

由于以上原因，在现代C++编程实践中，建议尽可能使用模板、内联函数和常量代替宏。这些语言特性提供了更好的类型安全、更明确的作用域，以及更容易阅读和维护的代码。例如，模板可以用来处理在编译时需要决定的泛型编程问题，而内联函数可以在不牺牲性能的情况下提供函数替代宏的功能。

## 代码换行显示
1. 如果在一行中写了一个完整的语句，但是发现该行代码过长，希望在以下行继续编写，你可以在行尾用反斜杠(`\`)来表示要在下一行继续这一语句。
```cpp
int sum = 1 + 2 + \
3 + 4;
```

2. 另外，也可以在一行代码中间位置直接换行，这个位置最好在一个操作符的前面或后面，或者在括号内。
```cpp
int sum = 1 +
2 + 3 +
4;
```
或者：
```cpp
int product = (1 +
2) * (3 +
4);
```

## 翻译单元
多个翻译单元是指一个程序被分割成多个源代码文件，每个文件被单独编译成目标文件。在C++中，源文件通常是以`.cpp`后缀结尾的文件，而翻译单元则是指编译器用来生成单个目标文件的源代码加上其所包含的全部头文件。

在多翻译单元的项目中，变量和函数的声明需要在多个单元中保持一致，如果一个全局变量或者函数被多个翻译单元使用，它的定义应该存放在一个单独的源文件中，而在其他需要使用它的源文件中，通过extern关键字声明它的存在。

例如，如果有一个全局变量`int g_Variable;`在`file1.cpp`文件中定义，那么在另外一个文件`file2.cpp`中就需要用`extern int g_Variable;`来声明它，表示这个变量在其他地方定义，这样其他的翻译单元就可以访问`file1.cpp`中定义的`g_Variable`。

## 字符串多行显示

### 字符串连接
一种实现多行字符串的方法是通过在编译时自动进行的字符串连接。在 C++ 中如果将两个或多个字符串字面量放置在一起，它们会自动连接成一个单独的字符串字面量。

```cpp
#include <iostream>

int main() {
    std::string multilineString = "This is the first line.\n"
                                   "This is the second line.\n"
                                   "This is the third line.";
    std::cout << multilineString << std::endl;
    return 0;
}
```

### 使用 Raw String Literals（原生字符串字面量）
C++11 引入了 raw string literals（原始字符串字面量），它允许开发者在字符串中包含任意字符，包括换行符和引号。这对于处理多行字符串、正则表达式或包含特殊字符的文件路径非常有用。
原生字符串字面量通过 `R"delimiter(...)delimiter"` 的形式使用，其中 `delimiter` 是一个可选的自定义分隔符，可以为空。使用原生字符串字面量时，可以直接在字符串中加入新行，而无需手动添加 `\n`。

#### 多行字符串
```cpp
#include <iostream>

int main() {
    std::string multilineString = R"(This is the first line.
This is the second line.
This is the third line.)";
    std::cout << multilineString << std::endl;
    return 0;
}
```

#### 字符串中包含引号
```cpp
#include <iostream>

int main() {
    auto rawStr = R"(Hello, "world"!)";
    std::cout << rawStr << std::endl;
    return 0;
}
```
输出：
```
Hello, "world"!
```

#### 使用分隔符
如果不使用分隔符，那如果字符串中有 `)"` 则会认为字符串已结束，如：
```cpp
#include <iostream>

int main() {
    auto delimStr = R"(a
)\)"
a""
e)";
    std::cout << delimStr << std::endl;
    return 0;
}
```
上面代码会报错，但如果加一个分隔符，则能正确识别字符串结束的地方：
```cpp
#include <iostream>

int main() {
    auto delimStr = R"a(a
)\)"
a""
e)a";
    std::cout << delimStr << std::endl;
    return 0;
}
```
输出：
```cpp
a
)\)"
a""
e
```

#### 包含特殊字符
```cpp
#include <iostream>

int main() {
    auto specialChars = R"(Special characters: \n \t \r \0)";
    std::cout << specialChars << std::endl;
    return 0;
}
```
输出：
```
Special characters: 
	 
 
```

### 在操作符或括号内换行
这种方法是在表达式的操作符后或在括号内换行，而不影响代码的逻辑结构。这样做可以让每一行的长度保持在合理的范围内，提高代码的可读性。
```cpp
std::string str = "这是一个非常非常长的字符串，" +
                  "我们需要将它分成多行显示，" +
                  "以提高代码的可读性。";
```

## gcc
> [gcc](https://gcc.gnu.org/)
> [G++ and GCC (Using the GNU Compiler Collection (GCC))](https://gcc.gnu.org/onlinedocs/gcc-11.5.0/gcc/G_002b_002b-and-GCC.html) 

- **gcc**：原本指 GNU C Compiler，用于编译 C 程序。随着时间的推移，gcc 发展成为一个编译器集合，支持多种编程语言，包括 C、C++、Objective-C、Fortran、Ada 和 Go 等。因此，gcc 也可以指代 GNU Compiler Collection。
- **g++**：是 gcc 的一部分，专门用于编译 C++ 程序。它可以理解 C++ 的语法和链接 C++ 程序需要的库。

1. **默认编译语言**：
   - **gcc**：默认为编译 C 程序。当使用 gcc 编译 C++ 程序时，需要明确指定使用 C++ 编译器，或者直接使用 g++。
   - **g++**：专门用于编译 C++ 程序。

2. **链接的库**：
   - **gcc**：当编译 C++ 程序时，不会自动链接 C++ 标准库，除非明确指定。
   - **g++**：编译 C++ 程序时会自动链接 C++ 标准库，例如 libstdc++。

3. **使用场景**：
   - **gcc**：适用于将多种语言混合在一个项目中时使用，例如 C 和 C++ 混合编程，你可以使用 gcc 处理 C 文件，而用 g++ 编译和链接 C++ 文件。
   - **g++**：专注于 C++ 程序的编译和链接。

## make 和 makefile
> [make](https://www.cs.tufts.edu/comp/15/reference/make/makefile.pdf)

如果程序很简单，可以直接用 gcc 进行编译和链接，但当一个工程复杂时，借助工具更方便，因此有了 make 工具。

`make` 和 `Makefile` 是 UNIX/Linux 系统中用于自动化构建程序的工具和文件。它们大大简化了编译和链接程序的过程，尤其是对于包含多个源文件的复杂项目。下面将详细介绍这两者。

`make` 是一个工具程序，它主要的作用是根据 `Makefile` 文件中定义的规则自动编译和链接程序。它检测源文件的修改时间，只重新编译那些自上次编译后被修改过的文件，从而避免了不必要的编译，加快了构建速度。`make` 的使用减少了人工干预，确保了构建过程的一致性和准确性。

`Makefile` 是一个文本文件，它包含了一系列的规则（rules）来告诉 `make` 如何构建程序。一个规则通常包括三个部分：目标（target）、依赖（dependencies）和命令（commands）。

- **目标**：你想要 `make` 构建的东西，通常是可执行文件或对象文件。
- **依赖**：目标文件依赖的文件，只有当依赖的文件比目标文件更新时，`make` 才会执行相应的命令。
- **命令**：构建目标所需执行的实际命令，比如编译源代码或链接对象文件。

一个简单的示例 `Makefile` 可能看起来像这样：
```makefile
all: program

program: main.o helper.o
    gcc main.o helper.o -o program

main.o: main.c
    gcc -c main.c

helper.o: helper.c
    gcc -c helper.c

clean:
    rm -f *.o program
```

在这个示例中，`all` 是一个自定义目标，它的依赖是 `program`，这意味着当在命令行中运行 `make` 或 `make all` 时，`make` 将尝试构建 `program`。

`program` 是最终目标，它依赖于 `main.o` 和 `helper.o`。如果这些对象文件不存在，或者相关的 `.c` 文件比它们新，`make` 将会执行定义在 `program` 规则下的命令来生成或更新 `program`。

`main.o` 和 `helper.o` 是两个中间目标，用于保存中间编译结果。当 `main.c` 或 `helper.c` 被修改后，`make` 将重新编译它们。

最后，`clean` 是一个自定义目标，它没有依赖，用于清理构建产物，使项目回到初始状态。

通过使用 `make` 和 `Makefile`，开发者可以有效地管理和自动化他们的构建流程，特别是对于大型和复杂的项目。

make 是一个用于构建和编译程序的工具，它使用 makefile 来描述构建规则。makefile 是一个文本文件，其中包含一系列规则，用于编译和链接程序。

## cmake
> [cmake](https://cmake.org/cmake/help/latest/)

cmake 是一个跨平台的构建系统生成器，它不直接建立构建，而是生成标准化的构建文件，如 Unix 的 Makefile 或 Windows Visual Studio 的项目文件。cmake 使用 CMakeLists.txt 文件来定义构建过程，可以根据这个定义生成适用于不同编译系统的构建文件。因此，cmake 提供了一种高级的方法来定义和管理项目构建过程，使之能够在多个平台上以一致的方式构建。

`cmake` 基本上是为了简化和标准化 `Makefile` 的编写过程。使用传统的 `make` 工具时，开发者需要手动编写 `Makefile` 文件，这可能既复杂又易于出错，尤其是在大型项目或跨平台项目中。`CMakeLists.txt` 文件用更抽象的方式来定义构建规则，并且 `cmake` 工具可以根据这些抽象的规则生成具体平台或编译器所需的 `Makefile` 或其他构建脚本。

简单来说，`cmake` 提供了以下优势：
- **跨平台兼容性**：`cmake` 能够生成适用于多种操作系统和编译环境的构建文件，例如可以在 Linux 上生成 Unix 风格的 `Makefile`，在 Windows 上生成 `nmake` 文件或 Visual Studio 项目文件。
- **简化构建配置**：相比手写复杂的 `Makefile`，`cmake` 的 `CMakeLists.txt` 文件编写简单，易于理解和维护。
- **自动化侦测与配置**：`cmake` 能够检测系统的编译环境，自动查找依赖库，配置编译器选项等，使构建过程自动化度更高。

总的来说，`cmake` 是现代化的构建配置工具，它让跨平台的构建过程不再依赖于手动编写和维护复杂的 `Makefile`。

## nmake
> [nmake](https://learn.microsoft.com/en-us/cpp/build/reference/nmake-reference?view=msvc-170)

nmake 是 Microsoft 提供的一个版本的 make 工具，用于支持 Microsoft 的编译环境，类似于 Unix/Linux 下的 make。nmake 使用的是 Microsoft 特有的 Makefile 格式，与 Unix 下的 Makefile 存在一些差异。nmake 通常用于编译和构建在 Visual Studio 环境下开发的项目。

## qmake
> [qmake](https://doc.qt.io/qt-6/qmake-manual.html)

`qmake` 是一个由 Qt 开发的跨平台构建系统工具，专门用于简化 Qt 应用程序的开发和构建过程。`qmake` 能够根据项目的需求自动生成适合不同编译器和开发环境的 `Makefile` 文件，从而让开发者能够更专注于软件开发本身，而不是花费时间在配置构建环境上。

### 特点
1. **跨平台性**：`qmake` 能够在多个平台上生成对应的构建配置，包括 Windows、macOS、Linux 等。
2. **简化构建配置**：使用 `qmake`，开发者只需编写简单的 `*.pro` 或 `*.pri` 文件来描述项目的结构和构建设置，`qmake` 就能自动为你生成复杂的 `Makefile`。
3. **Qt 集成**：`qmake` 与 Qt 库紧密集成，能够自动处理 Qt 库的依赖和相关配置，极大简化了 Qt 应用的构建过程。

### 使用方法
- **安装**：通常 `qmake` 会随 Qt SDK 一起安装，确保 Qt 的安装路径被加入到系统的环境变量中，这样就可以在命令行中直接使用 `qmake` 命令了。
- **编写项目文件**：项目文件（通常是 `.pro` 文件）是 `qmake` 的核心，它描述了项目的结构、源文件、头文件、资源文件以及特定的构建配置等。
```qmake
TEMPLATE = app
TARGET = your_application
QT += core gui

HEADERS += mainwindow.h
SOURCES += main.cpp mainwindow.cpp
```
- **生成 Makefile**：在命令行中，导航到包含 `.pro` 文件的目录，运行 `qmake` 命令。`qmake` 将会根据 `.pro` 文件的内容生成对应平台的 `Makefile`。
```bash
qmake
```
- **构建项目**：生成 `Makefile` 之后，使用对应平台的 `make` 工具来构建项目：
```bash
make  # 在 Unix/Linux 或 macOS 上
nmake # 在 Windows 上，如果是使用 nmake
```

### 高级用法
`qmake` 也支持比较复杂的项目结构，允许自定义构建变量、条件编译、模板生成等高级功能。这些功能通过在 `.pro` 文件中编写更为复杂的配置来实现。

总而言之，`qmake` 是一个强大的工具，尤其适合于 Qt 应用的开发。通过自动化处理构建配置，它让开发者能够把精力更多地集中在实现应用程序的功能上。

## build 和 rebuild
在 Qt 工程中，首先需要编写好 `.pro` 文件，然后执行 `qmake` 生成 `Makefile`。但是，执行 `qmake` 仅仅是生成了构建工程所需的 `Makefile`，而实际上还需要执行构建命令（如 `make` 或在 Qt Creator 中的 build 按钮）来编译和链接代码生成可执行文件或库。

- **`qmake`**: 执行 `qmake` 会根据 `.pro` 文件中定义的配置来生成 `Makefile`。这个步骤不会编译代码，它仅仅是为后续的编译步骤准备所需的 `Makefile` 文件。

- **`build`**: 执行 build（无论是在命令行中的 `make` 命令还是在 Qt Creator 中的 build 按钮）会根据 `qmake` 生成的 `Makefile` 来编译源代码和资源，链接所需的库，并最终生成可执行文件或库文件。如果之前已经进行过一次构建，且源代码没有变化，build 命令可能不会重新编译所有的文件，只会编译那些自上次 build 以来已经发生变化的源文件。

- **`rebuild`**: 当你需要从头开始重新构建你的工程时，你会执行一个 rebuild。在 Qt Creator 中执行 rebuild 会先执行一个 clean 操作清除所有之前编译生成的文件，然后像执行 build 那样重新编译整个工程。重建是用来确保所有东西都是最新的，通常在修改了 project 文件或者其他一些构建配置的时候需要做这个操作。

`qmake` 是用来生成 `Makefile` 文件的，而 `build` 是用 `Makefile` 来编译和链接生成最终的二进制文件，`rebuild` 则是完整地清除并重新构建整个项目。在日常开发中，你会频繁地进行 `build` 操作，而在工程配置变化后，你可能需要重新运行 `qmake`，并且在一些特定情况下执行 `rebuild`。

## compile build and make
- compile
这里的编译指将源代码转换成目标代码的过程。 在 C/C++ 中，这个过程通常由编译器完成。
- build
从源代码到可执行程序的一系列步骤。
- make
make 是一个自动化构建工具，用于管理和控制软件构建过程。

## 不同编译器

## 声明
> [declarations](https://en.cppreference.com/w/cpp/language/declarations)

在C++中，纯声明（不包含定义）是一种常用的实践，特别适用于向编译器介绍某个标识符（如变量、函数、类等）而不立即提供其完整的定义。这种方式特别有用于实现头文件和模块化编程，让编译器知道某个标识符的存在及其类型，但实现（定义）可能会在其他地方给出。

1. **函数声明（函数原型）**:
函数的声明（Function Declaration）告诉编译器函数的名字、返回类型以及参数（如果有的话），但不包括具体的函数实现。函数的定义（Function Definition）则包括了函数的具体实现。

在C++中，函数声明非常简单，仅仅需要指定函数的返回类型、函数名以及参数列表（如果有的话），通常以分号结尾。例如：

```cpp
int sum(int, int); // 函数声明
```

而函数的定义则需要提供函数的具体实现。如果函数声明后面紧跟着花括号 `{}` 和函数体，那么这就构成了函数的定义。在定义中编写具体的代码来实现函数的功能。例如：

```cpp
int sum(int a, int b) { // 函数定义
    return a + b;
}
```

在编写程序时，函数必须被定义一次，但可以被声明多次。通常在头文件中进行函数声明，在源文件中进行函数定义。这样做的目的是为了在链接程序的不同部分时，确保在调用函数之前它们已被声明。

当我们提到一个函数“有 `{}`”，就意味着函数体存在，这通常表示函数被定义了。即便函数体为空，例如：

```cpp
void doNothing() {} // 空函数体的函数定义
```

这仍然是一个函数定义，因为它提供了函数体（尽管函数体为空）。如果一个函数没有花括号 `{}`，就意味着它是一个声明，因为它没有提供函数体：

```cpp
void doNothing(); // 函数声明
```

2. **外部变量声明**:
在C++编程中，`extern`关键字非常重要，它用于声明一个变量或函数是在别处定义的，这意味着使用`extern`可以在多个文件之间共享全局变量和函数。

考虑一个项目，其中包含两个文件：`globals.cpp`和`main.cpp`。

- **globals.cpp**定义了一个全局变量。
- **main.cpp**想要访问在`globals.cpp`中定义的这个全局变量。

**globals.cpp:**
```cpp
// globals.cpp
int globalVariable = 42;  // 全局变量的定义
```

**main.cpp:**
```cpp
// main.cpp
#include <iostream>

extern int globalVariable;  // 使用extern声明外部变量

int main() {
    std::cout << "Global variable value: " << globalVariable << std::endl;
    return 0;
}
```

在这个例子中，在`globals.cpp`文件中定义了一个名为`globalVariable`的全局变量。然后，在`main.cpp`文件中，使用`extern`关键字声明了同名的全局变量，这表明该变量的定义在其他位置（即`globals.cpp`）。因此，当`main.cpp`访问`globalVariable`时，它实际上访问的是在`globals.cpp`中定义的变量。这就允许不同文件之间共享和访问同一个变量。

为了使上面的例子工作，需要编译两个文件并将它们链接在一起。使用g++的命令行示例如下：

```bash
g++ -c globals.cpp -o globals.o
g++ -c main.cpp -o main.o
g++ globals.o main.o -o myProgram
./myProgram
```

输出：
```
Global variable value: 42
```

使用`extern`关键字确保了在`main.cpp`中能正确访问在`globals.cpp`中定义的`globalVariable`变量，这展示了如何在C++中跨文件共享全局变量。

3. **类和结构体的前向声明**:
当你只需要知道一个类的存在而不需要知道类的细节时，你可以进行类的前向声明。这特别有用于处理循环依赖问题。
```cpp
class MyClass; // 类的前向声明
struct MyStruct; // 结构体的前向声明
```

4. **使用typedef声明类型别名**:
你可以声明一个类型的别名而不定义新类型，这对于简化复杂类型名称很有帮助。
```cpp
typedef unsigned long ulong; // 声明了一个unsigned long的别名为ulong
```

5. **声明模板**:
模板可以被声明但不立即定义，直到实例化时。
```cpp
template <typename T>
class MyTemplateClass; // 模板类的前向声明

template <typename T>
void myTemplateFunction(T); // 模板函数的声明
```

6. **使用extern "C"声明C语言链接**:
在C++中，当你想要声明一个函数遵循C语言的链接规则时，可以使用`extern "C"`。

```cpp
extern "C" void myCFunction(int); // 声明一个C语言函数
```

通过这些声明，可以让编译器在不需要完整定义的情况下对程序的不同部分进行编译，有助于模块化设计和减少编译依赖。这种做法在大型项目和库的设计中非常有用。

## 定义
> [Initialization](https://en.cppreference.com/w/cpp/language/definition)

在C++中，定义(Definition)是创建程序中使用的实体的过程。这些实体可以是变量、函数、类等。定义会为这些实体分配内存，并且可能会初始化它们。定义与声明(Declaration)是有区别的，声明只是告诉编译器某个标识符的存在，而定义则是为其分配存储空间。

### 变量定义
定义变量时，编译器会在内存中为此变量分配空间。

### 函数定义
> [function definition](https://en.cppreference.com/w/cpp/language/function#Function_definition)

函数定义是函数声明的一个超集，它包含了函数的声明以及函数体，因此，函数定义较之函数声明会多出函数体这一部分。即，函数定义除了提供声明的所有信息外，还提供了函数的实现细节，此时编译器已经能够知道函数将会执行什么操作。正在执行的程序中的函数代码存储在内存的代码段(code segment)中，这部分内存在程序启动时由操作系统加载到内存中，是只读的，并且共享于所有执行的线程。因此，我们通常不说函数的定义在内存中占用多少空间，因为这是由编译器处理的，并且在程序运行时由操作系统管理。例如`void func() { // some code }`，在这里我们不仅声明了函数，还给出了函数执行的具体代码，因此这是一个函数定义。

1. 当函数被调用时，在栈上为其参数和局部变量分配内存。
2. 每个函数调用都有其独立的栈帧（Stack Frame），它包含函数的参数、局部变量和返回地址。

函数定义本身，就是代码中的一段，它描述了函数如何执行其任务。只有在函数被调用时，才会发生内存的分配。在编写代码时，函数定义只是告诉编译器函数如何工作，但不会分配内存，直到函数实际执行时才会进行。

### 类定义
> [class definition](https://en.cppreference.com/w/cpp/language/class#Class_definition)

类定义本身在代码中是一种模板，它描述了一个类型的属性（成员变量）和行为（成员函数或方法）。类定义本身不直接涉及到内存分配。内存分配发生在创建类的实例（对象）时。

1. **类定义**：当你定义一个类时，你正在为对象创建一个“蓝图”。这个蓝图包含了对象的数据成员和方法，但是在这个阶段不会分配内存给这些数据成员和方法，因为你还没有创建任何对象。

2. **对象实例化**：当你基于类创建对象时，为这个对象的成员变量分配内存。这个过程通常发生在堆（动态分配）或栈上（自动分配），取决于你如何创建对象（例如，使用`new`关键字动态创建的对象分配在堆上，而自动变量通常分配在栈上）。

3. **构造函数**：类可以有一个或多个构造函数，这是特殊的函数，在创建类的新对象时自动调用。构造函数的任务之一就是初始化对象的数据成员，它也可以用来执行任何在对象创建时需要完成的其他初始化。

4. **析构函数**：当对象的生命周期结束时，析构函数会被调用。对于在堆上动态分配的对象，需要显式调用`delete`来释放内存。析构函数执行清理任务，如释放对象在生命周期内获取的资源。

因此，虽然类定义本身不涉及到内存分配，但是创建类的实例会根据数据成员和对象创建方式（如堆或栈分配）涉及到内存分配。

## 作用域
> [scope](https://en.cppreference.com/w/cpp/language/scope)

1. **全局作用域**：
   - 在函数外部定义的变量或函数拥有全局作用域。
   - 全局作用域的变量和函数在整个程序中都是可见的。
   
2. **局部作用域**：
   - 在函数内部定义的变量拥有局部作用域。
   - 局部变量仅在其定义的函数或块内可见。

3. **命名空间作用域**：
   - 命名空间提供了一种避免名称冲突的机制。
   - 在命名空间中定义的变量或函数，其作用域被限制在该命名空间内。
   
4. **类作用域**：
   - 在类内部定义的成员（包括变量、函数等）拥有类作用域。
   - 类的成员可以通过对象访问（或通过类名访问静态成员）。

## storage duration
> [storage duration](https://en.cppreference.com/w/cpp/language/storage_duration)

1. **静态存储期**：
   - The storage for the object is allocated when the program begins and deallocated when the program ends. Only one instance of the object exists.

2. **线程存储期**：
   - 声明周期依附于线程的存在期间。
   - 它们通常用`thread_local`关键字声明，并在相关线程开始时初始化，在线程结束时销毁。

3. **自动存储期**：
   - 对象的生命周期是局部的，通常与声明它的代码块有关。
   - 这些对象会在其声明的代码块执行时创建，出了作用域后销毁。

4. **动态存储期**：
   - 通过动态内存分配（使用`new`或`malloc`）创建的对象。
   - 不会自动销毁，需要使用`delete`或`free`来手动释放内存。

## linkage
> [1](https://en.cppreference.com/w/cpp/language/storage_duration)
> [2](https://blog.csdn.net/simonyucsdy/article/details/82728456)

在C++中，链接（linkage）描述了名称（例如函数或变量）在不同编译单元之间如何共享。这涉及到名称的可见性以及程序不同部分如何引用相同的实体。链接主要有三种类型：外部链接、内部链接和无链接。

1. **外部链接（External Linkage）**：
    - 表示名称在其他文件中是可见的，可以从其他编译单元（源文件）进行链接。
    - 对于非静态全局变量和函数来说，默认情况下它们具有外部链接。
    - 如果想让其他文件可以引用这个变量或函数，就应保持其外部链接。

2. **内部链接（Internal Linkage）**：
    - 当定义了一些不希望在当前文件之外可见的全局静态变量或函数时，它们就有了内部链接。
    - 在`.c`文件中，可以将变量或函数放入一个未命名的命名空间，或者用`static`关键字声明，来赋予内部链接。
    - 这意味着这些名称在其他编译单元中是不可见的。其他编译单元如果声明了相同名称的实体，这两个实体是完全独立的。
    - 在`.h`文件（头文件）中不应使用未命名命名空间或`static`关键字，因为头文件通常会被多个不同的`.c`文件包含，而内部链接意味着局部于一个编译单元。

3. **无链接（No Linkage）**：
    - 局部变量的名称，例如在函数内定义的变量，具有无链接属性。
    - 这意味着它们只在定义它们的代码块中可见，并且在这个作用域之外完全不能被引用。

在C++的编程实践中，合理使用链接属性可以更好地控制程序的模块化和封装，防止命名冲突，并且可以管理好程序的符号可见性。如上所述，可以通过将不需要跨文件共享的声明移入未命名命名空间或声明为`static`来实现内部链接。这是一种实用的封装手段，可用于隐藏辅助功能和细节实现，以免在文件间不小心产生冲突或误用。

## scaler type
> [C++ named requirements: ScalarType - cppreference.com](https://en.cppreference.com/w/cpp/named_req/ScalarType) 




## 初始化
> [Initialization](https://en.cppreference.com/w/cpp/language/initialization)
> [Back to Basics: Initialization in C++ - Ben Saks - CppCon 2023](https://www.youtube.com/watch?v=_23qmZtDBxg&ab_channel=CppCon) 

If no initializer is specified for an object, the object is default-initialized. 
If no initializer is specified for a reference, the program is ill-formed.
If the initializer specified for an object is () (cannot appear in declarators due to the syntax restriction), the object is value-initialized. 
If the initializer specified for a reference is (), the program is ill-formed.

定义一个变量时，例如 `int a;`，它会在内存中为变量 `a` 腾出空间，但不会自动给它赋值。这是定义阶段。

初始化指的是在定义变量的同时给它一个初始值，或者在定义后的某个时刻给它赋值。例如，`int a = 0;` 不仅定义了 `a`，同时也将其初始化为 0。这是初始化阶段。

在C++中，如果定义了一个类的对象而没有给出具体的初值，则调用该类的默认构造函数来进行初始化（如果没有提供任何构造函数，编译器会提供一个默认的构造函数）。

所以定义和初始化可以在同一行代码中完成，但它们描述的是不同的操作和阶段。

### copy-initialization
> [Copy-initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_initialization) 

### aggregate initialization
> [Aggregate initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/aggregate_initialization) 

```cpp
int x[] = {1, 3, 5}; // x has 3 elements
 
struct Y { int i, j, k; };
 
Y y[] = {1, 2, 3, 4, 5, 6}; // y has only 2 elements:
                            // 1, 2 and 3 appertain to y[0],
                            // 4, 5 and 6 appertain to y[1]
 
int z[] = {} // Error: cannot declare an array without any element
```

### direct-initialization
> [Direct-initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/direct_initialization) 

- Initializes an object from explicit set of constructor arguments.

- built-in types
```cpp
int i = 5;
int x(5);
double y(2.3);
```
- user-defined types
调用各种构造函数的初始化，都是直接初始化。
```cpp
std::string s1("hello");
std::string s2(s1); //调用拷贝构造函数，但还是 direct initialization
```

### value-initialization
> [Value-initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/value_initialization) 

- This is the initialization performed when an object is constructed with an empty initializer.

```cpp
T ()	(1)	
new T ()	(2)	
Class::Class(...) : member () { ... }	(3)	
T object {};	(4)	(since C++11)
T {}	(5)	(since C++11)
new T {}	(6)	(since C++11)
Class::Class(...) : member {} { ... }	(7)	(since C++11)
```

The effects of value-initialization are:
- If T is a (possibly cv-qualified) class type:
    - If the default-initialization for T selects a constructor, and the constructor is not user-declared(until C++11)user-provided(since C++11), the object is first zero-initialized.
    - In any case, the object is default-initialized.
- Otherwise, if T is an array type, each element of the array is value-initialized.
- Otherwise, the object is zero-initialized.

```cpp
int i = {};        // 值初始化，i 被初始化为 0
int j;            // 值初始化，j 被初始化为 0
int* p = {};      // 值初始化，p 被初始化为 nullptr
int* q;           // 值初始化，q 被初始化为 nullptr
int a[3] = {}; // 值初始化 所有元素初始化为 0
int a[3] = {0}; // 不是值初始化，仅显示将第一个元素初始化为 0
```

```cpp
#include <cassert>
#include <iostream>
#include <string>
#include <vector>
 
struct T1
{
    int mem1;
    std::string mem2;
    virtual void foo() {} // make sure T1 is not an aggregate
}; // implicit default constructor
 
struct T2
{
    int mem1;
    std::string mem2;
    T2(const T2&) {} // user-provided copy constructor
};                   // no default constructor
 
struct T3
{
    int mem1;
    std::string mem2;
    T3() {} // user-provided default constructor
};
 
std::string s{}; // class => default-initialization, the value is ""
 
int main()
{
    int n{};                // scalar => zero-initialization, the value is 0
    assert(n == 0);
    double f = double();    // scalar => zero-initialization, the value is 0.0
    assert(f == 0.0);
    int* a = new int[10](); // array => value-initialization of each element
    assert(a[9] == 0);      //          the value of each element is 0
    T1 t1{};                // class with implicit default constructor =>
    assert(t1.mem1 == 0);   //     t1.mem1 is zero-initialized, the value is 0
    assert(t1.mem2 == "");  //     t1.mem2 is default-initialized, the value is ""
//  T2 t2{};                // error: class with no default constructor
    T3 t3{};                // class with user-provided default constructor =>
    std::cout << t3.mem1;   //     t3.mem1 is default-initialized to indeterminate value
    assert(t3.mem2 == "");  //     t3.mem2 is default-initialized, the value is ""
    std::vector<int> v(3);  // value-initialization of each element
    assert(v[2] == 0);      // the value of each element is 0
    std::cout << '\n';
    delete[] a;
}
```

### reference initalization
> [Reference initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/reference_initialization) 

If the entity being initialized is a reference, see reference initialization.

### Zero-initialization
> [Zero-initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/zero_initialization) 

Sets the initial value of an object to zero.

Zero-initialization is performed in the following situations:
- For every named variable with static or thread-local(since C++11) storage duration that is not subject to constant initialization, before any other initialization.
```cpp
static T object ;
```
- As part of value-initialization sequence for non-class types and for members of value-initialized class types that have no constructors, including value initialization of elements of aggregates for which no initializers are provided.
```cpp
T () ;
T t = {} ;

T {} ; (since C++11)
```
- When an array of any character type is initialized with a string literal that is too short, the remainder of the array is zero-initialized.
```cpp
CharT array [ n ] = " short-sequence ";	
```

The effects of zero-initialization are:
- If T is a scalar type, the object is initialized to the value obtained by explicitly converting the integer literal ​0​ (zero) to T.
- If T is a non-union class type:
    - all padding bits are initialized to zero bits,
    - each non-static data member is zero-initialized,
    - each non-virtual base class subobject is zero-initialized, and
    - if the object is not a base class subobject, each virtual base class subobject is zero-initialized.
- If T is a union type:
    - all padding bits are initialized to zero bits, and
    th object’s first non-static named data member is zero-initialized.
- If T is array type, each element is zero-initialized.
- If T is reference type, nothing is done.

### 列表初始化
> [List-initialization (since C++11) - cppreference.com](https://en.cppreference.com/w/cpp/language/list_initialization) 

#### Direct-list-initialization

#### Copy-list-initialization

### 默认初始化
> [Initialization](https://en.cppreference.com/w/cpp/language/initialization)
> [default initialization](https://en.cppreference.com/w/cpp/language/default_initialization)

#### 内置类型的局部变量
如果没有显式初始化，它们不会被初始化。它们的初始值是未定义的。

#### 非类的静态和线程局部变量
> [static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)

如果没有显示初始化，在程序启动（对于全局或命名空间作用域的静态变量）或线程启动（对于线程局部变量）时被零初始化。

Variables declared at block scope with the specifier static or thread_local(since C++11) have static or thread(since C++11) storage duration but are initialized the first time control passes through their declaration (unless their initialization is zero- or constant-initialization, which can be performed before the block is first entered). On all further calls, the declaration is skipped.

If the initialization throws an exception, the variable is not considered to be initialized, and initialization will be attempted again the next time control passes through the declaration.

If the initialization recursively enters the block in which the variable is being initialized, the behavior is undefined.

#### 类
如果类有默认构造函数（无论是用户定义的还是编译器自动生成的），则使用这个默认构造函数进行初始化。

#### 类成员变量初始化
- 类的内置类型成员（如`int`、`double`等）在创建对象时不会被自动初始化，其值是未定义的，除非它们被显式地在类定义中赋初值。
- 类的成员如果是对象，并且这些对象属于的类具有默认构造函数，那么这些成员对象会使用它们的默认构造函数进行初始化。
- 类的成员如果是有静态存储期的内置类型或是有静态存储期的对象，那么在程序启动时它们会被初始化；如果是有动态存储期的对象，那么在分配内存时它们会使用默认构造函数进行初始化。

### 类中成员变量两种初始化方式
在C++中，初始化类成员变量可以在两个地方进行：在类定义中直接对成员变量进行初始化，或者在类的构造函数初始化列表中进行初始化。两种方法各有优缺点，适用情况也略有不同。

#### 在类定义中初始化
在C++11及之后的版本中，允许在类定义中直接初始化成员变量，这种方法也被称为非静态数据成员初始化（NSDMI）。例如：

```cpp
class A {
public:
    A() = default; // 使用默认构造函数
    A(int val) : anotherValue(val) {} // 构造函数初始化列表

private:
    int value = 5; // 在类定义中直接初始化
    int anotherValue;
};
```

**优点：**
- 简洁明了：在成员变量声明的地方直接初始化，增强了代码的可读性。
- 减少重复：对于默认值，不需要在每个构造函数中重复初始化。
- 更安全：提前初始化能减少因忘记初始化而导致的未定义行为。

**缺点：**
- 灵活性略低：不能基于构造函数参数动态初始化成员变量。

#### 在构造函数初始化列表中初始化
```cpp
class B {
public:
    B(int val) : value(val) {} // 在初始化列表中初始化

private:
    int value;
};
```

**优点：**
- 灵活性高：允许根据构造函数的参数动态初始化成员变量。

**缺点：**
- 重复性：如果类有多个构造函数，且多个构造函数都需要初始化相同的成员，可能会导致代码重复。

#### 两种初始化方式比较
选择哪种初始化方式，取决于具体情况：
- 如果成员变量可以有一个通用的默认值，那么在类定义中直接初始化它们很方便；
- 如果成员变量的初始化值依赖于构造函数的参数，或者成员是常量或引用，使用构造函数初始化列表是必须的。
- 构造函数的初始化列表中的初始化会覆盖类定义中的初始化值。
- 无论在构造函数初始化列表中的顺序如何，成员总是按照在类定义中的顺序被初始化。而对于非静态成员初始化，如果在构造函数初始化列表中也对该成员进行了初始化，那么构造函数初始化列表中的值会覆盖非静态成员初始器的值。
- 对于派生类，如果基类的构造函数需要参数，那么必须使用构造函数初始化列表。

所有的这些初始化都只有在创建对象的时候才会实际发生。不论是在类定义中初始化，还是在构造函数初始化列表中初始化，都不会为类定义本身分配内存。只有创建类的实例时，这些初始化才会发生，内存才会被分配。

### 函数无初始化
类中的成员函数并不像类中的成员变量那样进行初始化。函数，无论是成员函数还是非成员函数，它们都不需要初始化。函数在编译时被翻译为机器代码，并存储在程序代码的内存空间中。函数的“初始化”实际上是编译器编译函数代码的过程，这与变量的初始化是不一样的。 

变量的初始化涉及到给变量分配内存，并设置初始值。这个过程通常在运行时发生，例如在对象创建时或在调用函数时。变量的生命期是它所在的作用域，一旦离开这个作用域，变量就会被销毁。

相对的，函数一旦被编译，它在程序的整个生命周期内都存在。你可以在程序的任何地方、任何时间调用它，前提是你有足够的权限，并且它在你的作用域内。

总的来说，函数和变量的“初始化”在概念和过程上都有很大的区别。变量的初始化是在运行时动态发生的，涉及内存分配和初始值设置。而函数的初始化实际上是在编译时将源代码转化为机器代码的过程。

### 静态局部变量初始化
> [static local variables](https://en.cppreference.com/w/cpp/language/storage_duration#Static_local_variables)

Variables declared at block scope with the specifier static or thread_local(since C++11) have static or thread(since C++11) storage duration but are initialized the first time control passes through their declaration (unless their initialization is zero- or constant-initialization, which can be performed before the block is first entered). On all further calls, the declaration is skipped.

If the initialization throws an exception, the variable is not considered to be initialized, and initialization will be attempted again the next time control passes through the declaration.

If the initialization recursively enters the block in which the variable is being initialized, the behavior is undefined.

#### 静态局部变量初始化是线程安全的
If multiple threads attempt to initialize the same static local variable concurrently, the initialization occurs exactly once (similar behavior can be obtained for arbitrary functions with std::call_once).

## 初始化和内存分配
在C++编程中，初始化和内存分配是两个相关但又独立的过程。

1. 初始化：初始化是创建变量并给它赋予初始值的过程。这个过程可能简单，比如对整数变量进行初始化，就是赋予它一个整数值。对于更复杂的数据类型，例如对象或者数组，它们的初始化可能涉及到调用构造函数、内存分配等动作。

2. 内存分配：变量的内存分配通常在初始化之前发生。对于局部变量，它们的内存分配发生在栈上，当变量超出其作用域时，分配给它的内存会自动释放。对于动态分配的变量，如使用`new`关键字创建的对象，它们的内存分配发生在堆上，这些变量会一直存在，直到你明确删除它们。

局部变量和成员变量初始化的区别：

- 局部变量：当在函数中定义一个局部变量时，会立即为其分配内存。这个变量只存在于它所在的作用域内，在作用域结束时，变量将被销毁，分配的内存将被回收。

- 类的成员变量：类的成员变量的初始化有些不同。在类定义内部，只是声明了成员变量，并不一定进行初始化。可以在类的构造函数中初始化这些成员变量。这些成员变量的内存不是在类定义时分配的，而是在创建类的一个对象时分配的。当对象被创建时，它的成员变量被初始化，并分配内存。

在类定义时为成员变量赋初始值，并不意味着这个时候就要为它们分配内存，它们的内存分配发生在对象实例化时。类定义本身不拥有任何存储空间，只有当创建类的实例（对象）时，才会为实例中的每个成员变量分配内存，这就是为什么说“类中成员变量在类定义中初始化不分配内存空间”的原因。

在C++11及其后续版本中，允许在类体内给非静态成员变量提供一个初始值，这种方式叫做默认成员初始化器。这样做并不会引发实际的初始化行为，它只是提供了一份初始值的备份。当对象被创建时（也就是在对象的构造函数调用时）如果在初始化列表中明确地初始化了该成员，那么成员初始化器所指定的初始值就会被忽略，否则就会使用成员初始化器提供的初始值进行初始化。

## extern
> [C++ keyword: extern - cppreference.com](https://en.cppreference.com/w/cpp/keyword/extern) 

在C++编程中，`extern`关键字非常重要，它用于声明一个变量或函数是在别处定义的，这意味着使用`extern`可以在多个文件之间共享全局变量和函数。

### `extern`的关键点

1. **声明而非定义**: 当你使用`extern`时，它告诉编译器相应的变量或函数在其他地方定义，而不是在当前位置创建一个新的实例。
   
2. **全局变量共享**: 在大型项目中，可能需要在不同的文件中访问相同的全局变量，`extern`使得在不同文件之间访问和修改同一个全局变量成为可能。

3. **避免重复定义错误**: 如果没有`extern`，在多个文件中声明同一个全局变量会导致重复定义错误。使用`extern`可以避免这个问题。

### 示例

考虑一个项目，其中包含两个文件：`globals.cpp`和`main.cpp`。

- **globals.cpp**定义了一个全局变量。
- **main.cpp**想要访问在`globals.cpp`中定义的这个全局变量。

**globals.cpp:**
```cpp
// globals.cpp
int globalVariable = 42;  // 全局变量的定义
```

**main.cpp:**
```cpp
// main.cpp
#include <iostream>

extern int globalVariable;  // 使用extern声明外部变量

int main() {
    std::cout << "Global variable value: " << globalVariable << std::endl;
    return 0;
}
```

在这个例子中，我们在`globals.cpp`文件中定义了一个名为`globalVariable`的全局变量。然后，在`main.cpp`文件中，我们使用`extern`关键字声明了同名的全局变量，这表明该变量的定义在其他位置（即`globals.cpp`）。因此，当`main.cpp`访问`globalVariable`时，它实际上访问的是在`globals.cpp`中定义的变量。这就允许不同文件之间共享和访问同一个变量。

### 编译链接

为了使上面的例子工作，需要编译两个文件并将它们链接在一起。使用g++的命令行示例如下：

```bash
g++ -c globals.cpp -o globals.o
g++ -c main.cpp -o main.o
g++ globals.o main.o -o myProgram
./myProgram
```

输出：
```
Global variable value: 42
```

使用`extern`关键字确保了在`main.cpp`中能正确访问在`globals.cpp`中定义的`globalVariable`变量，这展示了如何在C++中跨文件共享全局变量。

## extern "C"
`extern` 关键字是C和C++中的一个存储类说明符，用于声明变量和函数的外部链接。这意味着它们的定义可能位于其他文件中。这在编写跨文件程序时尤其关键，通过`extern`可以共享数据和函数，而无需重复定义它们。

### `extern` 关键字的主要用途

1. **共享全局变量**:
    - 允许某一文件中的全局变量可以被其他文件访问。
    - 避免重复定义同一变量，从而节省内存并确保数据一致。

2. **函数声明**:
    - 默认情况下，函数声明带有外部链接。因此，`extern`通常省略，因为声明函数时其默认链接类型就是外部链接。

3. **与 `extern "C"` 配合使用**:
    - 在C++中，使函数或变量按照C语言的方式进行链接，以避免编译器对名称进行修饰（name mangling）。

### 使用`extern`的示例

#### 共享全局变量

假设我们有两个文件：`file1.c` 和 `file2.c`，它们需要共享一个全局变量。

文件结构：
```
project/
|-- file1.c
|-- file2.c
|-- main.c
```

**在 `file1.c` 中定义全局变量:**

```c
// file1.c
#include <stdio.h>

int sharedVariable = 42;

void modifySharedVariable(int newValue) {
    sharedVariable = newValue;
    printf("Shared Variable modified to %d in file1.c\n", sharedVariable);
}
```

**在 `file2.c` 中声明并使用该全局变量:**

```c
// file2.c
#include <stdio.h>

extern int sharedVariable;

void printSharedVariable() {
    printf("Shared Variable in file2.c is %d\n", sharedVariable);
}
```

**主程序文件 `main.c`：**

```c
// main.c
#include <stdio.h>

// 声明在其他文件中实现的函数
void modifySharedVariable(int newValue);
void printSharedVariable();

int main() {
    printSharedVariable();  // 初始值应为42
    modifySharedVariable(100);
    printSharedVariable();  // 修改后的值应为100
    return 0;
}
```

#### 函数声明

在C/C++程序中，函数声明通常默认带有外部链接。因此，直接在头文件中声明函数时并不需要显式使用`extern`。

头文件 (`example.h`)
```c
#ifndef EXAMPLE_H
#define EXAMPLE_H

void exampleFunction(int a); // 默认情况下具有外部链接

#endif // EXAMPLE_H
```

源文件 (`example.c`)
```c
#include "example.h"
#include <stdio.h>

void exampleFunction(int a) {
    printf("Example function: %d\n", a);
}
```

主程序文件 (`main.c`)
```c
#include "example.h"

int main() {
    exampleFunction(5);
    return 0;
}
```

#### `extern "C"` 使用
在C++中，编译器会对函数名称进行修饰，以支持函数重载和其他特性。而C语言并不支持名称修饰，这就导致C++编译器生成的库函数无法与C代码直接链接。这时，我们可以使用 `extern "C"` 来避免名称修饰，使得函数可以被C和C++代码共同使用。

示例头文件 (`example.h`)
```c
#ifndef EXAMPLE_H
#define EXAMPLE_H

#ifdef __cplusplus
extern "C" {
#endif

void c_function(int a);

#ifdef __cplusplus
}
#endif

#endif // EXAMPLE_H
```

源文件 (`example.cpp`)
```cpp
#include "example.h"
#include <iostream>

// C风格函数实现，用extern "C"来避免名称修饰
extern "C" void c_function(int a) {
    std::cout << "C Function: " << a << std::endl;
}
```

主程序文件 (`main.c`)
```c
#include "example.h"

int main() {
    c_function(10);
    return 0;
}
```

## 静态变量
> [C++ keyword: static - cppreference.com](https://en.cppreference.com/w/cpp/keyword/static) 

### 初始化
在C++中，静态变量的初始化时机取决于变量的定义位置和类型。主要有两类静态变量：**静态局部变量**和**静态全局变量**（包括静态成员变量）。

#### 静态局部变量

静态局部变量是在函数或方法内部定义的，并且使用 `static` 关键字声明。它们的初始化时机是**第一次使用时**（即懒初始化）。

```cpp
#include <iostream>

void foo() {
    static int x = 0;  // 静态局部变量
    x++;
    std::cout << "x = " << x << std::endl;
}

int main() {
    foo();  // 第一次调用，x 被初始化为 0，然后递增为 1
    foo();  // 第二次调用，x 已经初始化，递增为 2
    return 0;
}
```

```cpp
x = 1
x = 2
```

#### 静态全局变量和静态成员变量
静态全局变量和静态成员变量的初始化时机是**程序启动时**（即静态初始化）。

```cpp
#include <iostream>

static int x = 0;  // 静态全局变量

int main() {
    std::cout << "x = " << x << std::endl;  // x 在程序启动时初始化为 0
    return 0;
}
```

```cpp
#include <iostream>

class MyClass {
public:
    static int x;  // 静态成员变量声明
};

// 静态成员变量定义和初始化
int MyClass::x = 0;

int main() {
    std::cout << "MyClass::x = " << MyClass::x << std::endl;  // x 在程序启动时初始化为 0
    return 0;
}
```

## static members
> [static members - cppreference.com](https://en.cppreference.com/w/cpp/language/static) 

Inside a class definition, the keyword static declares members that are not bound to class instances.
Outside a class definition, it has a different meaning: see storage duration.

A declaration for a static member is a member declaration whose declaration specifiers contain the keyword static. The keyword static usually appears before other specifiers (which is why the syntax is often informally described as static data-member or static member-function), but may appear anywhere in the specifier sequence.

static members of a class are not associated with the objects of the class: they are independent variables with static or thread(since C++11) storage duration or regular functions.

The static keyword is only used with the declaration of a static member, inside the class definition, but not with the definition of that static member:
```cpp
class X { static int n; }; // declaration (uses 'static')
int X::n = 1;              // definition (does not use 'static')
```

### static member function
- Static member functions are not associated with any object. When called, they have no this pointer.
- Static member functions cannot be virtual, const, volatile, or ref-qualified.
- The address of a static member function may be stored in a regular pointer to function, but not in a pointer to member function.

### static data member

Static data members are not associated with any object. They exist even if no objects of the class have been defined. There is only one instance of the static data member in the entire program with static storage duration, unless the keyword thread_local is used, in which case there is one such object per thread with thread storage duration(since C++11).

Static data members cannot be mutable.

Static data members of a class in namespace scope have external linkage if the class itself has external linkage (is not a member of unnamed namespace). Local classes (classes defined inside functions) and unnamed classes, including member classes of unnamed classes, cannot have static data members.

#### inline static data member
A static data member may be declared inline. An inline static data member can be defined in the class definition and may specify an initializer. It does not need an out-of-class definition:
```cpp
struct X
{
    inline static int fully_usable = 1; // No out-of-class definition required, ODR-usable
    inline static const std::string class_name{"X"}; // Likewise
 
    static const int non_addressable = 1; // C.f. non-inline constants, usable
                                          // for its value, but not ODR-usable
    // static const std::string class_name{"X"}; // Non-integral declaration of this
                                                 // form is disallowed entirely
};
```

#### constant static data member
If a static data member of integral or enumeration type is declared const (and not volatile), it can be initialized with an initializer in which every expression is a constant expression, right inside the class definition:
```cpp
struct X
{
    const static int n = 1;
    const static int m{2}; // since C++11
    const static int k;
};
const int X::k = 3;
```

## implicit conversions
> [Implicit conversions - cppreference.com](https://en.cppreference.com/w/cpp/language/implicit_conversion) 


## const_cast
> [const_cast conversion - cppreference.com](https://en.cppreference.com/w/cpp/language/const_cast) 

`const_cast`是C++中的一种类型转换运算符，用于修改类型的`const`或`volatile`属性。

`const_cast`的主要用途是添加或去除对象的`const`性（或`volatile`性），允许我们在需要时对`const`对象进行修改。需要注意的是，`const_cast`只能改变运算对象的`const`或`volatile`属性，不能改变其类型。

### 使用场景

- **去除`const`性**：当你想要修改一个被声明为`const`的变量时，可以使用`const_cast`去除其`const`性质。
- **添加`const`性**：在某些情况下，你可能需要将一个非`const`对象传递给只接受`const`参数的函数，此时可以使用`const_cast`添加`const`性。

### 注意事项
Pointers to functions and pointers to member functions are not subject to const_cast.

const_cast makes it possible to form a reference or pointer to non-const type that is actually referring to a const object or a reference or pointer to non-volatile type that is actually referring to a volatile object. Modifying a const object through a non-const access path and referring to a volatile object through a non-volatile glvalue results in undefined behavior.

- 使用`const_cast`去除`const`性并修改`const`对象是未定义行为，除非该对象本身不是`const`。换句话说，如果对象在创建时被声明为`const`，那么你不应该使用`const_cast`去除其`const`性并修改它。

### 示例

#### 去除`const`性

```cpp
#include <iostream>
 
struct type
{
    int i;
 
    type(): i(3) {}
 
    void f(int v) const
    {
        // this->i = v;                 // compile error: this is a pointer to const
        const_cast<type*>(this)->i = v; // OK as long as the type object isn't const
    }
};
 
int main()
{
    int i = 3;                 // i is not declared const
    const int& rci = i;
    const_cast<int&>(rci) = 4; // OK: modifies i
    std::cout << "i = " << i << '\n';
 
    type t; // if this was const type t, then t.f(4) would be undefined behavior
    t.f(4);
    std::cout << "type::i = " << t.i << '\n';
 
    const int j = 3; // j is declared const
    [[maybe_unused]]
    int* pj = const_cast<int*>(&j);
    // *pj = 4;      // undefined behavior
 
    [[maybe_unused]]
    void (type::* pmf)(int) const = &type::f; // pointer to member function
    // const_cast<void(type::*)(int)>(pmf);   // compile error: const_cast does
                                              // not work on function pointers
}
```

#### 添加`const`性

虽然`const_cast`更常用于去除`const`性，但理论上也可以用于添加`const`性。然而，实际上很少有这种需求，因为非`const`到`const`的转换是隐式的。

### 总结

`const_cast`是C++中用于修改对象`const`或`volatile`属性的类型转换运算符。它应谨慎使用，主要用于特殊情况，如与旧C代码的兼容或在确实需要修改`const`对象时。在现代C++编程实践中，应尽量避免使用`const_cast`，并通过设计来保证类型安全和`const`正确性。

1. **违反原有的只读约定**：很多时候，对象被声明为`const`是有其设计上的理由的，表明这个对象或者是它的某些部分不应该被修改，以确保程序的正确性或者对象的状态安全。`const_cast`去除`const`属性并修改内容，意味着你在侵犯这种设计约定，可能会引起程序逻辑错误或不稳定。

2. **触发未定义的行为（Undefined Behavior）**：如果对由`const`对象派生而来的指针或引用去`const`后进行修改，而这个对象本身是在编译时被定义为不可变的，这将会导致未定义的行为。C++标准指出，修改一个本身是常量的值是未定义的行为。这意味着编译器可能会对原始常量进行优化（例如，将它们放入只读存储段），这种修改尝试可能导致程序崩溃。

3. **潜在的内存安全问题**：对某些因内部实现细节而被声明为`const`的对象内容进行修改，可能会破坏对象的内部结构，引起内存泄漏、数据损坏等问题。比如，修改一个被`const`修饰的内部缓存，可能会导致不一致性，从而触发程序运行时错误。

综上所述，虽然`const_cast`可以从语法层面去除对象的常量属性，使得编译时不会报错，但在运行时尝试修改那些应该是只读的数据仍然是非常危险的行为，可能会导致不可预料的后果。因此，只有在绝对确定去除`const`属性不会引发上述问题的情况下，才可以使用`const_cast`进行操作。

## static_cast 和 reinterpret_cast
> [static_cast conversion - cppreference.com](https://en.cppreference.com/w/cpp/language/static_cast) 
> [reinterpret_cast conversion - cppreference.com](https://en.cppreference.com/w/cpp/language/reinterpret_cast) 

`static_cast` 是在编译时进行的类型转换，它执行非多态类型的安全转换。这种转换依赖于编译时的类型信息，不涉及运行时的类型检查。`static_cast` 通常用于以下几种情况：
1. **基本数据类型之间的转换**：如将 `int` 转换为 `float`，或者在不同枚举类型之间转换。
2. **指针类型之间的转换**：派生类的指针转换为基类的指针，前提是这些类之间存在继承关系。
3. **算术类型和指针类型之间的转换**：如将 `void*` 转换为其他类型的指针。
4. **用户定义的转换**：如果类定义了转换运算符，`static_cast` 可以用来调用这些运算符。

`reinterpret_cast` 是一种低级别的转换，它用于进行解释方式的转换，即无视类型的字节内容的重新解释。这种转换不安全，因为它不进行任何类型检查，转换的结果完全取决于程序员的意图。`reinterpret_cast` 常用于以下几种情况：
Unlike static_cast, but like const_cast, the reinterpret_cast expression does not compile to any CPU instructions (except when converting between integers and pointers, or between pointers on obscure architectures where pointer representation depends on its type). It is primarily a compile-time directive which instructs the compiler to treat expression as if it had the type target-type.

1. **指针和足够大的整数类型之间的转换**：如将指针转换为整数，或者将整数转换为指针。
2. **指针类型之间的转换**：如将一种类型的指针转换为另一种类型的指针，无论这两种类型是否相关。
3. **指针和成员指针类型之间的转换**：如将普通指针转换为成员指针，或者反过来。
4. **函数指针类型之间的转换**：如将一个函数指针转换为另一个不相关的函数指针类型。

## static_cast 和 dynamic_cast 
> [dynamic_cast conversion - cppreference.com](https://en.cppreference.com/w/cpp/language/dynamic_cast) 

在 C++ 中，`static_cast` 和 `dynamic_cast` 是两种、操作符，它们有以下区别：

1. `static_cast`：
   - `static_cast` 在编译时进行类型转换，不进行运行时类型检查。
   - `static_cast` 可以用于基本数据类型之间的转换，以及具有继承关系的指针或引用类型之间的转换。
   - `static_cast` 可以用于将指针或引用从派生类转换为基类，或者从基类转换为派生类，但是不进行动态类型检查，因此可能导致不安全的转换。
   - `static_cast` 无法用于执行安全的向下转换（即将基类指针或引用转换为派生类指针或引用），因为它不进行运行时类型检查。

2. `dynamic_cast`：
   - `dynamic_cast` 在运行时进行类型转换，并且会进行类型检查，以确保转换的安全性。
   - `dynamic_cast` 主要用于具有继承关系的类之间的指针或引用类型转换。
   - `dynamic_cast` 可以用于将指针或引用从派生类转换为基类，或者从基类转换为派生类，并且会进行运行时类型检查，如果转换不安全，则返回空指针（对指针进行转换）或抛出 `std::bad_cast` 异常（对引用进行转换）。
   - `dynamic_cast` 只能在具有虚函数的类层次结构中使用，因为它依赖于运行时类型信息（RTTI）。

总的来说，`static_cast` 是一种较为简单的类型转换操作符，适用于一些基本的类型转换，但不进行运行时类型检查；而 `dynamic_cast` 则是一种更加安全的类型转换操作符，适用于具有继承关系的类之间的转换，并且会进行运行时类型检查以确保转换的安全性。

在 C++ 中，将派生类指针或引用转换为基类指针或引用（即派生类向基类的转换）是安全的。这是因为派生类对象中包含了基类对象的部分，而且派生类对象的地址通常与基类对象的地址相同。

当进行派生类向基类的转换时，编译器会调整指针或引用的偏移量，以指向基类对象的部分。因此，这种转换是安全的，不会导致内存访问越界或其他问题。

这种转换通常用于以下情况：
- 在继承关系中，需要将派生类对象传递给接受基类对象的函数或方法。
- 在继承关系中，需要将派生类对象赋值给基类指针或引用。

需要注意的是，尽管派生类向基类的转换是安全的，但在进行基类向派生类的转换时，应该使用 `dynamic_cast` 进行类型检查，以确保转换的安全性。基类向派生类的转换只有在对象实际是派生类对象时才是安全的。

### dynamic_cast 示例
```cpp
#include <iostream>
 
struct V
{
    virtual void f() {} // must be polymorphic to use runtime-checked dynamic_cast
};
 
struct A : virtual V {};
 
struct B : virtual V
{
    B(V* v, A* a)
    {
        // casts during construction (see the call in the constructor of D below)
        dynamic_cast<B*>(v); // well-defined: v of type V*, V base of B, results in B*
        dynamic_cast<B*>(a); // undefined behavior: a has type A*, A not a base of B
    }
};
 
struct D : A, B
{
    D() : B(static_cast<A*>(this), this) {}
};
 
struct Base
{
    virtual ~Base() {}
};
 
struct Derived : Base
{
    virtual void name() {}
};
 
int main()
{
    D d; // the most derived object
    A& a = d; // upcast, dynamic_cast may be used, but unnecessary
 
    [[maybe_unused]]
    D& new_d = dynamic_cast<D&>(a); // downcast
    [[maybe_unused]]
    B& new_b = dynamic_cast<B&>(a); // sidecast
 
    Base* b1 = new Base;
    if (Derived* d = dynamic_cast<Derived*>(b1); d != nullptr)
    {
        std::cout << "downcast from b1 to d successful\n";
        d->name(); // safe to call
    }
 
    Base* b2 = new Derived;
    if (Derived* d = dynamic_cast<Derived*>(b2); d != nullptr)
    {
        std::cout << "downcast from b2 to d successful\n";
        d->name(); // safe to call
    }
 
    delete b1;
    delete b2;
}
```


## class
> [Classes - cppreference.com](https://en.cppreference.com/w/cpp/language/classes) 

### class declaration
> [Class declaration - cppreference.com](https://en.cppreference.com/w/cpp/language/class) 

### polymorphic
> [Object - cppreference.com](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects) 

A class with at least one declared or inherited virtual member function is polymorphic. Objects of this type are polymorphic objects and have runtime type information stored as part of the object representation, which may be queried with dynamic_cast and typeid. Virtual member functions participate in dynamic binding.

### constuctor
> [Constructors and member initializer lists - cppreference.com](https://en.cppreference.com/w/cpp/language/constructor) 

- Constructors have no names and cannot be called directly. 

#### Member initializer list
> [Constructors and member initializer lists - cppreference.com](https://en.cppreference.com/w/cpp/language/constructor) 

#### 成员变量的初始化
> [Default-initialization - cppreference.com](https://en.cppreference.com/w/cpp/language/default_initialization) 

类的非静态成员变量，在类定义时如果没有给初始化值，并且在构造函数初始化列表中也没有赋值，则会进行 default initialization。
对于基本数据类型，默认初始化是未定义的值。
如果是数组，则数组的每个元素都进行默认初始化。

#### 构造顺序
> [Constructors and member initializer lists - cppreference.com](https://en.cppreference.com/w/cpp/language/constructor#Initialization_order) 

The order of member initializers in the list is irrelevant: the actual order of initialization is as follows:
1) If the constructor is for the most-derived class, virtual bases are initialized in the order in which they appear in depth-first left-to-right traversal of the base class declarations (left-to-right refers to the appearance in base-specifier lists).
2) Then, direct bases are initialized in left-to-right order as they appear in this class's base-specifier list.
3) Then, non-static data member are initialized in order of declaration in the class definition.
4) Finally, the body of the constructor is executed.
(Note: if initialization order was controlled by the appearance in the member initializer lists of different constructors, then the destructor wouldn't be able to ensure that the order of destruction is the reverse of the order of construction.)

#### explicit 
> [explicit specifier - cppreference.com](https://en.cppreference.com/w/cpp/language/explicit) 

在 C++ 中，`explicit` 关键字用于类中的构造函数，它阻止了编译器使用该构造函数进行类型转换。这可以防止编译器在某些情况下进行隐式转换，从而提高代码的清晰度和安全性。

##### 作用

1. **防止隐式转换**：当一个类的构造函数被标记为 `explicit` 时，它不能用于隐式转换。这意味着编译器不会自动将一个对象转换成该类型的实例，除非显式地进行转换。

2. **提高代码清晰度**：通过阻止隐式转换，`explicit` 关键字使得代码的意图更加明确，减少了因隐式转换引起的意外错误。

3. **增强类型安全**：`explicit` 关键字可以防止编译器在不适当的时候进行类型转换，从而增强了程序的类型安全性。

##### 使用场景
The explicit specifier may only appear within the decl-specifier-seq of the declaration of a constructor or conversion function(since C++11) within its class definition.

1. **构造函数**：当你有一个构造函数时，如果不希望它被用于隐式转换，可以将其标记为 `explicit`。

   ```cpp
   class MyClass {
   public:
       explicit MyClass(int value) : m_value(value) {}
   private:
       int m_value;
   };
   ```

   在这个例子中，`MyClass` 的构造函数是显式的，因此不能在需要 `MyClass` 类型的地方隐式地使用 `int` 类型的值。

2. **转换运算符**：如果你不希望类提供隐式转换功能，可以将转换运算符声明为 `explicit`。

   ```cpp
   class MyClass {
   public:
       explicit operator bool() const { return m_value != 0; }
   private:
       int m_value;
   };
   ```

   在这个例子中，`MyClass` 类型的实例不能隐式地转换为 `bool` 类型。

##### 示例
```cpp
struct A
{
    A(int) {}      // converting constructor
    A(int, int) {} // converting constructor (C++11)
    operator bool() const { return true; }
};
 
struct B
{
    explicit B(int) {}
    explicit B(int, int) {}
    explicit operator bool() const { return true; }
};
 
int main()
{
    A a1 = 1;      // OK: copy-initialization selects A::A(int)
    A a2(2);       // OK: direct-initialization selects A::A(int)
    A a3 {4, 5};   // OK: direct-list-initialization selects A::A(int, int)
    A a4 = {4, 5}; // OK: copy-list-initialization selects A::A(int, int)
    A a5 = (A)1;   // OK: explicit cast performs static_cast
    if (a1) { }    // OK: A::operator bool()
    bool na1 = a1; // OK: copy-initialization selects A::operator bool()
    bool na2 = static_cast<bool>(a1); // OK: static_cast performs direct-initialization
 
//  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
    B b2(2);       // OK: direct-initialization selects B::B(int)
    B b3 {4, 5};   // OK: direct-list-initialization selects B::B(int, int)
//  B b4 = {4, 5}; // error: copy-list-initialization does not consider B::B(int, int)
    B b5 = (B)1;   // OK: explicit cast performs static_cast
    if (b2) { }    // OK: B::operator bool()
//  bool nb1 = b2; // error: copy-initialization does not consider B::operator bool()
    bool nb2 = static_cast<bool>(b2); // OK: static_cast performs direct-initialization
 
    [](...){}(a4, a5, na1, na2, b5, nb2); // suppresses “unused variable” warnings
}
```

#### converting constructor
> [Converting constructor - cppreference.com](https://en.cppreference.com/w/cpp/language/converting_constructor) 

A constructor that is not declared with the specifier explicit and which can be called with a single parameter(until C++11) is called a converting constructor.

Unlike explicit constructors, which are only considered during direct initialization (which includes explicit conversions such as static_cast), converting constructors are also considered during copy initialization, as part of user-defined conversion sequence.

It is said that a converting constructor specifies an implicit conversion from the types of its arguments (if any) to the type of its class. Note that non-explicit user-defined conversion function also specifies an implicit conversion.

Implicitly-declared and user-defined non-explicit copy constructors and move constructors are converting constructors.

```cpp
struct A
{
    A() { }         // converting constructor (since C++11)  
    A(int) { }      // converting constructor
    A(int, int) { } // converting constructor (since C++11)
};
 
struct B
{
    explicit B() { }
    explicit B(int) { }
    explicit B(int, int) { }
};
 
int main()
{
    A a1 = 1;      // OK: copy-initialization selects A::A(int)
    A a2(2);       // OK: direct-initialization selects A::A(int)
    A a3{4, 5};    // OK: direct-list-initialization selects A::A(int, int)
    A a4 = {4, 5}; // OK: copy-list-initialization selects A::A(int, int)
    A a5 = (A)1;   // OK: explicit cast performs static_cast, direct-initialization
 
//  B b1 = 1;      // error: copy-initialization does not consider B::B(int)
    B b2(2);       // OK: direct-initialization selects B::B(int)
    B b3{4, 5};    // OK: direct-list-initialization selects B::B(int, int)
//  B b4 = {4, 5}; // error: copy-list-initialization selected an explicit constructor
                   //        B::B(int, int)
    B b5 = (B)1;   // OK: explicit cast performs static_cast, direct-initialization
    B b6;          // OK, default-initialization
    B b7{};        // OK, direct-list-initialization
//  B b8 = {};     // error: copy-list-initialization selected an explicit constructor
                   //        B::B()
 
    [](...){}(a1, a4, a4, a5, b5); // may suppress "unused variable" warnings
}
```

#### default constructor
> [Default constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/default_constructor) 

A default constructor is a constructor which can be called with no arguments.

Default constructors are called during default initializations and value initializations.

If there is no user-declared constructor or constructor template for a class type, the compiler will implicitly declare a default constructor as an inline public member of its class.

默认构造函数是 C++ 类中一种特殊的构造函数，它不接受任何参数。它的主要作用是在创建对象时初始化对象的成员变量。如果一个类中没有显式定义任何构造函数，编译器会隐式地生成一个默认构造函数。这个默认构造函数的行为取决于类中的成员变量。

1. **成员初始化**：
   - 如果成员变量是基本数据类型（如 `int`、`double` 等），它们不会被自动初始化为零。这意味着它们会被初始化为一个未定义的值。
   - 如果成员变量是类类型，那么它们的默认构造函数（如果存在）会被调用。
   - 如果成员变量是指针类型，它们会被初始化为 `nullptr`。

2. **隐式定义**：
   - 如果类中定义了任何显式的构造函数，编译器不会自动生成默认构造函数，除非显式地使用 `default` 关键字标记。

3. **调用顺序**：
   - 首先调用基类的默认构造函数（如果有的话）。
   - 然后按照成员变量在类中声明的顺序调用它们的构造函数。

##### 使用 `default` 关键字
在 C++11 及以后的版本中，可以使用 `default` 关键字显式要求编译器生成默认构造函数。这在类中已经定义了其他构造函数时特别有用，例如：

```cpp
class MyClass {
public:
    MyClass() = default; // 显式要求编译器生成默认构造函数
};
```

##### 默认构造函数与对象数组
当声明一个对象数组时，如 `MyClass myObjects[10];`，编译器会调用默认构造函数来初始化数组中的每个元素。


##### 示例
```cpp
struct A
{
    int x;
    A(int x = 1): x(x) {} // user-defined default constructor
};
 
struct B : A
{
    // B::B() is implicitly-defined, calls A::A()
};
 
struct C
{
    A a;
    // C::C() is implicitly-defined, calls A::A()
};
 
struct D : A
{
    D(int y) : A(y) {}
    // D::D() is not declared because another constructor exists
};
 
struct E : A
{
    E(int y) : A(y) {}
    E() = default; // explicitly defaulted, calls A::A()
};
 
struct F
{
    int& ref; // reference member
    const int c; // const member
    // F::F() is implicitly defined as deleted
};
 
// user declared copy constructor (either user-provided, deleted or defaulted)
// prevents the implicit generation of a default constructor
 
struct G
{
    G(const G&) {}
    // G::G() is implicitly defined as deleted
};
 
struct H
{
    H(const H&) = delete;
    // H::H() is implicitly defined as deleted
};
 
struct I
{
    I(const I&) = default;
    // I::I() is implicitly defined as deleted
};
 
int main()
{
    A a;
    B b;
    C c;
//  D d; // compile error
    E e;
//  F f; // compile error
//  G g; // compile error
//  H h; // compile error
//  I i; // compile error
}
```

#### copy constructor
> [Copy constructors - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_constructor) 
> [Copy Constructor in C++ - GeeksforGeeks](https://www.geeksforgeeks.org/copy-constructor-in-cpp/) 

A copy constructor is a constructor which can be called with an argument of the same class type and copies the content of the argument without mutating the argument.

If no user-defined copy constructors are provided for a class type, the compiler will always declare a copy constructor as a non-explicit inline public member of its class. 

A class can have multiple copy constructors, e.g. both T::T(const T&) and T::T(T&).

The copy constructor is called whenever an object is initialized (by direct-initialization or copy-initialization) from another object of the same type (unless overload resolution selects a better match or the call is elided), which includes: 
- initialization: T a = b; or T a(b);, where b is of type T;
- function argument passing: f(a);, where a is of type T and f is void f(T t);
- function return: return a; inside a function such as T f(), where a is of type T, which has no move constructor.

```cpp
struct X
{
    X(X& other); // copy constructor
//  X(X other);  // Error: incorrect parameter type
};
 
union Y
{
    Y(Y& other, int num = 1); // copy constructor with multiple parameters
//  Y(Y& other, int num);     // Error: `num` has no default argument
};
```

拷贝构造的参数不能是值传递，因为如果拷贝构造函数的参数是通过值传递的，那么每次调用拷贝构造函数时，都需要创建一个参数对象的副本。由于这个副本的创建本身又会调用拷贝构造函数，这样就形成了无限递归。这不仅会导致栈溢出，而且根本就没有意义，因为拷贝构造函数的目的就是避免不必要的对象复制。

如果拷贝构造函数还有其他参数，则其他参数需要有默认值。

#### copy assignment
> [Copy assignment operator - cppreference.com](https://en.cppreference.com/w/cpp/language/copy_assignment) 

拷贝赋值运算符是 C++ 中的一个成员函数，它用于将一个对象的内容赋值给另一个已经存在的对象。它的一般形式如下：
```cpp
ClassType& ClassType::operator=(const ClassType& other) {
    // ... 赋值操作 ...
    return *this;
}
```
这里的 `ClassType` 是你的类名，`other` 是一个对另一个同类型对象的引用，该对象的内容将被复制到调用赋值运算符的对象中。

拷贝赋值运算符的主要作用是：
1. **复制数据**：将一个对象的状态复制到另一个对象中。
2. **实现对象的赋值**：允许使用赋值运算符 `=` 来将一个对象赋予另一个对象。
3. **处理自我赋值**：如果赋值运算符没有正确处理自我赋值（self-assignment），即对象赋值给自己，可能会导致内存泄漏或多次释放同一资源等问题。
自我赋值是一个特殊的情况，当两个指向同一对象的指针或引用相互赋值时，如果不检查自我赋值，可能会导致问题。处理自我赋值的一种常见方法是：
```cpp
ClassType& ClassType::operator=(const ClassType& other) {
    if (this != &other) {
        // 清理现有资源
        // 复制新资源
    }
    return *this;
}
```

实现步骤：
1. **检查自我赋值**：首先检查是否是自我赋值，如果是，则直接返回。
2. **释放资源**：如果当前对象拥有动态分配的资源，应该先释放这些资源。
3. **复制资源**：复制 `other` 对象的资源到当前对象。
4. **返回当前对象的引用**：返回 `*this`。

```cpp
class MyClass {
public:
    // 拷贝赋值运算符
    MyClass& operator=(const MyClass& other) {
        if (this != &other) {
            delete[] data; // 释放原有资源
            data = new int[other.size]; // 复制资源
            size = other.size;
            std::copy(other.data, other.data + other.size, data);
        }
        return *this;
    }

private:
    int* data;
    size_t size;
};
```

##### Implicitly-declared copy assignment operator
If no user-defined copy assignment operators are provided for a class type, the compiler will always declare one as an inline public member of the class. 

Because the copy assignment operator is always declared for any class, the base class assignment operator is always hidden. If a using-declaration is used to bring in the assignment operator from the base class, and its argument type could be the same as the argument type of the implicit assignment operator of the derived class, the using-declaration is also hidden by the implicit declaration.

##### 自定义拷贝赋值
> [21.12 — Overloading the assignment operator – Learn C++](https://www.learncpp.com/cpp-tutorial/overloading-the-assignment-operator/) 

注释动态资源的分配和释放。

##### 示例
```cpp
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
 
struct A
{
    int n;
    std::string s1;
 
    A() = default;
    A(A const&) = default;
 
    // user-defined copy assignment (copy-and-swap idiom)
    A& operator=(A other)
    {
        std::cout << "copy assignment of A\n";
        std::swap(n, other.n);
        std::swap(s1, other.s1);
        return *this;
    }
};
 
struct B : A
{
    std::string s2;
    // implicitly-defined copy assignment
};
 
struct C
{
    std::unique_ptr<int[]> data;
    std::size_t size;
 
    // user-defined copy assignment (non copy-and-swap idiom)
    // note: copy-and-swap would always reallocate resources
    C& operator=(const C& other)
    {
        if (this != &other) // not a self-assignment
        {
            if (size != other.size) // resource cannot be reused
            {
                data.reset(new int[other.size]);
                size = other.size;
            }
            std::copy(&other.data[0], &other.data[0] + size, &data[0]);
        }
        return *this;
    }
};
 
int main()
{
    A a1, a2;
    std::cout << "a1 = a2 calls ";
    a1 = a2; // user-defined copy assignment
 
    B b1, b2;
    b2.s1 = "foo";
    b2.s2 = "bar";
    std::cout << "b1 = b2 calls ";
    b1 = b2; // implicitly-defined copy assignment
 
    std::cout << "b1.s1 = " << b1.s1 << "; b1.s2 = " << b1.s2 << '\n';
}
```

### desctructors
> [Destructors - cppreference.com](https://en.cppreference.com/w/cpp/language/destructor) 

A destructor is a special member function that is called when the lifetime of an object ends. The purpose of the destructor is to free the resources that the object may have acquired during its lifetime.

A destructor cannot be a coroutine.

The destructor is implicitly invoked whenever an object's lifetime ends, which includes:
- program termination, for objects with static storage duration
- thread exit, for objects with thread-local storage duration (since C++11)
- end of scope, for objects with automatic storage duration and for temporaries whose life was extended by binding to a reference
- delete-expression, for objects with dynamic storage duration
- end of the full expression, for nameless temporaries
- stack unwinding, for objects with automatic storage duration when an exception escapes their block, uncaught.

The destructor can also be invoked explicitly.

If no user-declared prospective(since C++20) destructor is provided for a class type, the compiler will always declare a destructor as an inline public member of its class.

#### 析构顺序
和构造顺序相反。

#### 虚析构函数
Deleting an object through pointer to base invokes undefined behavior unless the destructor in the base class is virtual:
```cpp
class Base
{
public:
    virtual ~Base() {}
};
 
class Derived : public Base {};
 
Base* b = new Derived;
delete b; // safe
```

在C++中，如果一个类继承自另一个类（基类），并且基类有一个非虚的析构函数，那么通过基类的指针来删除派生类的对象会导致问题。因为非虚析构函数不会调用派生类的析构函数，导致派生类中的资源可能不会被正确释放，从而引发内存泄漏或其他未定义行为。

为了避免这个问题，基类的析构函数应该被声明为虚函数。这样，当通过基类的指针删除派生类的对象时，首先会调用派生类的析构函数，然后再调用基类的析构函数，确保资源被正确释放。

#### 纯虚析构函数
A prospective(since C++20) destructor may be declared pure virtual, for example in a base class which needs to be made abstract, but has no other suitable functions that could be declared pure virtual. A pure virtual destructor must have a definition, since all base class destructors are always called when the derived class is destroyed:
```cpp
class AbstractBase
{
public:
    virtual ~AbstractBase() = 0;
};
AbstractBase::~AbstractBase() {}
 
class Derived : public AbstractBase {};
 
// AbstractBase obj; // compiler error
Derived obj;         // OK
```

#### 析构函数的访问级别
1. **析构函数是 `private`**：
   如果析构函数被声明为 `private`，那么它只能在类的内部成员函数或友元函数中被调用。这通常用于防止外部代码直接删除对象，确保对象的生命周期由类本身管理。

2. **非静态局部对象**：
   析构函数是 private，则编译会提示错误。
   在 C++ 中，普通对象（局部对象）的生命周期结束时，它们的析构函数会被自动调用。这一过程是由编译器生成的代码来管理的，编译器会在对象的作用域结束时插入析构函数的调用。如果一个对象的析构函数是 private 的，编译器生成的代码在尝试调用它时会遇到访问权限的问题，因为编译器生成的代码相当于是在类外部调用析构函数，这违反了类的封装性和访问控制规则，会导致编译错误。 (待验证？？)

3. **动态分配的对象**：
   对于动态分配的对象（使用 `new` 关键字创建的对象），即使析构函数是 `private` 的，也必须手动使用 `delete` 来调用析构函数。如果不这样做，对象的析构函数不会自动被调用，从而导致资源泄漏。

4. **静态局部对象**：
   析构函数是 private，程序结束后也会调用析构函数。(示例：[C++ Singleton - HackMD](https://hackmd.io/@WesleyCh3n/S1Rlm1kf9))
   对于静态对象，它们的生命周期与程序的运行期间是一致的。静态对象的析构函数通常在程序结束时由运行时系统调用。即使析构函数是 private 的，运行时系统也能调用它，因为这种调用不受常规访问控制的限制。这是因为静态对象的析构函数调用是由运行时系统在程序退出时自动进行的，它不受类内部访问控制的限制。(待验证？？)
   The destructor for a block variable with static storage duration is called at program exit, but only if the initialization took place successfully. ([Storage class specifiers - cppreference.com](https://en.cppreference.com/w/cpp/language/storage_duration))

#### 注意
Calling a destructor directly for an ordinary object, such as a local variable, invokes undefined behavior when the destructor is called again, at the end of scope.

如果手动调用析构函数后，作用域结束时再次自动调用析构函数，会导致未定义行为。

## 生存期
> [Lifetime - cppreference.com](https://en.cppreference.com/w/cpp/language/lifetime) 
> [生存期 - C++中文 - API参考文档](https://www.apiref.com/cpp-zh/cpp/language/lifetime.html) 
> [https://www.syntagm.co.uk/design/articles/exolmcpp.pdf](https://www.syntagm.co.uk/design/articles/exolmcpp.pdf) 

## RAII 资源获取即初始化
> [RAII - cppreference.com](https://en.cppreference.com/w/cpp/language/raii) 

## 内存管理
> [虚拟内存](https://xiaolincoding.com/os/3_memory/vmem.html#虚拟内存)


# object file
> [Getting Title at 18:47](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第七章%20Linking.md) 

# C 和 C++ 混合编程

# c++ 程序优化
> [CSAPP-5-程序优化](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第五章%20Optimizing%20Program%20Performance.md)

## 尽量不要在循环内做重复的计算
```cpp
// 假设我们要计算数组中所有元素的总和，并乘以一个常数因子
int sum = 0;
int constantFactor = 5; // 一个简单的常数因子
for (int i = 0; i < n; i++) {
    sum += array[i] * constantFactor; // 在循环中重复计算
}
```

在上面的例子中，`constantFactor`是一个不变的值，每次循环时`array[i] * constantFactor`都会被重复计算。虽然这个计算本身很简单，但如果循环次数非常多，或者计算更复杂，这将会成为性能的瓶颈。

优化后的代码，可以将重复计算移出循环，如下所示：
```cpp
int constantFactor = 5; // 常数因子
int sum = 0;
for (int i = 0; i < n; i++) {
    sum += array[i];
}
sum *= constantFactor; // 将重复计算移至循环外部
```

通过这种优化，我们只计算一次`sum * constantFactor`，而不是在每次循环迭代中都计算一次。这样可以减少执行时间，尤其是在处理大量数据或计算复杂时，优化效果会更加明显。

## 循环内尽量减少内存引用
> [CSAPP-第四章-pipeline](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第四章%20Processor%20Architecture.md#44-general-principles-of-pipelining)

在计算机体系结构中的pipeline（流水线）技术里，每个阶段都是由特定的硬件组件实现的。虽然具体的硬件实现可能因不同的CPU设计而异，下面是一个典型的基于5阶段流水线的硬件组件实现概览：

1. **取指阶段（Instruction Fetch, IF）**：
    - **指令寄存器（Instruction Register）**：临时存储当前正在获取的指令。
    - **程序计数器（Program Counter, PC）**：指向下一条要执行指令的内存地址。
    - **指令缓存（Instruction Cache）**：减少从主内存读取指令的延迟。

2. **译码阶段（Instruction Decode, ID）**：
    - **控制单元（Control Unit）**：解析指令操作码，生成用于管控其他组件操作的控制信号。
    - **寄存器文件（Register File）**：存储CPU中的寄存器，控制单元从中获取指令中引用的寄存器值。

3. **执行阶段（Execute, EX）**：
    - **算术逻辑单元（Arithmetic Logic Unit, ALU）**：执行算术与逻辑操作。
    - **移位器（Shifter）**：进行位移操作。
    - **乘除法单元（Multiplication and Division Unit）**：处理乘法和除法运算，可能并非所有CPU都有专用硬件。

4. **访存阶段（Memory Access, MEM）**：
    - **数据缓存（Data Cache）**：减少从主内存读写数据的延迟。
    - **内存管理单元（Memory Management Unit, MMU）**：处理虚拟地址到物理地址的转换。

5. **写回阶段（Write Back, WB）**：
    - **寄存器文件（Register File）**：将执行结果写回到指定的寄存器。
  
局部变量会尽量存储在 CPU 的寄存器中，放不下时再放到内存的栈中，如果在循环内访问一个变量，该变量是对外部变量的引用，那么每次该变量的值更新时会将值写入到内存中更新，防止其他地方有对该变量的引用，这样每次都要访问内存。而编译器无法预知是否其他地方会有内存的引用，因此不会自动优化。
如果循环内只用局部变量，那么可以只将值更新到寄存器中，提高效率。

```cpp
int globalValue = 0; // 假设这是一个全局变量

void UpdateValue(int &ref) {
    for (int i = 0; i < 1000000; ++i) {
        ref = i; // 这里每次循环都会将修改后的值写入内存中的ref所指向的地址
    }
}

int main() {
    UpdateValue(globalValue); // 传递引用到函数中
    return 0;
}
```
在这个例子中，每次循环都需要将`i`的值赋给引用`ref`，这实际上是对`globalValue`的更新操作，需要写入内存。

如果我们改用局部变量，就可以减少内存访问，如下：
```cpp
void UpdateValue() {
    int localValue = 0; // 这里使用一个局部变量
    for (int i = 0; i < 1000000; ++i) {
        localValue = i; // 这里更新的是寄存器中的值，不需要每次都写入内存
    }
    globalValue = localValue; // 只在循环结束后更新一次内存
}

int main() {
    UpdateValue(); // 不使用引用，内部采用局部变量
    return 0;
}
```

在这个优化后的版本中，我们只需要在寄存器层面更新`localValue`的值，最后只在循环结束后将最终的值写入`globalValue`。这样就减少了内存的访问频率，提高了程序的运行效率。

*******************

在循环中使用外部变量的引用时，编译器可能无法优化以避免将值写回内存，主要是因为可能存在对这个变量的别名化（aliasing）问题。别名化发生在当两个或两个以上的不同的指针或引用可能会指向相同的内存位置时，编译器无法保证这些引用或指针之间的独立性。

为了保证程序的正确性，当一个引用被用作循环的一部分，并且循环体可能被重复执行多次时，编译器必须保证每次变量的更新都能实时反映到内存中，这样其他引用到该变量的代码才能看到最新的值。特别是在以下情况中：

1. 如果循环中存在函数调用，并且该函数可能会访问或修改这个引用变量，编译器必须在每次调用之前或之后将变量的值写回内存。
2. 如果程序是多线程的，那么任何时候线程都可能需要访问最新的变量值。由于编译器不能保证不会有其他线程在访问同一个内存地址，所以它必须更新内存中的值，以防止数据竞争和一致性问题。
3. 如果存在中断服务例程或信号处理程序访问这个引用变量，编译器同样无法保证这些处理程序何时被调用，必须确保内存中的值是最新的。

因此，为了确保程序表现出的行为与程序员的意图一致，编译器在处理循环中引用外部变量时会倾向于保守策略，避免过度优化导致的潜在错误，它会选择在每次循环迭代后将变量的值写回内存。这种做法虽然可能会牺牲一些性能，但却能够保证程序的正确性和一致性。

***************

在循环中使用全局变量时，每次修改全局变量的值可能会写入内存中，但这并不总是必须的，这取决于编译器的优化。编译器有时会对代码进行优化，将全局变量的值缓存在寄存器中以提高性能，只有在需要时才写回内存。这种情况通常在循环的迭代中发生，其中编译器检测到通过使用寄存器可以提高访问速度和效率。

但如果程序中存在导致全局变量值需要在迭代间同步的因素（例如，可能有多线程或中断服务例程访问该全局变量），编译器通常会选择在每次修改后将变量写回内存中，以确保数据的一致性和正确性。

总的来说，是否将全局变量的值写回内存是由编译器的优化策略决定的，这会考虑到代码的具体情况和上下文需求。如果编译器能确定全局变量不会被其他线程或中断处理程序访问，它可能会选择在循环的多次迭代中缓存这个变量的值。但是如果有并发性要求或对变量实时性的特别要求，那么变量会被频繁地写回内存以确保数据的一致性。

## 减少循环内函数的调用
> [CSAPP-第三章-stack frame](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第三章%20Machine-Level%20Representation%20of%20Programs.md#371-the-run-time-stack)  
  
**栈帧（stack frame）**：栈上用于特定 `call` 的每个内存块成为栈帧（It is a frame for a particular instance of a procedure, a particular call to a procedure）。  
  
栈帧（Stack Frame）是一个函数调用和执行的上下文，是在程序运行时栈上分配的结构。每当一个函数被调用时，一个栈帧就会被创建，用于存储这个函数运行所需的信息。一个栈帧通常包括：  
  
1. 函数的返回地址：当函数执行完毕后程序控制权需要返回到调用函数的地方，因此需要保存一个返回指针。  
2. 参数：函数调用时的参数可以通过栈帧传递。  
3. 局部变量：函数内部定义的局部变量会存储在自己的栈帧中。  
4. 保存的寄存器值：在调用函数前，可能需要保存一些寄存器的值，这些保存的寄存器值也存储在栈帧中，以便在函数返回后能够恢复这些值。  
5. 在x86_64体系结构中使用的函数栈帧，两个寄存器指针——栈指针(%rsp)和基指针(%rbp)来进行分隔和定位的。%rsp（Stack Pointer Register）指向栈的顶部，是当前栈帧的结束位置，而%rbp（Base Pointer Register）则指向栈帧的底部，也就是当前函数栈帧的开始位置。  
调用一个函数时，首先将当前的%rsp的值保存下来，然后将%rbp设置为当前%rsp的值，这样栈帧的底部就被设置好了。随后，%rsp会移动来为这个函数的局部变量和其他数据分配空间。所以在一个函数运行过程中，%rsp可能会不断变动，但%rbp会保持不变，这样无论%rsp如何改变，我们都能通过%rbp找到栈帧的原点。这种做法使得函数中的数据操作变得稳定可靠。  
  
整个过程遵循栈（LIFO）的规则，每调用一个新的函数，就创建一个新的栈帧在栈顶，每当函数调用结束，其栈帧就会被销毁，并释放相应的栈空间。  
  
需要在**栈**中为每个**被调用且未返回的过程**保留一个**栈帧**。  
      
通常一个**栈帧**由**两个指针**分隔，一个是**栈指针**（指向栈顶），另一个是**基指针**（base pointer)，由寄存器 `%rbp` 保存，**基指针是可选的**，指向当前栈帧的起始位置，即当前的栈底。  
      
用**递归**的弊端：会不断增加**栈帧**，需要很多空间，而大多数系统会**限制栈的深度**。  
  
栈帧的具体实现和结构依赖于具体的平台和操作系统的约定，但上述提到的元素是大多数栈帧实现中常见的组成部分。栈的使用允许函数的递归调用，也方便了函数调用过程的管理。  

***********

在循环内进行函数调用通常不被推荐，主要有以下几个原因：

1. **性能开销**: 每次函数调用，不管其执行体多简单，都会有一定的性能开销。这包括保存寄存器状态，创建新的栈帧，跳转执行等操作。即便函数调用完成后立即释放栈帧，这个过程本身也是有成本的。在循环体内，这些开销被循环的迭代次数所放大，对性能的影响更为显著。

2. **栈溢出风险**: 虽然一个函数调用完成后会释放栈帧，不会线性增加栈的深度，但是如果循环次数极大，且函数调用路径较深，或循环内调用的函数中还有循环，依旧存在栈溢出的风险，特别是在递归调用的情况下。

3. **编译器优化**: 现代编译器对于循环内的代码有许多优化策略，如循环展开、向量化等。将代码逻辑封装在函数内部可能会阻碍编译器的这些优化过程。

4. **内联函数**: 对于一些非常简单的函数，如果希望在循环中调用而不影响性能，可以考虑将其定义为内联函数（使用`inline`关键字）。这样编译器在编译时会尝试将其展开，避免函数调用的开销，但这也取决于具体的编译器实现。

## 注意条件跳转指令的使用
> [CSAPP-第三章-条件跳转](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第三章%20Machine-Level%20Representation%20of%20Programs.md#366-implementing-conditional-branches-with-conditional-moves)

条件指令会做分支预测，如果预测失败，则降低效率，如果情况允许，条件表达式简单，可能改用条件转移指令更好。

```cpp
v = test-expr ? then-expr : else-expr;
```
改为
```cpp
v = then-expr;  
ve = else-expr;  
t = test-expr;  
if (!t) v = ve;  
```

但注意并非所有场景都能将条件跳转转换为条件转移，并且即使可以转换，但条件表达式如果很复杂，则会浪费大量的工作在错误的分支计算上。

## if else 和 switch 
> [CSAPP-第三章-switch](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第三章%20Machine-Level%20Representation%20of%20Programs.md#368-switch-statements)

switch 语句会生成跳转表，因此不像 if else 会进行分支预测。

- **Switch语句：** 对于`switch`语句，编译器可能会为其生成一个跳转表，特别是当case的值是连续或者相对密集的整数时。这使得在执行时，基于待比较值直接跳转到相应的代码块，而无需逐一判断每个情形。这样的直接跳转减少了条件判断的次数，从而在许多情况下提高了效率。由于这种直接跳转方式，理论上不存在传统意义上的分支预测错误问题。

- **If-Else结构：** 相比之下，`if-else`结构通常需要按顺序逐一比较每个条件，直到找到符合的条件。这种连续的条件判断在遇到不满足的条件时会引发分支预测，现代处理器通过分支预测优化来减少分支指令对性能的影响。然而，分支预测错误时仍然可能导致性能损失，因为处理器需要撤销错误预测的影响并重新执行正确的分支。

因此，通常来说，当处理大量密集的、离散的条件判断时，使用`switch`语句可能因为跳转表的优势而具有更高的效率。然而，这也取决于具体的编译器实现、处理器架构、以及case分布等因素。在某些情况下，如果`switch`语句中的case值非常稀疏，或者`if-else`结构相对简单，效率差异可能不大或者不明显。

******************

在编译器中生成跳转表（Jump Table）的过程通常涉及到分析和优化`switch`语句或类似的多路分支选择结构。下面是这一过程的一般步骤：

1. **分析`switch`语句**：编译器首先分析`switch`语句中的所有`case`值，以及每个`case`对应的代码块。这一步骤涉及到理解`switch`表达式以及它所涵盖的值范围。

2. **确定性**：编译器评估`case`值的密集程度和分布范围。对于密集的、连续的`case`值，使用跳转表是一个有效的优化方法。如果`case`值非常稀疏，编译器可能会选择其他优化策略，例如二分查找。

3. **创建跳转表**：对于合适的情况，编译器会生成一个跳转表。跳转表本质上是一个函数指针数组或标签地址数组。数组的每个元素对应于一个`case`标签的地址。`switch`表达式的结果被用作索引，直接从数组中获取相应的跳转地址。

4. **生成代码**：最后，编译器将`switch`语句翻译为相应的机器码，其中包括跳转表的生成和使用。生成的代码首先计算`switch`表达式的值，然后使用这个值作为索引从跳转表中获取跳转地址，并最终跳转到对应的`case`代码块。

通过这种方式，跳转表允许在执行时减少条件比较，实现快速的条件分支选择。这对于含有大量`case`分支的`switch`语句特别有用，能够显著提高执行效率。

值得注意的是，虽然跳转表可以提高效率，但它也会增加一定的内存使用。因此，编译器会权衡各种因素，以选择最适合当前上下文的优化策略。

******************

在某些情况下，`switch`语句的效率可能会低于`if-else`链，尤其当下列条件之一或多个发生时：

1. **Case值非常稀疏**：如果`switch`中的`case`标签非常分散，导致跳转表里有大量的未使用入口，那么跳转表的使用可能不会带来性能上的优势，相反，会占用更多的内存资源。

2. **只有少数几个条件**：对于仅有几个`case`的情况，可能直接使用`if-else`链会更有效率，因为不存在额外的间接跳转和多余的内存消耗。

3. **Case标签的值范围过大**：如果`case`的值范围很大，但实际用到的标签很少，则生成一个完整的跳转表不实际，编译器可能选择不使用跳转表，这时`switch`的性能不会比`if-else`好。

4. **Switch的执行路径不均匀**：如果某些`case`要远常被执行于其他`case`，而且这些常执行的`case`标签位于`if-else`链中的前面位置，则`if-else`可能因为减少条件判断次数而更高效。

5. **分支预测效率**：现代处理器有很好的分支预测机制，对于简单的`if-else`链，处理器可能可以准确预测执行路径，从而减少分支导致的性能损耗，特别是当`if-else`的条件具有明显的执行模式时。

总之，虽然`switch`通常被认为在包含多个条件分支时比`if-else`更有效率，但在上述的特定情况下，这一点并不总是成立。实际效率取决于具体情况，包括编译器的优化能力、处理器架构、运行时数据的特点等多种因素。

## 减少循环迭代的次数
循环展开是一种减少循环开销和提高循环性能的编译器优化技术。在执行循环时，每次迭代都涉及到迭代计数、边界检查和跳转指令。通过减少循环迭代的次数，循环展开可以减少这些开销。

例如，考虑以下普通循环代码：

```cpp
for (int i = 0; i < 4; ++i) {
  performOperation(i);
}
```

在这个例子中，`performOperation`函数将被调用四次，循环需要检查边界四次，更新迭代器四次，并且可能进行四次循环跳转。

现在，如果我们应用循环展开优化，可以手动或通过编译器自动将上述代码改写为：
```cpp
performOperation(0);
performOperation(1);
performOperation(2);
performOperation(3);
```

在这个展开后的版本中，循环开销被完全消除了，因为不再需要迭代和边界检查。但是，这可能会增加代码的长度，而且如果循环体内的代码较多或者循环迭代次数很大，全展开可能并不实际。因此，部分循环展开是比较常见的做法，它权衡了循环次数减少和代码膨胀之间的平衡。
```cpp
for (int i = 0; i < 4; i += 2) {
  performOperation(i);
  performOperation(i + 1);
}
```

在这个例子中，我们两次调用`performOperation`，这样使得循环次数减少了一半，从而减少了循环的边界检查和增量操作次数。

循环展开可以由编译器在优化阶段自动进行，也可以由开发者手动在代码中实现。在实际优化过程中，是否使用循环展开以及展开的程度取决于很多因素，包括循环体的大小和复杂度、处理器的架构、以及缓存的行为等。

# 程序启动过程
> [CSAPP-第八章-Loading and Running Programs](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第八章%20Exceptional%20Control%20Flow.md#845-loading-and-running-programs)

在许多操作系统和编程语言实现中，程序启动并不是直接从main函数开始执行的。相反，执行流程首先进入由运行时环境提供的一系列启动（start-up）代码，这些代码执行一些初始化操作，为main函数的执行做好准备。在使用C标准库（libc）的程序中，`libc_start_main`是这些启动代码的一部分。

`libc_start_main`函数通常完成以下任务：

1. 设置程序运行时的环境，包括初始化堆栈、堆、全局变量等。
2. 注册必要的终止处理函数，用于在程序退出时进行清理操作，例如调用`atexit`注册的函数。
3. 处理命令行参数和环境变量，并将它们作为参数传递给`main`函数。
4. 调用一些专门的初始化函数，这些函数对全局对象构造（例如C++中的全局对象）或者特定库的初始化非常重要。
5. 执行`main`函数。通常，`libc_start_main`接受`main`函数的地址作为参数，在适当的时候调用`main`函数。
6. 当`main`函数执行完毕后，捕获其返回值，并传递给操作系统（例如通过`exit`或`_Exit`函数）。

总结起来，`libc_start_main`充当启动代码和`main`函数之间的粘合剂，负责初始化程序执行所需的环境，然后调用`main`函数，并在`main`函数完成后进行适当的清理工作。

简而言之，程序的执行流程大致是这样的：
操作系统 -> 启动代码 (`libc_start_main`等) -> `main`函数 -> `exit`处理和程序终止。 `libc_start_main`函数确保了在进入`main`函数之前所有必要的环境都已经准备就绪，并在`main`函数执行完成后执行必要的终止操作。

# 异步信号安全函数
> [Safe Signal Handling](https://github.com/lxwcd/cs/blob/main/csapp/notes/深入理解计算机系统——第八章%20Exceptional%20Control%20Flow.md#855-writing-signal-handlers)


# 原子操作

## 原子操作函数

# 可重入函数


# 设计模式
## 单例模式
单例模式（Singleton Pattern）是一种设计模式，旨在确保一个类只有一个实例，并提供全局访问点。单例模式在C++中有多种实现方式，常见的有懒汉式（Lazy Initialization）和饿汉式（Eager Initialization）。

### 懒汉式单例模式（Lazy Initialization）
懒汉式单例模式在第一次使用时才创建实例，适用于需要延迟加载的场景。

#### 实现方式
```cpp
#include <iostream>
#include <mutex>

class Singleton {
public:
    // 获取实例的静态方法
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new Singleton();
        }
        return instance_;
    }

    // 删除拷贝构造函数和赋值运算符
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 供外界调用的示例方法
    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }

    // 析构函数
    ~Singleton() {
        std::cout << "Singleton destroyed" << std::endl;
    }

private:
    // 私有构造函数
    Singleton() {
        std::cout << "Singleton created" << std::endl;
    }

    static Singleton* instance_;
    static std::mutex mutex_;
};

// 初始化静态成员变量
Singleton* Singleton::instance_ = nullptr;
std::mutex Singleton::mutex_;

int main() {
    Singleton* s1 = Singleton::getInstance();
    s1->doSomething();

    Singleton* s2 = Singleton::getInstance();
    s2->doSomething();

    return 0;
}
```

#### 使用静态成员函数获取实例
在懒汉模式中，获取实例的方法通常是一个静态成员函数。这是因为静态成员函数可以在没有实例的情况下被调用，而普通成员函数则需要通过类的实例来调用。以下是详细的解释：

静态成员函数的特性：
- **无需实例**：静态成员函数属于类本身，而不是类的某个实例。因此，可以在没有创建任何实例的情况下调用静态成员函数。
- **全局访问**：静态成员函数可以通过类名直接调用，为全局访问点提供了便利。

在懒汉模式中，单例类的实例在第一次使用时才会被创建。如果获取实例的方法是普通成员函数，那么在调用该方法之前必须已经有一个实例存在，这就违背了单例模式的初衷。因此，获取实例的方法必须是静态成员函数。

##### 静态成员函数与普通成员函数的区别
静态成员函数：
- **无需实例**：静态成员函数可以通过类名直接调用，而不需要实例。
- **无法访问非静态成员**：静态成员函数不能访问类的非静态成员变量和非静态成员函数，因为它们没有 `this` 指针。
- **共享数据**：静态成员函数可以访问静态成员变量，这些变量在所有实例之间共享。

普通成员函数：
- **需要实例**：普通成员函数必须通过类的实例来调用。
- **可以访问所有成员**：普通成员函数可以访问类的所有成员变量和成员函数（包括静态和非静态）。
- **有 `this` 指针**：普通成员函数有一个隐含的 `this` 指针，指向调用该函数的实例。

##### 静态函数与静态成员函数的区别
静态函数：
- **全局作用域**：静态函数通常是指在文件作用域内的静态函数（使用 `static` 关键字修饰的函数），它们的作用范围仅限于定义它们的源文件。
- **文件内可见**：静态函数在定义它们的源文件之外不可见，通常用于实现文件内的辅助功能。

静态成员函数：
- **类作用域**：静态成员函数是类的成员函数，属于类本身，而不是某个实例。
- **类内可见**：静态成员函数可以在类的范围内访问，并且可以通过类名在全局范围内访问。

##### 静态函数和普通函数的区别
静态函数和普通函数在C++中的区别主要体现在它们的作用域、可见性和用途上。以下是详细的解释：

###### 静态函数（Static Function）
静态函数是指在文件作用域内使用 `static` 关键字修饰的函数。它们的作用范围仅限于定义它们的源文件。
特性：
1. **文件作用域**：
   - 静态函数只能在定义它们的源文件中被调用，其他文件无法访问这些函数。
   
2. **内部链接**：
   - 静态函数具有内部链接（internal linkage），这意味着它们在编译时不会暴露给其他翻译单元（translation units）。

3. **用途**：
   - 静态函数通常用于实现文件内的辅助功能，避免命名冲突和不必要的全局可见性。

示例代码：
```cpp
#include <iostream>

// 静态函数
static void staticFunction() {
    std::cout << "Static function called" << std::endl;
}

void anotherFunction() {
    // 可以在同一文件中调用静态函数
    staticFunction();
}

int main() {
    // 可以在同一文件中调用静态函数
    staticFunction();
    return 0;
}
```

###### 普通函数（Non-static Function）
普通函数是没有使用 `static` 关键字修饰的函数。它们的作用范围可以是全局的，具体取决于它们的声明位置。

特性：
1. **全局作用域**：
   - 普通函数可以在定义它们的源文件外部被调用，只要在其他文件中声明了该函数。
   
2. **外部链接**：
   - 普通函数具有外部链接（external linkage），这意味着它们在编译时可以被其他翻译单元访问。

3. **用途**：
   - 普通函数用于实现全局功能，可以在多个文件中共享和调用。

示例代码：
```cpp
#include <iostream>

// 普通函数
void normalFunction() {
    std::cout << "Normal function called" << std::endl;
}

void anotherFunction() {
    // 可以在同一文件中调用普通函数
    normalFunction();
}

int main() {
    // 可以在同一文件中调用普通函数
    normalFunction();
    return 0;
}
```

###### 静态函数和普通函数的对比
| 特性         | 静态函数 (Static Function)               | 普通函数 (Non-static Function)   |
| ------------ | ---------------------------------------- | -------------------------------- |
| **作用域**   | 文件作用域，只能在定义它们的源文件中调用 | 全局作用域，可以在多个文件中调用 |
| **链接类型** | 内部链接（internal linkage）             | 外部链接（external linkage）     |
| **可见性**   | 仅在定义它们的源文件中可见               | 可以在其他文件中声明并调用       |
| **用途**     | 文件内的辅助功能，避免命名冲突           | 全局功能，可以在多个文件中共享   |

###### 总结
- **静态函数**：用于文件内部的辅助功能，具有内部链接，仅在定义它们的源文件中可见。
- **普通函数**：用于全局功能，具有外部链接，可以在多个文件中共享和调用。

选择使用哪种函数取决于具体的需求。如果你希望函数只在定义它的源文件中使用，避免命名冲突和不必要的全局可见性，可以选择静态函数。如果你希望函数在多个文件中共享和调用，可以选择普通函数。

##### 示例代码
以下是一个示例代码，展示了静态成员函数、普通成员函数和静态函数的区别：

```cpp
#include <iostream>

class MyClass {
public:
    // 静态成员函数
    static void staticMemberFunction() {
        std::cout << "Static member function called" << std::endl;
        // 无法访问非静态成员变量或非静态成员函数
        // std::cout << "Non-static member variable: " << nonStaticMember << std::endl; // 错误
    }

    // 普通成员函数
    void nonStaticMemberFunction() {
        std::cout << "Non-static member function called" << std::endl;
        std::cout << "Non-static member variable: " << nonStaticMember << std::endl;
    }

    // 静态成员变量
    static int staticMember;

    // 非静态成员变量
    int nonStaticMember;

    // 构造函数
    MyClass() : nonStaticMember(42) {}
};

// 静态成员变量定义
int MyClass::staticMember = 0;

// 静态函数（文件作用域）
static void staticFunction() {
    std::cout << "Static function called" << std::endl;
}

int main() {
    // 调用静态成员函数
    MyClass::staticMemberFunction();

    // 创建类的实例
    MyClass obj;

    // 调用普通成员函数
    obj.nonStaticMemberFunction();

    // 调用静态函数
    staticFunction();

    return 0;
}
```

输出结果：
```
Static member function called
Non-static member function called
Non-static member variable: 42
Static function called
```

#### 多线程安全
在上面的单例模式中，`std::lock_guard<std::mutex>` 用于确保在多线程环境下对单例实例的访问是线程安全的。

在多线程环境中，如果多个线程同时调用 `getInstance` 方法，有可能会导致多个线程同时检测到 `instance_` 为 `nullptr`，从而创建多个实例。这违背了单例模式的初衷，即保证一个类只有一个实例。

`std::lock_guard` 是一个RAII（Resource Acquisition Is Initialization）类，用于在作用域内自动管理互斥锁的锁定和解锁。它的主要作用是确保在函数执行期间互斥锁被正确地锁定和解锁，以防止资源竞争和数据不一致。
1. **锁定互斥锁**：
   - 当 `std::lock_guard` 对象创建时，它会锁定传入的互斥锁，确保在 `getInstance` 方法执行期间，其他线程无法同时访问该方法。

2. **自动解锁**：
   - 当 `std::lock_guard` 对象超出作用域时（例如，函数返回时），它会自动解锁互斥锁，无需显式调用解锁操作。这减少了手动管理锁的复杂性和潜在的错误。

#### 关键点
- **线程安全**：使用 `std::mutex` 确保线程安全。
- **延迟初始化**：实例在第一次调用 `getInstance` 时创建。
- **防止拷贝和赋值**：删除拷贝构造函数和赋值运算符。

#### 析构问题
懒汉式单例在程序结束时不会自动销毁，需要手动管理其生命周期。可以通过 `atexit` 注册析构函数或智能指针来管理。

静态变量的生命周期：
1. **初始化**：
   - 静态变量在程序启动时或在第一次使用时分配内存并初始化。

2. **程序运行期间**：
   - 静态变量在整个程序运行期间一直存在，并且在所有实例之间共享。

3. **程序结束**
   - 当程序结束时，操作系统会回收静态变量所占用的内存，但不会自动调用其指向对象的析构函数。

动态分配的内存：
通过 `new` 操作符动态分配的内存需要显式释放。如果不显式释放，这些内存会在程序结束时由操作系统回收，但不会调用析构函数。这会导致资源泄漏（例如，文件句柄、网络连接等未被正确释放）。

##### 提供显式释放方法

```cpp
class Singleton {
public:
    // 获取实例的静态方法
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new Singleton();
        }
        return instance_;
    }

    // 显式删除实例的静态方法
    static void destroyInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ != nullptr) {
            delete instance_;
            instance_ = nullptr;
        }
    }

    // 禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // 私有构造函数，防止外部实例化
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
    }

    // 私有析构函数，防止外部删除实例
    ~Singleton() {
        std::cout << "Singleton instance destroyed" << std::endl;
    }

    // 静态成员变量，存储单例实例
    static Singleton* instance_;
    // 互斥锁，用于保护实例创建
    static std::mutex mutex_;
};

// 静态成员变量初始化
Singleton* Singleton::instance_ = nullptr;
std::mutex Singleton::mutex_;

int main() {
    // 获取单例实例
    Singleton* instance1 = Singleton::getInstance();
    Singleton* instance2 = Singleton::getInstance();

    // 验证两个指针是否指向同一个实例
    if (instance1 == instance2) {
        std::cout << "Both pointers point to the same instance." << std::endl;
    } else {
        std::cout << "Different instances exist!" << std::endl;
    }

    // 显式删除单例实例
    Singleton::destroyInstance();

    return 0;
}
```

##### 使用 `atexit` 函数

```cpp
class Singleton {
public:
    // 获取实例的静态方法
    static Singleton* getInstance() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (instance_ == nullptr) {
            instance_ = new Singleton();
            std::atexit(&Singleton::destroyInstance);
        }
        return instance_;
    }

    // 禁止拷贝构造和赋值操作
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    // 私有构造函数，防止外部实例化
    Singleton() {
        std::cout << "Singleton instance created" << std::endl;
    }

    // 私有析构函数，防止外部删除实例
    ~Singleton() {
        std::cout << "Singleton instance destroyed" << std::endl;
    }

    // 静态成员变量，存储单例实例
    static Singleton* instance_;
    // 互斥锁，用于保护实例创建
    static std::mutex mutex_;

    // 静态方法，用于删除单例实例
    static void destroyInstance() {
        delete instance_;
        instance_ = nullptr;
    }
};

// 静态成员变量初始化
Singleton* Singleton::instance_ = nullptr;
std::mutex Singleton::mutex_;

int main() {
    // 获取单例实例
    Singleton* instance1 = Singleton::getInstance();
    Singleton* instance2 = Singleton::getInstance();

    // 验证两个指针是否指向同一个实例
    if (instance1 == instance2) {
        std::cout << "Both pointers point to the same instance." << std::endl;
    } else {
        std::cout << "Different instances exist!" << std::endl;
    }

    return 0;
}
```

### 懒汉模式2

#### 实现方式

```cpp
#include <iostream>

class Singleton {
public:
    // 获取实例的静态方法
    static Singleton& getInstance() {
        static Singleton instance; // 局部静态变量，第一次调用时初始化
        return instance;
    }

    // 删除拷贝构造函数和赋值运算符
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // 供外界调用的示例方法
    void doSomething() {
        std::cout << "Doing something..." << std::endl;
    }

private:
    // 私有构造函数
    Singleton() {
        std::cout << "Singleton created" << std::endl;
    }
    // 析构函数
    ~Singleton() {
        std::cout << "Singleton destroyed" << std::endl;
    }

};

int main() {
    Singleton& s1 = Singleton::getInstance();
    s1.doSomething();

    Singleton& s2 = Singleton::getInstance();
    s2.doSomething();

    return 0;
}
```

#### 关键点
- **线程安全**：使用 `static` 局部变量，C++11 及之后的标准保证其线程安全。
- **防止拷贝和赋值**：删除拷贝构造函数和赋值运算符。

#### 析构问题
单例会在程序结束时自动销毁，不需要手动管理其生命周期。