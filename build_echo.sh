
cd libae && cmake . && make

cd ..


gcc echo-server.c  -stdlib=libc++ -Llibae -lredis-ae -Ilibae -o echo-server.out


gcc echo-client.c  -stdlib=libc++ -Llibae -lredis-ae -Ilibae -o echo-client.out



