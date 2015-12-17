#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/profile.cgi"
user="4"
target_user="4"
token=""
read_bin="/home/joshua/project/we-code/build64_release/cgi/tools/profile_tool -action=get -user=${target_user}"

#profile="{
#  'user': '1',
#  'nickname': '田伯光',
#  'head': 'http://xxxxxxx/田伯光_head.jpg',
#  'agender': 'male',
#  'birthday': 1447988674,
#  'address': '国家/省/市',
#  'signature': '快乐田伯光，万里独行田伯光',
#  'disease': '癌症',
#  'role': '家属',
#  'enrolltime': 1447988674,
#  }"
profile="{
  \"user\": \"4\"
  }"

#{"code":0,"message":"处理成功","data":{"user":"1","nickname":"","head":"","agender":"","birthday":"0","address":"","signature":"","disease":"","role":"","enrollTime":"1450094097"}}

# start to test post method
if [ $method = "alt" ];
then
  echo "--------------------------- alt  start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}&profile=${profile}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- alt end-------------------------------"
fi

if [ $method = "alt_head" ];
then
  echo "--------------------------- alt_head start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- alt_head end-------------------------------"
fi

if [ $method = "query" ];
then
  echo "-------------------------- query start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}&target_user=${target_user}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "-------------------------- query  end-------------------------------"
fi

