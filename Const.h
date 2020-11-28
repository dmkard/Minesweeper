#pragma once

constexpr int TILE_SIDE_SIZE = 50;
constexpr int FRAMERATE = 30;
constexpr int MINE_AMOUNT = 99;
constexpr int B_WIDTH = 30;
constexpr int B_HEIGHT = 16;

constexpr int B_MARGIN = TILE_SIDE_SIZE / 2;
constexpr int W_WIDTH = 2 * B_MARGIN + B_WIDTH * TILE_SIDE_SIZE;
constexpr int W_HEIGHT = 6 * B_MARGIN + B_HEIGHT * TILE_SIDE_SIZE;
