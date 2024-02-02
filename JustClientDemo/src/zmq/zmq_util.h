//
// Created by wizet on 2024/2/2.
//

#ifndef JUSTCLIENTDEMO_SRC_ZMQ_ZMQ_UTIL_H_
#define JUSTCLIENTDEMO_SRC_ZMQ_ZMQ_UTIL_H_

class zmq_util {
 public:
  // 向socket发送数据, 数据为string
  static int s_send(void *socket, char *string);

  // 从socket接收数据, 并将数据以字符串的形式返回
  static char *s_recv(void *socket);
};

#endif//JUSTCLIENTDEMO_SRC_ZMQ_ZMQ_UTIL_H_
