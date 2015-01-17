# course_project
Course Project for Network Programing

#direction
Please change IN_FILE_NAME in client.c to your own test file name

The default name of output file is "out".
You can change it by OUT_FILE_NAME in server.c

#intall
make clean

make

#run
./server          # must run first

./client

#version
2015/01/05    v1.0 one pthread send&recv 1G file
2015/01/06    v1.1 rollqueue for buffer
2015/01/16    v2.0 multi pthread
