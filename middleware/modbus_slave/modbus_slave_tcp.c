//
// Created by three on 2025/5/7.
//

#include "modbus_slave.h"
#include "modbus_slave_mapping.h"
#include "modbus_slave_sync_list.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>

static void* modbus_slave_tcp_handle_client(void *arg)
{
        const ModbusSlaveTcpThreadTransParam_T trans_param = *(ModbusSlaveTcpThreadTransParam_T*)arg;
        if (NULL == trans_param.pChannelInfo) {
                close(trans_param.client_socket);
                return NULL;
        }
        modbus_t *pCtx = NULL;
        pCtx = modbus_new_tcp(NULL,0);//空函数创建上下文
        if (NULL == pCtx) {
                printf("创建客户端上下文失败\n");
                close(trans_param.client_socket);
                return NULL;
        }
        //绑定客户端套接字到上下文
        modbus_set_socket(pCtx,trans_param.client_socket);
        modbus_set_response_timeout(pCtx,1,0);

        printf("客户端 %d 已链接\n",trans_param.client_socket);
        while (1) {
                const uint8_t rc = modbus_slave_tcp_reply(pCtx,trans_param.pChannelInfo);
                if ((rc & (1 << 1) != 0)) {
                        break; //接收时遇到超时以外的错误，断开链接
                }
                if ((rc & (1 << 4) != 0)) {
                        break; //发送返回信息失败，断开链接
                }
        }
        //清理资源
        printf("客户端 %d 断开",trans_param.client_socket);
        close(trans_param.client_socket);
        modbus_free(pCtx);
        return NULL;
}

//进入线程前需要完成数据缓存区的初始化

void* modbus_slave_tcp_thread(void *arg)
{
        ModbusSlaveTcpChannel_T channel = *(ModbusSlaveTcpChannel_T*)arg;
        ModbusSlaveTcpThreadTransParam_T trans_param = {
                .client_socket = 0,
                .pChannelInfo = &channel.channelInfo,
        };
        modbus_t *pCtx = modbus_new_tcp(channel.config.ipv4,channel.config.port);
        if (NULL == pCtx) {
                return NULL;
        }
        int server_socket = modbus_tcp_listen(pCtx,channel.config.maxConnectionNum);
        if (-1 == server_socket) {
                printf("监听失败：%s\n",modbus_strerror(errno));
                modbus_free(pCtx);
                return NULL;
        }
        printf("server socket is %d\n",server_socket);
        while (1) {
                pthread_t thread;

                const int clientSocket = modbus_tcp_accept(pCtx,&server_socket);
                if (-1 == clientSocket) {
                        continue;
                }
                trans_param.client_socket = clientSocket;
                if (pthread_create(&thread,NULL,modbus_slave_tcp_handle_client,(void*)&trans_param)) {
                        printf("线程创建失败，关闭连接\n");
                        close(clientSocket);
                }
                else {
                        pthread_detach(thread);
                }
        }
        modbus_close(pCtx);
        modbus_free(pCtx);
        return NULL;
}

