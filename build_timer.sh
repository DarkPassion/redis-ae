
cd libae && cmake . && make

gcc timer.c -Llibae -llredis-ae -Ilibae -o timer.out




