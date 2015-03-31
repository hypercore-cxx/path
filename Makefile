DEPS ?= gyp

all: build test

.PHONY: deps
deps: $(DEPS)

gyp: deps/gyp
deps/gyp:
	git clone --depth 1 https://chromium.googlesource.com/external/gyp.git ./deps/gyp

build: $(DEPS)
	deps/gyp/gyp --depth=. -Goutput_dir=./out -Icommon.gypi --generator-output=./build -Dlibrary=static_library -f make

.PHONY: test
test: ./test/test.cc
	make -C ./build/ test
	cp ./build/out/Release/test ./test/test

