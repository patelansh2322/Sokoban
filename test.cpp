// Copyright [2025] Ansh Patel

#include <fstream>
#include <sstream>
#include "Sokoban.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE SokobanTests
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(cannot_move) {
    SB::Sokoban game("walkover.lvl");
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Up);
    game.undo();

    BOOST_CHECK_NO_THROW(game.redo());
}

BOOST_AUTO_TEST_CASE(ignore_boxes) {
    SB::Sokoban game("pushright.lvl");
    game.movePlayer(SB::Direction::Right);

    BOOST_CHECK(game.isWon());
}

BOOST_AUTO_TEST_CASE(move_off_screen) {
    SB::Sokoban game("pushdown.lvl");
    auto initialPosX = game.playerLoc().x;
    auto initialPosY = 0;

    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    BOOST_CHECK_EQUAL(game.playerLoc().x, initialPosX);
    BOOST_CHECK_EQUAL(game.playerLoc().y, initialPosY);
}

BOOST_AUTO_TEST_CASE(push_off_screen) {
    SB::Sokoban game("pushright.lvl");
    auto initialPosX = 1;
    auto initialPosY = game.playerLoc().y;

    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    BOOST_CHECK_EQUAL(game.playerLoc().x, initialPosX);
    BOOST_CHECK_EQUAL(game.playerLoc().y, initialPosY);
}

BOOST_AUTO_TEST_CASE(box_wall) {
    SB::Sokoban game("pushup.lvl");
    auto initialPosX = 3;
    auto initialPosY = 1;

    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);

    BOOST_CHECK_EQUAL(game.playerLoc().x, initialPosX);
    BOOST_CHECK_EQUAL(game.playerLoc().y, initialPosY);
}

BOOST_AUTO_TEST_CASE(box_box) {
    SB::Sokoban game("level2.lvl");
    auto initialPosX = game.playerLoc().x;
    auto initialPosY = game.playerLoc().y;

    game.movePlayer(SB::Direction::Up);

    BOOST_CHECK_EQUAL(game.playerLoc().x, initialPosX);
    BOOST_CHECK_EQUAL(game.playerLoc().y, initialPosY);
}

BOOST_AUTO_TEST_CASE(more_crates) {
    SB::Sokoban game("level5.lvl");

    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Up);

    BOOST_CHECK(game.isWon());
}

BOOST_AUTO_TEST_CASE(more_targets) {
    SB::Sokoban game("level6.lvl");

    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Up);

    BOOST_CHECK(game.isWon());
}

BOOST_AUTO_TEST_CASE(missing_symbol) {
    SB::Sokoban game("swapoff.lvl");
    BOOST_CHECK(!game.isWon());

    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Up);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Right);
    game.movePlayer(SB::Direction::Down);
    game.movePlayer(SB::Direction::Left);
    game.movePlayer(SB::Direction::Left);

    BOOST_CHECK(game.isWon());
}
