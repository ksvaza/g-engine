#include <stdio.h>
#include <engine.hpp>

char InputBuffer[256];

int main(int argc, char* argv[])
{
    for (int i = 0; i < 256; i++)
    {
        InputBuffer[i] = 0;
    }
    printf("Hello, World!\n");
    Gengine::Engine engine;
    engine.Initialise();
    engine.Run();
    engine.Terminate();
    //fgets(InputBuffer, 256, stdin);
    return 0;
}
