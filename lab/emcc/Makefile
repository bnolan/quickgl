
all:
	emcc main.cpp -o index.html -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s LEGACY_GL_EMULATION=1 --no-heap-copy


http:
	python3 -m http.server