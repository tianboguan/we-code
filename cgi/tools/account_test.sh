#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/account.cgi"
user="18127813634"
passwd="111111"
new_passwd="222222"
code="6666";
read_bin="./get_account"

# start to test post method
if [ $method = "login" ];
then
  echo "---------------------------login start -------------------------------"
  curl -d"action=${method}&password=${passwd}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------login end-------------------------------"
fi

if [ $method = "enroll" ];
then
  echo "---------------------------enroll start -------------------------------"
  curl -d"action=${method}&password=${passwd}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------enroll end-------------------------------"
fi

if [ $method = "logout" ];
then
  echo "---------------------------logout start -------------------------------"
  curl -d"action=${method}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------logout end-------------------------------"
fi

if [ $method = "mod_pass" ];
then
  echo "---------------------------mod_pass start -------------------------------"
  curl -d"action=${method}&user=${user}&token=&o_pass=$passwd&n_pass=$new_passwd"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------mod_pass end-------------------------------"
fi

if [ $method = "reset_pass" ];
then
  echo "---------------------------reset_pass start -------------------------------"
  curl -d"action=${method}&user=${user}&token=&n_pass=$new_passwd"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------reset_pass end-------------------------------"
fi

if [ $method = "send_code" ];
then
  echo "---------------------------send_code start -------------------------------"
  curl -d"action=${method}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------send_code end-------------------------------"
fi

if [ $method = "verify_code" ];
then
  echo "---------------------------verify_code start -------------------------------"
  curl -d"action=${method}&user=${user}&token=&code=$code"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin $user;
  echo "---------------------------verify_code end-------------------------------"
fi
