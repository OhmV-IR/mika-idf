#pragma once
/** A light RAII wrapper over C FILE */
#include <cstdio>

class FileHandle {
    private:
        FILE* rawHandle;
    public:
        FileHandle(FILE* file);
        ~FileHandle();

        bool IsValid() const;

        operator FILE*() const;
        operator bool() const;
};