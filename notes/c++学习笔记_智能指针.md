C++ 学习笔记

# 智能指针
> [万字长文全面详解现代C++智能指针：原理、应用和陷阱](https://zhuanlan.zhihu.com/p/672745555) 

智能指针是一个模板类。 更准确地说，C++标准库提供的几种智能指针（std::unique_ptr, std::shared_ptr, std::weak_ptr）都是类模板。

在传统 C++ 中，我们使用 `new` 和 `delete` 手动管理动态内存。这很容易出现问题：
1.  **内存泄漏**：忘记 `delete` 分配的内存。
2.  **野指针**：提前 `delete` 了内存，但其他地方还在使用该指针。
3.  **双重释放**：对同一块内存 `delete` 两次。

智能指针通过 **RAII (Resource Acquisition Is Initialization)** 技术来解决这些问题。其核心思想是：
*   **将资源（如动态内存）的生命周期与一个对象的生命周期绑定。**
*   **在对象构造时获取资源**（在构造函数中 `new`）。
*   **在对象析构时释放资源**（在析构函数中 `delete`）。
*   由于析构函数是自动调用的，所以资源也能被自动释放。

C++11 引入了三种主要的智能指针，位于 `<memory>` 头文件中。

## `std::unique_ptr` - 独占所有权的智能指针

**含义**：`unique_ptr` 对其持有的资源拥有**独占的**所有权。同一时刻只能有一个 `unique_ptr` 指向一个特定的对象。

**特点**：
*   **无法被复制**（拷贝构造函数和赋值运算符被禁用）。这确保了所有权的唯一性。
*   **可以被移动**（使用 `std::move`）。移动后，原来的 `unique_ptr` 变为 `nullptr`，所有权转移给新的 `unique_ptr`。
*   当 `unique_ptr` 被销毁（例如离开作用域）时，它所管理的对象会自动被删除。
*   内存开销很小，通常与原始指针相同（取决于编译器）。

**适用场景**：**“在我生命期内，这个资源只属于我”**。这是最常用、性能最好的智能指针，应优先考虑使用它。

**举例**：
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "MyClass 构造函数\n"; }
    ~MyClass() { std::cout << "MyClass 析构函数\n"; }
    void do_something() { std::cout << "Doing something...\n"; }
};

int main() {
    std::cout << "创建 unique_ptr...\n";
    {
        // 推荐使用 std::make_unique (C++14)
        std::unique_ptr<MyClass> ptr1 = std::make_unique<MyClass>();
        // 也可以直接构造（不推荐，可能产生异常安全问题）
        // std::unique_ptr<MyClass> ptr2(new MyClass());

        ptr1->do_something(); // 使用 -> 操作符访问成员

        // std::unique_ptr<MyClass> ptr2 = ptr1; // 错误！无法复制
        std::unique_ptr<MyClass> ptr2 = std::move(ptr1); // 正确！移动语义，所有权转移

        if (ptr1) { // bool 重载，检查是否管理着对象
            std::cout << "ptr1 仍然拥有对象\n";
        } else {
            std::cout << "ptr1 现在是 nullptr\n"; // 这行会被执行
        }

        ptr2->do_something();
        // 离开这个作用域时，ptr2 被销毁，它管理的 MyClass 对象被自动删除
    }
    std::cout << "离开作用域，资源已自动清理\n";
    return 0;
}
```
**输出**：
```
创建 unique_ptr...
MyClass 构造函数
Doing something...
ptr1 现在是 nullptr
Doing something...
MyClass 析构函数
离开作用域，资源已自动清理
```

## `std::shared_ptr` - 共享所有权的智能指针

**含义**：`shared_ptr` 通过**引用计数**机制实现所有权的共享。多个 `shared_ptr` 可以指向同一个对象。

**特点**：
*   **可以被复制和移动**。
*   每次复制（赋值或初始化另一个 `shared_ptr`），内部的引用计数都会 +1。
*   每个 `shared_ptr` 析构时，引用计数 -1。
*   当最后一个指向对象的 `shared_ptr` 被销毁（引用计数变为 0）时，所管理的对象才会被删除。
*   因为有引用计数的开销，所以内存和性能开销比 `unique_ptr` 大。

**适用场景**：**“我不知道谁最后还需要这个资源，所以大家共用，最后一个人记得关灯”**。用于需要多个所有者管理同一资源生命周期的场景。

**举例**：
```cpp
#include <iostream>
#include <memory>

