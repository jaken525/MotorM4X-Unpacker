#include <fstream>

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
    case 'p':
        mode = Mode::pack;
        return;
    default:
        mode = Mode::none;
        return;
    }
}

void ShowMenu(Mode& mode)
{
    std::cout << "1. Unpack\n2. Encrypt one file\n\n=> ";
    auto input = -1;
    std::cin >> input;

    mode = static_cast<Mode>(input - 1);
}

int main(int argc, char* argv[])
{
    std::cout << "MotorM4X - Files unpacker/packer by JaKeN\n\n";

    std::string inputFile;
    std::string outputFile;
    auto mode = Mode::none;

    switch (argc)
    {
    case 2:
        SetMode((char)argv[1], mode);
        break;
    default:
        ShowMenu(mode);
        break;
    }

    Archive::Start(mode);
}