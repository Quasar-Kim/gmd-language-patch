An experimental project to translate text inside game Geometry Dash

Project created using VS 2019 with Windows SDK 10.0

# Directories
## fonts
This directory contains bitmap fonts with Korean glyphs.
They can be applied by copying them to game's resources folder.

## translation
This directory contains json translation files.

## languagePatch
This directory contains codes for dll which will be injected into ```GeometryDash.exe```.

## Todo
- [ ] Multiline support
- [ ] Custom game launcher with dll injection

# Note
Codes in this project has poor quality since I'm very new to C++.

# Licence
 - [Detours](https://github.com/microsoft/Detours/blob/master/LICENSE.md): MIT License
 - [JSON](https://github.com/nlohmann/json/blob/develop/LICENSE.MIT) (JSON for Modern C++): MIT License
