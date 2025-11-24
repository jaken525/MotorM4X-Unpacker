#pragma once

/*
    Copyright 2008,2009 Luigi Auriemma

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

    http://www.gnu.org/licenses/gpl-2.0.txt

    Only Header Library 2025 by JaKeN
*/

#include <iostream>
#include <cstring>
#include <algorithm>
#include <filesystem>

namespace MotorM4X
{
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;

    void DecryptGameFile(const std::string& fileName, u8* data, int dataSize)
    {
        for (int i = 0, j = 0, nameLength = fileName.length(); i < dataSize; i++)
        {
            if (!j)
                j = nameLength;

            data[i] += (0xd6 - fileName[--j]);
        }
    }

    void EncryptGameFile(const std::string& fileName, u8* data, int dataSize)
    {
        for (int i = 0, j = 0, nameLength = fileName.length(); i < dataSize; i++)
        {
            if (!j)
                j = nameLength;

            data[i] -= (0xd6 - fileName[--j]);
        }
    }

    // Decompiled code with IDA
    char byte_6126D0[256];
    char byte_612690[256];

    char String[256];

    int Hash(char* a1)
    {
        int v1; // edx
        char v2; // cl
        int v3; // ecx
        char* v4; // edx
        int result; // eax
        int v6; // esi

        if (!a1 || !*a1)
            return 0;
        v1 = String - a1;
        do
        {
            v2 = *a1;
            a1[v1] = *a1;
            ++a1;
        } while (v2);
        _strlwr(String);
        v3 = String[0];
        v4 = String;
        for (result = 5381; *v4; result += v6)
        {
            ++v4;
            v6 = v3 + 32 * result;
            v3 = *v4;
        }
        return result;
    }

    char* EncryptName(const char* a1)
    {
        char v1; // bl
        char* v2; // eax
        char* v3; // ebp
        char* v4; // eax
        int v6; // eax

        strcpy(byte_6126D0, a1);
        _strlwr(byte_6126D0);
        v1 = 0;
        v2 = strrchr(byte_6126D0, 46);
        v3 = v2;
        if (v2
            && (!strcmp(v2 + 1, "program")
                || !strcmp(v2 + 1, "material")
                || !strcmp(v2 + 1, "compositor")
                || !strcmp(v2 + 1, "particle")
                || !strcmp(v2 + 1, "overlay")
                || !strcmp(v2 + 1, "fontdef")))
        {
            v4 = byte_6126D0;
            if (v3 == byte_6126D0)
            {
            LABEL_12:
                strcpy(byte_612690, a1);
                return byte_612690;
            }
            while (*v4 >= 48 && *v4 <= 57)
            {
                if (++v4 == v3)
                    goto LABEL_12;
            }
            *v3 = 0;
            v1 = 1;
            ++v3;
        }
        v6 = Hash(byte_6126D0);
        sprintf(byte_612690, "%u", v6);
        if (v1)
        {
            strcat(byte_612690, ".");
            strcat(byte_612690, v3);
        }
        return byte_612690;
    }
}