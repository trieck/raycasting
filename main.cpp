#include "stdafx.h"
#include "Maze.h"

int APIENTRY wWinMain(_In_ HINSTANCE /*hInstance*/,
    _In_opt_ HINSTANCE /*hPrevInstance*/,
    _In_ LPWSTR /*lpCmdLine*/,
    _In_ int /*nCmdShow*/)
{
    auto retcode = Maze::run();

    return retcode;
}
