# OOPScape

Course project for Object-Oriented Programming (Sofia University "St. Kliment Ohridski", FMI). 
A C++ console game simulating a maze. The player's goal is to reach the exit while avoiding enemies and collecting coins along the way.

## Implemented Extensions
The following additional features have been implemented on top of the base logic:
* **Different hero roles:** 
  * *Wizard* – has a special teleportation ability (jumps over walls at a distance of 2 cells).
  * *Knight* – has a special attack ability (eliminates all adjacent enemies).
* **Different enemy types:** 
  * *SmartEnemy* – chases the hero along the shortest path using the BFS algorithm.
  * *RandomEnemy* – moves in a random valid direction every turn.
* **Sequential level progression:** The game loads a sequence of levels (`level1.txt`, `level2.txt`, etc.), and the player's score is preserved.
* **Additional game mechanics:** Collecting coins (`C`) to increase the final score.
* **Colored console visualization:** Uses the Windows API (`<windows.h>`) to color the different elements on the map.

## Controls
The game is turn-based. On each turn, the player enters one of the following commands (case-insensitive):
* `L` - Move left
* `R` - Move right
* `U` - Move up
* `D` - Move down
* `OOP` - Use the hero's special ability

## Map Legend
* `S` - Hero's starting position (Start)
* `F` - Level exit (Finish/Goal)
* `E` - Enemy
* `C` - Coin
* `*` - Wall (impassable cell)
* ` ` - Empty space (passable cell)

## Run Instructions (Visual Studio 2022)
1. Open `OOPScape.sln` with Visual Studio 2022.
2. Check the working directory settings so the game can locate the level files:
   * Go to **Project** -> **OOPScape Properties...**
   * Select **Debugging** from the left menu.
   * Ensure the **Working Directory** field is set to `$(ProjectDir)`.
3. Compile and run the program without the debugger by pressing `Ctrl + F5`.

## Level Format
Levels are loaded from the `levels/` folder. Every `.txt` file must follow this format:
* The first line contains an integer `N` (the size of the matrix).
* This is followed by `N` lines, each containing exactly `N` valid characters.
* The map must contain exactly one start (`S`) and exactly one exit (`F`).