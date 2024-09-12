#include "ZipFile.h"

#include <ZLib/zlib.h>

#include "EngineInterface/EngineSingleton.h"

ZipFile::ZipFile()
{
	memset(pBuffer, 0, kBufferSize);
	memset(&m_dirHeader, 0, sizeof(ZipDirHeader));
}

bool ZipFile::OpenZip(const char* path)
{
    //auto& lua = EngineSingleton::Get()->GetLuaState();
    //////////////////////////////////////////////////
    // Setup
    m_zipFile.open(path, std::ios::in | std::ios::binary);
    if (!m_zipFile.is_open())
    {
       
        //lua.DoString("Failed to open file: ");
        //lua.DoString(path);
        return false;
    }

    // Assuming no extra comment at the end, read the whole end record.

    // seek to the directory header and read it
    m_zipFile.seekg(kPtrDelta, std::ios::end);
    const auto dirHeaderOffset = m_zipFile.tellg();
    m_zipFile.read(reinterpret_cast<char*>(&m_dirHeader), sizeof(ZipDirHeader));
    if (m_dirHeader.sig != ZipDirHeader::kSignature)
    {
        //lua.DoString("Corrupted file: ");
        //lua.DoString(path);
        return false;
    }

    // Go to the beginning of the directory
    m_zipFile.seekg((ptrdiff_t)dirHeaderOffset - m_dirHeader.dirSize, std::ios::beg);

    // Allocate the data buffer, and read the whole thing.
    const size_t allocSize = m_dirHeader.dirSize + m_dirHeader.nDirEntries * sizeof(*m_ppDirEntries);
    char* pDirData = new char[allocSize];
    memset(pDirData, 0, allocSize);
    m_zipFile.read(reinterpret_cast<char*>(pDirData), m_dirHeader.dirSize);

    // process each entry
    char* pFileHeaders = pDirData;
    m_ppDirEntries = reinterpret_cast<const ZipDirFileHeader**>(pDirData + m_dirHeader.dirSize);

    for (int i = 0; i < m_dirHeader.nDirEntries; ++i)
    {
        ZipDirFileHeader& fileHeader = *(ZipDirFileHeader*)pFileHeaders;

        // Store the address of the nth file for quicker access
        m_ppDirEntries[i] = &fileHeader;

        if (fileHeader.sig == ZipDirFileHeader::kSignature)
        {
            pFileHeaders += sizeof(fileHeader);

            // Convert UNIX slashes to DOS backslashes
            for (int ii = 0; ii < fileHeader.fnameLen; ++ii)
            {
                if (pFileHeaders[ii] == '/')
                    pFileHeaders[ii] = '\\';  // TODO: Windows only?
            }

            char filename[MAX_PATH];
            memcpy(filename, pFileHeaders, fileHeader.fnameLen);
            filename[fileHeader.fnameLen] = '\0';  // add the terminating zero
            std::string pathString = filename;  // TODO: do we need std::string here?
            std::cout << "Path: " << pathString << "\n";

            // add to our map
            filenameToIndexMap.emplace(pathString, i);

            // Skip name, extra and comment fields.
            pFileHeaders += fileHeader.fnameLen + fileHeader.xtraLen + fileHeader.cmntLen;
        }
        else  // invalid header
        {
            /*lua.DoString("invalid file within zip: ");*/
            return false;
        }
    }
    return true;
}

bool ZipFile::Load(const char* path)
{
    //////////////////////////////////////////////////
    // Loading - do this as necessary
    //auto& lua = EngineSingleton::Get()->GetLuaState();
    auto findIt = filenameToIndexMap.find(path);
    if (findIt != filenameToIndexMap.end())
    {
        // Go to the actual file and read the local header.
        ZipLocalHeader localHeader;
        memset(&localHeader, 0, sizeof(localHeader));
        m_zipFile.seekg(m_ppDirEntries[findIt->second]->hdrOffset, std::ios::beg);
        m_zipFile.read(reinterpret_cast<char*>(&localHeader), sizeof(ZipLocalHeader));
        if (localHeader.sig != ZipLocalHeader::kSignature)
        {
            //lua.DoString("Corrupted local header\n");
            return false;
        }

        // Skip the extra fields
        m_zipFile.seekg(localHeader.fnameLen + localHeader.xtraLen, std::ios::cur);

        // uncompressed file
        if (localHeader.compression == Z_NO_COMPRESSION)
        {
            // Simple read in the raw stored data
            m_zipFile.read(reinterpret_cast<char*>(pBuffer), localHeader.cSize);
        }

        // compressed file
        else if (localHeader.compression == Z_DEFLATED)
        {
            // Alloc compressed data buffer and read the whole stream
            char* pRawData = new char[localHeader.cSize];
            memset(pRawData, 0, localHeader.cSize);
            m_zipFile.read(pRawData, localHeader.cSize);

            // Setup the inflate stream.
            z_stream stream;

            stream.next_in = (Bytef*)pRawData;
            stream.avail_in = (uInt)localHeader.cSize;
            stream.next_out = (Bytef*)pBuffer;
            stream.avail_out = localHeader.ucSize;
            stream.zalloc = (alloc_func)0;
            stream.zfree = (free_func)0;

            // Perform inflation. wbits < 0 indicates no zlib header inside the data.
            int err = inflateInit2(&stream, -MAX_WBITS);
            if (err == Z_OK)
            {
                err = inflate(&stream, Z_FINISH);
                inflateEnd(&stream);
                if (err == Z_STREAM_END)
                    err = Z_OK;
                inflateEnd(&stream);
            }
            if (err != Z_OK)
            {
                //lua.DoString("Error inflating zip file\n");
                return false;
            }

            delete[] pRawData;
        }

        // unknown compression
        else
        {
           //lua.DoString("Unknown compression in zip file\n");
            return false;
        }
    }
    else
    {
        //lua.DoString("Couldn't find file: ");
        //lua.DoString(path);
        return false;
    }
    return true;
}

void ZipFile::CloseZip()
{

    //////////////////////////////////////////////////
    // Cleanup

    delete[] pBuffer;
    //~ZipFile();
}
