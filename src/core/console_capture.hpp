#pragma once

#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <stdio.h>
#include <windows.h>

namespace rl
{
#ifndef _USE_OLD_IOSTREAMS
    using namespace std;
#endif

#define READ_FD  0
#define WRITE_FD 1

#define CHECK(a)  \
    if ((a) != 0) \
        return -1;

    class StdOutRedirect
    {
    public:
        StdOutRedirect(int bufferSize);
        ~StdOutRedirect();

        int Start();
        int Stop();
        int GetBuffer(char* buffer, int size);

    private:
        int fdStdOutPipe[2] = { 0 };
        int fdStdOut = -1;
        FILE* stdout_file{ nullptr };
    };
}
