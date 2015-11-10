#!/bin/bash
file=$1
fileid=`basename $file`
appid='10010517'
bucket='feelings'
echo "file: $file"
echo "fileid: $fileid"

# start to download picture
auth=`./test $fileid`
url="http://web.image.myqcloud.com/photos/v2/${appid}/${bucket}/0/${fileid}/del"
curl -X POST -H "Authorization: $auth" $url --trace-ascii del.trace --output del.res
cat del.res
echo "---------------------------del end-------------------------------"
echo ""
echo ""

# start to upload test
auth=`./test`
header="Content-Type: multipart/form-data;"
url="http://web.image.myqcloud.com/photos/v2/${appid}/${bucket}/0/${fileid}"
echo "test str:"
echo "------------------------------------"
echo "filecontent=@$file;filename=${fileid}"
curl --verbose -F "filecontent=@$file;filename=${fileid}" -H "$header" -H "Authorization: $auth" $url --output upload.res --trace-ascii upload.trace 
#cat upload.trace
cat upload.res
echo "---------------------------upload end-------------------------------"
echo ""
echo ""

#start download
url="http://feelings-10010517.image.myqcloud.com/google.jpg"
curl --verbose $url --output download.res --trace-ascii download.trace
#echo "---------------------------download end-------------------------------"
origin_file_size=`du -b $file | awk '{print $1}'`
download_file_size=`du -b download.res | awk '{print $1}'`
if ((origin_file_size == download_file_size))
then
  echo "DOWNLOAD TEST SUCCESSFULL"
else
  echo "DOWNLOAD TEST FAILED"
fi
echo "---------------------------download end-------------------------------"
echo ""
echo ""
