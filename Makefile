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
headers = \
	include/me_gl_window.h \
	include/me_shader_code.h

GLEW_VERSION = 2.2.0
GLFW_VERSION = 3.4

.PHONY: clean clean-vendor

main: main.c $(OBJ) $(headers)

src/me_gl_window.o: src/me_gl_window.c $(headers)

vendor/glew-$(GLEW_VERSION).zip:
	mkdir -p vendor
	curl -L -o vendor/glew-$(GLEW_VERSION).zip https://github.com/nigels-com/glew/releases/download/glew-$(GLEW_VERSION)/glew-$(GLEW_VERSION).zip

vendor/lib/libGLEW.a: vendor/glew-$(GLEW_VERSION).zip
	mkdir -p vendor/build
	mkdir -p vendor/include
	mkdir -p vendor/lib
	unzip vendor/glew-$(GLEW_VERSION).zip
	mv glew-* vendor/build/glew
	cd vendor/build/glew && \
		make glew.lib.static
	cp -r vendor/build/glew/include/. vendor/include/
	cp vendor/build/glew/lib/libGLEW.a vendor/lib
	rm -rf vendor/build/glew

vendor/glfw-$(GLFW_VERSION).zip:
	mkdir -p vendor
	curl -L -o vendor/glfw-$(GLFW_VERSION).zip https://github.com/glfw/glfw/releases/download/$(GLFW_VERSION)/glfw-$(GLFW_VERSION).zip

vendor/lib/libglfw3.a: vendor/glfw-$(GLFW_VERSION).zip
	mkdir -p vendor/build
	mkdir -p vendor/include
	mkdir -p vendor/lib
	unzip vendor/glfw-$(GLFW_VERSION).zip
	rm -rf vendor/build/glfw
	mv glfw-* vendor/build/glfw
	cd vendor/build/glfw && \
		cmake . -DGLFW_BUILD_EXAMPLES=OFF -DGLFW_BUILD_TESTS=OFF
	cd vendor/build/glfw && \
		make
	cp vendor/build/glfw/src/libglfw3.a ./vendor/lib/libglfw3.a
	cp -r vendor/build/glfw/include/. vendor/include/
	rm -rf vendor/build/glfw

clean:
	-rm main
	-rm ./src/*.o

clean-vendor:
	-rm -rf ./vendor/
