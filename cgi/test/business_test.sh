#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/business.cgi"
user="4"
token=""
address_code=$2;
tag_class=$2

# start to test post method
if [ $method = "address" ];
then
  echo "--------------------------- address  start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}&address_code=${address_code}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- address end-------------------------------"
fi

if [ $method = "disease" ];
then
  echo "--------------------------- disease start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- disease end-------------------------------"
fi

if [ $method = "tag" ];
then
  echo "-------------------------- tag start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}&tag_class=${tag_class}"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "-------------------------- tag end-------------------------------"
fi

