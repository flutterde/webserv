#!/bin/bash
echo "🧪Starting Compiling🛠 files📁.🔋.🔋."
sleep 0.4
make -j 10 && clear && ./webserv configs/config.conf 
