CFLAGS = \
	-I./vendor/include \
	-I./include
LDLIBS = \
	-L./vendor/lib \
	-lm \
	-lGL \
	-lGLEW \
	-lglfw3
OBJ = \
	vendor/lib/libGLEW.a \
	vendor/lib/libglfw3.a \
	src/me_gl_window.o

GLEW_VERSION = 2.2.0
GLFW_VERSION = 3.4

.PHONY: clean clean-vendor

main: main.c $(OBJ)

src/me_gl_window.o: src/me_gl_window.c

vendor/glew-$(GLEW_VERSION).zip:
	mkdir -p vendor
	curl -L -o vendor/glew-$(GLEW_VERSION).zip https://github.com/nigels-com/glew/releases/download/glew-$(GLEW_VERSION)/glew-$(GLEW_VERSION).zip

vendor/lib/libGLEW.a: vendor/glew-$(GLEW_VERSION).zip
	mkdir -p vendor/include
	mkdir -p vendor/lib
	unzip vendor/glew-$(GLEW_VERSION).zip
	mv glew-* glew-build
	cd glew-build && \
		make glew.lib.static
	cp -r glew-build/include/. vendor/include/
	cp glew-build/lib/libGLEW.a vendor/lib
	rm -rf glew-build

vendor/glfw-$(GLFW_VERSION).zip:
	mkdir -p vendor
	curl -L -o vendor/glfw-$(GLFW_VERSION).zip https://github.com/glfw/glfw/releases/download/$(GLFW_VERSION)/glfw-$(GLFW_VERSION).zip

vendor/src/gl.o: vendor/src/gl.c

vendor/lib/libglfw3.a: vendor/glfw-$(GLFW_VERSION).zip
	mkdir -p vendor/include
	mkdir -p vendor/lib
	unzip vendor/glfw-$(GLFW_VERSION).zip
	rm -rf glfw-build
	mv glfw-* glfw-build
	cd glfw-build && \
		cmake . -DGLFW_BUILD_TESTS=OFF
	cd glfw-build && \
		make
	cp glfw-build/src/libglfw3.a ./vendor/lib/libglfw3.a
	cp -r glfw-build/include/. vendor/include/
	rm -rf glfw-build

clean:
	-rm main
	-rm ./src/*.o

clean-vendor:
	-rm -rf ./vendor/
