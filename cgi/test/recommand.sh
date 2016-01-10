#!/bin/bash
url="http://182.254.220.116/cgi-bin/recommand.cgi"
user="25"
token="7a6fdf06ff92c65e3b91c8022772adeb"
page=$1

# start to test post method
echo "--------------------------- get start -------------------------------"
#curl -d"action=${method}&user=${user}&token=${token}&target_user=$2&page=$page"  $url --trace-ascii post.trace --output post.res
curl -d"action=get&user=${user}&token=${token}&page=${page}"  $url --trace-ascii post.trace --output post.res
cat post.res
echo "--------------------------- get end-------------------------------"