class MyClass {
public:
    MyClass() { std::cout << "MyClass 构造函数\n"; }
    ~MyClass() { std::cout << "MyClass 析构函数\n"; }
};

int main() {
    std::cout << "创建 shared_ptr...\n";
    std::shared_ptr<MyClass> shared1 = std::make_shared<MyClass>(); // 引用计数 = 1

    {
        std::shared_ptr<MyClass> shared2 = shared1; // 拷贝，引用计数 +1 => 2
        std::cout << "内部作用域，引用计数: " << shared2.use_count() << "\n"; // 输出 2
        // shared2 离开作用域，析构，引用计数 -1 => 1
    }

    std::cout << "外部作用域，引用计数: " << shared1.use_count() << "\n"; // 输出 1
    // shared1 离开作用域，析构，引用计数 -1 => 0，对象被删除
    return 0;
}
```
**输出**：
```
创建 shared_ptr...
MyClass 构造函数
内部作用域，引用计数: 2
外部作用域，引用计数: 1
MyClass 析构函数
```

## `std::weak_ptr` - 弱引用的智能指针

**含义**：`weak_ptr` 是为了解决 `shared_ptr` 的**循环引用**问题而设计的。它指向一个由 `shared_ptr` 管理的对象，但**不增加引用计数**。

**特点**：
*   **不拥有对象的所有权**，不影响对象的生命周期。
*   不能直接访问资源，必须通过 `lock()` 方法转换为一个 `shared_ptr` 才能使用。如果对象还存在，`lock()` 返回一个有效的 `shared_ptr`（引用计数+1）；如果对象已被释放，则返回一个空的 `shared_ptr`。
*   用于打破 `shared_ptr` 的循环引用。

**适用场景**：**“我想用那个资源，但如果别人都用完了，没了也行，我不强求”**。用于观察、缓存、以及解决循环引用。

### 解决循环引用问题

想象两个小朋友：**小明** 和 **小红**。

他们各有一个**计数手环**（这就是引用计数），显示有多少人正在拉着他们的手。

现在，我们定一个规则：
**只有当一个人的计数手环显示为0时（没有人拉他的手），这个小朋友才能回家（内存被释放）。**

正常情况（没有循环引用）：
- 老师（`main`函数）拉着小明的手 → 小明的计数：**1**
- 老师拉着小红的手 → 小红的计数：**1**
- 老师松开手离开 → 小明的计数：**0**，小红的计数：**0**
- 两人都回家了 ✅

循环引用的情况：
现在，让小明和小红**互相拉手**：

1. 老师拉着小明的手 → 小明的计数：**1**
2. 老师拉着小红的手 → 小红的计数：**1**
3. **小明也拉起小红的手** → 小红的计数：**2** (老师 + 小明)
4. **小红也拉起小明的手** → 小明的计数：**2** (老师 + 小红)

现在，老师松开手离开了：
- 小明的计数：**2 - 1 = 1** (还剩小红拉着他)
- 小红的计数：**2 - 1 = 1** (还剩小明拉着他)

**问题来了：**
- 小明不能回家，因为小红还拉着他（计数 > 0）
- 小红不能回家，因为小明还拉着他（计数 > 0）
- 但他们俩都在等着对方先松手！

这就是**循环引用**：两人互相牵制，谁也走不了，造成了"内存泄漏"。

```cpp
#include <iostream>
#include <memory>

class B;

class A {
public:
    std::shared_ptr<B> b_ptr;  // A 里面有一个指向B的共享指针
    ~A() { std::cout << "A 被销毁\n"; }
};

class B {
public:
    std::shared_ptr<A> a_ptr;  // B 里面有一个指向A的共享指针
    ~B() { std::cout << "B 被销毁\n"; }
};

