#include "FileHandle.h"

FileHandle::FileHandle(FILE* raw) : rawHandle(raw){

}

FileHandle::~FileHandle() {
    fclose(rawHandle);
}

FileHandle::operator FILE*() const {
    return rawHandle;
}

bool FileHandle::IsValid() const {
    return rawHandle != NULL;
}

FileHandle::operator bool() const {
    return IsValid();
}