#pragma once
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

class VirtualFileSystem
{
  public:
    struct Header
    {
        char id[4] = {"VFS"};
        char packageName[100];
        uint32_t numEntries = 0;
    };

    struct Entry
    {
        bool compressed = false; // TODO: compression not supported yet
        uint32_t uncompressedSize = 0;
        uint32_t compressedSize = 0; // TODO: same as uncompressedSize for now until compression is implemented
        uint32_t offset = 0;         // byte offset in binary blob
    };

  public:
    VirtualFileSystem(std::string path) : mPath(path)
    {
    }

    void CreatePhysicalFile(bool overwrite = false);
    void WriteData(std::vector<char> data);

    void WriteEntry(Entry& e, std::vector<char>& data, int32_t eIdx);

    template <typename T> T ReadData(uint32_t size, uint32_t offset);

    template <typename T> T ReadData();

    std::unordered_map<std::string, Entry> mEntries = {};
  private:
    void WriteHeader();

  private:
    std::string mPath = "default.dat";
    std::fstream mPhysicalFile;

    bool mDirty = true;
    std::string mPackageName = "default";

};