int main() {
    // 创建两个对象
    std::shared_ptr<A> a = std::make_shared<A>(); // A的计数: 1
    std::shared_ptr<B> b = std::make_shared<B>(); // B的计数: 1
    
    // 建立互相引用
    a->b_ptr = b; // B的计数: 2 (b + a->b_ptr)
    b->a_ptr = a; // A的计数: 2 (a + b->a_ptr)
    
    // main函数结束，a和b离开作用域
    // a析构：A的计数: 2 - 1 = 1 (还剩b->a_ptr)
    // b析构：B的计数: 2 - 1 = 1 (还剩a->b_ptr)
    
    // 结果：A和B的计数都不为0，都不会被销毁！
    return 0;
}
```

运行这段代码，你会发现**没有任何析构输出**，证明对象确实没有被销毁。

如何解决：使用 weak_ptr

现在我们把规则改一下：有一种**弱拉手**（`weak_ptr`），这种拉手**不增加计数手环的数字**。

重新来一遍：

1. 老师拉着小明的手 → 小明的计数：**1**
2. 老师拉着小红的手 → 小红的计数：**1**
3. **小明用普通方式拉起小红的手** → 小红的计数：**2** (老师 + 小明)
4. **小红用弱拉手方式拉起小明的手** → 小明的计数：**1** (只有老师，弱拉手不计入)

现在，老师松开手离开：
- 小明的计数：**1 - 1 = 0** → 小明可以回家了！
- 小明回家时，他松开了小红的手 → 小红的计数：**2 - 1 = 1**
- 但是小红看到小明都回家了，她也松开手 → 小红的计数：**1 - 1 = 0** → 小红也可以回家了！

✅ **问题解决了！**

代码实现解决方案

```cpp
#include <iostream>
#include <memory>

class B;

class A {
public:
    std::shared_ptr<B> b_ptr;  // 仍然是强引用
    ~A() { std::cout << "A 被销毁\n"; }
};

class B {
public:
    std::weak_ptr<A> a_ptr;    // 改为弱引用！不增加引用计数
    ~B() { std::cout << "B 被销毁\n"; }
};

int main() {
    std::shared_ptr<A> a = std::make_shared<A>(); // A的计数: 1
    std::shared_ptr<B> b = std::make_shared<B>(); // B的计数: 1
    
    a->b_ptr = b; // B的计数: 2 (b + a->b_ptr)
    b->a_ptr = a; // A的计数: 1 (只有a，weak_ptr不计入)
    
    // main函数结束，a和b离开作用域
    // a析构：A的计数: 1 - 1 = 0 → A被销毁！
    // A被销毁时，它的成员b_ptr也被销毁 → B的计数: 2 - 1 = 1
    // b析构：B的计数: 1 - 1 = 0 → B被销毁！
    
    return 0;
}
```

运行这段代码，你会看到正确的输出：
```
A 被销毁
B 被销毁
```

### 总结

1. **循环引用问题**：当两个对象互相用 `shared_ptr` 引用对方时，就像两个人互相拉住对方，谁也无法离开。

2. **解决方案**：将其中一个引用改为 `weak_ptr`。`weak_ptr` 可以"看见"对象，但不会"拉住"对象（不增加引用计数），这样就打破了循环。

3. **什么时候用**：当你需要引用一个对象，但又不想拥有它（不想影响它的生命周期）时，就用 `weak_ptr`。

## 总结与最佳实践

| 智能指针类型      | 所有权模型 | 复制行为               | 使用场景                                           |
| :---------------- | :--------- | :--------------------- | :------------------------------------------------- |
| `std::unique_ptr` | 独占       | **只能移动**           | 单一所有者的资源管理。**默认首选**。               |
| `std::shared_ptr` | 共享       | 可以拷贝，引用计数+1   | 需要多个所有者共享同一资源的场景。                 |
| `std::weak_ptr`   | 弱引用     | 可以拷贝，但不增加计数 | 配合 `shared_ptr` 使用，解决循环引用，用作观察者。 |

**最佳实践**：
1.  **优先选择 `unique_ptr`**，除非明确需要共享所有权。
2.  **优先使用 `std::make_unique` 和 `std::make_shared`** 来创建智能指针。
    *   **更安全**：防止因异常导致的内存泄漏（`make_*` 是原子操作）。
    *   **更高效**：尤其是 `make_shared`，它可以将引用计数和控制块与对象本身分配在同一块内存中。
3.  不要使用 `delete` 来释放由智能指针管理的资源。
4.  不要将同一个原始指针初始化多个智能指针（尤其是 `shared_ptr`），这会创建多个控制块，导致双重释放。如果需要，从已有的智能指针进行拷贝。
5.  原始指针和引用在涉及所有权传递的 API 中仍然有用，但对于资源管理，应让智能指针成为代码的骨干。

# 栈展开

**栈展开** 是C++异常处理机制的一部分。当异常被抛出时，C++运行时环境会保证：

*   从当前的 `try` 块开始，沿着调用链向上寻找匹配的 `catch` 处理器。
*   **在这个寻找过程中，会自动销毁所有已创建但尚未销毁的局部对象（即栈上的对象），并调用它们的析构函数。**

## 场景一：手动管理（灾难现场）
```cpp
void riskyFunction() {
    MyClass* ptr = new MyClass(); // 申请资源

    someFunctionThatMightThrow(); // 可能抛出异常的调用！

    delete ptr; // 如果上面抛出异常，这行代码永远执行不到！
}
```
*   **如果 `someFunctionThatMightThrow()` 抛出异常**，程序流程会立刻跳转去寻找 `catch`。`delete ptr;` 这条语句被跳过。
*   **结果**：内存泄漏。

## 场景二：使用智能指针（安全可靠）
```cpp
#include <memory>

