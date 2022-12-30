#include "VirtualFileSystem.h"
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <ios>
#include "Debug.h"

void VirtualFileSystem::CreatePhysicalFile(bool overwrite)
{
    bool fileExists = std::filesystem::exists(mPath);

    if (fileExists && !overwrite)
    {
        return;
    }

    mPhysicalFile.open(mPath.c_str(), std::ios::out | std::ios::binary);

    mPhysicalFile.close();
    return;
}

void VirtualFileSystem::WriteData(std::vector<char> data)
{
    Header h;
    strncpy(h.id, "VFS", sizeof(h.id) - 1);
    strncpy(h.packageName, "TestPack", sizeof(h.packageName) - 1);
    h.numEntries = 8;

    mPhysicalFile.open(mPath, std::ios::in | std::ios::out | std::ios::binary);
    // write header
    mPhysicalFile.write((char *)&h, sizeof(Header));

	std::vector<char> emptyData;
	emptyData.resize(100000);
	std::fill(emptyData.begin(), emptyData.end(), 0);
	mPhysicalFile.write(emptyData.data(), emptyData.size() * sizeof(char));

    // write entries
    for (int i = 0; i < h.numEntries; i++)
    {
        Entry e;
        e.compressed = false;
        e.uncompressedSize = 8392;
        e.compressedSize = 10;
        e.offset = sizeof(Header) + (sizeof(Entry) * i);

		mEntries[std::to_string(i)] = e;
		mPhysicalFile.seekg(e.offset, std::ios::beg);
        mPhysicalFile.write((char *)&e, sizeof(Entry));
    }

    mPhysicalFile.close();
}

void VirtualFileSystem::WriteEntry(VirtualFileSystem::Entry& e, std::vector<char>& data, int32_t eIdx)
{
	Entry e1 = mEntries[std::to_string(eIdx)];
	e1.uncompressedSize = data.size() * sizeof(char);
	
	Debug::CoreLog("offset: {} size: {}", e1.offset, e1.uncompressedSize);
	
    mPhysicalFile.open(mPath, std::ios::in | std::ios::out | std::ios::binary);

	std::fill(data.begin(), data.end(), eIdx);
	
	mPhysicalFile.seekp(e1.offset * 9, std::ios::beg);
    mPhysicalFile.write(data.data(), e1.uncompressedSize);
    mPhysicalFile.close();
}
