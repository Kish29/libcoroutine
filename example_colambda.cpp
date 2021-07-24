
#include "coroutine_lambda.h"
#include "pthread.h"
#include "vector"
#include "cstdio"

struct args_struct {
    int argsid{};
};

int main() {
    std::vector<coroutine_func> coroutines{};
    coroutines.reserve(11);
    int a = 100;
    int b = 200;
    for (int i = 0; i < 10; ++i) {
        coroutines.emplace_back(coroutine_lambda(args, =) {
            printf("a is %d\tb is %d\n", a, b);
            return nullptr;
        });
    }

    coroutine_lambda_name(test_lambda, args, =) {
        printf("a is %d\tb is %d\n", a, b);
        return nullptr;
    };

    coroutines.emplace_back(test_lambda);

    for (coroutine_func &t: coroutines) {
        t(nullptr);
    }
    args_struct iam{};
    iam.argsid = 999;
    pthread_t t1{};
    pthread_create(&t1, nullptr, thread_lambda(args) {
        args_struct *p = (args_struct *) args;
        printf("i am in a new thread, argsid=%d\n", p->argsid);
        pthread_exit(nullptr);
    }, &iam);

    pthread_t t2{};
    pthread_create(&t2, nullptr, thread_lambda(args) {
        printf("i am in a new thread\n");
        pthread_exit(nullptr);
    }, nullptr);

    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
}
