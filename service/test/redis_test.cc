#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

//#include "thirdparty/hiredis-master/hiredis.h"
#include "common/redis_utils/RedisString.h"
#include "common/redis_utils/RedisPB.h"
#include "proto/hello_proto/hello.pb.h"

using namespace std;
int main(int argc, char **argv) {
#if 0
  // unsigned int j;
  redisContext *c;
  redisReply *reply;

  struct timeval timeout = { 1, 500000 }; // 1.5 seconds
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

  HelloMsg hello_msg;
  hello_msg.set_hello("lalala we lalala");
  hello_msg.set_standard_int(1);
  hello_msg.set_long_int(123456789012);

  string serial_str;
  hello_msg.SerializeToString(&serial_str);
  cout << "msg: \n-------------------\n" << hello_msg.DebugString() << endl;
  /* Set a key */
  //reply = redisCommand(c,"SET %s %s", "name", "credis_test");
  reply = redisCommand(c,"SET %b %b", "name", (size_t) 4, serial_str.data(), serial_str.size());
  printf("SET: %s\n", reply->str);
  freeReplyObject(reply);

  /* Try a GET  */
  reply = redisCommand(c,"GET %b", "name", (size_t) 4);
  // printf("GET name: %s\n", reply->str);
  // string test(reply->str, sizeof(buff));
  cout << "GET name: " << reply->str << endl;
  HelloMsg ret_msg;
  ret_msg.ParseFromArray(reply->str, reply->len);
  cout << "GET name: " << ret_msg.DebugString() << endl;
  freeReplyObject(reply);

  /* Disconnects and frees the context */
  redisFree(c);
#endif

#if 0         // test redis string utils
  RedisString rs;
  string key = "key";
  string value = "value";
  int ret;

  ret = rs.Set(key, value);
  if (ret != 0) {
    cout << "err: " << rs.Error() << endl;
    return -1;
  }

  string ret_value;
  ret = rs.Get(key, ret_value);
  // ret = rs.Get("haha", ret_value);
  if (ret != 0) {
    cout << "err: " << rs.Error() << endl;
    return -1;
  }
  cout << "get value: " << ret_value << endl;
  return 0;
#endif

#if 1    // test redis pb utils
  HelloMsg key;
  key.set_hello("key");
  key.set_standard_int(1);
  key.set_long_int(123456789012);
  HelloMsg value;
  value.set_hello("value");
  value.set_standard_int(2);
  value.set_long_int(222222);

  RedisPB<HelloMsg, HelloMsg> rp;
  int ret;
  ret = rp.Set(key, value);
  if (ret != 0) {
    cout << "err: " << rp.Error() << endl;
    return -1;
  }

  HelloMsg ret_value;
  ret = rp.Get(key, ret_value);
  if (ret != 0) {
    cout << "err: " << rp.Error() << endl;
    return -1;
  }
  cout << "get value: " << ret_value.DebugString() << endl;

  string pbkey("pbkey");
  ret = rp.Set(pbkey, key);
  if (ret != 0) {
    cout << "err: " << rp.Error() << endl;
    return -1;
  }

  ret = rp.Get(pbkey, ret_value);
  if (ret != 0) {
    cout << "err: " << rp.Error() << endl;
    return -1;
  }
  cout << "get value: " << ret_value.DebugString() << endl;
#endif

  return 0;
}
