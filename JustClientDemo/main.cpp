#include "src/zmq/zmq_util.h"
#include <assert.h>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <zmq.h>

int main() {



  // 1.创建上下文
  void *context = zmq_ctx_new();

  // 2.创建套接字、连接服务器
  void *requester = zmq_socket(context, ZMQ_REQ);
  zmq_connect(requester, "tcp://localhost:5555"); // tcp://127.0.0.1:5555

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

  return 0;
}