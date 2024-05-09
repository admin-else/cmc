#include <stdio.h>
#include <cmc/err.h>
#include <cmc/conn.h>

int main() {
  cmc_conn conn = cmc_conn_init(37);

  puts("test");
  return 0;
}