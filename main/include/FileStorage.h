#pragma once
#include <string>
#include "FileHandle.h"

class FileStorage {
    public:
    virtual FileHandle OpenFile(const char* path, const char* mode) = 0;
};