
#include "src/zmq/zmq_util.h"
#include "event.h"

int main() {

  //  zmq_util::req_rep_server();
  //  zmq_util::pub_sub_server();
  //  zmq_util::push_pull_server();

  event_base *event = event_init();

  return 0;
}
