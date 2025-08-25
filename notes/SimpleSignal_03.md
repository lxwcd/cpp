SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 

# Collector 的双重职责
每个 Collector 策略都有两个核心方法：

如面有返回值的实现：
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