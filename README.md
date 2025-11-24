# MotorM4X Unpacker
Unzips the entire mdl/dtf archive with the game's resources.

To get the original file name, you need to create a hash table.

DJBA2 is used to hash file names. In theory, all the tables are in the archive table.dtf. All tables use the xml format, but the original file names have the format .xml2

After decryption, you can open the file via notepad++.

The program has the function "MotorM4X::EncryptName", which returns the hash of the string. You can use it for verification.

Tables that have been found so far:
| Hash | Original Name | Description |
| ------------- | ------------- | ------------- |
| 3384124599 | music.xml2 | All music in the game with its id |
| 3885410554 | _master_game.xml2 | Set up for the entire game with a split by scene. (for example: music for each territory and in the main menu) |
| 2736722086 | sound_table.xml2 | Sounds lol |
| 1574276988 | car_setup.xml2 | Car settings and texture files |

____
To find more tables, you can search through OllyDbg.

__Instruction manual:__
1. Load the game into the program and select the MotorM4X module.
2. Right click -> Search for -> All referenced text strings.
3. In the open window, search for the lines "table" and then containing ".xml". 

Example of strings:
```shell
table
music.xml
```
Final hashing file: __music.xml2__

# Requirements
- zlib

# Special Thanks
- [MotorM4X files decoder](https://aluigi.altervista.org/search.php?src=motorm4x) - by Luigi Auriemma
- [zip library](https://github.com/kuba--/zip) - by kuba--
