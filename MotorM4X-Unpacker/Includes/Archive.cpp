#include "Archive.h"
#include "Utils.h"
#include "Decoder.h"
#include "pugixml/pugixml.hpp"

#include <fstream>
#include <regex>
#include <cctype>

static std::map<std::string, std::string> m_HashTable = {
        {"3384124599", "music.xml"},
        {"3885410554", "_master_game.xml"},
        {"2736722086", "sound_table.xml"},
        {"1574276988", "car_setup.xml"},
        {"3732462774", "grass.xml"},
        {"3379029554", "lensflare.xml"},
        {"1472400356", "_material.xml"},
        {"857797314", "humans.xml"},
        {"160744625", "wheel_particles.xml"},
        {"1742682589", "garage.xml"},
        {"3909575627", "menu.xml"},
        {"1940296839", "ingame.xml"},
        {"1034138367", "menu_pda.xml"},
        {"1107833686", "menu_options.xml"},
        {"2526605317", "questioner.xml"},
        {"396587131", "menu_settings.xml"},
        {"1368379637", "ride_sounds.xml"},
        {"3639724233", "slip_sounds.xml"},
        {"2746002524", "prices.xml"},
        {"3287369807", "camera_settings.xml"},
        {"954124220", "_particles.xml"},
        {"1318407574", "weather_setup.xml"},
        {"2731777293", "tyre_names.xml"},
        {"3203771124", "skids.xml"},
        {"550915094", "tree_setup.xml"},
        {"3691583168", "tape.xml"},
        {"1819162635", "atlas.xml"},
        {"4133041888", "car_tuning.xml"}
};

namespace fs = std::filesystem;

void Archive::UnpackAll(const std::string& inputFile, const std::string& outputFile)
{
    m_Input = inputFile;
    m_Output = outputFile;
    try
    {
        if (m_Input.empty())
            m_Input = Utils::OpenDirectory("Select MotorM4X media Folder");
        if (m_Output.empty())
            m_Output = Utils::OpenDirectory();
    }
    catch (...)
    {
        return;
    }

    std::cout << "Unpacking table.dtf archive..." << std::endl;
    Archive archive("", "");
    archive.Unpack(m_Input + "\\table.dtf", m_Output, true);

    if (fs::exists(m_Input) && fs::is_directory(m_Input))
    {
        for (const auto& entry : fs::directory_iterator(m_Input))
            if (fs::is_regular_file(entry))
            {
                const auto fileName = entry.path().filename().string();
                if (fileName == "table.dtf")
                    continue;

                auto extention = entry.path().extension().string();
                std::transform(extention.begin(), extention.end(), extention.begin(),
                    [](unsigned char c) { return std::tolower(c); });
                if (extention == ".mdl" || extention == ".dtf")
                {
                    std::cout << "Unpacking " + fileName + " archive..." << std::endl;

                    archive = Archive("", "");
                    archive.Unpack(m_Input + "\\" + fileName, m_Output, false);
                }
            }
    }
}

void Archive::PackAll(const std::string& inputFile, const std::string& outputFile, const bool createBackups)
{
    std::cout << "You can pack in the zip archive manually. This method only for the archives after Unpack All" << std::endl;

    m_Input = inputFile;
    m_Output = outputFile;
    try
    {
        if (m_Input.empty())
            m_Input = Utils::OpenDirectory("Select Folder with Unpacked Archives");
        if (m_Output.empty())
            m_Output = Utils::OpenDirectory("Select MotorM4X media Folder");
    }
    catch (...)
    {
        return;
    }

    if (!(fs::exists(m_Input) && fs::is_directory(m_Input)))
        return;

    for (const auto& entry : fs::directory_iterator(m_Input))
    {
        if (!fs::is_directory(entry))
            continue;

        const auto folderName = entry.path().filename().string();
        const auto archiveOutput = m_Output + "\\" + folderName;

        if (createBackups && fs::exists(archiveOutput))
            fs::copy_file(archiveOutput, archiveOutput + ".bak", fs::copy_options::overwrite_existing);

        auto zip = zip_open(archiveOutput.c_str(), 0, 'w');
        std::cout << "Packing the " << folderName << " archive" << std::endl;
        for (const auto& file : fs::directory_iterator(fs::path(m_Input + "\\" + folderName)))
        {
            if (!fs::is_regular_file(file))
                continue;

            auto fileName = file.path().filename().string();
            if (BinaryStream::GetFileExtention(fileName) == ".xml")
                fileName.append("2");
            fileName = Utils::IsDigits(fileName) ? fileName : MotorM4X::EncryptName(fileName.c_str());

            std::cout << "Writting " << fileName << " file" << std::endl;

            BinaryStream bstream(file.path().string());
            MotorM4X::EncryptGameFile(BinaryStream::GetFileName(fileName), bstream.GetBufferUInt8(), bstream.GetSize());

            zip_entry_open(zip, fileName.c_str());
            zip_entry_write(zip, bstream.GetBuffer(), bstream.GetSize());
            zip_entry_close(zip);
        }
        zip_close(zip);
    }
}

