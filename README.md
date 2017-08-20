# RVGL Companion
 A companion app for RVGL.

## Supported platforms
 * Windows
 * Linux

## What to expect
 * Asset Manager: easy download and install of any RVGL-related content meant to be directly extracted to your RVGL install dir (no removal yet)
 * Re-Volt Race month tracks installer
 * RVGL Launcher
 * Click-to-install and click-to-launch thanks to the rvgl: custom URI scheme

## Prebuilt binaries
### Windows
 * Statically linked (one single portable ±30Mo file, no external dependencies)
 * Ready to be distributed
### Linux
 * Dynamically linked, but limited set of dependencies:
   * [Qt5](https://www.qt.io/download-open-source/)
   * [p7zip](http://p7zip.sourceforge.net/)
 * Distribution method yet to choose

## How to build
 Using Qt Creator with adequate Qt libs: import, build*, run**!
 *Building for Windows requires 7za.exe to be put in win32/<target_arch>/
 **Runing on Linux requires p7zip

## Contributing
 Please do! Everything is welcome, from ideas to implementations

## rvgl: scheme definition
 * `rvgl:run?opt1&opt2=arg&opt3` → runs `rvgl <default options> -opt1 -opt2 arg -opt3`
 * `rvgl:join?IP=<IP>` → runs `rvgl <default options> -lobby <IP>`
 * `rvgl:install_month` → opens the app ready to install month tracks
 * `rvgl:install_asset?URL=<URL>` → opens the app ready to install asset at <URL>
