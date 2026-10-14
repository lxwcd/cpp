ThreadPool 学习笔记

# 资源
> [ThreadPool/ThreadPool.h at master · progschj/ThreadPool](https://github.com/progschj/ThreadPool/blob/master/ThreadPool.h) 


# 头文件

1. **头文件保护**：`#ifndef THREAD_POOL_H` 和 `#define THREAD_POOL_H` 防止头文件被多次包含。

2. **包含的必要头文件**：
   - `<vector>`: 用于存储工作线程
   - `<queue>`: 用于存储任务队列
   - `<memory>`: 智能指针支持
   - `<thread>`: 线程支持
   - `<mutex>`: 互斥锁支持
   - `<condition_variable>`: 条件变量支持
   - `<future>`: 异步操作支持
   - `<functional>`: 函数对象支持
   - `<stdexcept>`: 异常支持

3. **类成员**：
   - `workers`: 存储工作线程的向量
   - `tasks`: 存储待执行任务的队列，每个任务是返回void的无参函数
   - `queue_mutex`: 保护任务队列的互斥锁
   - `condition`: 用于线程间通信的条件变量
   - `stop`: 标志位，指示线程池是否应该停止

# enqueue 函数模板

```cpp
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;
```

**创建一个能够接受任意可调用对象和参数，并返回对应返回类型的 `std::future` 的函数模板**。

1. **类型安全**：确保返回的 `future` 类型与函数实际返回类型匹配
2. **灵活性**：可以接受任何可调用对象和任意参数
3. **性能**：使用完美转发避免不必要的拷贝
4. **通用性**：适用于函数指针、lambda、函数对象等各种情况

## `template<class F, class... Args>`
- **模板参数声明**：定义函数模板的参数
- `class F`：第一个模板参数，代表可调用对象的类型（函数、lambda、函数对象等）
- `class... Args`：**可变参数模板**，表示可以有 0 个或多个额外的类型参数
- `...`：参数包展开符号，允许接受任意数量的类型参数

## `auto enqueue(F&& f, Args&&... args)`
- **`auto`**：C++11 的自动返回类型推导，这里用作占位符
- **`F&& f`**：**万能引用**（Universal Reference）
  - 当 `F` 是模板参数时，`&&` 成为万能引用
  - 可以绑定到左值、右值、const/非const对象
- **`Args&&... args`**：万能引用的参数包
  - 接受任意数量和类型的参数
  - `...` 表示参数包展开

## 尾置返回类型

`-> std::future<typename std::result_of<F(Args...)>::type>` 是 C++11 的**尾置返回类型语法**，用于处理复杂的返回类型推导。

```cpp
std::future<                               // 返回一个 future 对象
    typename std::result_of<               // 计算调用结果的类型
        F(Args...)                         // 用 Args... 参数调用 F 的类型
    >::type                                // 提取结果类型
>
```

## `std::result_of<F(Args...)>::type`
- **作用**：在编译时推导 `F` 用 `Args...` 参数调用后的返回类型
- **工作原理**：这是一个类型特征（type trait），在编译时计算
- **示例**：
  ```cpp
  // 如果 F 是 int(int, double)，Args... 是 int, double
  std::result_of<F(int, double)>::type  // 推导为 int
  ```

## C++14 简化版本
```cpp
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
    -> std::future<std::result_of_t<F(Args...)>>;
```
- 使用 `std::result_of_t` 替代 `typename std::result_of<...>::type`
- `_t` 后缀是 C++14 的便利别名

## C++14 自动返回类型推导
```cpp
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
{
    using return_type = std::invoke_result_t<F, Args...>;
    // ... 函数实现
    return std::future<return_type>(...);
}
```

## 使用

```cpp
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;

// 使用示例
ThreadPool pool(4);
std::vector<std::future<int>> results;

for(int i = 0; i < 8; ++i) {
    results.emplace_back(
        pool.enqueue([i] {  // 这一行的语法需要理解
            std::cout << "hello " << i << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "world " << i << std::endl;
            return i*i;
        })
    );
}
```

### Lambda 表达式分析

首先看传递给 `enqueue` 的参数：

```cpp
[i] {  // 这是一个 lambda 表达式
    std::cout << "hello " << i << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "world " << i << std::endl;
    return i*i;  // 返回 int 类型
}
```

这个 lambda 表达式的特点是：
- **捕获列表**：`[i]` - 按值捕获变量 `i`
- **参数列表**：`()` - 没有参数
- **返回类型**：`int`（编译器自动推导）

所以这个 lambda 的类型可以看作是：`一个无参且返回int的可调用对象`

### `enqueue` 函数模板实例化过程

当编译器看到这个调用时：

```cpp
pool.enqueue([i] { ... })
```

它会进行模板参数推导：

```cpp
// 原始模板
template<class F, class... Args>
auto enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>;

// 我们的调用
pool.enqueue([i] { return i*i; });

// 编译器推导：
1. F = 推导为 lambda 表达式的实际类型
2. Args... = 空（因为没有额外参数）

3. 计算返回类型：
   std::result_of<F(Args...)>::type 
   = std::result_of<LambdaType()>::type
   = decltype( 使用()调用lambda的结果类型 ) 
   = decltype( lambda() ) 
   = int

4. 最终实例化：
   auto enqueue(LambdaType&& f) -> std::future<int>
```

# 构造函数实现

```cpp
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads)
    :   stop(false)
{
    for(size_t i = 0;i<threads;++i)
        workers.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock,
                            [this]{ return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty())
                            return;
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                    }

                    task();
                }
            }
        );
}
```

1. **初始化列表**：`: stop(false)` 初始化 `stop` 标志为 `false`

2. **创建工作线程**：
   - 使用 `emplace_back` 直接在向量中构造线程对象
   - 每个线程执行一个lambda函数

3. **工作线程逻辑**：
   - `for(;;)` 无限循环，直到线程池停止
   - 使用 `std::unique_lock` 和条件变量实现线程等待/通知机制
   - `condition.wait(lock, predicate)` 等待条件满足：
     - 当 `stop` 为 `true` 或任务队列不为空时，停止等待
   - 如果线程池已停止且任务队列为空，线程退出
   - 从队列中取出任务并执行

4. **关键点**：
   - 使用 `std::move` 转移任务所有权，避免不必要的拷贝
   - 任务执行在锁外进行，允许其他线程同时处理任务

## enqueue 方法实现

```cpp
// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){ (*task)(); });
    }
    condition.notify_one();
    return res;
}
```

### 代码解析：

1. **模板声明**：
   - `template<class F, class... Args>`: 可变参数模板，接受任意可调用对象和参数
   - 使用完美转发 (`F&& f`, `Args&&... args`) 保持参数的值类别

2. **返回类型推导**：
   - `-> std::future<typename std::result_of<F(Args...)>::type>`: 尾置返回类型
   - `std::result_of<F(Args...)>::type` 推导函数调用结果的类型
   - C++17 后可以使用 `std::invoke_result_t` 替代

3. **任务包装**：
   - 使用 `std::packaged_task` 包装任务，可以获取未来结果
   - `std::bind` 绑定函数和参数，创建无参数的可调用对象
   - `std::make_shared` 创建共享指针，确保任务生命周期足够长

4. **异常处理**：
   - 如果线程池已停止，抛出 `std::runtime_error`

5. **添加任务到队列**：
   - 使用 lambda 包装 `packaged_task`，使其成为 `std::function<void()>`
   - `tasks.emplace([task](){ (*task)(); })` 将任务添加到队列

6. **通知工作线程**：
   - `condition.notify_one()` 唤醒一个等待的工作线程

7. **返回future**：
   - 返回与任务关联的 `std::future`，调用者可以通过它获取结果

## 析构函数实现

```cpp
// the destructor joins all threads
inline ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for(std::thread &worker: workers)
        worker.join();
}
```

### 代码解析：

1. **设置停止标志**：
   - 在锁的保护下设置 `stop = true`
   - 使用作用域 `{}` 确保锁及时释放

2. **通知所有线程**：
   - `condition.notify_all()` 唤醒所有等待的线程

3. **等待线程结束**：
   - 遍历所有工作线程，调用 `join()` 等待它们完成
   - 由于 `stop` 已设置为 `true`，线程会在处理完所有任务后退出

## 设计模式与关键技术

### 1. 生产者-消费者模式
- 主线程（或其他线程）是生产者，通过 `enqueue` 添加任务
- 工作线程是消费者，从队列中取出并执行任务

### 2. RAII (资源获取即初始化)
- 使用 `std::unique_lock` 管理互斥锁，确保异常安全
- 析构函数中清理所有资源

### 3. 完美转发与可变参数模板
- 使用 `F&&` 和 `Args&&...` 保持参数的值类别（左值/右值）
- 使用 `std::forward` 完美转发参数

### 4. 类型推导
- 使用 `std::result_of` 推导函数返回类型
- 使用 `auto` 和尾置返回类型简化模板代码

### 5. 异常安全
- 在适当的地方抛出异常（如已停止的线程池添加任务）
- 确保资源在任何情况下都能正确释放

## 使用示例

```cpp
#include "ThreadPool.h"
#include <iostream>

int main() {
    ThreadPool pool(4);
    
    // 添加任务并获取future
    auto future = pool.enqueue([](int answer) {
        return answer;
    }, 42);
    
    // 获取结果
    std::cout << "Answer: " << future.get() << std::endl;
    
    return 0;
}
```

## 注意事项

1. **异常传播**：任务中抛出的异常会在 `future.get()` 时重新抛出
2. **线程安全**：所有对共享数据的访问都通过互斥锁保护
3. **生命周期管理**：使用 `shared_ptr` 确保任务对象生命周期足够长
4. **性能考虑**：任务执行在锁外进行，最大化并发性能
5. **停止机制**：析构函数确保所有任务完成后再销毁线程池

这个线程池实现简洁而强大，展示了现代C++中线程、异步编程和模板编程的最佳实践。