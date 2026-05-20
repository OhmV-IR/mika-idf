#include "FileStorage.h"

class SpiffsFS : public FileStorage {
    private:
    SpiffsFS();
    public:
    FileHandle OpenFile(const char* path, const char* mode);

    static SpiffsFS& Get();
};