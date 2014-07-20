#include "game.hpp"

#if defined(_WIN32)

#include <windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    Game game;
    game.run();
    return 0;
}

#elif defined(__WIN32__)

#include <Windows.h>
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
    Game game;
    game.run();
    return 0;
}

#else

int main(int argc, char** argv) {
    Game game;
    game.run();
    return 0;
}

#endif
