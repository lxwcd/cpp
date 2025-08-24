SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 

```cpp
accu = "";
Simple::Signal<char(float, int, std::string)> sig1;
size_t id1 = sig1.connect(float_callback);
size_t id2 = sig1.connect([](float, int i, std::string) { accu += string_printf("int: %d\n", i); return 0; });
size_t id3 = sig1.connect([](float, int, const std::string& s) { accu += string_printf("string: %s\n", s.c_str()); return 0; });
sig1.emit(.3, 4, "huhu");
```

# 信号创建

```cpp
Simple::Signal<char(float, int, std::string)> sig1;
```

1. **命名空间**: `Simple::`
2. **模板类**: `Signal`
3. **模板参数**: `char(float, int, std::string)`
4. **变量名**: `sig1`

## 模板参数解析
编译器首先解析模板参数 `char(float, int, std::string)`:

- 这是一个**函数类型**表示法，不是函数调用
- **返回类型**: `char`
- **参数类型**: `float`, `int`, `std::string`

## 模板实例化
Singal 模板里的定义如下：
```cpp
template <typename SignalSignature, class Collector = Lib::CollectorDefault<typename std::function<SignalSignature>::result_type> >
struct Signal /*final*/ :
    Lib::ProtoSignal<SignalSignature, Collector>
{
  using ProtoSignal = Lib::ProtoSignal<SignalSignature, Collector>;
  using CbFunction = typename ProtoSignal::CbFunction;
  /// Signal constructor, supports a default callback as argument.
  Signal (const CbFunction &method = CbFunction()) : ProtoSignal (method) {}
};
```
编译器实例化 `Simple::Signal` 模板类：

```cpp
// 编译器生成的代码大致如下：
Simple::Signal<
    char(float, int, std::string),                    // SignalSignature
    Simple::Lib::CollectorDefault<char>               // 默认Collector
> sig1;
```

## 确定默认Collector
由于没有显式指定Collector，使用默认的，且有返回值，选择下面的版本：
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

```cpp
// CollectorDefault 的定义
template<typename Result>
struct CollectorDefault : CollectorLast<Result> {};

// 所以实际使用的是：
Simple::Lib::CollectorLast<char>
```

## 继承关系解析
`Signal` 继承自 `ProtoSignal`:

```cpp
// Signal 的定义
template <typename SignalSignature, class Collector = ...>
struct Signal : Lib::ProtoSignal<SignalSignature, Collector>

// 所以这里实例化：
Lib::ProtoSignal<char(float, int, std::string), CollectorLast<char>>
```

## ProtoSignal 特化选择
`ProtoSignal` 的特化版本：

```cpp
/// ProtoSignal template specialised for the callback signature and collector.
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)> 

// 其中：
// R = char
// Args... = float, int, std::string
// Collector = CollectorLast<char>
```

## ProtoSignal 继承自 CollectorInvocation
CollectorInvocation 提供 invoke 接口，该接口在信号的 emit 接口中调用，是所有信号都需要有的接口。
但信号有的有返回值，有的无返回值，即为 void。
因此这里 CollectorInvocation 提供了两种特化的实现。
private 继承表示"实现继承"而非"接口继承"。

本例中返回值不为空，因此采用下面模板：
```cpp
/// CollectorInvocation specialisation for regular signals.
template<class Collector, class R, class... Args>
struct CollectorInvocation<Collector, R (Args...)> {
  inline bool
  invoke (Collector &collector, const std::function<R (Args...)> &cbf, Args... args) const
  {
    return collector (cbf (args...));
  }
};
```

## 类型别名定义
在 `ProtoSignal` 内部定义类型别名：

```cpp
protected:
  using CbFunction = std::function<char (float, int, std::string)>;
  using Result = typename CbFunction::result_type;  // = char
  using CollectorResult = typename Collector::CollectorResult;  // = char
```

## 构造函数调用
最后调用构造函数：

```cpp
// Signal 的构造函数
Signal (const CbFunction &method = CbFunction()) : ProtoSignal (method) {}

// ProtoSignal 的构造函数
ProtoSignal (const CbFunction &method) {
  if (method)
    add_cb(method); 
}
```

## 生成的完整类型结构

编译器最终会生成类似这样的类型：

```cpp
// 实例化的 ProtoSignal 特化
class ProtoSignal<char(float, int, std::string), CollectorLast<char>> 
    : private CollectorInvocation<CollectorLast<char>, char(float, int, std::string)> {
    
    // 类型别名
    using CbFunction = std::function<char(float, int, std::string)>;
    using Result = char;
    using CollectorResult = char;
    
    // 成员变量
    std::list<std::shared_ptr<std::function<char(float, int, std::string)>>> callback_list_;
    
    // 方法
    size_t connect(const CbFunction& cb);
    bool disconnect(size_t id);
    char emit(float, int, std::string) const;
    // ...
};

// Signal 只是薄包装
class Signal<char(float, int, std::string), CollectorLast<char>>
    : public ProtoSignal<char(float, int, std::string), CollectorLast<char>> {
    
    // 构造函数
    Signal(const CbFunction& method = CbFunction()) 
        : ProtoSignal<char(float, int, std::string), CollectorLast<char>>(method) {}
};
```

