#include "Archive.h"
#include "Utils.h"
#include "Decoder.h"
#include "BinaryStream.h"

#include <fstream>

void Archive::Unpack(const std::string& inputFile, const std::string& outputFile)
{
    Archive archive(inputFile, outputFile);
    try
    {
        if (archive.m_InputFile.empty())
            archive.m_InputFile = Utils::OpenFile();
        const auto fileName = Utils::GetFileName(archive.m_InputFile);
        if (archive.m_OutputFile.empty())
            archive.m_OutputFile = Utils::OpenDirectory();
    }
    catch (...)
    {
        return;
    }

    auto zip = zip_open(archive.m_InputFile.c_str(), 0, 'r');
    for (int i = 0, n = zip_entries_total(zip); i < n; ++i)
    {
        zip_entry_openbyindex(zip, i);
        {
            void* buffer = nullptr;
            size_t bufferSize{0};
            zip_entry_read(zip, &buffer, &bufferSize);

            const auto fileName = std::string(zip_entry_name(zip));
            MotorM4X::DecryptGameFile(fileName, reinterpret_cast<uint8_t*>(buffer), bufferSize);

            BinaryStream::write_file(archive.m_OutputFile + "\\" + fileName, reinterpret_cast<char*>(buffer), bufferSize);
        }
        zip_entry_close(zip);
    }
    zip_close(zip);
}

void Archive::EncryptOneFile(const std::string& inputFile, const std::string& outputFile)
{
    const auto file = Utils::OpenFile();
    if (file.empty())
        throw "Choose file to open!";
    
    auto filePath = Utils::GetFilePath(file);
    const auto fileName = Utils::GetFileNameWithExt(file);
    const auto encryptedName = std::string(MotorM4X::EncryptName(fileName.c_str()));
    filePath.append(encryptedName);

    BinaryStream bstream(file);
    MotorM4X::EncryptGameFile(encryptedName, bstream.get_buffer_uint8(), bstream.get_file_size());

    bstream.write_file(filePath);
}
