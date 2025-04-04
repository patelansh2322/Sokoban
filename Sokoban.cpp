// Copyright [2025] Ansh Patel

#include "Sokoban.hpp"
#include <fstream>

// Default Constructor
SB::Sokoban::Sokoban() : h(0), w(0) {}

// Constructor to load the required images
SB::Sokoban::Sokoban(const std::string& levelFile) : h(0), w(0) {
    if (!playerTexture.loadFromFile("player_05.png")) {
        std::cerr << "Error loading player texture\n";
    }
    if (!wallTexture.loadFromFile("block_06.png")) {
        std::cerr << "Error loading wall texture\n";
    }
    if (!emptyTexture.loadFromFile("ground_01.png")) {
        std::cerr << "Error loading empty texture\n";
    }
    if (!boxTexture.loadFromFile("crate_03.png")) {
        std::cerr << "Error loading box texture\n";
    }
    if (!storageTexture.loadFromFile("ground_04.png")) {
        std::cerr << "Error loading storage texture\n";
    }

    std::ifstream level(levelFile);
    if (level) {
        level >> *this;
    }
}

// Get height
unsigned int SB::Sokoban::height() const { return h; }

// Get width
unsigned int SB::Sokoban::width() const { return w; }

// Get the player location
sf::Vector2u SB::Sokoban::playerLoc() const { return playerPos; }

// Get the the key pressed count
unsigned int SB::Sokoban::count() { return moveCount; }

void SB::Sokoban::undo() {
    if (!undoStack.empty()) {
        redoStack.push({board, playerPos});
        board = undoStack.top().first;
        playerPos = undoStack.top().second;
        undoStack.pop();
    }
}

void SB::Sokoban::redo() {
    if (!redoStack.empty()) {
        undoStack.push({board, playerPos});
        board = redoStack.top().first;
        playerPos = redoStack.top().second;
        redoStack.pop();
    }
}

// Draw the game on the window
void SB::Sokoban::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    sf::Sprite sprite;
    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            sprite.setPosition(x * TILE_SIZE, y * TILE_SIZE);
            char cell = board[x + y * w];

            // Draw base tile first (ground or storage)
            if (cell == '@' || cell == 'A' || cell == '.' || cell == '+') {
                sprite.setTexture(emptyTexture);
                target.draw(sprite, states);
            } else if (cell == 'a' || cell == '1' || cell == '+') {
                sprite.setTexture(storageTexture);
                target.draw(sprite, states);
            } else if (cell == '#') {
                sprite.setTexture(wallTexture);
                target.draw(sprite, states);
            }

            // Draw objects on top of the base tile
            switch (cell) {
                case '@':
                case '+':  // Player on storage
                    sprite.setTexture(playerTexture);
                    target.draw(sprite, states);
                    break;
                case 'A':
                case '1':  // Crate on storage
                    sprite.setTexture(boxTexture);
                    target.draw(sprite, states);
                    break;
            }
        }
    }
}

// Move the player
void SB::Sokoban::movePlayer(Direction dir) {
    // Save current state for undo
    undoStack.push({board, playerPos});

    int dx = 0, dy = 0;
    switch (dir) {
        case Direction::Up: dy = -1; break;
        case Direction::Down: dy = 1; break;
        case Direction::Left: dx = -1; break;
        case Direction::Right: dx = 1; break;
    }
    unsigned int newX = playerPos.x + dx;
    unsigned int newY = playerPos.y + dy;
    if (newX >= w || newY >= h) return;

    char& target = board[newX + newY * w];
    char& current = board[playerPos.x + playerPos.y * w];

    bool validMove = false;

    // Moving to empty space or storage
    if (target == '.' || target == 'a') {
        // Mark current position as empty or storage
        if (current == '@') {
            current = '.';
        } else if (current == '+') {
            current = 'a';
        }

        // Move player to new position
        if (target == 'a') {
            target = '+';
        } else {
            target = '@';
        }
        playerPos = {newX, newY};
        validMove = true;
    } else if (target == 'A' || target == '1') {
        unsigned int boxNewX = newX + dx;
        unsigned int boxNewY = newY + dy;
        if (boxNewX >= w || boxNewY >= h) return;

        char& boxDest = board[boxNewX + boxNewY * w];
        if (boxDest == '.' || boxDest == 'a') {
            // Move the box
            boxDest = (boxDest == 'a') ? '1' : 'A';

            // Mark current position as empty or storage
            if (current == '@') {
                current = '.';
            } else if (current == '+') {
                current = 'a';
            }

            // Move player to box's previous position
            if (target == '1') {
                target = '+';
            } else {
                target = '@';
            }

            playerPos = {newX, newY};
            validMove = true;
        }
    }

    if (validMove) {
        // Clear redo stack since we made a valid move
        redoStack = std::stack<std::pair<std::vector<char>, sf::Vector2u>>();
        moveCount++;
    }
}

bool SB::Sokoban::isWon() const {
    unsigned int crateCount = std::count_if(board.begin(), board.end(), [](char c)
    { return c == 'A'; });
    unsigned int cratesOnStorageCount = std::count_if(board.begin(), board.end(), [](char c)
    { return c == '1'; });
    unsigned int storageCount = std::count_if(board.begin(), board.end(), [](char c)
    { return c == 'a' || c == '1' || c == '+'; });

    return (storageCount == crateCount) ? (cratesOnStorageCount == storageCount) :
    (crateCount == 0 && cratesOnStorageCount > 0);
}

// Initialize the state
void SB::Sokoban::setInitialState() {
    initialBoard = board;
}

// Reset the level
void SB::Sokoban::reset() {
    board = initialBoard;
    moveCount = 0;
    playerPos.x = findPlayerStart().x;
    playerPos.y = findPlayerStart().y;
}

// Start position of player
sf::Vector2f SB::Sokoban::findPlayerStart() const {
    for (unsigned int y = 0; y < h; y++) {
        for (unsigned int x = 0; x < w; x++) {
            if (board[x + y * w] == '@') {
                return {static_cast<float>(x), static_cast<float>(y)};
            }
        }
    }
    return {0.0f, 0.0f};
}

namespace SB {
// Read the lvl files
std::istream& operator>>(std::istream& in, SB::Sokoban& s) {
    in >> s.h >> s.w;
    s.board.resize(s.h * s.w);

    std::string line;
    std::getline(in, line);
    for (unsigned int y = 0; y < s.h; y++) {
        std::getline(in, line);
        for (unsigned int x = 0; x < s.w; x++) {
            char cell = line[x];
            s.board[x + y * s.w] = cell;
            if (cell == '@') s.playerPos = {x, y};
        }
    }
    return in;
}

// Print the map after game
std::ostream& operator<<(std::ostream& out, const Sokoban& s) {
    for (unsigned int y = 0; y < s.height(); y++) {
        for (unsigned int x = 0; x < s.width(); x++) {
            out << s.board[x + y * s.width()];
        }
        out << '\n';
    }
    out << "Player Position: (" << s.playerPos.x << ", " << s.playerPos.y << ")\n";
    return out;
}
}  // namespace SB
