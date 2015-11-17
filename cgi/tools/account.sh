#!/bin/bash
param_str=$1
url="http://182.254.220.116/cgi-bin/account.cgi"

# start to test post method
echo "---------------------------post start -------------------------------"
curl -d"action=login&password=111111&user=18127813634&token="  $url --trace-ascii post.trace --output post.res
cat post.res
echo "---------------------------post end-------------------------------"
echo ""
echo ""
