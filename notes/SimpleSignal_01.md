SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 


# CollectorLast

```cpp
/// CollectorLast returns the result of the last signal handler from a signal emission.
template<typename Result>
struct CollectorLast {
  using CollectorResult = Result;
  explicit CollectorLast() : last_() {}
  inline bool operator()(Result r) { last_ = r; return true; }
  CollectorResult result() { return last_; }
private:
  Result last_;
};
```

CollectorLast 的作用是：在信号发射过程中，保存最后一个槽函数的返回值，并作为整个信号发射的最终结果返回。

# CollectorDefault

```cpp
/// CollectorDefault implements the default signal handler collection behaviour.
template<typename Result>
struct CollectorDefault : CollectorLast<Result>
{};

/// CollectorDefault specialisation for signals with void return type.
template<>
struct CollectorDefault<void> {
  using CollectorResult = void;
  void                  result     ()           {}
  inline bool           operator() (void)       { return true; }
};
```

CollectorLast 的特化，信号返回类型为 void。

# CollectorInvocation 

```cpp
template<class Collector, class R, class... Args>
struct CollectorInvocation<Collector, R (Args...)> {
  inline bool
  invoke (Collector &collector, const std::function<R (Args...)> &cbf, Args... args) const
  {
    return collector (cbf (args...));
  }
};
```

这是一个关键的模板特化，负责处理**非void返回值类型**的信号调用。

**`CollectorInvocation` 的主要作用是为不同返回类型的信号提供统一的调用接口，正确处理返回值传递给Collector的策略。**

1. **调用槽函数**：`cbf(args...)`
2. **传递返回值**：将槽函数的返回值传递给Collector
3. **控制执行流**：返回Collector的决定（是否继续执行后续槽函数）

## 1. 模板参数
```cpp
template<class Collector, class R, class... Args>
```
- **Collector**: 返回值处理策略（如 `CollectorLast`, `CollectorUntil0`）
- **R**: 槽函数的返回类型
- **Args...**: 槽函数的参数类型包

## 2. 模板特化
```cpp
struct CollectorInvocation<Collector, R (Args...)>
```
- 这是对 `CollectorInvocation` 主模板的**特化**
- 特化模式：`R (Args...)` - 函数类型签名
- 匹配所有**非void返回值**的函数类型

## 3. 方法签名
```cpp
inline bool invoke(Collector &collector, 
                  const std::function<R (Args...)> &cbf, 
                  Args... args) const
```
- **collector**: 返回值处理器的引用
- **cbf**: 要调用的回调函数（`std::function`）
- **args**: 调用参数包
- **const**: 方法不修改对象状态
- **返回bool**: 指示是否继续执行后续槽函数

## 4. 实现逻辑
```cpp
return collector(cbf(args...));
```
1. **`cbf(args...)`**: 调用回调函数，获取返回值
2. **`collector(返回值)`**: 将返回值传递给Collector处理
3. **返回Collector的决定**: Collector的`operator()`返回bool，指示是否继续

## 与其他特化的对比

### 非void版本（当前代码）
```cpp
return collector(cbf(args...));
// 传递槽函数的返回值给Collector
```

### void特化版本
```cpp
cbf(args...);          // 调用函数，无返回值
return collector();    // 调用无参版本的Collector
```

## 示例1：使用 CollectorLast<int>
```cpp
Simple::Signal<int(int), Simple::CollectorLast<int>> signal;
signal.connect([](int x) { return x * 2; });

// emit 内部的调用过程：
CollectorLast<int> collector;
std::function<int(int)> callback = [](int x) { return x * 2; };

// CollectorInvocation 的 invoke 被调用：
bool continue = invoke(collector, callback, 5);
// 等效于：
// int temp = callback(5);    // temp = 10
// bool continue = collector(temp);  // collector保存10，返回true
```

## 示例2：使用 CollectorUntil0<bool>
```cpp
Simple::Signal<bool(int), Simple::CollectorUntil0<bool>> signal;
signal.connect([](int x) { return x > 0; });

// emit(0) 时的调用过程：
CollectorUntil0<bool> collector;
std::function<bool(int)> callback = [](int x) { return x > 0; };

bool continue = invoke(collector, callback, 0);
// 等效于：
// bool temp = callback(0);    // temp = false
// bool continue = collector(temp);  // collector返回false（停止）
```

**`CollectorInvocation` 是 SimpleSignal 库中的关键桥梁组件：**

✅ **类型安全的分发** - 为不同返回类型提供正确的调用方式  
✅ **高效的执行** - 编译时确定调用策略，零运行时开销  
✅ **清晰的架构** - 分离调用逻辑和返回值处理逻辑  
✅ **灵活的扩展** - 模板设计允许轻松添加新的特化版本  

这个组件展示了C++模板元编程的强大能力，通过编译时多态实现了既类型安全又高性能的回调机制。

# a

## 📚 推荐学习顺序

学习这个库应该采用**自底向上**的方式，从基础组件到高级接口：

### 第一阶段：基础组件（1-2天）
1. **Collector 系列类**（最简单）
   - `CollectorLast` - 基础返回值处理
   - `CollectorDefault` - 默认策略（特别是void特化）
   - `CollectorUntil0`/`CollectorWhile0` - 条件中断策略
   - `CollectorVector` - 多值收集策略

2. **CollectorInvocation**（调用机制）
   - 理解普通返回值和void的特化区别
   - 学习模板特化技术

### 第二阶段：核心实现（2-3天）
3. **ProtoSignal 类**（最复杂但最重要）
   - 回调存储机制（`std::list<std::shared_ptr<std::function>>`）
   - 连接管理（add_cb/remove_cb）
   - 信号发射逻辑（emit方法）