void Archive::Unpack(const std::string& inputFile, const std::string& outputFile, const bool getHash)
{
    m_Input = inputFile;
    m_Output = outputFile;
    try
    {
        if (m_Input.empty())
            m_Input = Utils::OpenFile();
        if (m_Output.empty())
            m_Output = Utils::OpenDirectory();
    }
    catch (...)
    {
        return;
    }

    {
        m_Output.append("\\" + BinaryStream::GetFileName(m_Input) + BinaryStream::GetFileExtention(m_Input));
        if (!fs::exists(m_Output))
            fs::create_directory(m_Output);
    }

    if (getHash)
        CollectHashFiles(BinaryStream::GetFilePath(m_Input));

    auto zip = zip_open(m_Input.c_str(), 0, 'r');
    if (!zip)
        return;
    for (int i = 0, n = zip_entries_total(zip); i < n; ++i)
    {
        zip_entry_openbyindex(zip, i);
        {
            void* buffer = nullptr;
            size_t bufferSize{0};
            zip_entry_read(zip, &buffer, &bufferSize);

            auto zipFileName = std::string(zip_entry_name(zip));
            const auto zipFileExt = BinaryStream::GetFileExtention(zipFileName);
            zipFileName = BinaryStream::GetFileName(zipFileName);
            const auto fileName = m_HashTable.contains(zipFileName) ? m_HashTable[zipFileName] + zipFileExt : zipFileName + zipFileExt;
            MotorM4X::DecryptGameFile(zipFileName, reinterpret_cast<uint8_t*>(buffer), bufferSize);

            BinaryStream::WriteFile(m_Output + "\\" + fileName, reinterpret_cast<char*>(buffer), bufferSize);

            delete[] buffer;
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
    
    auto filePath = BinaryStream::GetFilePath(file);
    const auto fileName = BinaryStream::GetFileNameWithExt(file);
    const auto encryptedName = std::string(MotorM4X::EncryptName(fileName.c_str()));
    filePath.append(encryptedName);

    BinaryStream bstream(file);
    MotorM4X::EncryptGameFile(BinaryStream::GetFileName(encryptedName), bstream.GetBufferUInt8(), bstream.GetSize());

    bstream.WriteFile(filePath);
    bstream.Clear();
}

void Archive::ShowHashFile(const std::string& inputFile)
{
    auto file{ inputFile };
    if (file.empty())
    {
        std::cout << "Type file name: ";
        std::cin >> file;
    }

    std::cout << MotorM4X::EncryptName(file.c_str()) << std::endl;
}

void Archive::RestoreBackups(const std::string& inputFile)
{
    m_Input = inputFile;
    try
    {
        if (m_Input.empty())
            m_Input = Utils::OpenDirectory("Select MotorM4X media Folder");
    }
    catch (...)
    {
        return;
    }

    std::cout << "Searching for backup archives..." << std::endl;

    for (const auto& entry : fs::directory_iterator(m_Input))
    {
        if (fs::is_directory(entry))
            continue;

        const auto fileName = entry.path().filename().string();
        if (BinaryStream::GetFileExtention(fileName) != ".bak")
            continue;

        const auto originalName = BinaryStream::GetFileName(fileName);
        const auto originalFullPath = m_Input + "\\" + originalName;
        const auto backupFullPath = m_Input + "\\" + fileName;
        if (fs::exists(originalFullPath))
        {
            std::cout << "Deleting " << originalFullPath << std::endl;
            fs::remove(originalFullPath);
        }

        std::cout << "Restoring archive " << fileName << std::endl;
        fs::rename(backupFullPath, originalFullPath);
    }
}

namespace
{
    void CheckMusic(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("xml").first_child(); child; child = child.next_sibling())
            for (auto attr = child.first_attribute(); attr; attr = attr.next_attribute())
            {
                if (std::string(attr.name()) != "file")
                    continue;

                const auto value = std::regex_replace(std::string(attr.value()), std::regex("&apos;"), "'");
                if (!m_HashTable.contains(value))
                    m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
            }

        delete data;
    }

    void CheckSounds(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("sound_table").first_child(); child; child = child.next_sibling())
        {
            for (auto category = child.first_child(); category; category = category.next_sibling())
                for (auto attr = category.first_attribute(); attr; attr = attr.next_attribute())
                {
                    if (std::string(attr.name()) != "filename")
                        continue;

                    const auto value = std::string(attr.value()) + ".wav";
                    if (!m_HashTable.contains(value))
                        m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
                }
        }
    }

    void CheckCarSetup(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("xml").first_child(); child; child = child.next_sibling())
        {
            if (std::string(child.name()) != "model")
                continue;

            for (auto category = child.first_child(); category; category = category.next_sibling())
            {
                if (std::string(category.name()) != "skin")
                    continue;

                for (auto attr = category.first_attribute(); attr; attr = attr.next_attribute())
                {
                    if (std::string(attr.name()) != "texture")
                        continue;

                    const auto value = std::string(attr.value());
                    if (!m_HashTable.contains(value))
                        m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
                }
            }
        }
    }

    void CheckGrass(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("grass_desc").first_child(); child; child = child.next_sibling())
            for (auto attr = child.first_attribute(); attr; attr = attr.next_attribute())
            {
                if (std::string(attr.name()) != "texture")
                    continue;

                const auto value = std::string(attr.value());
                if (!m_HashTable.contains(value))
                    m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
            }

        delete data;
    }

    void CheckLensFlares(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("atlas").first_child(); child; child = child.next_sibling())
            for (auto attr = child.first_attribute(); attr; attr = attr.next_attribute())
            {
                if (std::string(attr.name()) != "texture_name")
                    continue;

                const auto value = std::string(attr.value());
                if (!m_HashTable.contains(value))
                    m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
            }

        delete data;
    }

    void CheckMaterials(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("materials").first_child(); child; child = child.next_sibling())
        {
            if (std::string(child.name()) != "texture")

            for (auto attr = child.first_attribute(); attr; attr = attr.next_attribute())
            {
                if (std::string(attr.name()) == "name")
                {
                    const auto value = std::string(attr.value()) + ".dds";
                    if (!m_HashTable.contains(value))
                        m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
                    break;
                }
            }
        }

        delete data;
    }

    void CheckGarage(BinaryStream* data)
    {
        if (!data)
            return;

        pugi::xml_document doc;
        const auto xml = doc.load_buffer(
            reinterpret_cast<void*>(data->GetBuffer()),
            data->GetSize(),
            pugi::parse_eol);
        if (xml.status != pugi::status_ok)
        {
            delete data;
            return;
        }

        for (auto child = doc.child("materials").first_child(); child; child = child.next_sibling())
        {
            if (std::string(child.name()) != "texture")

                for (auto attr = child.first_attribute(); attr; attr = attr.next_attribute())
                {
                    if (std::string(attr.name()) == "name")
                    {
                        const auto value = std::string(attr.value()) + ".dds";
                        if (!m_HashTable.contains(value))
                            m_HashTable.emplace(MotorM4X::EncryptName(value.c_str()), value);
                        break;
                    }
                }
        }

        delete data;
    }
}

void Archive::CollectHashFiles(const std::string& path)
{

    auto zip = zip_open((path + "table.dtf").c_str(), 0, 'r');
    if (!zip)
        return;

    const auto GetFileFromZip = [zip](const std::string& fileName)
    {
        BinaryStream* result = nullptr;

        zip_entry_open(zip, fileName.c_str());
        {
            void* buffer;
            size_t bufferSize;

            zip_entry_read(zip, &buffer, &bufferSize);
            MotorM4X::DecryptGameFile(fileName, reinterpret_cast<uint8_t*>(buffer), bufferSize);
            result = new BinaryStream(reinterpret_cast<char*>(buffer), bufferSize, 0);
        }
        zip_entry_close(zip);

        return std::move(result);
    };

    CheckMusic(GetFileFromZip("3384124599"));
    CheckSounds(GetFileFromZip("2736722086"));
    CheckCarSetup(GetFileFromZip("1574276988"));
    CheckGrass(GetFileFromZip("3732462774"));
    CheckLensFlares(GetFileFromZip("3379029554"));
    CheckMaterials(GetFileFromZip("1472400356"));
    CheckGarage(GetFileFromZip("1742682589"));
}