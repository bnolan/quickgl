# QuickGL

A javascript runtime with tinygl bindings.

<img width="522" alt="Screenshot 2024-02-06 at 5 40 29 PM" src="https://github.com/bnolan/quickgl/assets/17499/8680ce19-96db-4154-a04e-72f9a7dfb998">

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
