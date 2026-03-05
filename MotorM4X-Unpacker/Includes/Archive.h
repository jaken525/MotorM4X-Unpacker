#pragma once

#include "zip/zip.h"
#include "BinaryStream.h"

#include <iostream>
#include <cstring>
#include <map>

const enum Mode
{
    none = -1,
    unpack = 0,
    encryptOneFile = 1,
    hashFileName = 2,
    unpackAll = 3,
    packAll = 4,
    resBackups = 5
};

struct zip_t;

class Archive final
{
public:
    Archive() {};
    Archive(const std::string& inputFile = "", const std::string& outputFile = "") :
        m_Input(inputFile), m_Output(outputFile) {
    };
    Archive(Mode mode) { Start(mode); }

    ~Archive() = default;

public:
    void Start(Mode mode)
    {
        switch (mode)
        {
        case unpack:
            Unpack("", "", true);
            break;
        case unpackAll:
            UnpackAll();
            break;
        case packAll:
            PackAll("", "", true);
            break;
        case encryptOneFile:
            EncryptOneFile();
            break;
        case hashFileName:
            ShowHashFile();
            break;
        case resBackups:
            RestoreBackups();
            break;
        default:
            break;
        }
    }
    void UnpackAll(const std::string& inputFile = "", const std::string& outputFile = "");
    void PackAll(const std::string& inputFile = "", const std::string& outputFile = "", const bool createBackups = false);
    void Unpack(const std::string& inputFile = "", const std::string& outputFile = "", const bool getHash = false);
    void EncryptOneFile(const std::string& inputFile = "", const std::string& outputFile = "");
    void ShowHashFile(const std::string& inputFile = "");
    void RestoreBackups(const std::string& inputFile = "");

private:
    void CollectHashFiles(const std::string& path);

private:
    std::string m_Input{ "" };
    std::string m_Output{ "" };
};