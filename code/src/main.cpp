#include <stdio.h>
#include "../include/engine.hpp"

int main(int argc, char* argv[])
{
    printf("Hello, World!\n");
    Gengine::Engine engine;
    engine.Initialise();
    engine.Run();
    engine.Terminate();
    return 0;
}
