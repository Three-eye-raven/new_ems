#if 0
#        include <assert.h>
#        include <stdint.h>
#        include <stdio.h>

#        include "pvpcs_open_interface.h"

#        include "modbus_master_api.h"

int main() {
    bool ret = false;
    ret = pvpcs_tcp_communication_thread_init();
    if (!ret) {
        return 0;
    }
    pvpcs_tcp_communication_thread_join();
}

#endif

#if 0
#        include <errno.h>
#        include <modbus.h>
#        include <pthread.h>
#        include <stdio.h>
#        include <stdlib.h>
#        include <unistd.h>

#        define MODBUS_PORT 1502
#        define SLAVE_ID 1
#        define NUM_REGISTERS 100

// 共享数据映射（需要线程安全访问）
modbus_mapping_t *mapping;

modbus_mapping_t empty_mapping = {
        .nb_bits = 0,
        .nb_input_bits = 0,
        .nb_input_registers = 0,
        .nb_registers = 0,
        .start_bits = 0,
        .start_input_bits = 0,
        .start_input_registers = 0,
        .start_registers = 0,
        .tab_bits = NULL,
        .tab_input_bits = NULL,
        .tab_input_registers = NULL,
        .tab_registers = NULL,
};

// 互斥锁保护共享数据
pthread_mutex_t mapping_mutex = PTHREAD_MUTEX_INITIALIZER;

void* handle_client(void *arg) {
    int client_socket = *(int*)arg;
    modbus_t *ctx;
    uint8_t req[MODBUS_TCP_MAX_ADU_LENGTH];
    struct timeval timeout = {1, 0};

    // 1. 为每个连接创建新上下文
    ctx = modbus_new_tcp(NULL, 0); // 空参数创建上下文
    if (ctx == NULL) {
        fprintf(stderr, "创建客户端上下文失败\n");
        close(client_socket);
        return NULL;
    }

    // 2. 绑定客户端套接字到上下文
    modbus_set_socket(ctx, client_socket);
    modbus_set_response_timeout(ctx, 1,0);

    printf("客户端 %d 已连接\n", client_socket);

    while (1) {
        int rc;

        // 3. 接收请求
        rc = modbus_receive(ctx, req);
        if (rc == -1) {
            if (errno != ETIMEDOUT) {
                    break;
            }
            continue; // 忽略超时
        }

        // 4. 加锁访问共享数据
        pthread_mutex_lock(&mapping_mutex);
        rc = modbus_reply(ctx, req, rc, &empty_mapping);
        pthread_mutex_unlock(&mapping_mutex);

        if (rc == -1) break;
    }

    // 5. 清理资源
    printf("客户端 %d 断开\n", client_socket);
    close(client_socket);
    modbus_free(ctx);
    return NULL;
}

void* modbus_slave_thread(void *arg) {
    modbus_t *ctx;
    int server_socket;

    // 初始化共享数据映射
    mapping = modbus_mapping_new(0, 0, NUM_REGISTERS, NUM_REGISTERS);
    if (!mapping) {
        fprintf(stderr, "初始化数据映射失败\n");
        return NULL;
    }

    // 主监听上下文
    ctx = modbus_new_tcp("0.0.0.0", MODBUS_PORT);
    server_socket = modbus_tcp_listen(ctx, 5);
    if (server_socket == -1) {
        fprintf(stderr, "监听失败: %s\n", modbus_strerror(errno));
        modbus_free(ctx);
        return NULL;
    }

    printf("从站已启动，监听端口 %d\n", MODBUS_PORT);

    while (1) {
        int client_socket;
        pthread_t thread;

        // 接受新连接
        client_socket = modbus_tcp_accept(ctx, &server_socket);
        if (client_socket == -1) continue;

        // 为每个客户端创建独立线程
        if (pthread_create(&thread, NULL, handle_client, &client_socket) != 0) {
            fprintf(stderr, "线程创建失败，关闭连接\n");
            close(client_socket);
        } else {
            pthread_detach(thread); // 分离线程自动回收资源
        }
    }

    modbus_mapping_free(mapping);
    modbus_close(ctx);
    modbus_free(ctx);
    return NULL;
}

