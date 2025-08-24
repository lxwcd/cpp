SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 

# 信号创建

```cpp
Simple::Signal<char(float, int, std::string)> sig1;
```

这行代码虽然看起来简单，但背后涉及了复杂的模板实例化和类型推导过程。让我为你详细分解每一步的执行过程。

## 🏗️ 代码结构分解

```cpp
Simple::Signal<char(float, int, std::string)> sig1;
```

让我们将这行代码分解为几个部分来理解：

1. **命名空间**: `Simple::`
2. **模板类**: `Signal`
3. **模板参数**: `char(float, int, std::string)`
4. **变量名**: `sig1`

## 🔄 编译器的处理步骤

### 步骤1: 模板参数解析
编译器首先解析模板参数 `char(float, int, std::string)`:

- 这是一个**函数类型**表示法，不是函数调用
- **返回类型**: `char`
- **参数类型**: `float`, `int`, `std::string`

### 步骤2: 模板实例化
编译器实例化 `Simple::Signal` 模板类：

```cpp
// 编译器生成的代码大致如下：
Simple::Signal<
    char(float, int, std::string),                    // SignalSignature
    Simple::Lib::CollectorDefault<char>               // 默认Collector
> sig1;
```

### 步骤3: 确定默认Collector
由于没有显式指定Collector，使用默认的：

```cpp
// CollectorDefault 的定义
template<typename Result>
struct CollectorDefault : CollectorLast<Result> {};

// 所以实际使用的是：
Simple::Lib::CollectorLast<char>
```

### 步骤4: 继承关系解析
`Signal` 继承自 `ProtoSignal`:

```cpp
// Signal 的定义
template <typename SignalSignature, class Collector = ...>
struct Signal : Lib::ProtoSignal<SignalSignature, Collector>

// 所以这里实例化：
Lib::ProtoSignal<char(float, int, std::string), CollectorLast<char>>
```

### 步骤5: ProtoSignal 特化选择
`ProtoSignal` 选择正确的特化版本：

```cpp
// 匹配这个特化版本
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector>

// 其中：
// R = char
// Args... = float, int, std::string
// Collector = CollectorLast<char>
```

### 步骤6: 类型别名定义
在 `ProtoSignal` 内部定义类型别名：

```cpp
protected:
  using CbFunction = std::function<char (float, int, std::string)>;
  using Result = typename CbFunction::result_type;  // = char
  using CollectorResult = typename Collector::CollectorResult;  // = char
```

### 步骤7: 构造函数调用
最后调用构造函数：

```cpp
// Signal 的构造函数
Signal (const CbFunction &method = CbFunction()) : ProtoSignal (method) {}

// ProtoSignal 的构造函数
ProtoSignal (const CbFunction &method) {
  if (method)
    add_cb(method);  // 如果有默认回调，添加它
}
```

由于没有提供默认回调，所以创建一个空的 `std::function`，不添加任何回调。

## 📊 生成的完整类型结构

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

## 🔧 方法签名生成

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

## 💡 设计模式和技术

### 1. 模板元编程
- 使用模板特化根据函数签名生成特定代码
- 编译时多态而不是运行时多态

### 2. 类型推导
- 从函数类型 `char(float, int, std::string)` 推导出:
  - 返回类型 `char`
  - 参数类型 `float, int, std::string`

### 3. 策略模式
- `Collector` 作为模板参数提供可配置的行为
- 默认使用 `CollectorLast` 策略

### 4. 继承与组合
- `Signal` 继承自 `ProtoSignal`，获得实现
- `ProtoSignal` 使用 `CollectorInvocation` 处理调用

## 🎯 实际创建的对象

当这行代码执行后，内存中创建了一个 `sig1` 对象，包含：

1. **一个空的回调列表**：`std::list<std::shared_ptr<std::function<char(...)>>>`
2. **Collector实例**：在emit时创建，用于处理返回值
3. **类型信息**：编译时确定的特定类型签名

## 🔍 与其他声明的对比

### 不同的信号签名
```cpp
// 无参数无返回值
Simple::Signal<void()> sig1;

// 多参数有返回值
Simple::Signal<int(std::string, double)> sig2;

// 使用不同的Collector
Simple::Signal<bool(int), Simple::CollectorUntil0<bool>> sig3;
```

每种声明都会导致不同的模板实例化，生成特定类型的信号对象。

## 💎 总结

**这行代码 `Simple::Signal<char(float, int, std::string)> sig1;` 的完整执行过程：**

1. **模板解析**：解析函数类型 `char(float, int, std::string)`
2. **模板实例化**：实例化 `Signal` 和 `ProtoSignal` 模板
3. **类型推导**：推导出返回类型 `char` 和参数类型 `float, int, std::string`
4. **Collector选择**：使用默认的 `CollectorLast<char>`
5. **继承关系建立**：`Signal` 继承自特化的 `ProtoSignal`
6. **类型别名定义**：定义 `CbFunction`, `Result`, `CollectorResult`
7. **对象构造**：创建信号对象，初始化空回调列表

这个过程中，C++编译器进行了复杂的模板元编程工作，生成了一个类型安全、高效的特化信号类，专门用于处理 `char(float, int, std::string)` 类型的回调函数。


# 信号返回值
信号可以有返回值，也可以为空。

### 1. 流程控制（你提到的部分）
**通过Collector策略控制是否继续执行后续槽函数**

```cpp
// 使用CollectorUntil0：遇到false/0时停止
Simple::Signal<bool(), Simple::CollectorUntil0<bool>> signal;
signal.connect([] { return true; });   // 继续
signal.connect([] { return false; });  // 停止！后续槽函数不会执行
signal.connect([] { return true; });   // 不会执行

bool result = signal.emit();  // result = false
```

### 2. 结果收集与聚合
**收集和聚合多个槽函数的返回值**

```cpp
// 使用CollectorVector：收集所有返回值
Simple::Signal<int(), Simple::CollectorVector<int>> signal;
signal.connect([] { return 1; });
signal.connect([] { return 2; });
signal.connect([] { return 3; });

auto results = signal.emit();  // results = {1, 2, 3}
```

### 3. 业务逻辑返回值
**槽函数可以返回有意义的业务结果**

```cpp
// 验证链：每个验证器返回验证结果
Simple::Signal<bool(User), Simple::CollectorUntil0<bool>> validators;

validators.connect([](User u) { return validateAge(u); });
validators.connect([](User u) { return validateEmail(u); });
validators.connect([](User u) { return validatePassword(u); });

bool allValid = validators.emit(user);  // 所有验证都通过？
```

## 技术实现机制

### Collector 的双重职责
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