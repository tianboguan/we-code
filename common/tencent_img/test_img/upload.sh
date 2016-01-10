#!/bin/bash
user=$1
file=$2
filename=`basename $file`
fileid="${user}_`date +%s`_${filename}"
appid='10010517'
bucket='feelings'
echo "file: $file"
echo "fileid: $fileid"

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

