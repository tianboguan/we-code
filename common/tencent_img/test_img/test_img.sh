#!/bin/bash
file=$1
fileid=`basename $file`
appid='10010517'
bucket='feelings'
echo "file: $file"
echo "fileid: $fileid"

# start to del picture
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
url="http://${bucket}-${appid}.image.myqcloud.com/$fileid"
curl --verbose $url --output download.jpg
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

#start edit test
url="http://${bucket}-${appid}.image.myqcloud.com/${file}_test"
curl --verbose $url --output test.jpg 
url="http://${bucket}-${appid}.image.myqcloud.com/${file}_original"
curl --verbose $url --output orignal.jpg
