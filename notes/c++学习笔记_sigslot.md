sigslot 库学习笔记

# 学习笔记
> [GitHub - palacaze/sigslot: A simple C++14 signal-slots implementation](https://github.com/palacaze/sigslot) 

# 使用 sigslot 库
只需包含头文件 signal.hpp 即可。

# RTTI 声明
```cpp
#if defined(__GXX_RTTI) || defined(__cpp_rtti) || defined(_CPPRTTI)
#define SIGSLOT_RTTI_ENABLED 1
#include <typeinfo>
#endif
```

这段代码是 `signal.hpp` 文件开头的一部分，用于检测编译器是否启用了运行时类型信息（RTTI，Run-Time Type Information）功能，并据此定义宏 `SIGSLOT_RTTI_ENABLED`。

- `defined(__GXX_RTTI)`：检查是否定义了 `__GXX_RTTI` 宏。这是 GCC 编译器启用 RTTI 时定义的宏。
- `defined(__cpp_rtti)`：检查是否定义了 `__cpp_rtti` 宏。这是 C++20 标准中引入的宏，用于检测编译器是否支持 RTTI。
- `defined(_CPPRTTI)`：检查是否定义了 `_CPPRTTI` 宏。这是 Microsoft Visual C++ 编译器启用 RTTI 时定义的宏。

- 如果检测到编译器启用了 RTTI 功能，则定义宏 `SIGSLOT_RTTI_ENABLED` 为 1。
- 这个宏在后续代码中用于控制是否启用与 RTTI 相关的功能，例如类型安全的信号和槽连接。

- 如果检测到 RTTI 功能启用，则包含 `<typeinfo>` 头文件。
- `<typeinfo>` 是 C++ 标准库中的头文件，提供了运行时类型信息的支持，例如 `std::type_info` 和 `std::type_index`。
- 这些功能在后续代码中可能用于实现类型安全的信号和槽机制。

# signal_base

```cpp
template <typename, typename...>
class signal_base;
```

- 这是一个模板类的声明，表示 `signal_base` 是一个模板类。
- 它有两个模板参数：
  - 第一个模板参数是一个普通类型参数，通常用于表示信号的返回类型。
  - 第二个模板参数是一个可变参数模板，表示信号的参数列表。


```cpp
/**
 * signal_base is an implementation of the observer pattern, through the use
 * of an emitting object and slots that are connected to the signal and called
 * with supplied arguments when a signal is emitted.
 *
 * signal_base is the general implementation, whose locking policy must be
 * set in order to decide thread safety guarantees. signal and signal_st
 * are partial specializations for multi-threaded and single-threaded use.
 *
 * It does not allow slots to return a value.
 *
 * Slot execution order can be constrained by assigning group ids to the slots.
 * The execution order of slots in a same group is unspecified and should not be
 * relied upon, however groups are executed in ascending group ids order. When
 * the group id of a slot is not set, it is assigned to the group 0. Group ids
 * can have any value in the range of signed 32 bit integers.
 *
 * @tparam Lockable a lock type to decide the lock policy
 * @tparam T... the argument types of the emitting and slots functions.
 */
template <typename Lockable, typename... T>
class signal_base final : public detail::cleanable {
    template <typename L>
    using is_thread_safe = std::integral_constant<bool, !std::is_same<L, detail::null_mutex>::value>;

    template <typename U, typename L>
    using cow_type = std::conditional_t<is_thread_safe<L>::value,
                                        detail::copy_on_write<U>, U>;

    template <typename U, typename L>
    using cow_copy_type = std::conditional_t<is_thread_safe<L>::value,
                                             detail::copy_on_write<U>, const U&>;

    using lock_type = std::unique_lock<Lockable>;
    using slot_base = detail::slot_base<T...>;
    using slot_ptr = detail::slot_ptr<T...>;
    using slots_type = std::vector<slot_ptr>;
    struct group_type { slots_type slts; group_id gid; };
    using list_type = std::vector<group_type>;  // kept ordered by ascending gid

public:
    using arg_list = trait::typelist<T...>;
    using ext_arg_list = trait::typelist<connection&, T...>;

    signal_base() noexcept : m_block(false) {}
    ~signal_base() override {
        disconnect_all();
    }

    signal_base(const signal_base&) = delete;
    signal_base & operator=(const signal_base&) = delete;

    signal_base(signal_base && o) /* not noexcept */
        : m_block{o.m_block.load()}
    {
        lock_type lock(o.m_mutex);
        using std::swap;
        swap(m_slots, o.m_slots);
    }

    signal_base & operator=(signal_base && o) /* not noexcept */ {
        lock_type lock1(m_mutex, std::defer_lock);
        lock_type lock2(o.m_mutex, std::defer_lock);
        std::lock(lock1, lock2);

        using std::swap;
        swap(m_slots, o.m_slots);
        m_block.store(o.m_block.exchange(m_block.load()));
        return *this;
    }

    /**
     * Emit a signal
     *
     * Effect: All non blocked and connected slot functions will be called
     *         with supplied arguments.
     * Safety: With proper locking (see pal::signal), emission can happen from
     *         multiple threads simultaneously. The guarantees only apply to the
     *         signal object, it does not cover thread safety of potentially
     *         shared state used in slot functions.
     *
     * @param a... arguments to emit
     */
    template <typename... U>
    void operator()(U && ...a) const {
        if (m_block) {
            return;
        }

        // Reference to the slots to execute them out of the lock
        // a copy may occur if another thread writes to it.
        cow_copy_type<list_type, Lockable> ref = slots_reference();

        for (const auto &group : detail::cow_read(ref)) {
            for (const auto &s : group.slts) {
                s->operator()(a...);
            }
        }
    }

    /**
     * Connect a callable of compatible arguments.
     *
     * Effect: Creates and stores a new slot responsible for executing the
     *         supplied callable for every subsequent signal emission.
     * Safety: Thread-safety depends on locking policy.
     *
     * @param c a callable
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Callable>
    std::enable_if_t<trait::is_callable_v<arg_list, Callable>, connection>
    connect(Callable && c, group_id gid = 0) {
        using slot_t = detail::slot<Callable, T...>;
        auto s = make_slot<slot_t>(std::forward<Callable>(c), gid);
        connection conn(s);
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Connect a callable with an additional connection argument.
     *
     * The callable's first argument must be of type connection. The callable
     * can manage its own connection through this argument.
     *
     * @param c a callable
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Callable>
    std::enable_if_t<trait::is_callable_v<ext_arg_list, Callable>, connection>
    connect_extended(Callable && c, group_id gid = 0) {
        using slot_t = detail::slot_extended<Callable, T...>;
        auto s = make_slot<slot_t>(std::forward<Callable>(c), gid);
        connection conn(s);
        std::static_pointer_cast<slot_t>(s)->conn = conn;
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for pointers over member functions derived from
     * observer.
     *
     * @param pmf a pointer over member function
     * @param ptr an object pointer derived from observer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Pmf, typename Ptr>
    std::enable_if_t<trait::is_callable_v<arg_list, Pmf, Ptr> &&
                     trait::is_observer_v<Ptr>, connection>
    connect(Pmf && pmf, Ptr && ptr, group_id gid = 0) {
        using slot_t = detail::slot_pmf<Pmf, Ptr, T...>;
        auto s = make_slot<slot_t>(std::forward<Pmf>(pmf), std::forward<Ptr>(ptr), gid);
        connection conn(s);
        add_slot(std::move(s));
        ptr->add_connection(conn);
        return conn;
    }

    /**
     * Overload of connect for pointers over member functions.
     *
     * @param pmf a pointer over member function
     * @param ptr an object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Pmf, typename Ptr>
    std::enable_if_t<trait::is_callable_v<arg_list, Pmf, Ptr> &&
                     !trait::is_observer_v<Ptr> &&
                     !trait::is_weak_ptr_compatible_v<Ptr>, connection>
    connect(Pmf && pmf, Ptr && ptr, group_id gid = 0) {
        using slot_t = detail::slot_pmf<Pmf, Ptr, T...>;
        auto s = make_slot<slot_t>(std::forward<Pmf>(pmf), std::forward<Ptr>(ptr), gid);
        connection conn(s);
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for pointer over member functions and additional
     * connection argument.
     *
     * The callable's first argument must be of type connection. The callable
     * can manage its own connection through this argument.
     *
     * @param pmf a pointer over member function
     * @param ptr an object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Pmf, typename Ptr>
    std::enable_if_t<trait::is_callable_v<ext_arg_list, Pmf, Ptr> &&
                     !trait::is_weak_ptr_compatible_v<Ptr>, connection>
    connect_extended(Pmf && pmf, Ptr && ptr, group_id gid = 0) {
        using slot_t = detail::slot_pmf_extended<Pmf, Ptr, T...>;
        auto s = make_slot<slot_t>(std::forward<Pmf>(pmf), std::forward<Ptr>(ptr), gid);
        connection conn(s);
        std::static_pointer_cast<slot_t>(s)->conn = conn;
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for lifetime object tracking and automatic disconnection.
     *
     * Ptr must be convertible to an object following a loose form of weak pointer
     * concept, by implementing the ADL-detected conversion function to_weak().
     *
     * This overload covers the case of a pointer over member function and a
     * trackable pointer of that class.
     *
     * Note: only weak references are stored, a slot does not extend the lifetime
     * of a supplied object.
     *
     * @param pmf a pointer over member function
     * @param ptr a trackable object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Pmf, typename Ptr>
    std::enable_if_t<!trait::is_callable_v<arg_list, Pmf> &&
                     trait::is_weak_ptr_compatible_v<Ptr>, connection>
    connect(Pmf && pmf, Ptr && ptr, group_id gid = 0) {
        using trait::to_weak;
        auto w = to_weak(std::forward<Ptr>(ptr));
        using slot_t = detail::slot_pmf_tracked<Pmf, decltype(w), T...>;
        auto s = make_slot<slot_t>(std::forward<Pmf>(pmf), w, gid);
        connection conn(s);
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for lifetime object tracking and automatic disconnection
     * with additional connection management.
     *
     * The callable's first argument must be of type connection. The callable
     * can manage its own connection through this argument.
     *
     * Ptr must be convertible to an object following a loose form of weak pointer
     * concept, by implementing the ADL-detected conversion function to_weak().
     *
     * This overload covers the case of a pointer over member function and a
     * trackable pointer of that class.
     *
     * Note: only weak references are stored, a slot does not extend the lifetime
     * of a supplied object.
     *
     * @param pmf a pointer over member function
     * @param ptr a trackable object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Pmf, typename Ptr>
    std::enable_if_t<!trait::is_callable_v<ext_arg_list, Pmf> &&
                     trait::is_weak_ptr_compatible_v<Ptr>, connection>
    connect_extended(Pmf && pmf, Ptr && ptr, group_id gid = 0) {
        using trait::to_weak;
        auto w = to_weak(std::forward<Ptr>(ptr));
        using slot_t = detail::slot_pmf_tracked_extended<Pmf, decltype(w), T...>;
        auto s = make_slot<slot_t>(std::forward<Pmf>(pmf), w, gid);
        connection conn(s);
        std::static_pointer_cast<slot_t>(s)->conn = conn;
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for lifetime object tracking and automatic disconnection.
     *
     * Trackable must be convertible to an object following a loose form of weak
     * pointer concept, by implementing the ADL-detected conversion function to_weak().
     *
     * This overload covers the case of a standalone callable and unrelated trackable
     * object.
     *
     * Note: only weak references are stored, a slot does not extend the lifetime
     * of a supplied object.
     *
     * @param c a callable
     * @param ptr a trackable object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Callable, typename Trackable>
    std::enable_if_t<trait::is_callable_v<arg_list, Callable> &&
                     trait::is_weak_ptr_compatible_v<Trackable>, connection>
    connect(Callable && c, Trackable && ptr, group_id gid = 0) {
        using trait::to_weak;
        auto w = to_weak(std::forward<Trackable>(ptr));
        using slot_t = detail::slot_tracked<Callable, decltype(w), T...>;
        auto s = make_slot<slot_t>(std::forward<Callable>(c), w, gid);
        connection conn(s);
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Overload of connect for lifetime object tracking and automatic disconnection
     * with additional connection management.
     *
     * The callable's first argument must be of type connection. The callable
     * can manage its own connection through this argument.
     *
     * Trackable must be convertible to an object following a loose form of weak
     * pointer concept, by implementing the ADL-detected conversion function to_weak().
     *
     * This overload covers the case of a standalone callable and unrelated trackable
     * object.
     *
     * Note: only weak references are stored, a slot does not extend the lifetime
     * of a suppied object.
     *
     * @param c a callable
     * @param ptr a trackable object pointer
     * @param gid an identifier that can be used to order slot execution
     * @return a connection object that can be used to interact with the slot
     */
    template <typename Callable, typename Trackable>
    std::enable_if_t<trait::is_callable_v<ext_arg_list, Callable> &&
                     trait::is_weak_ptr_compatible_v<Trackable>, connection>
    connect_extended(Callable && c, Trackable && ptr, group_id gid = 0) {
        using trait::to_weak;
        auto w = to_weak(std::forward<Trackable>(ptr));
        using slot_t = detail::slot_tracked_extended<Callable, decltype(w), T...>;
        auto s = make_slot<slot_t>(std::forward<Callable>(c), w, gid);
        connection conn(s);
        std::static_pointer_cast<slot_t>(s)->conn = conn;
        add_slot(std::move(s));
        return conn;
    }

    /**
     * Creates a connection whose duration is tied to the return object.
     * Uses the same semantics as connect
     */
    template <typename... CallArgs>
    scoped_connection connect_scoped(CallArgs && ...args) {
        return connect(std::forward<CallArgs>(args)...);
    }

    /**
     * Disconnect slots bound to a callable
     *
     * Effect: Disconnects all the slots bound to the callable in argument.
     * Safety: Thread-safety depends on locking policy.
     *
     * If the callable is a free or static member function, this overload is always
     * available. However, RTTI is needed for it to work for pointer to member
     * functions, function objects or and (references to) lambdas, because the
     * C++ spec does not mandate the pointers to member functions to be unique.
     *
     * @param c a callable
     * @return the number of disconnected slots
     */
    template <typename Callable>
    std::enable_if_t<(trait::is_callable_v<arg_list, Callable> ||
                      trait::is_callable_v<ext_arg_list, Callable> ||
                      trait::is_pmf_v<Callable>) &&
                     detail::function_traits<Callable>::is_disconnectable, size_t>
    disconnect(const Callable &c) {
        return disconnect_if([&] (const auto &s) {
            return s->has_full_callable(c);
        });
    }

    /**
     * Disconnect slots bound to this object
     *
     * Effect: Disconnects all the slots bound to the object or tracked object
     *         in argument.
     * Safety: Thread-safety depends on locking policy.
     *
     * The object may be a pointer or trackable object.
     *
     * @param obj an object
     * @return the number of disconnected slots
     */
    template <typename Obj>
    std::enable_if_t<!trait::is_callable_v<arg_list, Obj> &&
                     !trait::is_callable_v<ext_arg_list, Obj> &&
                     !trait::is_pmf_v<Obj>, size_t>
    disconnect(const Obj &obj) {
        return disconnect_if([&] (const auto &s) {
            return s->has_object(obj);
        });
    }

    /**
     * Disconnect slots bound both to a callable and object
     *
     * Effect: Disconnects all the slots bound to the callable and object in argument.
     * Safety: Thread-safety depends on locking policy.
     *
     * For naked pointers, the Callable is expected to be a pointer over member
     * function. If obj is trackable, any kind of Callable can be used.
     *
     * @param c a callable
     * @param obj an object
     * @return the number of disconnected slots
     */
    template <typename Callable, typename Obj>
    size_t disconnect(const Callable &c, const Obj &obj) {
        return disconnect_if([&] (const auto &s) {
            return s->has_object(obj) && s->has_callable(c);
        });
    }

    /**
     * Disconnect slots in a particular group
     *
     * Effect: Disconnects all the slots in the group id in argument.
     * Safety: Thread-safety depends on locking policy.
     *
     * @param gid a group id
     * @return the number of disconnected slots
     */
    size_t disconnect(group_id gid) {
        lock_type lock(m_mutex);
        for (auto &group : detail::cow_write(m_slots)) {
            if (group.gid == gid) {
                size_t count = group.slts.size();
                group.slts.clear();
                return count;
            }
        }
        return 0;
    }

    /**
     * Disconnects all the slots
     * Safety: Thread safety depends on locking policy
     */
    void disconnect_all() {
        lock_type lock(m_mutex);
        clear();
    }

    /**
     * Blocks signal emission
     * Safety: thread safe
     */
    void block() noexcept {
        m_block.store(true);
    }

    /**
     * Unblocks signal emission
     * Safety: thread safe
     */
    void unblock() noexcept {
        m_block.store(false);
    }

    /**
     * Tests blocking state of signal emission
     */
    bool blocked() const noexcept {
        return m_block.load();
    }

    /**
     * Get number of connected slots
     * Safety: thread safe
     */
    size_t slot_count() noexcept {
        cow_copy_type<list_type, Lockable> ref = slots_reference();
        size_t count = 0;
        for (const auto &g : detail::cow_read(ref)) {
            count += g.slts.size();
        }
        return count;
    }

protected:
    /**
     * remove disconnected slots
     */
    void clean(detail::slot_state *state) override {
        lock_type lock(m_mutex);
        const auto idx = state->index();
        const auto gid = state->group();

        // find the group
        for (auto &group : detail::cow_write(m_slots)) {
            if (group.gid == gid) {
                auto &slts = group.slts;

                // ensure we have the right slot, in case of concurrent cleaning
                if (idx < slts.size() && slts[idx] && slts[idx].get() == state) {
                    std::swap(slts[idx], slts.back());
                    slts[idx]->index() = idx;
                    slts.pop_back();
                }

                return;
            }
        }
    }

private:
    // used to get a reference to the slots for reading
    inline cow_copy_type<list_type, Lockable> slots_reference() const {
        lock_type lock(m_mutex);
        return m_slots;
    }

    // create a new slot
    template <typename Slot, typename... A>
    inline auto make_slot(A && ...a) {
        return detail::make_shared<slot_base, Slot>(*this, std::forward<A>(a)...);
    }

    // add the slot to the list of slots of the right group
    void add_slot(slot_ptr &&s) {
        const group_id gid = s->group();

        lock_type lock(m_mutex);
        auto &groups = detail::cow_write(m_slots);

        // find the group
        auto it = groups.begin();
        while (it != groups.end() && it->gid < gid) {
            it++;
        }

        // create a new group if necessary
        if (it == groups.end() || it->gid != gid) {
            it = groups.insert(it, {{}, gid});
        }

        // add the slot
        s->index() = it->slts.size();
        it->slts.push_back(std::move(s));
    }

    // disconnect a slot if a condition occurs
    template <typename Cond>
    size_t disconnect_if(Cond && cond) {
        lock_type lock(m_mutex);
        auto &groups = detail::cow_write(m_slots);

        size_t count = 0;

        for (auto &group : groups) {
            auto &slts = group.slts;
            size_t i = 0;
            while (i < slts.size()) {
                if (cond(slts[i])) {
                    std::swap(slts[i], slts.back());
                    slts[i]->index() = i;
                    slts.pop_back();
                    ++count;
                } else {
                    ++i;
                }
            }
        }

        return count;
    }

    // to be called under lock: remove all the slots
    void clear() {
        detail::cow_write(m_slots).clear();
    }

private:
    mutable Lockable m_mutex;
    cow_type<list_type, Lockable> m_slots;
    std::atomic<bool> m_block;
};


```

## 类介绍
### 1. **观察者模式的实现**
```cpp
/**
 * signal_base is an implementation of the observer pattern, through the use
 * of an emitting object and slots that are connected to the signal and called
 * with supplied arguments when a signal is emitted.
```
- **观察者模式（Observer Pattern）**：这是一种设计模式，用于实现对象间的依赖关系。当一个对象（称为“主题”或“信号发射器”）的状态发生变化时，所有依赖于它的对象（称为“观察者”或“槽”）都会自动得到通知并更新。
- **`signal_base`**：实现了观察者模式。它定义了一个信号发射器，可以连接多个槽函数。当信号被发射时，所有连接的槽函数都会被调用，并传递相应的参数。

### 2. **线程安全策略**
```cpp
 *
 * signal_base is the general implementation, whose locking policy must be
 * set in order to decide thread safety guarantees. signal and signal_st
 * are partial specializations for multi-threaded and single-threaded use.
```
- **通用实现**：`signal_base` 是一个通用的信号实现，它通过模板参数 `Lockable` 提供线程安全策略。
- **线程安全策略**：`Lockable` 是一个模板参数，用于指定锁的类型。根据不同的锁类型，`signal_base` 可以实现不同的线程安全策略。
  - **多线程安全**：如果使用 `std::mutex` 或其他线程安全的锁类型，`signal_base` 将支持多线程环境。
  - **单线程安全**：如果使用 `detail::null_mutex`（一个空的锁实现），`signal_base` 将不支持线程安全，适用于单线程环境。
- **特化实现**：
  - **`signal`**：是一个多线程安全的信号实现，通常使用 `std::mutex` 作为锁类型。
  - **`signal_st`**：是一个单线程安全的信号实现，通常使用 `detail::null_mutex` 作为锁类型。

### 3. **槽的返回值**
```cpp
 *
 * It does not allow slots to return a value.
```
- **槽的返回值**：`signal_base` 不允许槽函数返回值。所有槽函数的返回类型必须是 `void`。这是因为信号的发射是广播式的，无法处理多个槽函数返回的不同值。

### 4. **槽的执行顺序**
```cpp
 *
 * Slot execution order can be constrained by assigning group ids to the slots.
 * The execution order of slots in a same group is unspecified and should not be
 * relied upon, however groups are executed in ascending group ids order. When
 * the group id of a slot is not set, it is assigned to the group 0. Group ids
 * can have any value in the range of signed 32 bit integers.
```
- **槽的分组**：槽可以被分配到不同的组中，通过组 ID 来管理槽的执行顺序。
  - **组 ID**：每个槽可以被分配一个组 ID，用于控制槽的执行顺序。
  - **默认组**：如果槽的组 ID 没有被显式设置，它将被分配到默认组（组 ID 为 0）。
  - **执行顺序**：
    - **组间顺序**：组 ID 较小的组会先执行。
    - **组内顺序**：同一组内的槽执行顺序是未指定的，不应依赖于特定的顺序。
  - **组 ID 范围**：组 ID 可以是任何 32 位有符号整数。

### 5. **模板参数**
```cpp
 *
 * @tparam Lockable a lock type to decide the lock policy
 * @tparam T... the argument types of the emitting and slots functions.
 */
```
- **`Lockable`**：模板参数，用于指定锁的类型。它决定了信号和槽操作的线程安全策略。
- **`T...`**：模板参数，表示信号和槽函数的参数类型。这些参数类型将被传递给槽函数。

### 总结
这段注释详细描述了 `signal_base` 的设计目标、功能、线程安全策略以及槽的执行顺序等关键特性。具体来说：
- **观察者模式**：`signal_base` 实现了观察者模式，允许信号发射器和槽之间的通信。
- **线程安全策略**：通过模板参数 `Lockable`，`signal_base` 提供了灵活的线程安全策略。
- **槽的返回值**：槽函数必须返回 `void`，信号不支持返回值。
- **槽的执行顺序**：通过组 ID 管理槽的执行顺序，组 ID 较小的组先执行，同一组内的槽执行顺序未指定。
- **模板参数**：`Lockable` 用于指定锁类型，`T...` 用于指定信号和槽函数的参数类型。

## 类模板参数
```cpp
template <typename Lockable, typename... T>
class signal_base final : public detail::cleanable {
```
- **`Lockable`**：这是一个模板参数，用于指定锁的类型。它决定了信号和槽操作的线程安全策略。例如：
  - 如果使用 `std::mutex`，则信号和槽操作是线程安全的。
  - 如果使用 `detail::null_mutex`（一个空的锁实现），则信号和槽操作是非线程安全的。
- **`T...`**：这是一个可变参数模板，表示信号的参数类型。例如，如果信号接受两个参数 `int` 和 `double`，则 `T...` 会被实例化为 `int, double`。

## 类型别名和工具
```cpp
template <typename L>
using is_thread_safe = std::integral_constant<bool, !std::is_same<L, detail::null_mutex>::value>;

template <typename U, typename L>
using cow_type = std::conditional_t<is_thread_safe<L>::value, detail::copy_on_write<U>, U>;

template <typename U, typename L>
using cow_copy_type = std::conditional_t<is_thread_safe<L>::value, detail::copy_on_write<U>, const U&>;
```
- **`is_thread_safe`**：一个类型特征，用于检查锁类型是否为线程安全的。如果锁类型不是 `detail::null_mutex`，则返回 `true`。
- **`cow_type`** 和 **`cow_copy_type`**：用于根据锁类型选择合适的容器类型。如果锁类型是线程安全的，则使用 `detail::copy_on_write` 容器；否则，使用普通的容器或引用。

### 3. **成员类型定义**
```cpp
using lock_type = std::unique_lock<Lockable>;
using slot_base = detail::slot_base<T...>;
using slot_ptr = detail::slot_ptr<T...>;
using slots_type = std::vector<slot_ptr>;
struct group_type { slots_type slts; group_id gid; };
using list_type = std::vector<group_type>;
```
- **`lock_type`**：用于获取锁的类型。
- **`slot_base`**：槽的基类，用于封装槽的基本操作。
- **`slot_ptr`**：槽的智能指针类型。
- **`slots_type`**：槽的容器类型，存储所有槽的指针。
- **`group_type`**：槽的分组结构，包含槽的列表和组 ID。
- **`list_type`**：槽的分组列表，存储所有槽的分组。

### 4. **构造函数和析构函数**
```cpp
signal_base() noexcept : m_block(false) {}
~signal_base() override {
    disconnect_all();
}
```
- **构造函数**：初始化信号，设置阻塞标志 `m_block` 为 `false`。
- **析构函数**：调用 `disconnect_all`，断开所有槽的连接，避免悬挂指针。

### 5. **拷贝和移动操作**
```cpp
signal_base(const signal_base&) = delete;
signal_base & operator=(const signal_base&) = delete;

signal_base(signal_base && o) /* not noexcept */
    : m_block{o.m_block.load()} {
    lock_type lock(o.m_mutex);
    using std::swap;
    swap(m_slots, o.m_slots);
}

signal_base & operator=(signal_base && o) /* not noexcept */ {
    lock_type lock1(m_mutex, std::defer_lock);
    lock_type lock2(o.m_mutex, std::defer_lock);
    std::lock(lock1, lock2);

    using std::swap;
    swap(m_slots, o.m_slots);
    m_block.store(o.m_block.exchange(m_block.load()));
    return *this;
}
```
- **拷贝构造函数和拷贝赋值运算符**：被删除，信号对象不能被拷贝。
- **移动构造函数和移动赋值运算符**：支持移动语义，允许将一个信号对象的内容移动到另一个对象中。

### 6. **信号发射**
```cpp
template <typename... U>
void operator()(U && ...a) const {
    if (m_block) {
        return;
    }

    cow_copy_type<list_type, Lockable> ref = slots_reference();
    for (const auto &group : detail::cow_read(ref)) {
        for (const auto &s : group.slts) {
            s->operator()(a...);
        }
    }
}
```
- **`operator()`**：用于发射信号。
  - 如果信号被阻塞（`m_block` 为 `true`），则直接返回。
  - 获取槽的引用（可能涉及复制，取决于锁类型）。
  - 遍历所有槽的分组，并调用每个槽的函数。

### 7. **连接槽**
```cpp
template <typename Callable>
std::enable_if_t<trait::is_callable_v<arg_list, Callable>, connection>
connect(Callable && c, group_id gid = 0) {
    using slot_t = detail::slot<Callable, T...>;
    auto s = make_slot<slot_t>(std::forward<Callable>(c), gid);
    connection conn(s);
    add_slot(std::move(s));
    return conn;
}
```
- **`connect`**：用于将一个可调用对象（如函数、lambda 表达式或成员函数指针）连接到信号。
  - 使用 `trait::is_callable_v` 检查可调用对象是否兼容信号的参数类型。
  - 创建一个槽对象（`slot_t`），并将其添加到槽的列表中。
  - 返回一个 `connection` 对象，用于管理槽的生命周期。

### 8. **断开槽**
```cpp
template <typename Callable>
std::enable_if_t<(trait::is_callable_v<arg_list, Callable> ||
                  trait::is_callable_v<ext_arg_list, Callable> ||
                  trait::is_pmf_v<Callable>) &&
                 detail::function_traits<Callable>::is_disconnectable, size_t>
disconnect(const Callable &c) {
    return disconnect_if([&] (const auto &s) {
        return s->has_full_callable(c);
    });
}
```
- **`disconnect`**：用于断开与某个可调用对象绑定的槽。
  - 使用 `trait::is_callable_v` 和 `trait::is_pmf_v` 检查可调用对象是否可以被断开。
  - 遍历所有槽，找到与指定可调用对象匹配的槽，并将其从列表中移除。

### 9. **阻塞和解阻塞**
```cpp
void block() noexcept {
    m_block.store(true);
}

void unblock() noexcept {
    m_block.store(false);
}

bool blocked() const noexcept {
    return m_block.load();
}
```
- **`block`** 和 **`unblock`**：用于阻塞和解阻塞信号的发射。
- **`blocked`**：用于检查信号是否被阻塞。

### 10. **槽的管理**
```cpp
void add_slot(slot_ptr &&s) {
    const group_id gid = s->group();

    lock_type lock(m_mutex);
    auto &groups = detail::cow_write(m_slots);

    auto it = groups.begin();
    while (it != groups.end() && it->gid < gid) {
        it++;
    }

    if (it == groups.end() || it->gid != gid) {
        it = groups.insert(it, {{}, gid});
    }

    s->index() = it->slts.size();
    it->slts.push_back(std::move(s));
}
```
- **`add_slot`**：将一个槽添加到槽的列表中。
  - 根据槽的组 ID，找到或创建对应的分组。
  - 将槽添加到分组的槽列表中。

### 11. **清理和断开**
```cpp
void clean(detail::slot_state *state) override {
    lock_type lock(m_mutex);
    const auto idx = state->index();
    const auto gid = state->group();

    for (auto &group : detail::cow_write(m_slots)) {
        if (group.gid == gid) {
            auto &slts = group.slts;

            if (idx < slts.size() && slts[idx] && slts[idx].get() == state) {
                std::swap(slts[idx], slts.back());
                slts[idx]->index() = idx;
                slts.pop_back();
            }

            return;
        }
    }
}
```
- **`clean`**：清理已断开的槽。
  - 找到对应的槽分组和槽对象，从列表中移除。

### 12. **成员变量**
```cpp
mutable Lockable m_mutex;
cow_type<list_type, Lockable> m_slots;
std::atomic<bool> m_block;
```
- **`m_mutex`**：用于同步访问槽的列表。
- **`m_slots`**：槽的分组列表，存储所有槽的指针。
- **`m_block`**：一个原子变量，用于标记信号是否被阻塞。

### 总结
`signal_base` 是一个通用的信号实现，支持多线程安全和灵活的槽管理。它通过模板参数 `Lockable` 提供了线程安全策略的灵活性，并通过模板参数 `T...` 支持不同参数类型的信号。这个类的核心功能包括：
- **信号发射**：调用所有连接的槽。
- **槽的连接和断开**：支持多种类型的槽，包括函数、lambda 表达式和成员函数指针。
- **阻塞和解阻塞**：控制信号的发射。
- **槽的分组**：支持按组管理槽的执行顺序。

