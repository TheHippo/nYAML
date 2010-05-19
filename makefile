uname_O := $(shell sh -c 'uname -o 2>/dev/null || echo not')
uname_S := $(shell sh -c 'uname -s 2>/dev/null || echo not')

YAML-PATH = yaml-cpp/include/

ifeq ($(uname_S),Linux)
	CC = c++
	CFLAGS = -Wall -shared -I$(NEKOPATH) -I$(YAML-PATH) -fPIC
	NEKOPATH = /usr/lib/neko/include/
	LDFLAGS = -lneko
endif

ifeq ($(uname_O),Cygwin)
	CC = c++
	CFLAGS = -Wall -shared -I$(NEKOPATH) -I$(YAML-PATH)	-mno-cygwin
	NEKOPATH = /cygdrive/c/Program\ Files/Motion-Twin/neko/include/
	LDFLAGS = -L. -lneko
endif

OUT = nyaml.ndll

OBJ = src/nyaml.o

YAML = yaml-cpp/src/aliascontent.o yaml-cpp/src/conversion.o yaml-cpp/src/emitter.o yaml-cpp/src/emitterstate.o yaml-cpp/src/emitterutils.o yaml-cpp/src/exp.o yaml-cpp/src/iterator.o yaml-cpp/src/map.o yaml-cpp/src/node.o yaml-cpp/src/null.o yaml-cpp/src/ostream.o yaml-cpp/src/parser.o yaml-cpp/src/parserstate.o yaml-cpp/src/regex.o yaml-cpp/src/scalar.o yaml-cpp/src/scanner.o yaml-cpp/src/scanscalar.o yaml-cpp/src/scantag.o yaml-cpp/src/scantoken.o yaml-cpp/src/sequence.o yaml-cpp/src/simplekey.o yaml-cpp/src/stream.o yaml-cpp/src/tag.o 

all: $(OBJ) $(YAML)
	$(CC) $(CFLAGS) -o $(OUT) $(OBJ) $(LDFLAGS) $(YAML)

src/%.o: src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
	
yaml-cpp/src/%.o: yaml-cpp/src/%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean haxe test

clean:
	rm -f src/*.o
	rm -f $(OUT)
	rm -f yaml-cpp/src/*.o

haxe:
	haxe build.hxml
	
test: haxe
	neko test.n