## 方法签名生成

### emit 方法签名
基于模板参数，生成特定的 emit 方法：

```cpp
// 在 ProtoSignal 中
CollectorResult emit(Args... args) const;

// 实例化为：
char emit(float arg1, int arg2, std::string arg3) const;
```

### connect 方法签名
```cpp
// 在 ProtoSignal 中
size_t connect(const CbFunction& cb);

// 实例化为：
size_t connect(const std::function<char(float, int, std::string)>& cb);
```

## 实际创建的对象

当这行代码执行后，内存中创建了一个 `sig1` 对象，包含：

1. **一个空的回调列表**：`std::list<std::shared_ptr<std::function<char(...)>>>`
2. **Collector实例**：在emit时创建，用于处理返回值
3. **类型信息**：编译时确定的特定类型签名

# connect 信号连接
```cpp
static char
    float_callback(float f, int, std::string)
{
    accu += string_printf("float: %.2f\n", f);
    return 0;
}

accu = "";
Simple::Signal<char(float, int, std::string)> sig1;
size_t id1 = sig1.connect(float_callback);
size_t id2 = sig1.connect([](float, int i, std::string) { accu += string_printf("int: %d\n", i); return 0; });
size_t id3 = sig1.connect([](float, int, const std::string& s) { accu += string_printf("string: %s\n", s.c_str()); return 0; });
sig1.emit(.3, 4, "huhu");
```

上面创建一个信号 sig1 后，该信号连接三个槽函数，三个槽函数的参数和返回值都要和信号中的一致。

三个槽函数都被转换为下面类型：
```cpp
using CbFunction = std::function<R (Args...)>;
```
connect 接口：
```cpp
using CallbackSlot = std::shared_ptr<CbFunction>;
using CallbackList = std::list<CallbackSlot>;
CallbackList callback_list_;

size_t add_cb(const CbFunction& cb)
{
callback_list_.emplace_back(std::make_shared<CbFunction>(cb));
return size_t (callback_list_.back().get());
}

/// Operator to add a new function or lambda as signal handler, returns a handler connection ID.
size_t connect (const CbFunction &cb)      { return add_cb(cb); }
```

将三个槽函数都放到一个 std::list 中。

# emit 发射信号
```cpp
sig1.emit(.3, 4, "huhu");
```

```cpp
  using CollectorResult = typename Collector::CollectorResult;

  /// Emit a signal, i.e. invoke all its callbacks and collect return types with the Collector.
  CollectorResult
  emit (Args... args) const
  {
    Collector collector;
    for (auto &slot : callback_list_) {
        if (slot) {
            const bool continue_emission = this->invoke (collector, *slot, args...);
            if (!continue_emission)
              break;
        }
    }
    return collector.result();
  }
```

这里的 Collector 是 CollectorLast，这里会构造一个 Collector 对象，返回的类型为 Result 类型，这里为 char。
```cpp
/// CollectorLast returns the result of the last signal handler from a signal emission.
template<typename Result>
struct CollectorLast {
  using CollectorResult = Result;
  explicit        CollectorLast ()              : last_() {}
  inline bool     operator()    (Result r)      { last_ = r; return true; }
  CollectorResult result        ()              { return last_; }
private:
  Result last_;
};
```

调用 invoke 方法继承自 CollectorInvocation：
```cpp
/// CollectorInvocation specialisation for regular signals.
template<class Collector, class R, class... Args>
struct CollectorInvocation<Collector, R (Args...)> {
  inline bool
  invoke (Collector &collector, const std::function<R (Args...)> &cbf, Args... args) const
  {
    return collector (cbf (args...));
  }
};
```

这里 cbf 函数执行的返回值作为参数传递给 colletor，最后调用带参数 Result 的仿函数，这里槽函数的返回值保存到 last_ 中，并返回 true。
```cpp
/// CollectorLast returns the result of the last signal handler from a signal emission.
template<typename Result>
struct CollectorLast {
  using CollectorResult = Result;
  explicit        CollectorLast ()              : last_() {}
  inline bool     operator()    (Result r)      { last_ = r; return true; }
  CollectorResult result        ()              { return last_; }
private:
  Result last_;
};
```

