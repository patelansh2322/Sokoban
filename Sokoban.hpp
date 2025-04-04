// Copyright [2025] Ansh Patel

#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>

namespace SB {

enum class Direction {
    Up, Down, Left, Right
};

class Sokoban : public sf::Drawable {
 public:
    static const int TILE_SIZE = 64;

    Sokoban();
    explicit Sokoban(const std::string&);  // Optional

    unsigned int pixelHeight() const;  // Optional
    unsigned int pixelWidth() const;  // Optional

    unsigned int height() const;
    unsigned int width() const;

    sf::Vector2u playerLoc() const;

    bool isWon() const;

    void movePlayer(Direction dir);
    void reset();
    void setInitialState();
    sf::Vector2f findPlayerStart() const;

    unsigned int count();

    void undo();  // Optional XC
    void redo();  // Optional XC

    friend std::ostream& operator<<(std::ostream& out, const Sokoban& s);
    friend std::istream& operator>>(std::istream& in, Sokoban& s);

 protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

 private:
    unsigned int h, w;
    sf::Vector2u playerPos;
    std::vector<char> board;
    std::vector<char> initialBoard;
    std::stack<std::pair<std::vector<char>, sf::Vector2u>> undoStack;
    std::stack<std::pair<std::vector<char>, sf::Vector2u>> redoStack;


    sf::Texture playerTexture, wallTexture,
    emptyTexture, boxTexture, storageTexture,
    boxOnStorageTexture;

    unsigned int moveCount;
};
}  // namespace SB
