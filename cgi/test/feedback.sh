#!/bin/bash
url="http://182.254.220.116/cgi-bin/feedback.cgi"
user="25"
token="7a6fdf06ff92c65e3b91c8022772adeb"
page=$1

# start to test post method
echo "--------------------------- get start -------------------------------"
curl -d"action=supply&user=${user}&token=${token}&content=gogogo&img_count=1"  $url --trace-ascii post.trace --output post.res
cat post.res
echo "--------------------------- get end-------------------------------"

