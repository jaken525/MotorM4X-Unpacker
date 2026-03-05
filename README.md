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
| 3732462774 | grass.xml2 | Settings for the grass, field, snow and a likes |
| 3379029554 | lensflare.xml2 | Lensflares lol |
| 1472400356 | _material.xml2 | Materials lol |
| 857797314 | humans.xml2 | Settings for humans and their animations |
| 160744625 | wheel_particles.xml2 | It's not really necessary to create a full-fledged table |
| 1742682589 | garage.xml2 | ... |
| 3909575627 | menu.xml2 | ... |
| 1940296839 | ingame.xml2 | ... |
| 1034138367 | menu_pda.xml2 | ... |
| 1107833686 | menu_options.xml2 | ... |
| 2526605317 | questioner.xml2 | ... |
| 396587131 | menu_settings.xml2 | ... |
| 1368379637 | ride_sounds.xml2 | It's not really necessary to create a full-fledged table |
| 3639724233 | slip_sounds.xml2 | It's not really necessary to create a full-fledged table |
| 2746002524 | prices.xml2 | Prices for bets and stars |
| 3287369807 | camera_settings.xml2 | Settings for cameras |
| 954124220 | _particles.xml2 | Particles lol |
| 1318407574 | weather_setup.xml2 | Settings for weather |
| 2731777293 | tyre_names.xml2 | It's not really necessary to create a full-fledged table |
| 3203771124 | skids.xml2 | It's not really necessary to create a full-fledged table |
| 550915094 | tree_setup.xml2 | ... |
| 3691583168 | tape.xml2 | ... |
| 1819162635 | atlas.xml2 | ... |
| 4133041888 | car_tuning.xml2 | ... |

____
To find more tables, you can search through OllyDbg.

All hashes are collected in the [Archive::CollectHashFiles in Archive.cpp](https://github.com/jaken525/MotorM4X-Unpacker/blob/main/MotorM4X-Unpacker/Includes/Archive.cpp#L495)

An example of adding and searching for a file name can be found in the methods used in the function above.

Xml files are usually used, but perhaps the name of the files can be found in other places.

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
