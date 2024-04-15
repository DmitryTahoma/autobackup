#include "../Include/CLZMAUtil.h"

bool CLZMAUtil::Compress(const std::string &_rInputFileName, const std::string &_rOutputFileName)
{
    std::ifstream cInputFile(_rInputFileName.c_str(), std::ios::binary);
    if (!cInputFile)
    {
        std::string sStr = "Failed to open file " + _rInputFileName;
        CLog::WriteLine(sStr);
        std::cerr << sStr << std::endl;
        return false;
    }

    std::ofstream cOutputFile(_rOutputFileName, std::ios::binary);
    if (!cOutputFile)
    {
        std::string sStr = "Failed to open file " + _rOutputFileName;
        CLog::WriteLine(sStr);
        std::cerr << sStr << std::endl;
        cInputFile.close();
        return false;
    }

    lzma_stream sStream = LZMA_STREAM_INIT;
    if (lzma_easy_encoder(&sStream, LZMA_PRESET_DEFAULT | LZMA_PRESET_EXTREME, LZMA_CHECK_CRC64) != LZMA_OK)
    {
        std::string sStr = "Encoder initialization failed";
        CLog::WriteLine(sStr);
        std::cerr << sStr << std::endl;
        cInputFile.close();
        cOutputFile.close();
        return false;
    }

    const uint32_t iBufferSize = 64 * 1024;
    uint8_t aInBuffer[iBufferSize];
    uint8_t aOutBuffer[iBufferSize];

    lzma_ret eRet = LZMA_OK;
    while (eRet != LZMA_STREAM_END)
    {
        sStream.next_out = aOutBuffer;
        sStream.avail_out = iBufferSize;

        cInputFile.read(reinterpret_cast<char *>(aInBuffer), iBufferSize);
        const std::streamsize iBytesRead = cInputFile.gcount();

        sStream.next_in = aInBuffer;
        sStream.avail_in = iBytesRead;

        lzma_action eAction = (cInputFile.eof() ? LZMA_FINISH : LZMA_RUN);
        eRet = lzma_code(&sStream, eAction);
        if (eRet != LZMA_OK && eRet != LZMA_STREAM_END)
        {
            std::string sStr = "Compression failed. lzma_ret=" + std::to_string((int)eRet);
            CLog::WriteLine(sStr);
            std::cerr << sStr << std::endl;
            cInputFile.close();
            cOutputFile.close();
            return false;
        }

        const unsigned long iBytesWritten = iBufferSize - sStream.avail_out;
        if (iBytesWritten > 0)
        {
            cOutputFile.write(reinterpret_cast<char *>(aOutBuffer), iBytesWritten);
        }
    }

    lzma_end(&sStream);
    cInputFile.close();
    cOutputFile.close();
    return true;
}