int main() {
    pthread_t slave_thread;
    pthread_create(&slave_thread, NULL, modbus_slave_thread, NULL);
    pthread_join(slave_thread, NULL);
    return 0;
}
#endif

#if 0

#        include "ring_lockfree_sync_queue_api.h"

int main()
{
        ring_lockfree_sync_queue_test();
        return 0;
}

#endif

#if 0

#        include "ems_modbus_slave.h"

#        include <linux/can/j1939.h>

int main()
{
        ems_modbus_slave_thread_init();
        ems_modbus_slave_thread_join();
        return 0;
}

#endif

#if 0

#        include "pthread.h"
#        include "unistd.h"

#        include <can_netlink.h>
#        include <libsocketcan.h>
#        include <linux/can.h>
#        include <net/if.h>
#        include <stdint.h>
#        include <stdio.h>
#        include <string.h>
#        include <sys/ioctl.h>
#        include <sys/socket.h>

static int can_sock = -1;
static pthread_t monitor_thread;

// 协议常量定义
#        define BCU_ADDR 0xE8
#        define PC_ADDR 0xF4
#        define CAN_PRIORITY 6
#        define CAN_DATA_PAGE 0

// J1939 PDU格式构造宏
#        define BUILD_PDU(p, pf, ps, sa)                                                   \
                (((p & 0x7) << 26) | ((CAN_DATA_PAGE & 0x1) << 25) | ((pf & 0xFF) << 16) | \
                 ((ps & 0xFF) << 8) | (sa & 0xFF))


// 状态监控线程
static void* can_monitor(void *arg) {
        const char *ifname = (const char*)arg;
        printf("can 状态监控线程启动\n");
        while(1) {
                int state;
                uint32_t txerr = 0, rxerr = 0;
                can_get_state(ifname, &state);

                if (state == CAN_STATE_BUS_OFF) {
                        can_do_restart(ifname);
                }
                sleep(1);
        }
        return NULL;
}

int can_init(const char *ifname, int bitrate) {
        // 配置硬件
        if (can_do_stop(ifname) < 0) return -1;
        if (can_set_bitrate(ifname, bitrate) < 0) return -1;
        if (can_do_start(ifname) < 0) return -1;

        // 创建Socket
        can_sock = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        struct ifreq ifr;
        strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
        ioctl(can_sock, SIOCGIFINDEX, &ifr);

        struct sockaddr_can addr = { .can_family = AF_CAN, .can_ifindex = ifr.ifr_ifindex };
        bind(can_sock, (struct sockaddr*)&addr, sizeof(addr));

        // 启动监控线程
        pthread_create(&monitor_thread, NULL, can_monitor, (void*)ifname);

        return can_sock;
}

int can_send(uint32_t can_id, const uint8_t *data, uint8_t dlc) {
        struct can_frame frame = {
                .can_id = can_id | CAN_EFF_FLAG,
                .can_dlc = dlc
            };
        memcpy(frame.data, data, dlc);
        return write(can_sock, &frame, sizeof(frame));
}

// BCU地址定义
#        define BCU_ADDR 0xE8
#        define PC_ADDR 0xF4

// 电压数据结构
typedef struct {
        uint8_t afe_id;
        uint8_t frame_seq;
        uint16_t voltages[3];  // 每帧3个电压值
} VoltageData;

void handle_voltage_data(const uint8_t *data) {
        VoltageData vdata = {
                .afe_id = data[0],
                .frame_seq = data[1],
                .voltages = {
                        (data[2] << 8) | data[3],
                        (data[4] << 8) | data[5],
                        (data[6] << 8) | data[7]
                    }
        };
        // 数据转换：0.001V/bit
        printf("电压数据: AFE%d 帧%d | %.3fV, %.3fV, %.3fV\n",
               vdata.afe_id, vdata.frame_seq,
               vdata.voltages[0]*0.001,
               vdata.voltages[1]*0.001,
               vdata.voltages[2]*0.001);
}