void safeFunction() {
    std::unique_ptr<MyClass> smartPtr = std::make_unique<MyClass>(); // RAII：资源在构造时获取

    someFunctionThatMightThrow(); // 可能抛出异常的调用！

} // 无论是否异常，smartPtr 离开作用域时，其析构函数都会自动调用 delete
```
*   **如果 `someFunctionThatMightThrow()` 抛出异常**，C++会启动**栈展开**。
*   **栈展开过程**：在跳出 `safeFunction()` 函数栈帧之前，编译器会插入代码，**自动调用所有局部对象的析构函数**，包括 `smartPtr` 的析构函数。
*   **`smartPtr` 的析构函数**：这个函数内部已经写好了 `delete` 逻辑，因此内存会被安全释放。
*   **结果**：**无内存泄漏**。资源被完美清理。

这种机制的可靠性根植于C++语言标准：

1.  **编译器保证**：由编译器在编译时自动插入析构函数调用代码，而不是在运行时依赖某种监控机制。只要对象在栈上，其析构函数就一定会在离开作用域时被调用。
2.  **异常安全保证**：C++标准明确规定了栈展开的行为，这是一条“语言法则”，任何兼容的编译器都必须遵守。

# 简单实现 unique_ptr

```cpp
#include <iostream>
#include <utility> // for std::swap

template<typename T>
class SimpleUniquePtr {
private:
    T* ptr = nullptr;

public:
    // 构造函数
    explicit SimpleUniquePtr(T* p = nullptr) : ptr(p) {}
    
    // 禁止拷贝
    SimpleUniquePtr(const SimpleUniquePtr&) = delete;
    SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;
    
    // 移动构造函数 - 转移所有权
    SimpleUniquePtr(SimpleUniquePtr&& other) noexcept : ptr(other.ptr) {
        other.ptr = nullptr; // 重要：使源对象不再拥有指针
    }
    
