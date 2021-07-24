//
// Created by 蒋澳然 on 2021/7/24.
// email: 875691208@qq.com
// $desc
//

#include "coroutine.h"

#include "cassert"
#include "coroutine_lambda.h"

// 每个线程都会有一个协程执行的环境
// 换言之，每个线程(之上)都可以同时运行128个协程任务(fake)
static __thread coroutine_env<> *gCurrThreadCoEnv = nullptr;

static __thread bool gShutdown = false;

//  初始化当前线程的协程环境
void init_curr_thread_coroutine_env() {
    auto *self = new coroutine;
    self->is_main_ = true;
    gCurrThreadCoEnv = new coroutine_env<>;
    gCurrThreadCoEnv->call_stack_[gCurrThreadCoEnv->stack_top_++] = self;
}


// 创建一个协程
int coroutine_create(
        coroutine **co
#ifdef __restrict
        __restrict
#endif
        , const coroutine_attribute *attr, coroutine_func routine, void *args
) {
    if (co == nullptr) {
        return -1;
    }
    if (!gCurrThreadCoEnv) {
        init_curr_thread_coroutine_env();
    }
    *co = new coroutine;
    if (attr) {
        // todo:解析属性参数
    }
    (*co)->co_env_ = gCurrThreadCoEnv;
    (*co)->func_ = std::move(routine);
    (*co)->args_ = args;
    return 0;
}

int coroutine_startup(const coroutine_func &co_ctl, void *args) {
    if (!gCurrThreadCoEnv) {
        init_curr_thread_coroutine_env();
    }
    assert(gCurrThreadCoEnv);
    while (true) {
        // todo:编写执行协程的算法,目前只是为了测试和演示
        auto *curr_work = gCurrThreadCoEnv->call_stack_[gCurrThreadCoEnv->stack_top_ - 1];
        if (!curr_work->is_main_ && !curr_work->finish_) {
            curr_work->exe();
            curr_work->finish_ = true;
            gCurrThreadCoEnv->stack_top_--;
        }
        if (co_ctl) {
            co_ctl(args);
            if (gShutdown) {
                break;
            }
        }
    }
    return 0;
}

void coroutine_shutdown() {
    gShutdown = true;
}

void coroutine::routine() {
    auto *curr_co = co_env_->call_stack_[co_env_->stack_top_ - 1];
    if (!curr_co->start_) {
        //
        curr_co->start_ = true;
    }
    if (co_env_->call_stack_[co_env_->stack_top_]) {
        // coroutine通过coroutine_create进行new申请的内存，替换时，应当释放掉资源
        delete co_env_->call_stack_[co_env_->stack_top_];
    }
    // todo:检查数组的越界情况
    co_env_->call_stack_[co_env_->stack_top_++] = this;
}

