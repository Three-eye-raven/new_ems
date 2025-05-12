//
// Created by Three on 2025/3/1.
//
#include "atomic_test_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define THREAD_NUM 10
#define ITERATIONS 100000

// 原子计数器
atomic_int atomic_counter = ATOMIC_VAR_INIT(0);
// 普通计数器
int non_atomic_counter = 0;

void *thread_func(void *arg) {
    for (int i = 0; i < ITERATIONS; ++i) {
        // 原子操作
        //atomic_fetch_add(&atomic_counter, 1);
        atomic_counter++;
        atomic_store(&atomic_counter,1);
        // 非原子操作
        non_atomic_counter++;
    }
    return NULL;
}

int atomic_test(void)
{
    pthread_t threads[THREAD_NUM];

    // 创建线程
    for (int i = 0; i < THREAD_NUM; ++i) {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    // 等待线程完成
    for (int i = 0; i < THREAD_NUM; ++i) {
        pthread_join(threads[i], NULL);
    }

    // 输出结果
    printf("预期结果: %d\n", THREAD_NUM * ITERATIONS);
    printf("原子计数器结果: %d\n", atomic_counter);
    printf("普通计数器结果: %d\n", non_atomic_counter);

    return 0;
}
