Grid Adventure Game
Introduction
This is a terminal-based grid adventure game implemented in C++ using the ncurses library. The objective of the game is to move the player around the grid, collect coins, avoid bombs, and reach the door after collecting the key. The player has a limited number of moves based on the selected difficulty level. The game also includes an undo feature, allowing players to reverse their previous movements a limited number of times, depending on the difficulty chosen.

Features:
Grid-based navigation: The player moves on a grid, represented by different entities (walls, player, key, door, coins, and bombs).
Key and door: The player must find the hidden key before accessing the door.
Coins and bombs: Collect coins to increase your score and avoid bombs, which end the game instantly if hit.
Limited moves: The player has a set number of moves based on difficulty and the positions of the key and door.
Undo system: Players can undo their previous moves, with the number of undo actions depending on the difficulty level.
Difficulty levels:
Easy: 10x10 grid with more moves and undo chances.
Medium: 15x15 grid with fewer moves and undos.
Hard: 20x20 grid with very limited moves and undo actions.
Game Rules
Movement: Use the W, A, S, and D keys to move up, left, down, and right respectively.
Key: You need to find the hidden key (K) before you can open the door (D).
Coins: Collect coins (C) to increase your score.
Bombs: Avoid bombs (B); hitting one ends the game instantly.
Undo: Press U to undo your last move, if you have remaining undo actions.
Exit: Press Q to quit the game early.
Win Condition: Reach the door (D) after collecting the key to win the game.
Lose Condition: You lose if you either run out of moves or hit a bomb.

Setup Instructions:
Prerequisites:
Compiler: A C++ compiler (like g++).
ncurses library: This game uses the ncurses library for terminal handling.

Installing ncurses:
On Ubuntu:
sudo apt-get install libncurses5-dev libncursesw5-dev

On macOS, you can install ncurses using Homebrew:
brew install ncurses

Compilation:
To compile the game, run the following command:
g++ game.cpp -o game -lncurses

Running the Game:
After compiling, you can run the game with:
./game

Gameplay Instructions:
Start the game by selecting a difficulty level (Easy, Medium, or Hard).
Move the player using the following controls:
W - Move up
A - Move left
S - Move down
D - Move right
Undo a move by pressing U.
Quit the game at any time by pressing Q.
Collect the key and reach the door to win.
Avoid the bombs and collect coins to increase your score.

Scoring System:
Collecting a coin: +2 points
Remaining moves at the end of the game are added to your final score.

Structure:
Game class: Manages the overall game flow and difficulty selection.
Grid class: Responsible for generating the grid and managing player movement.
Stack class: Used to store player movement history for undo functionality.
List class: Tracks the locations of collected coins.
ncurses: Handles terminal display for the game interface.
