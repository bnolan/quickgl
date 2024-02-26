
MODULES := calib3d dnn flann highgui imgproc js objc photo stitching video world core features2d gapi imgcodecs java ml objdetect python ts videoio
BUILD := $(HOME)/Projects/build

CV_INCLUDE_FLAGS := $(foreach module,$(MODULES),-I$(BUILD)/opencv/modules/$(module)/include)



all:
	emcc main.cpp -o index.html -s USE_GLFW=3 -s USE_WEBGL2=1 -s USE_SDL=2 -s LEGACY_GL_EMULATION=1 --no-heap-copy\
		-I$(BUILD)/opencv/include \
		$(CV_INCLUDE_FLAGS) \
		-I$(BUILD)/build_js \
		-L$(BUILD)/build_js/lib \
		-lopencv_core \
		-lopencv_imgproc \
		--no-heap-copy



http:
	python3 -m http.server