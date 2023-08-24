#include "core/console_capture.hpp"

#include "util/utils.hpp"

#include <WinBase.h>
#include <ios>
#include <processenv.h>
#include <stdio.h>
#include <string_view>

namespace rl
{
    StdOutRedirect::~StdOutRedirect()
    {
        //_close(fdStdOut);
        //_close(fdStdOutPipe[WRITE_FD]);
        //_close(fdStdOutPipe[READ_FD]);
        _flushall();
    }

    StdOutRedirect::StdOutRedirect(int bufferSize)
    {
        // ios::sync_with_stdio(false);

        auto pipe_handle{ _pipe(fdStdOutPipe, bufferSize, O_TEXT) };
        debug::assert(pipe_handle == 0);
        if (pipe_handle == 0)
        {
            debug::assert(stdout_file == nullptr);
            freopen_s(&stdout_file, "aaaa", "w+", stdout);
            setvbuf(stdout, nullptr, _IONBF, 0);
            std::cout.clear();
            //_get_osfhandle(_fileno(stdout));
            // HANDLE asdf = GetStdHandle(STD_OUTPUT_HANDLE);
            // const int existing_stdout1 = _fileno(reinterpret_cast<FILE*>(&asdf));
            // const int existing_stdout2 = _fileno(stdout);
            //// fdStdOut = _dup(existing_stdout);
            // auto test{ _fileno(stdout) };
            // fdStdOut = _dup(_fileno(stdout));
        }
    }

    int StdOutRedirect::Start()
    {
        // fflush(stdout);
        //// , _fileno(stdout)));
        // CHECK(_dup2(fdStdOutPipe[WRITE_FD], _fileno(stdout)));
        //// ios::sync_with_stdio();
        // setvbuf(stdout, NULL, _IONBF, 0);  // absolutely needed
        return 0;
    }

    int StdOutRedirect::Stop()
    {
        // CHECK(_dup2(fdStdOut, _fileno(stdout)));
        //// ios::sync_with_stdio();
        return 0;
    }

    int StdOutRedirect::GetBuffer(char* buffer, int size)
    {
        // std::cout.flush();
        //  fflush(stdout_file);
        char* read_bytes = fgets(buffer, size, stdout_file);

        // debug::assert(read_bytes != nullptr);
        if (read_bytes != nullptr)
            return 0;

        std::string_view asdf{ buffer };
        return asdf.size();

        /*int nOutRead = _read(fdStdOutPipe[READ_FD], buffer, size);
        buffer[nOutRead] = '\0';
        return nOutRead;*/
    }
}
