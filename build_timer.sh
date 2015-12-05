
cd libae && cmake . && make

cd ..

gcc timer.c -Llibae -lredis-ae -Ilibae -o timer.out





