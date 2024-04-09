#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <lzma.h>

class LZMA
{
public:
    static bool Compress(const std::string& _rInputFileName, const std::string& _rOutputFileName)
    {
        std::ifstream inputFile(_rInputFileName.c_str(), std::ios::binary);
        if (!inputFile)
        {
            std::cerr << "Failed to open file " << _rInputFileName << std::endl;
            return false;
        }

        std::ofstream outputFile(_rOutputFileName, std::ios::binary);
        if (!outputFile)
        {
            std::cerr << "Failed to open file " << _rOutputFileName << std::endl;
            inputFile.close();
            return false;
        }

        lzma_stream stream = LZMA_STREAM_INIT;
        const uint32_t bufferSize = 64 * 1024;
        uint8_t inBuffer[bufferSize];
        uint8_t outBuffer[bufferSize];

        if (lzma_easy_encoder(&stream, LZMA_PRESET_DEFAULT | LZMA_PRESET_EXTREME, LZMA_CHECK_CRC64) != LZMA_OK)
        {
            std::cerr << "Encoder initialization failed." << std::endl;
            inputFile.close();
            outputFile.close();
            return false;
        }

        stream.next_out = outBuffer;
        stream.avail_out = bufferSize;

        while (true)
        {
            inputFile.read(reinterpret_cast<char*>(inBuffer), bufferSize);
            const auto bytesRead =inputFile.gcount();

            stream.next_in = inBuffer;
            stream.avail_in = bytesRead;

            auto action = (inputFile.eof() ? LZMA_FINISH : LZMA_RUN);
            auto ret = lzma_code(&stream, action);
            if (ret != LZMA_OK && ret != LZMA_STREAM_END)
            {
                std::cerr << "Compression failed." << std::endl;
                inputFile.close();
                outputFile.close();
                return false;
            }

            const auto bytesWritten = bufferSize - stream.avail_out;
            if (bytesWritten > 0)
            {
                outputFile.write(reinterpret_cast<char*>(outBuffer), bytesWritten);
            }

            if (ret == LZMA_STREAM_END)
            {
                break;
            }

            stream.next_out = outBuffer;
            stream.avail_out = bufferSize;
        }

        lzma_end(&stream);

        inputFile.close();
        outputFile.close();
        return true;
    }
};