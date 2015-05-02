# Readme Documentation 

## Inline

For now the documentation is only inline: if you have to code something tricky, please explain **why** you're doing it (only a description is useless). And remember that the best way too have a maintainable code is to have a simple code !

## Doxygen

To generate the documentation, install `Doxygen` then go in the `doc` folder

    cd doc

and generate the documentation

    doxygen doxygen.conf

# Deployment and installer

## Windows

* Compile with Qt in release mode in the folder build-win
* go in this folder and execute from the command line: `windeployqt.exe Macaw-Movies.exe`
* add the dll `libgcc_s_dw2-1.dll` `libstdc++-6.dll` `libwinpthread-1.dll` to the `build-win` folder.
* Create the installer
 * *if you know what you are doing,* modify the concerned lines in `NSIS_Installer\Macaw-Movies.nsi`. **Don't forget the uninstall part!!**
 * *else* download http://nsis.sourceforge.net/NSIS_Quick_Setup_Script_Generator do the configuration. Then go in the file and change all the absolute links by relative links (follow the current `NSIS_Installer\Macaw-Movies.nsi`)
 
## MacOSX

* Compile with Qt in release mode in the folder build-osx
* go in this folder and execute from the command line: `macdeployqt Macaw-Movies.app -dmg`

## Linux

* We leave the sources for now to be compiled by the users.
