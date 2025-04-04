# PS4: Sokoban

## Contact
Name: Ansh Patel
Section: 202
Time to Complete: ~10 Hours


## Description
Explain what the project does.

This project is an implementation of the classic Sokoban game using C++ and SFML for graphics rendering. The game reads level data from a file, renders the board, and allows the player to move a character to push boxes onto storage locations. The project includes fundamental object-oriented programming concepts, operator overloading, file I/O, and graphical rendering.

### Features
Describe what your major decisions were and why you did things that way.

The project was designed with an object-oriented approach. The core game logic is covered within the SB::Sokoban class, which manages the board state, player movement, and rendering. Operator overloading is used for reading and printing of the game board, allowing levels to be easily saved and loaded using the << and >> operators. For graphics rendering, SFML was chosen to provide a lightweight yet powerful framework to visualize the game. The game board is displayed using textures and sprites, making the experience visually appealing. A key design decision was the separation of concerns between game logic and rendering. Also using <algorithm> (count_if) made thing much easier for me.

#### Part a

Level data is loaded from an external file and stored in memory for easy access. The board is represented using a 1D vector, which stores the grid in row-major order, allowing efficient iteration and direct indexing. SFML sprites are used to render the different game elements such as walls, boxes, players, and storage locations. The rendering process updates the screen in real-time based on the player's movements, ensuring smooth gameplay.

#### Part b

In this part, I implemented the movePlayer() and isWon() functionality. The movePlayer function is responsible for handling player movement, ensuring that the player can navigate the level while obeying Sokoban's rules. It first checks whether the next position is walkable and, if a box is present, verifies whether it can be pushed without violating constraints. The function updates the board accordingly and maintains a history of moves to allow undo and redo functionality. The isWon() function determines if the level is completed by checking whether all storage locations are occupied by boxes. It iterates through the board, counting the number of storage locations with boxes, and returns true if all are correctly placed. The undo() function reverts the last move, restoring the previous board state, while redo() reapplies an undone move, allowing the player to navigate between past actions. The reset() function resets the game to its original state by reloading the level from the stored initial configuration, clearing all moves and restoring the board to its starting position.

### Memory
Describe how you decided to store the level data including whether you used smart pointers.

To store level data, a std::vector<char> is used, where each character represents a specific tile type, such as walls (#), boxes (A), and storage locations (.). Instead of using a 2D array, a 1D vector with row-major indexing (board[x + y * width]) was chosen for simplicity and flexibility. This approach also makes serialization easier since the level data can be written directly to an output stream. Smart pointers were not used for managing the level grid since std::vector automatically handles memory allocation and deallocation. However, textures are stored as member variables inside the Sokoban class to avoid redundant loading operations and ensure efficient resource management.

### Lambdas
Describe what <algorithm> functions you used and what lambda expressions you wrote.

For part a, there was no use of <algorithm> or lambda expressions. Part a, was a very basic reading and rendering of the map based on the given level file. For part b, specifically for isWon() I used std::count_if to get rid of long loops, which made my code much easier to understand and much better.

### Issues
What did you have trouble with?  What did you learn?  What doesn't work?  Be honest.  You might be penalized if you claim something works and it doesn't.

I mostly had trouble with rendering the ground texture below the player texture. A black tile was rendered below the player and crate texture. But after some tweaking I got it working. Mostly I learned about row and column major order. So far the program works as intended to. I faced issues with the player moving onto the storage unit and during this the storage unit changing its location. Once I fixed it, my isWon() was glitching, it considered the player moving into a storage unit as a crate too but after some work, I got it all working.

### Extra Credit
Anything special you did.  This is required to earn bonus points.

To enhance the gameplay experience, a move counter has been implemented to track the number of moves the player takes to solve the puzzle. Every time the player moves, the counter increments and displays the updated count on the screen. This feature allows players to challenge themselves to complete levels in the fewest moves possible. I have also implemented a undo feature that lets the user reverse the game to the previous move. I also have a redo function that lets the user reverse the action that was previously undone. The game also outputs a victory audio and text when the player wins and is successfull in placing all the crates into the storage units.

## Acknowledgements
List all sources of help including the instructor or TAs, classmates, and web pages.
If you used images or other resources than the ones provided, list them here.

Kenney Sokoban Pack (CC0): https://kenney.nl/assets/sokoban
Matrices: https://en.wikipedia.org/wiki/Row-_and_column-major_order
Matrices: https://www.geeksforgeeks.org/row-major-order-and-column-major-order/
Victory Sound: https://freesound.org
