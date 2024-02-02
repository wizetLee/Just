//
// Created by wizet on 2024/2/2.
//

#include <zmq.h>
#include "zmq_util.h"
#include <iostream>


int zmq_util::s_send(void *socket, char *string) {
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

char * zmq_util::s_recv(void *socket) {
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