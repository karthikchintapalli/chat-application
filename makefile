%.o: %.cpp %.h
	g++ $< -c -g

peer: server.o client.o split.o main.cpp
	g++ $^ -o $@ -g