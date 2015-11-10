#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "thirdparty/hiredis-master/hiredis.h"

int main(int argc, char **argv) {
  // unsigned int j;
  redisContext *c;
  redisReply *reply;

#if 0
  if (argc < 4) {
    printf("Usage: 192.168.0.195 6379 instance_id password\n");
    exit(0);
  }
  const char *hostname = argv[1];
  const int port = atoi(argv[2]);
  const char *instance_id = argv[3];
  const char *password    = argv[4];
#endif


  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
  // c = redisConnectWithTimeout(hostname, port, timeout);
  c = redisConnectWithTimeout("10.66.89.11", 6379, timeout);
  if (c == NULL || c->err) {
    if (c) {
      printf("Connection error: %s\n", c->errstr);
      redisFree(c);
    } else {
      printf("Connection error: can't allocate redis context\n");
    }
    exit(1);
  }

  /* AUTH */
  // reply = redisCommand(c, "AUTH %s:%s", instance_id, password);
  reply = redisCommand(c, "AUTH 8b9fb2df-7603-4155-887b-f70c1ae12dca:patientsclub0755");
  printf("AUTH: %s\n", reply->str);
  freeReplyObject(reply);

  /* PING server */
  reply = redisCommand(c,"PING");
  printf("PING: %s\n", reply->str);
  freeReplyObject(reply);

  /* Set a key */
  reply = redisCommand(c,"SET %s %s", "name", "credis_test");
  printf("SET: %s\n", reply->str);
  freeReplyObject(reply);

  /* Try a GET  */
  reply = redisCommand(c,"GET name");
  printf("GET name: %s\n", reply->str);
  freeReplyObject(reply);

  /* Disconnects and frees the context */
  redisFree(c);

  return 0;
}
