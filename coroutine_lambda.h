//
// Created by 蒋澳然 on 2021/7/24.
// email: 875691208@qq.com
// $desc
//

#ifndef LIBCOROUTINE_COROUTINE_LAMBDA_H
#define LIBCOROUTINE_COROUTINE_LAMBDA_H

#include "functional"

typedef std::function<void *(void *)> coroutine_func;

// args是自己可以传入的参数指针void *
#define coroutine_lambda(args, ...) \
[__VA_ARGS__](void *(args)) -> void *

#define coroutine_lambda_name(name, args, ...) \
auto (name) = [__VA_ARGS__](void *(args)) -> void *

// args是void *参数
#define thread_lambda(args) \
[](void *(args)) -> void *

#define thread_lambda_name(name, args) \
auto (name) = [](void *(args)) -> void *

#endif //LIBCOROUTINE_COROUTINE_LAMBDA_H
