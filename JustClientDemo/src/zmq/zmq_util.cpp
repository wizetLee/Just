//
// Created by wizet on 2024/2/2.
//

#include "zmq_util.h"
#include <iostream>
#include <unistd.h>
#include <zmq.h>

int zmq_util::s_send(void *socket, const char *string) {
  int rc;

  size_t size = strlen(string);
  zmq_msg_t msg;
  zmq_msg_init_size(&msg, size);
  memcpy(zmq_msg_data(&msg), string, size);

  rc = zmq_msg_send(&msg, socket, 0);
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

void zmq_util::req_rep_client() {
  // 1.创建上下文
  void *context = zmq_ctx_new();

  // 2.创建套接字、连接服务器
  void *requester = zmq_socket(context, ZMQ_REQ);
  zmq_connect(requester, "tcp://localhost:5555");// tcp://127.0.0.1:5555

  int rc;
  // 3.循环发送数据、接收数据
  std::string sendStr("");

  while (true) {
    std::string userInput;
    std::cout << "请输入字符与服务器通信: ";
    std::getline(std::cin, userInput);

    // 发送
    rc = zmq_util::s_send(requester, (char *) userInput.c_str());
    if (rc > 0) {
      {
        auto time = std::chrono::system_clock::now();
        auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
        auto t = time_since_epoch.count();
        std::cout << "已发送了数据" << t << std::endl;
      }
      // 5.接收回复数据
      char *reply = zmq_util::s_recv(requester);// zmq_recv是阻塞等待的
      {
        auto time = std::chrono::system_clock::now();
        auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
        auto t = time_since_epoch.count();
        std::cout << "已接收了数据" << t << "__" << reply << std::endl;
      }
      if (reply != nullptr) {
        free(reply);
      }
    } else {
      std::cout << "zmq_util::s_send(requester, (char *) userInput.c_str());  有问题" << std::endl;
    }
  }

  //  for (int i = 0; i < 10; i++) {
  //    sendStr += "嘻";
  //    std::ostringstream oss;
  //    oss << sendStr << "_" << i;
  //    std::string wts(oss.str());
  //
  //    // 4.发送数据
  //    rc = zmq_util::s_send(requester, (char *) wts.c_str());
  //    if (rc > 0) {
  //      {
  //        auto time = std::chrono::system_clock::now();
  //        auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
  //        auto t = time_since_epoch.count();
  //        std::cout << "已发送了数据" << t << std::endl;
  //      }
  //      // 5.接收回复数据
  //      char *reply = zmq_util::s_recv(requester);// zmq_recv是阻塞等待的
  //      {
  //        auto time = std::chrono::system_clock::now();
  //        auto time_since_epoch = std::chrono::duration_cast<std::chrono::seconds>(time.time_since_epoch());
  //        auto t = time_since_epoch.count();
  //        std::cout << "已接收了数据" << t << "__" << reply << std::endl;
  //      }
  //      if (reply != nullptr) {
  //        free(reply);
  //      }
  //    } else {
  //      std::cout << "??" << std::endl;
  //    }
  //  }

  // 6.关闭套接字、销毁上下文
  zmq_close(requester);
  zmq_ctx_destroy(context);
}

void zmq_util::pub_sub_client() {

  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_SUB);
  int result = zmq_connect(socket, "tcp://localhost:5555");// tcp://127.0.0.1:5555

  // QUESTION： 如何设置只接收某个主题的消息
  //  const char *topic = "example_topic";
  //  zmq_setsockopt(socket, ZMQ_SUBSCRIBE, topic, strlen(topic));

  char *filter = "Topic"; // 设置我们需要的消息类型
  zmq_setsockopt(socket, ZMQ_SUBSCRIBE, filter, strlen(filter));

  while (true) {
    char *reply = zmq_util::s_recv(socket);
    if (reply != nullptr) {
      auto now = std::chrono::system_clock::now();
      auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
      std::cout << "已接收了数据" << timestamp << "__" << reply << std::endl;
      free(reply);
    }
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}

void zmq_util::push_pull_client() {
  void *context = zmq_ctx_new();
  void *socket = zmq_socket(context, ZMQ_PULL);
  zmq_connect(socket, "tcp://127.0.0.1:5555");
  while (true) {
    char *reply = zmq_util::s_recv(socket);
    if (reply != nullptr) {
      auto now = std::chrono::system_clock::now();
      auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
      std::cout << "已接收了数据" << timestamp << "__" << reply << std::endl;
      free(reply);
    }
  }
  zmq_close(socket);
  zmq_ctx_destroy(context);
}