// 协议命令码处理
void protocol_handler(uint32_t can_id, const uint8_t *data, uint8_t dlc) {
        uint8_t pf = (can_id >> 16) & 0xFF;
        uint8_t sa = can_id & 0xFF;

        if (sa != BCU_ADDR) return;  // 过滤非BCU消息

        switch(pf) {
        case 0x40:  // 电压数据
                handle_voltage_data(data);
                break;
        case 0x44:  // 主控状态
                //handle_main_status(data);
                break;
                // 其他命令码处理...
        }
}



void send_voltage_request(void) {
        uint8_t req_data[8] = {0x40, 0x00};
        uint32_t pdu = BUILD_PDU(6, 0xA2, PC_ADDR, BCU_ADDR);
        can_send(pdu, req_data, 2);
}

// 状态回调示例
void status_monitor(int state, uint32_t txerr, uint32_t rxerr) {
        if (state != CAN_STATE_ERROR_ACTIVE) {
                fprintf(stderr, "异常状态: %d | 错误计数 TX:%u RX:%u\n", state, txerr, rxerr);
        }
}

void can_recv_loop(void) {
        struct can_frame frame;
        while(1) {
                ssize_t nbytes = read(can_sock, &frame, sizeof(frame));
                if (nbytes > 0) {
                        // 协议解析回调
                        protocol_handler(frame.can_id, frame.data, frame.can_dlc);
                }
        }
}

void can_shutdown(void) {
        pthread_cancel(monitor_thread);
        close(can_sock);
}

int main() {
        // 初始化CAN接口
        if (can_init("can0", 250000) < 0) {
                fprintf(stderr, "CAN接口初始化失败\n");
                return 1;
        }

        // 发送电压请求
        sleep(20);
        send_voltage_request();
        printf("电压请求发送\n");
        // 启动接收循环
        can_recv_loop();

        // 清理（通常不会执行到这里）
        can_shutdown();
        return 0;
}

typedef struct
{
    unsigned int bit1:1;

}bis;

#endif

#if 0

/* For sockaddr_in */
#        include <netinet/in.h>
/* For socket functions */
#        include <sys/socket.h>
/* For gethostbyname */
#        include <netdb.h>

#        include <stdio.h>
#        include <string.h>
#        include <unistd.h>

int main(int c, char **v)
{

        return 0;
}

#endif

#if 0
#        include <event2/event.h>
#        include <libsocketcan.h>
#        include <linux/can/raw.h>
#        include <net/if.h>
#        include <pthread.h>
#        include <stdatomic.h>
#        include <stdbool.h>
#        include <stdio.h>
#        include <stdlib.h>
#        include <string.h>
#        include <sys/eventfd.h>
#        include <sys/ioctl.h>
#        include <sys/socket.h>
#        include <unistd.h>

#        define CAN_INTERFACE "vcan0"
#        define WATCHDOG_TIMEOUT 5.0   // 通信超时5秒
#        define MAX_RESET_RETRY 3      // 最大复位重试次数

// 程序状态管理结构体
struct can_manager {
    // 事件循环相关
    struct event_base *base;
    struct event *can_event;
    struct event *watchdog_event;
    struct event *notify_event;

    // 线程控制
    pthread_t maintenance_tid;
    pthread_mutex_t lock;
    int notify_fd;  // 线程间通知的eventfd
    atomic_bool running;
    atomic_int reset_retry;

    // CAN接口相关
    int can_fd;
    char ifname[16];
};

/*-------------------------------------
 CAN接口硬件复位（维护线程专用）
--------------------------------------*/
static void hardware_reset(const char *ifname)
{
    printf("[MAINT] Resetting %s hardware...\n", ifname);

    // 优先使用libsocketcan的优雅复位
    if (can_do_restart(ifname) != 0) {
        // 备用方案：传统ip命令复位
        char cmd[64];
        snprintf(cmd, sizeof(cmd), "ip link set %s down", ifname);
        system(cmd);
        usleep(100000); // 100ms延迟
        snprintf(cmd, sizeof(cmd), "ip link set %s up type can bitrate 500000", ifname);
        system(cmd);
    }

    // 检查复位结果
    int state;
    if (can_get_state(ifname, &state) == 0) {
        printf("[MAINT] Interface state: %s\n",
              (state == CAN_STATE_ERROR_ACTIVE) ? "Normal" : "Error");
    }
}