    // 移动赋值运算符 - 转移所有权
    SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept {
        if (this != &other) { // 防止自赋值
            delete ptr;       // 释放当前资源
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }
    
    // 析构函数
    ~SimpleUniquePtr() {
        delete ptr;
    }
    
    // 重载操作符，使其表现得像指针
    T& operator*() const { return *ptr; }
    T* operator->() const { return ptr; }
    
    // 获取原始指针
    T* get() const { return ptr; }
    
    // 释放所有权，返回指针并将内部指针置为空
    T* release() {
        T* temp = ptr;
        ptr = nullptr;
        return temp;
    }
    
    // 重置指针，删除当前管理的对象
    void reset(T* p = nullptr) {
        delete ptr;
        ptr = p;
    }
    
    // 交换两个unique_ptr
    void swap(SimpleUniquePtr& other) {
        std::swap(ptr, other.ptr);
    }
    
    // 检查是否为空
    explicit operator bool() const {
        return ptr != nullptr;
    }
};

// 示例使用
int main() {
    SimpleUniquePtr<int> p1(new int(42));
    std::cout << *p1 << std::endl; // 输出: 42
    
    // SimpleUniquePtr<int> p2 = p1; // 错误: 拷贝构造函数被删除
    
    SimpleUniquePtr<int> p3 = std::move(p1); // 正确: 移动构造
    std::cout << *p3 << std::endl; // 输出: 42
    // std::cout << *p1 << std::endl; // 错误: p1现在为空
    
    SimpleUniquePtr<int> p4(new int(100));
    p4 = std::move(p3); // 移动赋值
    std::cout << *p4 << std::endl; // 输出: 42
    
    return 0;
} // 所有资源自动释放
```

## 1. 禁止拷贝 (Copy Prevention)
```cpp
SimpleUniquePtr(const SimpleUniquePtr&) = delete;
SimpleUniquePtr& operator=(const SimpleUniquePtr&) = delete;
```
这是保证独占性的基础。通过将拷贝构造函数和拷贝赋值运算符标记为 `= delete`，编译器会阻止任何拷贝操作。

## 2. 移动语义 (Move Semantics)
```cpp
SimpleUniquePtr(SimpleUniquePtr&& other) noexcept; // 移动构造
SimpleUniquePtr& operator=(SimpleUniquePtr&& other) noexcept; // 移动赋值
```
虽然禁止了拷贝，但允许所有权的转移。移动操作将资源从一个对象转移到另一个对象，并将源对象置为空。

## 3. RAII (Resource Acquisition Is Initialization)
```cpp
~SimpleUniquePtr() {
    delete ptr;
}
```
这是智能指针的核心。当 `SimpleUniquePtr` 对象离开作用域时，其析构函数会自动调用 `delete` 释放资源。

## 4. 异常安全
移动操作标记为 `noexcept` 很重要，因为这允许标准库容器（如 `std::vector`）在重新分配内存时高效地移动 `unique_ptr` 对象，而不是进行拷贝。

## 更完整的实现考虑

实际的标准库 `std::unique_ptr` 实现还包括：

1. **自定义删除器支持**：
   ```cpp
   template<typename T, typename Deleter = std::default_delete<T>>
   class unique_ptr {
       // ...
   };
   ```
   允许指定如何释放资源（如 `delete[]` 用于数组，或自定义释放函数）。

2. **数组特化**：
   ```cpp
   template<typename T>
   class unique_ptr<T[]> {
       // 专门用于数组，使用 delete[]
   };
   ```

3. **与 nullptr 的比较操作**：
   ```cpp
   bool operator==(std::nullptr_t) const;
   bool operator!=(std::nullptr_t) const;
   ```

4. **与标准库容器的兼容性**：
   确保 `std::unique_ptr` 可以在标准库容器中正确工作。

## 谨慎使用 `.get()` 方法

`.get()` 返回托管指针的裸指针，但不释放所有权。必须确保裸指针的**生命周期不会超过**其来源的 `unique_ptr`。

```cpp
void dangerous_function(int* raw_ptr) {
    // 如果这个函数存储了 raw_ptr 供以后使用，将会导致灾难
}

int main() {
    std::unique_ptr<int> up = std::make_unique<int>(42);
    int* raw_ptr = up.get(); // “借出”指针
    
    // 危险操作：
    // delete raw_ptr;       // 灾难！up 析构时还会再 delete 一次
    // std::unique_ptr<int> another_up(raw_ptr); // 灾难！会造成双重释放
    
    // 相对安全的用法：仅在当前作用域内临时使用，且确保 up 一直存在
    dangerous_function(raw_ptr);
    
    return 0;
} // up 析构，raw_ptr 随之失效
```

**最佳实践**：仅在需要向不操作所有权的旧式 C 风格 API 传递指针时使用 `.get()`，并且确保该 API 不会尝试删除或存储这个指针。

## 正确管理数组

`std::unique_ptr` 为数组提供了特化版本，会自动使用 `delete[]` 而不是 `delete`。

```cpp
// 管理单个对象（默认）
std::unique_ptr<MyClass> up_obj = std::make_unique<MyClass>();

// 管理对象数组：必须使用方括号 []
std::unique_ptr<MyClass[]> up_array = std::make_unique<MyClass[]>(10); // 10个元素的数组

up_array[0].doSomething(); // 可以通过下标访问
// up_array->doSomething(); // 错误！数组版本没有 -> 操作符

// 对于C风格数组，自定义删除器不是必须的，但也可以使用
std::unique_ptr<int[], void(*)(int*)> legacy_array(
    new int[100],
    [](int* ptr) { delete[] ptr; } // 自定义删除器，虽然效果和特化版本一样
);
```

**注意事项**：不要混淆对象和数组的版本。用 `std::unique_ptr<T>` 管理数组，或者用 `std::unique_ptr<T[]>` 管理单个对象都会导致未定义行为。

## 自定义删除器

`unique_ptr` 允许指定一个自定义的删除器，用于管理不是通过 `new` 分配的资源。

```cpp
#include <iostream>
#include <memory>

// 例子：管理一个使用 malloc 分配的 C 风格字符串
struct FreeDeleter {
    void operator()(void* ptr) const {
        std::cout << "Freeing memory with free()\n";
        std::free(ptr);
    }
};

int main() {
    // 使用 malloc 分配，因此不能用默认的 delete
    std::unique_ptr<char, FreeDeleter> c_str(
        static_cast<char*>(std::malloc(100)),
        FreeDeleter() // 传入删除器实例
    );
    
    // C++11 更优雅的方式：使用 lambda 作为删除器
    auto FileDeleter = [](std::FILE* file_ptr) {
        if (file_ptr) {
            std::cout << "Closing FILE*\n";
            std::fclose(file_ptr);
        }
    };
    
    std::unique_ptr<std::FILE, decltype(FileDeleter)> file_ptr(
        std::fopen("data.txt", "r"),
        FileDeleter
    );
    
    return 0;
} // 离开作用域时，自定义删除器会被自动调用
```

**注意事项**：自定义删除器的类型是 `unique_ptr` 类型的一部分。两个拥有不同删除器类型的 `unique_ptr` 是不同类型，不能互相移动或赋值。

## 作为函数参数和返回值

这是体现其所有权语义的绝佳场景。

*   **作为函数参数**：按值传递表示**所有权转移**。
    ```cpp
    //  sinks 该函数接收 up 的所有权
    void sink_function(std::unique_ptr<MyClass> up) {
        // ... 函数内部现在拥有资源的所有权
    } // 函数结束，up 析构，资源被释放
    
