# 3DS-PhysGame

## Building

### Dependencies
+ [DevkitPro](https://devkitpro.org/) installed normally
+ Same as [3ds-template](https://github.com/TricksterGuy/3ds-template)

### Steps
1. (Only once) Run `build-chipmunk.bat` to build chipmunk from source!
2. Run `build.bat` or any of the make commands in [3ds-template](https://github.com/TricksterGuy/3ds-template) within the `PhysGame` folder.
3. Check output folder
4. ???
5. Profit

## Known issues

### Why does everything fall when I launch the game after compiling!
Blame devkit, they somehow broke math.h's INFINITE, I'm probably going to find a proper fix later on.

:godmode:
