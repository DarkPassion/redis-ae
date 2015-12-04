
cd libae && cmake . && make

gcc timer.c -Ilibae -llredis-ae -Ilibae -o timer.out