### 第三阶段：用户接口（1天）
4. **Signal 类**
   - 继承关系和模板参数
   - 简洁的API设计

5. **slot 辅助函数**
   - 成员函数绑定机制
   - lambda捕获技巧

### 第四阶段：综合应用（1-2天）
6. **测试用例分析**
   - 查看BasicSignalTests等测试代码
   - 理解实际使用方式

## 🎯 详细学习路径

### 第1步：理解Collector设计模式（2小时）
```cpp
// 先看最简单的CollectorLast
template<typename Result>
struct CollectorLast {
  using CollectorResult = Result;
  explicit CollectorLast() : last_() {}
  inline bool operator()(Result r) { last_ = r; return true; }
  CollectorResult result() { return last_; }
private:
  Result last_;
};
```
**学习要点**：
- 理解策略模式的应用
- 掌握`operator()`的重载使用
- 了解返回值处理的基本概念

### 第2步：分析void特化（1小时）
```cpp
template<>
struct CollectorDefault<void> {
  using CollectorResult = void;
  void result() {}
  inline bool operator()(void) { return true; }
};
```
**学习要点**：
- 模板特化语法
- void类型的特殊处理
- 空函数的设计意义

### 第3步：研究调用机制（2小时）
```cpp
template<class Collector, class R, class... Args>
struct CollectorInvocation<Collector, R (Args...)> {
  inline bool invoke(Collector &collector, const std::function<R (Args...)> &cbf, Args... args) const
  {
    return collector(cbf(args...));
  }
};
```
**学习要点**：
- 函数签名匹配
- 参数转发机制
- 调用链设计

### 第4步：深入ProtoSignal（3-4小时）
```cpp
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)> {
  // 重点研究这些部分：
  CallbackList callback_list_;  // 回调存储
  size_t add_cb(const CbFunction& cb);  // 添加回调
  bool remove_cb(size_t id);  // 移除回调
  CollectorResult emit(Args... args) const;  // 发射信号
};
```
**学习要点**：
- `std::shared_ptr`的生命周期管理
- 连接ID生成机制（指针地址作为ID）
- 信号发射时的迭代安全
- 收集器与调用机制的协同工作

### 第5步：理解用户接口Signal（1小时）
```cpp
template <typename SignalSignature, class Collector = Lib::CollectorDefault<typename std::function<SignalSignature>::result_type>>
struct Signal : Lib::ProtoSignal<SignalSignature, Collector> {
  using ProtoSignal = Lib::ProtoSignal<SignalSignature, Collector>;
  Signal(const CbFunction &method = CbFunction()) : ProtoSignal(method) {}
};
```
**学习要点**：
- 模板默认参数的使用
- 继承构造函数的简洁设计
- 类型别名的合理使用

### 第6步：掌握slot辅助函数（1小时）
```cpp
template<class Instance, class Class, class R, class... Args>
std::function<R (Args...)> slot(Instance &object, R (Class::*method)(Args...))
{
  return [&object, method](Args... args) { 
    return (object.*method)(args...);
  };
}
```
**学习要点**：
- 成员函数指针语法
- lambda捕获引用和指针的区别
- 类型擦除的std::function使用

## 🧪 实践学习方法

### 方法1：编写测试代码
```cpp
// 逐步测试每个组件
void test_collector_last() {
    Simple::CollectorLast<int> collector;
    collector(1); collector(2); collector(3);
    assert(collector.result() == 3); // 只保留最后一个值
}

void test_basic_signal() {
    Simple::Signal<void(int)> signal;
    int count = 0;
    signal.connect([&count](int x) { count += x; });
    signal.emit(5);
    assert(count == 5);
}
```

### 方法2：跟踪执行流程
使用调试器或打印语句跟踪一个简单信号的完整生命周期：
1. 信号创建
2. 回调连接
3. 信号发射
4. 回调执行
5. 结果收集
6. 回调断开

### 方法3：实现自定义Collector
```cpp
// 实现一个收集最大值的Collector
template<typename Result>
struct CollectorMax {
    using CollectorResult = Result;
    Result max_value = std::numeric_limits<Result>::min();
    bool operator()(Result r) {
        if (r > max_value) max_value = r;
        return true;
    }
    CollectorResult result() { return max_value; }
};
```

## 💡 重点理解的概念

### 1. 类型擦除（Type Erasure）
`std::function`如何存储各种可调用对象

### 2. 模板特化（Template Specialization）
针对void返回值的特殊处理

### 3. 策略模式（Policy-based Design）
Collector作为模板参数提供灵活的行为定制

### 4. SFINAE（Substitution Failure Is Not An Error）
模板参数匹配和重载解析

### 5. 生命周期管理
`std::shared_ptr`在回调管理中的应用

## 🚀 进阶学习建议

### 1. 性能优化思考
- 为什么选择`std::list`而不是`std::vector`？
- 连接ID生成机制的优势和局限性
- 内存分配优化的可能性

### 2. 线程安全扩展
- 如何添加线程安全支持？
- 递归锁的使用场景
- 原子操作的应用

### 3. 与其他库对比
- 对比Boost.Signals2的设计差异
- 分析Qt信号槽的moc机制优劣
- 研究folly::Function的实现

## 📝 学习检查清单

- [ ] 理解所有Collector的工作原理
- [ ] 掌握ProtoSignal的回调管理机制
- [ ] 能解释emit方法的完整执行流程
- [ ] 会使用slot函数绑定成员函数
- [ ] 能实现自定义的Collector策略
- [ ] 理解类型擦除和模板特化技术
- [ ] 能指出代码中的C风格痕迹并知道现代C++替代方案

按照这个顺序和方法，你应该能在1-2周内彻底掌握这个信号库的设计精髓和实现细节。记得多写测试代码，实际动手是理解的最佳途径！