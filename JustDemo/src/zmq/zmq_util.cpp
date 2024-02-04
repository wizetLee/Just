//
// Created by wizet on 2024/2/2.
//

#include "zmq_util.h"
#include <iostream>
#include <sstream>
#include <time.h>
#include <unistd.h>
#include <zmq.h>

int zmq_util::s_send(void *socket, const char *string) {
  int rc;

  size_t size = strlen(string);
  zmq_msg_t msg;
  zmq_msg_init_size(&msg, size);
  memcpy(zmq_msg_data(&msg), string, size);

  rc = zmq_msg_send(&msg, socket, 0);// ZMQ_SNDMORE
  zmq_msg_close(&msg);

  std::cout << "发送size:" << size << std::endl;
  return rc;
}

char *zmq_util::s_recv(void *socket) {
  int rc;
  zmq_msg_t msg;
  zmq_msg_init(&msg);

  rc = zmq_msg_recv(&msg, socket, 0);
  if (rc == -1)
    return nullptr;

  std::cout << "接收size:" << rc << std::endl;
  char *string = (char *) malloc(rc + 1);
  memcpy(string, zmq_msg_data(&msg), rc);
  zmq_msg_close(&msg);

  string[rc] = 0;
  return string;
}

void zmq_util::req_rep_server() {

  // 1.创建上下文
  void *context = zmq_ctx_new();

  // 2.创建、绑定套接字
  void *responder = zmq_socket(context, ZMQ_REP);
  zmq_bind(responder, "tcp://*:5555");// 绑定地址的字符串。格式是 "<protocol>://<address>"，其中 <protocol> 是通信协议，<address> 是绑定的网络地址。
                                      //  tcp://: 这里使用TCP协议作为通信协议。
                                      //  *: 表示在所有网络接口上监听，而不仅仅是一个特定的IP地址。
                                      //  5555: 是指定的端口号。
  int rc;
  // 3.循环接收数据、发送数据
  while (true) {
    // 4.接收数据
    char *request = zmq_util::s_recv(responder);
    assert(request != nullptr);
    printf("Request: %s\n", request);
    free(request);

    // 休眠1秒再继续回复
    sleep(1);

    // 5.回送数据
    char *reply = "World";
    rc = zmq_util::s_send(responder, reply);
    assert(rc > 0);
  }

  // 6.关闭套接字、销毁上下文
  zmq_close(responder);
  zmq_ctx_destroy(context);
}

void zmq_util::pub_sub_server() {
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_PUB);// 返回一个指向新创建套接字的指针，该套接字已经被初始化为发布者类型。
  zmq_bind(socket, "tcp://*:5555");

  // Question：如何send某个主题的消息

  int i = 0;
  while (true) {
        {
          int rc;
          char *string = "Topic";
          size_t size = strlen(string);
          zmq_msg_t msg;
          zmq_msg_init_size(&msg, size);
          memcpy(zmq_msg_data(&msg), string, size);
          rc = zmq_msg_send(&msg, socket, ZMQ_SNDMORE);// ZMQ_SNDMORE
          zmq_msg_close(&msg);
        }
    // ZMQ_SNDMORE 是 ZeroMQ（ZMQ）中用于发送多部分消息（multipart messages）的选项标识。该标识告诉 ZeroMQ 库在发送消息时，当前消息后面还有更多的消息部分。
    // 在 ZeroMQ 中，消息可以分为多个部分，每个部分都可以单独发送或接收。通过在发送消息时使用 ZMQ_SNDMORE 标识，
    // 你可以告诉 ZeroMQ 库，当前发送的消息是多部分消息的一部分，后面还会有其他部分。这使得可以构建具有多部分内容的复杂消息。

    std::ostringstream oss;
    oss << "你好，这里是发布订阅模式 " << i;
    std::string msg = oss.str();
    int rc = zmq_util::s_send(socket, msg.c_str());
    sleep(1);
    i++;
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void zmq_util::push_pull_server() {
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_PUSH);
  zmq_bind(socket, "tcp://*:5555");

  int i = 0;
  while (true) {
    std::ostringstream oss;
    oss << "你好，这里是推送拉取模式 " << i;
    std::string msg = oss.str();
    int rc = zmq_util::s_send(socket, msg.c_str());
    sleep(1);
    i++;
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}