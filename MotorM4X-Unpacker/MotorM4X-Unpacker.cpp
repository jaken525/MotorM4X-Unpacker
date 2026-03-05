#include "Includes/Archive.h"

#define VERSION = "1.0";

void SetMode(const char& pattern, Mode& mode)
{
    if (!pattern)
    {
        mode = Mode::none;
        throw "The pattern is incorrect";
    }

    switch (pattern)
    {
    case 'u':
        mode = Mode::unpack;
        return;
    case 'ua':
        mode = Mode::unpackAll;
        return;
    case 'pa':
        mode = Mode::packAll;
        return;
    case 'eo':
        mode = Mode::encryptOneFile;
        return;
    case 'h':
        mode = Mode::hashFileName;
        return;
    case 'b':
        mode = Mode::resBackups;
        return;
    default:
        mode = Mode::none;
        return;
    }
}

void ShowMenu(Mode& mode)
{
    std::cout << "MotorM4X - Files unpacker/packer by JaKeN\n\n";
    std::cout << "0. Exit\n1. Unpack\n2. Encrypt one file\n3. Hash file name\n4. Unpack All\n5. Pack All\n6. Restore Backups\n\n=> ";
    auto input = -1;
    std::cin >> input;

    if (input <= 0)
    {
        mode = Mode::none;
        return;
    }

    mode = static_cast<Mode>(input - 1);
}

int main(int argc, char* argv[])
{
    std::string inputFile;
    std::string outputFile;
    auto mode = Mode::none;

    while (true)
    {
        switch (argc)
        {
        case 2:
            SetMode((char)argv[1], mode);
            break;
        default:
            ShowMenu(mode);
            break;
        }

        if (mode == none)
            return 0;

        system("cls");
        Archive archive(mode);
    }
}