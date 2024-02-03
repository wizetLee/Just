//
// Created by wizet on 2024/2/2.
//

#ifndef JUSTDEMO_SRC_ZMQ_ZMQ_UTIL_H_
#define JUSTDEMO_SRC_ZMQ_ZMQ_UTIL_H_

class zmq_util {

 public:
  // 向socket发送数据, 数据为string
  static int s_send(void *socket, const char *string);

  // 从socket接收数据, 并将数据以字符串的形式返回
  static char *s_recv(void *socket);

  // 请求应答模式
  static void req_rep_server();

  // 发布订阅模式
  static void pub_sub_server();

  // 推送拉取模式
  static void push_pull_server();
};

#endif//JUSTDEMO_SRC_ZMQ_ZMQ_UTIL_H_
