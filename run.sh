#!/bin/bash
echo "🧪Starting Compiling🛠 files📁.🔋.🔋."
rm  upload_*.zip*
rm  "upload_uploadtestfile.txt "
rm upload_testzzzzzippp.zip*
rm upload_arch01.zip
sleep 0.4
make -j 10 && clear && ./webserv configs/config.conf 
