//
// Created by three on 2025/5/6.
//
#include "ring_lockfree_sync_queue_api.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

typedef struct
{
        char msg[250];
}test_msg_t;

RingLockfreeSyncQueue_T *pQueue = NULL;

/**
 * @brief 生成随机字符串
 * @param buffer 存储生成的字符串的缓冲区
 * @param length 字符串长度（不含终止符）
 * @param charset 可选的自定义字符集，传NULL使用默认字符集
 *
 * 默认字符集：大小写字母 + 数字 (A-Za-z0-9)
 */
static void generate_random_string(char *buffer, const size_t length, const char *charset) {
        if (!buffer || length == 0) return;

        // 设置默认字符集
        const char *default_charset = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz"
                                      "0123456789";
        const char *valid_chars = charset ? charset : default_charset;
        const size_t charset_len = strlen(valid_chars);

        // 生成随机字符
        for (size_t i = 0; i < length; i++) {
                const size_t index = rand() % charset_len;
                buffer[i] = valid_chars[index];
        }

        buffer[length] = '\0'; // 终止符
}

static void *write_thread(void *arg)
{
        srand(time(NULL));
        test_msg_t msg;
        while (1) {
                generate_random_string(msg.msg,55,NULL);
                ring_lockfree_sync_queue_put(pQueue,&msg,0,55);
                usleep(10 * 1000);
        }
}

static void *read_thread(void *arg)
{
        test_msg_t msg;
        const uint8_t threadId = *(uint8_t*)arg;
        uint16_t ret = 0;
        while (1) {
                ret = ring_lockfree_sync_queue_get(pQueue,&msg,0,50);
                if (0 == ret) {
                        printf("Thread id %d \n%s\n",threadId,msg.msg);
                }
                usleep(100 * 1000);
        }
}

void ring_lockfree_sync_queue_test(void)
{
        pthread_t write_thread_id;
        pthread_t read_thread_id[5];
        pQueue = ring_lockfree_sync_queue_new(sizeof(test_msg_t),10,30);
        if (NULL == pQueue) {
                printf("ring_lockfree_sync_queue_new failed\n");
        }
        else {
                pthread_create(&write_thread_id, NULL, write_thread, NULL);
                for (int i = 0; i < 5; i++) {
                        pthread_create(&read_thread_id[i], NULL, read_thread, &i);
                }
                for (int i = 0; i < 5; i++) {
                        pthread_join(read_thread_id[i], NULL);
                }
                pthread_join(write_thread_id, NULL);
        }
}


