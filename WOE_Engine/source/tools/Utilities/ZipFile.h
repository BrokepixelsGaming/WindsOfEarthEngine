#pragma once
 
#include <stdint.h>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <algorithm>
#include <unordered_map>





class ZipFile
{
    // Define MAX_PATH here if it hasn't been defined already to keep from having to include any OS files.
    // NOTE: This was snagged from WinDef.h
#ifndef MAX_PATH
#define MAX_PATH 260
#endif

//---------------------------------------------------------------------------------------------------------------------
// ZIP file structures. Note these have to be packed.
//---------------------------------------------------------------------------------------------------------------------
#pragma pack(1)
    struct ZipLocalHeader
    {
        static constexpr unsigned int kSignature = 0x04034b50;

        uint32_t sig;
        uint16_t version;
        uint16_t flag;
        uint16_t compression;      // Z_NO_COMPRESSION or Z_DEFLATED
        uint16_t modTime;
        uint16_t modDate;
        uint32_t crc32;
        uint32_t cSize;
        uint32_t ucSize;
        uint16_t fnameLen;         // Filename string follows header.
        uint16_t xtraLen;          // Extra field follows filename.
    };

    struct ZipDirHeader
    {
        static constexpr unsigned int kSignature = 0x06054b50;

        uint32_t sig;
        uint16_t nDisk;
        uint16_t nStartDisk;
        uint16_t nDirEntries;
        uint16_t totalDirEntries;
        uint32_t dirSize;
        uint32_t dirOffset;
        uint16_t cmntLen;
    };

    struct ZipDirFileHeader
    {
        static constexpr unsigned int kSignature = 0x02014b50;

        uint32_t sig;
        uint16_t verMade;
        uint16_t verNeeded;
        uint16_t flag;
        uint16_t compression;      // COMP_xxxx
        uint16_t modTime;
        uint16_t modDate;
        uint32_t crc32;
        uint32_t cSize;            // Compressed size
        uint32_t ucSize;           // Uncompressed size
        uint16_t fnameLen;         // Filename string follows header.
        uint16_t xtraLen;          // Extra field follows filename.
        uint16_t cmntLen;          // Comment field follows extra field.
        uint16_t diskStart;
        uint16_t intAttr;
        uint32_t extAttr;
        uint32_t hdrOffset;

        char* GetName() const { return (char*)(this + 1); }
        char* GetExtra() const { return GetName() + fnameLen; }
        char* GetComment() const { return GetExtra() + xtraLen; }
    };
#pragma pack()


    static constexpr ptrdiff_t kPtrDelta = -(static_cast<ptrdiff_t>(sizeof(ZipDirHeader)));  // sizeof() returns an unsigned result, so we have to cast to a signed one
    const int kBufferSize = 128;

private:
    std::unordered_map<std::string, uint32_t> filenameToIndexMap;
    std::byte* pBuffer = new std::byte[kBufferSize];


    // headers
    ZipDirHeader m_dirHeader;
    std::fstream m_zipFile;
    const ZipDirFileHeader** m_ppDirEntries = nullptr;  // Pointers to the dir entries in pDirData

public:
    ZipFile();
    bool OpenZip(const char* path);
    bool Load(const char* path);
    void CloseZip();

};