因为这种始终返回 true，因此 emit 中会根据连接槽函数的顺序，将信号所有的槽函数都执行一遍。最后返回的是最后一个槽函数的执行结果。
```cpp
  /// Emit a signal, i.e. invoke all its callbacks and collect return types with the Collector.
  CollectorResult
  emit (Args... args) const
  {
    Collector collector;
    for (auto &slot : callback_list_) {
        if (slot) {
            const bool continue_emission = this->invoke (collector, *slot, args...);
            if (!continue_emission)
              break;
        }
    }
    return collector.result();
  }
```

# Collector 的双重职责
每个 Collector 策略都有两个核心方法：

```cpp
template<typename Result>
struct CollectorLast {
    // 1. 处理单个返回值
    bool operator()(Result r) {
        last_ = r;      // 保存返回值
        return true;    // 是否继续执行
    }
    
    // 2. 提供最终结果
    Result result() {
        return last_;   // 返回收集的结果
    }
};
```

### emit 方法的执行流程
```cpp
CollectorResult emit(Args... args) const {
    Collector collector;  // 创建收集器实例
    
    for (auto &slot : callback_list_) {
        if (slot) {
            // 调用槽函数并获取返回值
            Result slot_result = (*slot)(args...);
            
            // 将返回值传递给Collector，并决定是否继续
            bool continue_emission = collector(slot_result);
            
            if (!continue_emission)
                break;  // 中断执行
        }
    }
    
    return collector.result();  // 返回最终收集的结果
}
```

## 实际应用场景

### 场景1：条件中断处理链
```cpp
// 数据处理管道：任何一步失败就停止
Simple::Signal<ProcessingResult(Data&), Simple::CollectorUntil0<ProcessingResult>> pipeline;

pipeline.connect([](Data& d) { return validateData(d); });
pipeline.connect([](Data& d) { return cleanData(d); });
pipeline.connect([](Data& d) { return transformData(d); });
pipeline.connect([](Data& d) { return storeData(d); });

ProcessingResult finalResult = pipeline.emit(data);
```

### 场景2：结果聚合
```cpp
// 投票系统：收集所有投票结果
Simple::Signal<VoteResult(), Simple::CollectorVector<VoteResult>> voting;

// 多个投票者
voting.connect([] { return member1.vote(); });
voting.connect([] { return member2.vote(); });
voting.connect([] { return member3.vote(); });

auto allVotes = voting.emit();  // 分析所有投票结果
```

### 场景3：优先级处理
```cpp
// 事件处理：高优先级处理器可以中断后续处理
Simple::Signal<EventResult(Event&), Simple::CollectorUntil0<EventResult>> eventHandlers;

// 高优先级处理器先执行
eventHandlers.connect([](Event& e) { return securityCheck(e); });
// 常规处理器
eventHandlers.connect([](Event& e) { return processEvent(e); });
// 日志处理器（总是执行，除非被中断）
eventHandlers.connect([](Event& e) { logEvent(e); return true; });

EventResult result = eventHandlers.emit(event);
```

## 默认行为：CollectorLast

在大多数情况下，当使用默认的 `CollectorLast` 时：

```cpp
Simple::Signal<int()> signal;  // 默认使用CollectorLast<int>
```

返回值的主要作用是**提供业务逻辑结果**，而不是控制流程，因为 `CollectorLast` 总是返回 `true`（继续执行）。

## 流程控制的不同策略

### 1. CollectorUntil0
```cpp
// 遇到假值（0/false）时停止
template<typename Result>
struct CollectorUntil0 {
    bool operator()(Result r) {
        result_ = r;
        return result_ ? true : false;  // 真值继续，假值停止
    }
};
```

### 2. CollectorWhile0
```cpp
// 只有遇到假值（0/false）时才继续
template<typename Result>
struct CollectorWhile0 {
    bool operator()(Result r) {
        result_ = r;
        return result_ ? false : true;  // 假值继续，真值停止
    }
};
```

### 3. CollectorLast（默认）
```cpp
// 总是继续，不管返回值
template<typename Result>
struct CollectorLast {
    bool operator()(Result r) {
        last_ = r;
        return true;  // 总是继续
    }
};
```

## 💎 总结

**信号返回值的核心用途确实包括流程控制，但不止于此：**

✅ **流程控制** - 通过Collector策略决定是否继续执行后续槽函数  
✅ **结果收集** - 聚合多个槽函数的返回值（如CollectorVector）  
✅ **业务逻辑** - 槽函数可以返回有意义的业务结果  
✅ **灵活性** - 通过不同的Collector策略支持各种使用场景  

**你的理解是正确的**：返回值确实可以用于控制是否继续发射后面的槽函数，但这只是其中一种用法。根据使用的Collector策略不同，返回值可以发挥不同的作用。

这种设计提供了极大的灵活性，使得SimpleSignal库能够适应各种不同的应用场景，从简单的事件通知到复杂的数据处理管道。