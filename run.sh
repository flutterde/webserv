#!/bin/bash
rm -rf var/www/html/main/uploads/*
echo "🧪Starting Compiling🛠 files📁.🔋.🔋."
sleep 0.4
make -j 10 && clear && ./webserv configs/config.conf 
