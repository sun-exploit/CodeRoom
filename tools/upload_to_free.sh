#!/bin/bash

REMOTE_DIR="my_dir"
LOCAL_FILE="my_file"
LOGIN="xxxx"
PASS="xxxx"

# cookie
curl -s --cookie-jar "cookie" -X POST -F "login=$LOGIN" -F "passwd=$PASS" http://ftpperso.free.fr/index.pl > /dev/null

# dir
curl -s --cookie cookie http://ftpperso.free.fr/index.pl?dir="$REMOTE_DIR" > /dev/null

# upload
curl -s --cookie cookie  -X POST  -F "file_upload1=@${LOCAL_FILE}" -F "filename=$LOCAL_FILE" http://ftpperso.free.fr/index.pl > /dev/null

