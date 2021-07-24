//
// Created by 蒋澳然 on 2021/7/24.
// email: 875691208@qq.com
// $desc
//

#ifndef LIBCOROUTINE_COROUTINE_H
#define LIBCOROUTINE_COROUTINE_H

#include <cstdint>
#include "cstdlib"
#include "coroutine_lambda.h"

struct coroutine_attribute;
struct coroutine_share_stack;

class coroutine;

template<uint8_t SIZE = 128>
class coroutine_env;


struct coroutine_share_stack {

} __attribute__ ((packed));

struct coroutine_attribute {
    int stack_size;

} __attribute__ ((packed));   // 紧凑模式

// 协程执行体
class coroutine {
public:

    explicit coroutine(bool _main = false) : is_main_(_main) {}

    bool is_main() const { return is_main_; }

    bool is_finish() const { return finish_; }

    bool is_start() const { return start_; }

    void routine();

private:
    coroutine_env<> *co_env_{};
    coroutine_func func_{};
    void *args_{};

    bool is_main_{};
    bool start_{};
    bool finish_{};

    void exe() const { func_(args_); }

private:
    friend void init_curr_thread_coroutine_env();

    friend int coroutine_create(coroutine **, const coroutine_attribute *, coroutine_func, void *);

    friend int coroutine_startup(const coroutine_func &, void *args);
};


template<uint8_t SIZE>
class coroutine_env {
public:

private:
    coroutine **call_stack_{new coroutine *[SIZE]};
    int stack_top_{};
    coroutine *pending_co{};
    coroutine *occupy_co{};

private:
    friend void init_curr_thread_coroutine_env();

    friend int coroutine_startup(const coroutine_func &, void *args);

    friend class coroutine;

};

int coroutine_create(coroutine **co, const coroutine_attribute *attr, coroutine_func routine, void *args);

// co_ctl函数会在每一次协程执行完之后调用，根据返回值来判断是否停止协程的运行，true停止，false继续执行
int coroutine_startup(const coroutine_func &co_ctl, void *args);

void coroutine_shutdown();

coroutine_env<> *coroutine_curr_thread_env();

#endif //LIBCOROUTINE_COROUTINE_H