/*-------------------------------------
 创建CAN Socket资源（主线程）
--------------------------------------*/
static int create_can_socket(struct can_manager *mgr)
{
        struct sockaddr_can addr;
        struct ifreq ifr;

        // 创建新Socket
        mgr->can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        if (mgr->can_fd < 0) {
                perror("socket");
                return -1;
        }

        // 获取接口索引
        strncpy(ifr.ifr_name, mgr->ifname, sizeof(ifr.ifr_name)-1);
        if (ioctl(mgr->can_fd, SIOCGIFINDEX, &ifr) < 0) {
                perror("ioctl");
                close(mgr->can_fd);
                return -1;
        }

        // 绑定到接口
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        if (bind(mgr->can_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
                perror("bind");
                close(mgr->can_fd);
                return -1;
        }

        return 0;
}

/*-------------------------------------
 CAN数据接收回调（主线程）
--------------------------------------*/
static void can_read_cb(evutil_socket_t fd, short events, void *arg)
{
        struct can_manager *mgr = arg;
        struct can_frame frame;

        // 接收数据
        int n = read(fd, &frame, sizeof(frame));
        if (n <= 0) return;

        // 重置看门狗定时器
        struct timeval tv = { .tv_sec = (int)WATCHDOG_TIMEOUT, .tv_usec = 0 };
        evtimer_add(mgr->watchdog_event, &tv);

        // 处理数据（示例：打印内容）
        printf("CAN ID:0x%03X [%d] ", frame.can_id & CAN_EFF_MASK, frame.can_dlc);
        for(int i=0; i<frame.can_dlc; i++) printf("%02X ", frame.data[i]);
        printf("\n");
}

/*-------------------------------------
 重建CAN资源（主线程）
--------------------------------------*/
static void recreate_can_resources(struct can_manager *mgr)
{
        pthread_mutex_lock(&mgr->lock);

        // 清理旧资源
        if (mgr->can_event) {
                event_del(mgr->can_event);
                event_free(mgr->can_event);
                mgr->can_event = NULL;
        }
        close(mgr->can_fd);

        // 创建新Socket
        if (create_can_socket(mgr) != 0) {
                fprintf(stderr, "Failed to recreate CAN resources\n");
                exit(EXIT_FAILURE);
        }

        // 注册新事件
        mgr->can_event = event_new(mgr->base, mgr->can_fd,
                                  EV_READ|EV_PERSIST, can_read_cb, mgr);
        event_add(mgr->can_event, NULL);

        // 重启看门狗
        struct timeval tv = {WATCHDOG_TIMEOUT, 0};
        evtimer_add(mgr->watchdog_event, &tv);

        pthread_mutex_unlock(&mgr->lock);
}


/*-------------------------------------
 看门狗超时回调（主线程）
--------------------------------------*/
static void watchdog_cb(evutil_socket_t fd, short event, void *arg)
{
    struct can_manager *mgr = arg;
    printf("[WATCHDOG] No data for %.1fs, initiating reset...\n", WATCHDOG_TIMEOUT);

    // 发送复位请求给维护线程
    uint64_t req = 1;
    write(mgr->notify_fd, &req, sizeof(req));
}

/*-------------------------------------
 线程间通知回调（主线程）
--------------------------------------*/
static void notify_cb(evutil_socket_t fd, short events, void *arg)
{
    struct can_manager *mgr = arg;
    uint64_t val;

    if (read(fd, &val, sizeof(val)) != sizeof(val)) return;

    // 处理复位完成通知
    if (val == 0xDEAD) {
        printf("[MAIN] Reset completed, recreating resources\n");
        recreate_can_resources(mgr);
        atomic_store(&mgr->reset_retry, 0);
        return;
    }

    // 处理错误通知
    if (val == 0xFFFF) {
        fprintf(stderr, "[MAIN] Hardware reset failed after %d retries\n",
                atomic_load(&mgr->reset_retry));
        return;
    }
}



/*-------------------------------------
 维护线程（硬件操作专用）
--------------------------------------*/
static void* maintenance_thread(void *arg)
{
    struct can_manager *mgr = arg;
    uint64_t req;

    while (atomic_load(&mgr->running)) {
        // 等待复位请求
        if (read(mgr->notify_fd, &req, sizeof(req)) != sizeof(req)) continue;

        // 执行硬件复位
        int retry = 0;
        while (retry++ < MAX_RESET_RETRY) {
            hardware_reset(mgr->ifname);

            // 验证复位结果
            int state;
            if (can_get_state(mgr->ifname, &state) == 0 &&
                state == CAN_STATE_ERROR_ACTIVE) {
                break;
            }
            sleep(1);
        }

        // 通知主线程
        uint64_t status = (retry <= MAX_RESET_RETRY) ? 0xDEAD : 0xFFFF;
        write(mgr->notify_fd, &status, sizeof(status));
    }
    return NULL;
}

/*-------------------------------------
 初始化管理器
--------------------------------------*/
static int init_can_manager(struct can_manager *mgr, const char *ifname)
{
    // 初始化结构体
    memset(mgr, 0, sizeof(*mgr));
    strncpy(mgr->ifname, ifname, sizeof(mgr->ifname)-1);
    atomic_init(&mgr->running, true);
    atomic_init(&mgr->reset_retry, 0);
    pthread_mutex_init(&mgr->lock, NULL);

    // 创建通知fd
    mgr->notify_fd = eventfd(0, EFD_NONBLOCK);
    if (mgr->notify_fd == -1) {
        perror("eventfd");
        return -1;
    }

    // 初始化libevent
    mgr->base = event_base_new();
    if (!mgr->base) {
        fprintf(stderr, "Failed to create event base\n");
        close(mgr->notify_fd);
        return -1;
    }

    // 创建CAN Socket
    if (create_can_socket(mgr) != 0) {
        event_base_free(mgr->base);
        close(mgr->notify_fd);
        return -1;
    }

    // 注册事件
    mgr->can_event = event_new(mgr->base, mgr->can_fd,
                             EV_READ|EV_PERSIST, can_read_cb, mgr);
    mgr->watchdog_event = evtimer_new(mgr->base, watchdog_cb, mgr);
    mgr->notify_event = event_new(mgr->base, mgr->notify_fd,
                                 EV_READ|EV_PERSIST, notify_cb, mgr);

    // 添加事件
    event_add(mgr->can_event, NULL);
    event_add(mgr->notify_event, NULL);
    struct timeval tv = {WATCHDOG_TIMEOUT, 0};
    evtimer_add(mgr->watchdog_event, &tv);

    return 0;
}

/*-------------------------------------
 清理资源
--------------------------------------*/
static void cleanup_can_manager(struct can_manager *mgr)
{
    atomic_store(&mgr->running, false);

    // 关闭事件和socket
    event_del(mgr->can_event);
    event_free(mgr->can_event);
    evtimer_del(mgr->watchdog_event);
    event_free(mgr->watchdog_event);
    event_free(mgr->notify_event);
    close(mgr->can_fd);
    close(mgr->notify_fd);
    event_base_free(mgr->base);

    pthread_mutex_destroy(&mgr->lock);
}

/*-------------------------------------
 主程序
--------------------------------------*/
int main(void)
{
    struct can_manager mgr;

    // 初始化管理器
    if (init_can_manager(&mgr, CAN_INTERFACE) != 0) {
        fprintf(stderr, "Initialization failed\n");
        return EXIT_FAILURE;
    }

    // 启动维护线程
    pthread_create(&mgr.maintenance_tid, NULL, maintenance_thread, &mgr);

    // 进入事件循环
    printf("CAN Monitor started on %s\n", CAN_INTERFACE);
    event_base_dispatch(mgr.base);

    // 等待线程结束
    pthread_join(mgr.maintenance_tid, NULL);

    // 清理资源
    cleanup_can_manager(&mgr);
    return EXIT_SUCCESS;
}
#endif

#if 0

#        include <event2/event.h>
#        include <libsocketcan.h>
#        include <linux/can/raw.h>
#        include <net/if.h>
#        include <stdio.h>
#        include <stdlib.h>
#        include <string.h>
#        include <sys/ioctl.h>
#        include <sys/socket.h>
#        include <unistd.h>

#        define CAN_INTERFACE "vcan0"
#        define CHECK_INTERVAL 2      // 状态检查间隔(秒)
#        define SEND_INTERVAL_MS 10   // 发送间隔10ms

struct can_context {
        struct event_base *base;
        struct event *state_check_event;
        struct event *read_event;
        struct event *send_event;       // 新增发送事件
        struct event *cleanup_event;
        struct event *reset_event;
        struct event *recreate_event;

        int can_fd;//socket
        int stage;
};
/*-------------------------------------
 CAN发送回调（新增核心函数）
--------------------------------------*/
void can_send_cb(evutil_socket_t fd, short events, void *arg)
{
        struct can_context *ctx = arg;
        struct can_frame send_frame = {
                .can_id = 0x123,
                .can_dlc = 4,
                .data = {
                        [0] = 0x01,
                        [1] = 0x02,
                        [2] = 0x03,
                        [4] = 0x04,
                },
        };
        // 检查是否在正常状态
        if (ctx->stage != 0) return;

        // 发送CAN帧
        if (write(ctx->can_fd, &send_frame, sizeof(send_frame)) < 0) {
                perror("CAN send error");
                return;
        }

        // 重新调度发送定时器
        struct timeval tv = {
                .tv_sec = 0,
                .tv_usec = SEND_INTERVAL_MS * 1000
            };
        evtimer_add(ctx->send_event, &tv);
}
/*-------------------------------------
 CAN数据接收回调（新增核心函数）
--------------------------------------*/
void can_read_cb(evutil_socket_t fd, short events, void *arg)
{
        struct can_context *ctx = arg;
        struct can_frame frame;

        int n = read(fd, &frame, sizeof(frame));
        if(n <= 0) return;

        // 打印接收到的CAN数据
        printf("Received CAN frame: ID=0x%X DLC=%d Data=",
              frame.can_id & CAN_EFF_MASK, frame.can_dlc);
        for(int i=0; i<frame.can_dlc; i++) {
                printf("%02X ", frame.data[i]);
        }
        printf("\n");
}

/*-------------------------------------
 状态检查回调（定时触发）
--------------------------------------*/
void state_check_cb(evutil_socket_t fd, short events, void *arg)
{
    struct can_context *ctx = arg;

    // 获取CAN接口状态
    int state;
    if (can_get_state(CAN_INTERFACE, &state) == 0) {
        if (state == CAN_STATE_BUS_OFF && ctx->stage == 0) {
            printf("[STATE] BUS_OFF detected, trigger cleanup\n");

            // 进入清理阶段
            ctx->stage = 1;
            event_active(ctx->cleanup_event, EV_WRITE, 0);
        }
    }

    // 重新注册定时检查
    struct timeval tv = {CHECK_INTERVAL, 0};
    evtimer_add(ctx->state_check_event, &tv);
}

/*-------------------------------------
 更新后的资源清理回调
--------------------------------------*/
void cleanup_cb(evutil_socket_t fd, short events, void *arg)
{
        struct can_context *ctx = arg;

        if (ctx->stage != 1) return;

        printf("[CLEAN] Closing resources\n");

        // 移除读事件
        if(ctx->read_event) {
                event_del(ctx->read_event);
                event_free(ctx->read_event);
                ctx->read_event = NULL;
        }

        // 移除发送事件（新增）
        if(ctx->send_event) {
                event_del(ctx->send_event);
                event_free(ctx->send_event);
                ctx->send_event = NULL;
        }

        // 关闭socket
        close(ctx->can_fd);
        ctx->can_fd = -1;

        // 进入复位阶段
        ctx->stage = 2;
        event_active(ctx->reset_event, EV_WRITE, 0);
}

/*-------------------------------------
 硬件复位回调
--------------------------------------*/
void reset_cb(evutil_socket_t fd, short events, void *arg)
{
    struct can_context *ctx = arg;

    if (ctx->stage != 2) return;

    printf("[RESET] Performing hardware reset\n");

    // 执行复位（可加入重试逻辑）
    if (can_do_restart(CAN_INTERFACE) != 0) {
        system("ip link set can0 down && ip link set can0 up");
    }

    // 进入重建阶段
    ctx->stage = 3;
    event_active(ctx->recreate_event, EV_WRITE, 0);
}

/*-------------------------------------
 更新后的资源重建回调
--------------------------------------*/
void recreate_cb(evutil_socket_t fd, short events, void *arg)
{
        struct can_context *ctx = arg;

        if (ctx->stage != 3) return;

        printf("[RECREATE] Rebuilding resources\n");

        // 创建新socket
        struct sockaddr_can addr;
        struct ifreq ifr;
        ctx->can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        strcpy(ifr.ifr_name, CAN_INTERFACE);
        ioctl(ctx->can_fd, SIOCGIFINDEX, &ifr);
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(ctx->can_fd, (struct sockaddr*)&addr, sizeof(addr));

        // 重建读事件
        ctx->read_event = event_new(ctx->base, ctx->can_fd,
                                   EV_READ|EV_PERSIST, can_read_cb, ctx);
        event_add(ctx->read_event, NULL);

        // 重建发送事件（新增）
        ctx->send_event = evtimer_new(ctx->base, can_send_cb, ctx);
        struct timeval tv = {0, SEND_INTERVAL_MS * 1000};
        evtimer_add(ctx->send_event, &tv);

        // 返回正常状态
        ctx->stage = 0;
}

/*-------------------------------------
 更新后的初始化函数
--------------------------------------*/
void init_event_chain(struct can_context *ctx)
{
        // 状态检查事件
        ctx->state_check_event = evtimer_new(ctx->base, state_check_cb, ctx);

        // 读事件
        ctx->read_event = event_new(ctx->base, ctx->can_fd,
                                  EV_READ|EV_PERSIST, can_read_cb, ctx);
        event_add(ctx->read_event, NULL);

        // 发送事件（新增初始化）
        ctx->send_event = evtimer_new(ctx->base, can_send_cb, ctx);
        struct timeval tv = {0, SEND_INTERVAL_MS * 1000};
        evtimer_add(ctx->send_event, &tv);

        // 其他阶段事件
        ctx->cleanup_event = event_new(ctx->base, -1, EV_PERSIST, cleanup_cb, ctx);
        ctx->reset_event = event_new(ctx->base, -1, EV_PERSIST, reset_cb, ctx);
        ctx->recreate_event = event_new(ctx->base, -1, EV_PERSIST, recreate_cb, ctx);

        // 启动状态检查
        struct timeval check_tv = {CHECK_INTERVAL, 0};
        evtimer_add(ctx->state_check_event, &check_tv);
}


/*-------------------------------------
 主程序（保持不变）
--------------------------------------*/
int main()
{
        struct can_context ctx = {0};
        ctx.base = event_base_new();

        // 初始创建CAN socket
        struct sockaddr_can addr = {0};
        struct ifreq ifr;
        ctx.can_fd = socket(PF_CAN, SOCK_RAW, CAN_RAW);
        strcpy(ifr.ifr_name, CAN_INTERFACE);
        ioctl(ctx.can_fd, SIOCGIFINDEX, &ifr);
        addr.can_family = AF_CAN;
        addr.can_ifindex = ifr.ifr_ifindex;
        bind(ctx.can_fd, (struct sockaddr*)&addr, sizeof(addr));

        // 初始化事件链
        init_event_chain(&ctx);

        // 进入事件循环
        event_base_dispatch(ctx.base);

        // 清理资源
        event_free(ctx.state_check_event);
        event_free(ctx.read_event);
        event_free(ctx.send_event);  // 新增清理
        event_free(ctx.cleanup_event);
        event_free(ctx.reset_event);
        event_free(ctx.recreate_event);
        event_base_free(ctx.base);
        close(ctx.can_fd);

        return 0;
}

#endif


#if 1
#        include <arpa/inet.h>
#        include <errno.h>
#        include <event2/event.h>
#        include <event2/listener.h>
#        include <fcntl.h>
#        include <modbus.h>
#        include <stdio.h>
#        include <stdlib.h>
#        include <string.h>
#        include <unistd.h>

typedef struct
{
        modbus_t*         ctx;
        struct event*     event;
        modbus_mapping_t* mb_mapping;
} client_t;

void close_client(client_t* client)
{
        if (client) {
                if (client->event) {
                        event_free(client->event);
                }
                if (client->ctx) {
                        modbus_close(client->ctx);
                        modbus_free(client->ctx);
                }
                if (client->mb_mapping) {
                        modbus_mapping_free(client->mb_mapping);
                }
                free(client);
        }
}

void client_read_cb(evutil_socket_t fd, short events, void* arg)
{
        client_t* client = (client_t*)arg;
        modbus_t* ctx    = client->ctx;
        uint8_t   req[MODBUS_TCP_MAX_ADU_LENGTH];

        int rc = modbus_receive(ctx, req);
        if (rc > 0) {
                // 处理请求并回复
                rc = modbus_reply(ctx, req, rc, client->mb_mapping);
                if (rc == -1) {
                        fprintf(stderr, "Failed to reply: %s\n", modbus_strerror(errno));
                        close_client(client);
                }
        }
        else if (rc == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // 数据未就绪，等待下次事件
                        return;
                }
                else {
                        perror("modbus_receive");
                        close_client(client);
                }
        }
        else {
                // 连接关闭
                close_client(client);
        }
}

