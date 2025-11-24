#pragma once

#include "zip/zip.h"

#include <iostream>
#include <cstring>

const enum Mode
{
    none = -1,
    unpack = 0,
    pack = 1
};

struct zip_t;

class Archive final
{
public:
    Archive() {};
    Archive(const std::string& inputFile = "", const std::string& outputFile = "") :
        m_InputFile(inputFile), m_OutputFile(outputFile) {
    };
    Archive(Mode mode) { Start(mode); }

public:
    static void Start(Mode mode)
    {
        switch (mode)
        {
        case unpack:
            Unpack();
            break;
        case pack:
            EncryptOneFile();
            break;
        default:
            break;
        }
    }
    static void Unpack(const std::string& inputFile = "", const std::string& outputFile = "");
    static void EncryptOneFile(const std::string& inputFile = "", const std::string& outputFile = "");

private:
    std::string m_InputFile{ "" };
    std::string m_OutputFile{ "" };
};