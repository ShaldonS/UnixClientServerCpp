# UnixClientServerCpp
Simple client server using linux socket

# Compilation
-Server
g++ -o server server.cpp -pthread
-Client
g++ -o client client.cpp -pthread

# Launch:
Server
./server 3000
-Client
./client IlyaClient 3000 5

# See log:
tail -f log.txt
