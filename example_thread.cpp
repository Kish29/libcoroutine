//
// Created by 蒋澳然 on 2021/7/24.
// email: 875691208@qq.com
// $desc
//

#include "coroutine.h"
#include "unistd.h"
#include "cstdio"
#include "pthread.h"
#include "mutex"

static uint8_t thread_id = 0;
std::mutex mtx{};

int main(int argc, char *argv[]) {
    int thread_num;
    if (argc < 2) {
        thread_num = 8;
    } else {
        thread_num = atoi(argv[1]);
    }
    pthread_t tid[thread_num];
    // 一共开启8个线程，每个线程执行8个协程，共64个任务
    for (int i = 0; i < thread_num; ++i) {
        pthread_create(tid + i, nullptr, thread_lambda(args) {
            char name[64];
            // 读加锁
            uint8_t num{};
            {
                std::lock_guard<std::mutex> lck(mtx);
                num = thread_id;
                thread_id++;
            }
            snprintf(name, 64, "thread-%d", num);
#ifdef __APPLE__
            pthread_setname_np(name);
#else
            pthread_setname_np(pthread_self(), name);
#endif
            bool monitor = false;
            coroutine *co[8];
            for (int j = 0; j < 8; ++j) {
                coroutine_create(co + j, nullptr, coroutine_lambda(args, &monitor, j) {
                    char buf[255]{};
                    pthread_getname_np(pthread_self(), buf, 255);
                    printf("%s.%d j-%d %s!\n", __func__, __LINE__, j, buf);
                    if (j == 0) {
                        printf("i am the last coroutine that my thread will exe\nafter 2 seconds, coroutine will shutdown\n");
                        sleep(2);
                        monitor = true;
                    }
                    return nullptr;
                }, nullptr);
                co[j]->routine();
            }
            coroutine_startup(coroutine_lambda(args, &monitor) {
                if (monitor) {
                    coroutine_shutdown();
                }
                return nullptr;
            }, nullptr);
            printf("coroutine exit!\n");
            return nullptr;
        }, nullptr);
    }
    sleep(5);
    return 0;
}
