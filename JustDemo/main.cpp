
#include "src/zmq/zmq_util.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <zmq.h>

int main() {
  // 1.创建上下文
  void *context = zmq_ctx_new();

  // 2.创建、绑定套接字
  void *responder = zmq_socket(context, ZMQ_REP);
  zmq_bind(responder, "tcp://*:5555");

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

  return 0;
}
