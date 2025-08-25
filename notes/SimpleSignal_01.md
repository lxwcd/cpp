SimpleSignal 开源库

# 学习资料
> [SimpleSignal/README.md at master · larspensjo/SimpleSignal](https://github.com/larspensjo/SimpleSignal/blob/master/README.md) 

# Collector 系列类
作为 信号 类的第二个参数，用于存储和返回信号的返回值。

信号可以有返回值或者返回void。

## 默认 Collector
```cpp
/// CollectorDefault implements the default signal handler collection behaviour.
template<typename Result>
struct CollectorDefault : CollectorLast<Result>
{};
```

## 默认 Collector 特化无返回值版本
```cpp
/// CollectorDefault specialisation for signals with void return type.
template<>
struct CollectorDefault<void> {
  using CollectorResult = void;
  void                  result     ()           {}
  inline bool           operator() (void)       { return true; }
};
```

根据上面仿函数的调用，始终返回 true，因此在 emit 发射信号时，会按照信号 connect 槽函数的顺序，依次调用槽函数，并且全部调用。

## CollectorLast 默认有返回值版本
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

/// CollectorDefault implements the default signal handler collection behaviour.
template<typename Result>
struct CollectorDefault : CollectorLast<Result>
{};
```

根据上面仿函数的调用，始终返回 true，因此在 emit 发射信号时，会按照信号 connect 槽函数的顺序，依次调用槽函数，并且全部调用。

上面会将槽函数的返回结果保存，但所有槽函数都会执行，最终 emit 返回最后一个槽函数返回的结果。

## CollectorUntil0 条件中断
```cpp
/// Keep signal emissions going while all handlers return !0 (true).
template<typename Result>
struct CollectorUntil0 {
  using CollectorResult = Result;
  explicit                      CollectorUntil0 ()      : result_() {}
  const CollectorResult&        result          ()      { return result_; }
  inline bool
  operator() (Result r)
  {
    result_ = r;
    return result_ ? true : false;
  }
private:
  CollectorResult result_;
};
```

初始 result_ 是值初始化，初始化为 0，即 flase。
emit 发射信号后，依次调用槽函数，每个槽函数的返回值保存下来，并返回槽函数的返回值，并非全部为 true。
因此，如果有个槽函数返回 false，则后面的槽函数不会被执行了。

### 示例
```cpp

class TestCollectorUntil0 {
    bool check1, check2;
    TestCollectorUntil0() : check1(0), check2(0) {}
    bool handler_true() { check1 = true; return true; }
    bool handler_false() { check2 = true; return false; }
    bool handler_abort() { std::abort(); }
public:
    static void
        run()
    {
        TestCollectorUntil0 self;
        Simple::Signal<bool(), Simple::CollectorUntil0<bool>> sig_until0;
        sig_until0.connect(Simple::slot(self, &TestCollectorUntil0::handler_true));
        sig_until0.connect(Simple::slot(self, &TestCollectorUntil0::handler_false));
        sig_until0.connect(Simple::slot(self, &TestCollectorUntil0::handler_abort));
        assert(!self.check1 && !self.check2);
        const bool result = sig_until0.emit();
        assert(!result && self.check1 && self.check2);
    }
};
```

上面第一次调用 assert 时，check1 和 check2 都是 0，因此没问题。
调用 emit 后，先执行类成员槽函数 handler_true，返回 true，因此继续执行下一个槽函数，即 handler_false,
这个执行后返回 false，这时 emit 终止后续槽函数的执行，返回 false，因此 result 为 false。

## CollectorWhile0 条件中断
```cpp
/// Keep signal emissions going while all handlers return 0 (false).
template<typename Result>
struct CollectorWhile0 {
  using CollectorResult = Result;
  explicit                      CollectorWhile0 ()      : result_() {}
  const CollectorResult&        result          ()      { return result_; }
  inline bool
  operator() (Result r)
  {
    result_ = r;
    return result_ ? false : true;
  }
private:
  CollectorResult result_;
};
```

槽函数返回 true 时终止后续槽函数的调用，而槽函数返回 false 时继续后续槽函数的调用。

### 示例
```cpp

class TestCollectorWhile0 {
    bool check1, check2;
    TestCollectorWhile0() : check1(0), check2(0) {}
    bool handler_0() { check1 = true; return false; }
    bool handler_1() { check2 = true; return true; }
    bool handler_abort() { std::abort(); }
public:
    static void
        run()
    {
        TestCollectorWhile0 self;
        Simple::Signal<bool(), Simple::CollectorWhile0<bool>> sig_while0;
        sig_while0.connect(Simple::slot(self, &TestCollectorWhile0::handler_0));
        sig_while0.connect(Simple::slot(self, &TestCollectorWhile0::handler_1));
        sig_while0.connect(Simple::slot(self, &TestCollectorWhile0::handler_abort));
        assert(!self.check1 && !self.check2);
        const bool result = sig_while0.emit();
        assert(result == true && self.check1 && self.check2);
    }
};
```

## CollectorVector 多值收集
```cpp
/// CollectorVector returns the result of the all signal handlers from a signal emission in a std::vector.
template<typename Result>
struct CollectorVector {
  using CollectorResult = std::vector<Result>;
  const CollectorResult&        result ()       { return result_; }
  inline bool
  operator() (Result r)
  {
    result_.push_back (r);
    return true;
  }
private:
  CollectorResult result_;
};
```

这个返回值不是单值，而是用 vector 存储，因此能获取所有槽函数的返回结果。
emit 发射信号后，每个槽函数的返回值存储到 vector 中保存，但始终返回 true，因此会执行全部槽函数。

### 示例
```cpp
class TestCollectorVector {
    static int handler1() { return 1; }
    static int handler42() { return 42; }
    static int handler777() { return 777; }
public:
    static void
        run()
    {
        Simple::Signal<int(), Simple::CollectorVector<int>> sig_vector;
        sig_vector.connect(handler777);
        sig_vector.connect(handler42);
        sig_vector.connect(handler1);
        sig_vector.connect(handler42);
        sig_vector.connect(handler777);
        std::vector<int> results = sig_vector.emit();
        const std::vector<int> reference = { 777, 42, 1, 42, 777, };
        assert(5 == sig_vector.size());
        assert(results == reference);
    }
};
```

# connect 连接方式
```cpp
/// ProtoSignal template specialised for the callback signature and collector.
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)> {
protected:
  using CbFunction = std::function<R (Args...)>;
  using Result = typename CbFunction::result_type;
  using CollectorResult = typename Collector::CollectorResult;

/// Operator to add a new function or lambda as signal handler, returns a handler connection ID.
size_t connect (const CbFunction &cb)      { return add_cb(cb); }
```

## 连接类的静态成员函数
```cpp
    static char
        float_callback(float f, int, std::string)
    {
        accu += string_printf("float: %.2f\n", f);
        return 0;
    }
public:
    static void
        run()
    {
        accu = "";
        Simple::Signal<char(float, int, std::string)> sig1;
        size_t id1 = sig1.connect(float_callback);
```

## 连接 lambda 表达式
```cpp
size_t id2 = sig1.connect([](float, int i, std::string) { accu += string_printf("int: %d\n", i); return 0; });
```

## 连接 slot 辅助函数
```cpp
/// This function creates a std::function by binding @a object to the member function pointer @a method.
template<class Instance, class Class, class R, class... Args> std::function<R (Args...)>
slot (Instance &object, R (Class::*method) (Args...))
{
  return [&object, method] (Args... args) { return (object .* method) (args...); };
}

/// This function creates a std::function by binding @a object to the member function pointer @a method.
template<class Class, class R, class... Args> std::function<R (Args...)>
slot (Class *object, R (Class::*method) (Args...))
{
  return [object, method] (Args... args) { return (object ->* method) (args...); };
}
```

```cpp
class TestCollectorUntil0 {
    bool check1, check2;
    TestCollectorUntil0() : check1(0), check2(0) {}
    bool handler_true() { check1 = true; return true; }
    bool handler_false() { check2 = true; return false; }
    bool handler_abort() { std::abort(); }
public:
    static void
        run()
    {
        TestCollectorUntil0 self;
        Simple::Signal<bool(), Simple::CollectorUntil0<bool>> sig_until0;
        sig_until0.connect(Simple::slot(self, &TestCollectorUntil0::handler_true));
```

# 信号类

核心设计理念：接口与实现分离

- **`Signal`**: 用户友好的接口类（面向用户）
- **`ProtoSignal`**: 核心实现类（面向内部实现）

## Signal 类（用户接口层）
```cpp
template <typename SignalSignature, class Collector = ...>
struct Signal : Lib::ProtoSignal<SignalSignature, Collector> {
    using ProtoSignal = Lib::ProtoSignal<SignalSignature, Collector>;
    using CbFunction = typename ProtoSignal::CbFunction;
    
    Signal(const CbFunction &method = CbFunction()) : ProtoSignal(method) {}
};
```
**职责**：
- 提供简洁的用户API
- 设置合理的默认参数
- 隐藏实现细节
- 提供类型别名方便使用

## ProtoSignal 类（实现层）
```cpp

/// ProtoSignal template specialised for the callback signature and collector.
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)> {
protected:
  using CbFunction = std::function<R (Args...)>;
  using Result = typename CbFunction::result_type;
  using CollectorResult = typename Collector::CollectorResult;
```
**职责**：
- 实现核心信号槽逻辑
- 实现信号的连接，取消连接，发射实现
- 处理信号发射和返回值收集

## CollectorInvocation 类

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

/// CollectorInvocation specialisation for signals with void return type.
template<class Collector, class... Args>
struct CollectorInvocation<Collector, void (Args...)> {
  inline bool
  invoke (Collector &collector, const std::function<void (Args...)> &cbf, Args... args) const
  {
    cbf (args...); return collector();
  }
};
```

**`CollectorInvocation` 的主要作用是为不同返回类型的信号提供统一的调用接口，正确处理返回值传递给Collector的策略。**
这里有两个版本，有返回值和无返回值，在 emit 接口中会用到 invoke。

信号类继承自这个类，所有信号都需要发射。

# emit 方法
```cpp
/// ProtoSignal template specialised for the callback signature and collector.
template<class Collector, class R, class... Args>
class ProtoSignal<R (Args...), Collector> : private CollectorInvocation<Collector, R (Args...)> {
protected:
  using CbFunction = std::function<R (Args...)>;
  using Result = typename CbFunction::result_type;
  using CollectorResult = typename Collector::CollectorResult;
```

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

emit 返回类型槽函数的返回值 R。
emit 会按照槽函数的连接顺序依次调用槽函数，获取返回值，如果返回值为 false 则终止，否则继续调用。
emit 返回 Collector 的 result，这个看不同 result 的实现，有的返回最后一个槽函数返回值，有的返回一个 vector。