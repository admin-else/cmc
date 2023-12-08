#include "mconn.h"

int main() {
  char *errmsg = NULL;
  MConn *conn = MConn_init(&errmsg);
  return 0;
}