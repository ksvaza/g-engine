@echo off
g++ -o build/main src/*.cpp ^
-Wall -pedantic -std=c++20 C:/glad-4.6/src/glad.c -I./include -IC:/glfw-3.4.bin.WIN64/include -IC:/glad-4.6/include -IC:/glm-1.0.1/include C:/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3dll.a -lgdi32 -lopengl32
cd build
.\main
cd ..
::
::src/main.cpp src/engine.cpp  src/winlib.cpp src/hwinputs.cpp src/sprite.cpp ^
:: `Gengine::HWInputs::RESETTABLE_MOUSE_BUTTONS'
:: `Gengine::HWInputs::LAST_MOUSE_POSITION'
:: `Gengine::HWInputs::Mouse'
:: `Gengine::HWInputs::RESETTABLE_MOUSE_BUTTON_COUNT'
:: `Gengine::HWInputs::Keyboard'
:: `Gengine::HWInputs::RESETTABLE_KEYS'
:: `Gengine::HWInputs::RESETTABLE_KEY_COUNT'
:: `Gengine::HWInputs::TEST_MODE'
