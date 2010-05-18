NEKOPATH = /usr/lib/neko/include/
CC = c++
CFLAGS = -Wall -shared -I$(NEKOPATH) -fPIC
LDFLAGS = -static -s -lyaml-cpp

OUT = nyaml.ndll

OBJ = src/nyaml.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(LDFLAGS)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean haxe test

clean:
	rm -f src/*.o

haxe:
	haxe build.hxml
	
test: haxe
	neko test.n
