@echo off
g++ -o build/main main.cpp -Wall -pedantic C:/glad-4.6/src/glad.c -I./include -IC:/glfw-3.4.bin.WIN64/include -IC:/glad-4.6/include C:/glfw-3.4.bin.WIN64/lib-mingw-w64/libglfw3dll.a -lgdi32 -lopengl32
:: gcc -o myprog myprog.c -I path/to/glfw/include path/to/glfw/lib-mingw-w64/libglfw3dll.a -lgdi32
:: gcc main.c -g libs/glad.c -IC:/glfw-3.4.bin.WIN64/include -IC:/glew-2.1.0/include -IC:/glad-4.6/include -IC:/cglm-0.9.4/include -LC:/glfw-3.4.bin.WIN64/lib-mingw-w64 -LC:/glew-2.1.0/lib/Release/x64 -lregex -lglfw3dll -lglew32 -lopengl32 -o build/main
.\build\main