void accept_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa,
               int socklen, void* user_data)
{
        struct event_base* base = evconnlistener_get_base(listener);
        modbus_t*          ctx  = modbus_new_tcp(NULL, 0);   // 使用已连接的socket
        if (!ctx) {
                fprintf(stderr, "Failed to create modbus context\n");
                close(fd);
                return;
        }
        modbus_set_socket(ctx, fd);

        if (evutil_make_socket_nonblocking(fd) == -1) {
                perror("evutil_make_socket_nonblocking");
                modbus_free(ctx);
                close(fd);
                return;
        }

        modbus_mapping_t* mb_mapping = modbus_mapping_new(500, 500, 500, 500);
        if (!mb_mapping) {
                fprintf(stderr, "Failed to create modbus mapping\n");
                modbus_free(ctx);
                close(fd);
                return;
        }

        client_t* client = (client_t*)malloc(sizeof(client_t));
        if (!client) {
                perror("malloc");
                modbus_free(ctx);
                modbus_mapping_free(mb_mapping);
                close(fd);
                return;
        }
        client->ctx        = ctx;
        client->mb_mapping = mb_mapping;

        client->event = event_new(base, fd, EV_READ | EV_PERSIST, client_read_cb, client);
        event_add(client->event, NULL);
}

int main()
{
        struct event_base* base = event_base_new();
        if (!base) {
                fprintf(stderr, "Could not initialize libevent\n");
                return 1;
        }

        modbus_t* ctx = modbus_new_tcp("0.0.0.0", 1502);
        if (!ctx) {
                fprintf(stderr, "Failed to create modbus context\n");
                return 1;
        }

        int listen_fd = modbus_tcp_listen(ctx, 1);
        if (listen_fd == -1) {
                fprintf(stderr, "Listen failed: %s\n", modbus_strerror(errno));
                modbus_free(ctx);
                return 1;
        }

        if (evutil_make_socket_nonblocking(listen_fd) == -1) {
                perror("evutil_make_socket_nonblocking");
                close(listen_fd);
                modbus_free(ctx);
                return 1;
        }

        struct evconnlistener* listener = evconnlistener_new(
            base, accept_cb, NULL, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, listen_fd);
        if (!listener) {
                fprintf(stderr, "Could not create listener\n");
                close(listen_fd);
                modbus_free(ctx);
                return 1;
        }

        event_base_dispatch(base);

        evconnlistener_free(listener);
        modbus_free(ctx);
        event_base_free(base);

        return 0;
}
#endif