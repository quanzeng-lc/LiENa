#! /bin/bash

stringOut=$(ps -ef | grep "python3")
echo $stringOut
stringOut1=$(echo $stringOut | cut -d "p" -f2)
echo $stringOut1
stringOut2=$(echo $stringOut1 | cut -d " " -f2)
echo $stringOut2
$(kill -s 9 $stringOut2)
