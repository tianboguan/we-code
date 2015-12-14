#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/account.cgi"
phone="18127813634"
user="1"
passwd="222222"
new_passwd="222222"
code="1234";
read_bin="./account_tool"

# start to test post method
if [ $method = "login" ];
then
  echo "---------------------------login start -------------------------------"
  curl -d"action=${method}&password=${passwd}&phone=${phone}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------login end-------------------------------"
fi

if [ $method = "enroll" ];
then
  echo "---------------------------enroll start -------------------------------"
  curl -d"action=${method}&password=${passwd}&phone=${phone}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------enroll end-------------------------------"
fi

if [ $method = "logout" ];
then
  echo "---------------------------logout start -------------------------------"
  curl -d"action=${method}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------logout end-------------------------------"
fi

if [ $method = "mod_pass" ];
then
  echo "---------------------------mod_pass start -------------------------------"
  curl -d"action=${method}&user=${user}&token=&o_pass=$passwd&n_pass=$new_passwd"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------mod_pass end-------------------------------"
fi

if [ $method = "reset_pass" ];
then
  echo "---------------------------reset_pass start -------------------------------"
  curl -d"action=${method}&user=${user}&phone=${phone}&token=&n_pass=$new_passwd"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------reset_pass end-------------------------------"
fi

if [ $method = "send_code" ];
then
  echo "---------------------------send_code start -------------------------------"
  curl -d"action=${method}&user=${user}&token="  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------send_code end-------------------------------"
fi

if [ $method = "verify_code" ];
then
  echo "---------------------------verify_code start -------------------------------"
  curl -d"action=${method}&user=${user}&token=&code=$code"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin -phone=${phone} -action=get;
  echo "---------------------------verify_code end-------------------------------"
fi