    int main() {
        auto up = std::make_unique<MyClass>();
        sink_function(std::move(up)); // 必须显式移动转移所有权
        // 此时 up 已为 nullptr
        return 0;
    }
    ```

*   **作为函数返回值**：编译器会自动处理移动，这是**隐式所有权的转移**。
    ```cpp
    std::unique_ptr<MyClass> create_object() {
        return std::make_unique<MyClass>(); // 没问题，编译器会优化（RVO/NRVO）
    }
    
    int main() {
        // 从函数接收所有权，非常清晰安全
        std::unique_ptr<MyClass> obj = create_object();
        return 0;
    }
    ```

**最佳实践**：
*   如果函数只是**使用**对象而不获取所有权，应传递**引用**或**裸指针** (`*.get()`)。
    ```cpp
    void use_object(const MyClass& obj); // 推荐：只读
    void modify_object(MyClass* obj);    // 推荐：需要修改
    ```
*   如果函数需要**接管**对象的所有权，则按值传递 `std::unique_ptr`。

## 总结：unique_ptr 的最佳实践

1.  **默认选择**：优先使用 `std::unique_ptr` 而不是 `std::shared_ptr`，除非明确需要共享所有权。
2.  **优先使用 `std::make_unique`** (C++14)：它更安全（避免裸指针暴露）、更高效（减少代码重复）。
3.  **明确所有权转移**：使用 `std::move` 来显式转移所有权，使代码意图清晰。
4.  **小心 `.get()`**：将其视为临时“借用”，绝不用于创建另一个智能指针或手动删除。
5.  **区分对象和数组**：正确使用 `std::unique_ptr<T>` 和 `std::unique_ptr<T[]>`。
6.  **善用自定义删除器**：用于管理任何类型的资源（文件句柄、套接字等），实现完整的 RAII。
7.  **设计函数接口时明确语义**：按值传递表示接收所有权，使用引用或裸指针表示不使用所有权。