#pragma once

enum class Turn {
  PLAYER,
  AI,
};

struct PongRules {
  Turn turn = Turn::PLAYER;
  uint32_t playerScore = 0;
  uint32_t aiScore = 0;
};