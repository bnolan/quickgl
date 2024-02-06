# QuickGL

A javascript runtime with tinygl bindings.

<img width="545" alt="Screenshot 2024-02-06 at 6 22 25 PM" src="https://github.com/bnolan/quickgl/assets/17499/8a81fd01-36b1-464d-91cc-0c83e56ed4a9">


## Configuring on MacOS

    brew install sdl2
    brew install sdl2_ttf
    brew install clang-foramt
    cd third_party/quickjs && make
    cd third_party/tinygl && make
    mkdir build
    cd build
    cmake ..

## Build and run

    make && ./QuickGL
