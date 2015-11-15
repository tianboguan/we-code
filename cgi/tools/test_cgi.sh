#!/bin/bash
param_str=$1
url="http://182.254.220.116/cgi-bin/test.cgi"

# start to test get method
echo "---------------------------get start -------------------------------"
curl -G -d"p1=aaaa&p2=2222&p3=3333"  $url --trace-ascii get.trace --output get.res
cat get.res
echo "---------------------------get end-------------------------------"
echo ""
echo ""


# start to test post method
echo "---------------------------post start -------------------------------"
curl -d"p1=aaaa&p2=2222&p3=3333"  $url --trace-ascii post.trace --output post.res
cat post.res
echo "---------------------------post end-------------------------------"
echo ""
echo ""
