#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/follow.cgi"
user="4"
token=""
target_user=$2

# start to test post method
if [ $method = "add" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi

if [ $method = "del" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi
if [ $method = "follow_list" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi
if [ $method = "followed_list" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi
if [ $method = "follow_status" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi

if [ $method = "block" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi

if [ $method = "unblock" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi
#action -> home, page -> 1, target_user -> 25, token -> 7a6fdf06ff92c65e3b91c8022772adeb, user -> 25
