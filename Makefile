CFLAGS=     -W -Wall -std=c99 -pedantic -O2 $(COPT)
LINFLAGS=   -ldl -pthread $(CFLAGS)

all: src/control.cpp src/http.cpp src/main.cpp
	g++ -o bin/refin $^ -lmongoose -Llib

so: src/mongoose/mongoose.c libdir lib/libmongoose.so
	$(CC) src/mongoose/mongoose.c -shared -fPIC -fpic -o lib/libmongoose.so $(LINFLAGS) 

libdir: lib 
	mkdir lib >/dev/null 2>/dev/null
