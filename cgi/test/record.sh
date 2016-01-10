#!/bin/bash
method=$1
url="http://182.254.220.116/cgi-bin/record.cgi"
user="4"
token=""
target_user=$2
page=$3

# start to test post method
if [ $method = "create" ];
then
  echo "--------------------------- create start -------------------------------"
  curl -d"action=${method}&user=${user}&token=${token}&weather=&mood=http://i0.sinaimg.cn/gm/2011/0822/U5334P115DT20110822104124.jpg&status=&text=hello patientsclub&picture=6&address=&is_public=yes"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
else
  echo "--------------------------- create start -------------------------------"
  #curl -d"action=${method}&user=${user}&token=${token}&target_user=$2&page=$page"  $url --trace-ascii post.trace --output post.res
  curl -d"action=${method}&user=25&token=7a6fdf06ff92c65e3b91c8022772adeb&target_user=25&page=1"  $url --trace-ascii post.trace --output post.res
  cat post.res
  $read_bin 
  echo "--------------------------- create end-------------------------------"
fi

