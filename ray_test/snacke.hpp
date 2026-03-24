#include "consts.hpp"
#include "raylib.h"
#include "timer.hpp"
#include <vector>

#ifndef SNACKE
#define SNACKE
using namespace std;
using namespace timer;

namespace snacke {
enum SegmentType { Head, Middel, end };

class SnackeSegment {
public:
  SegmentType type;
  Vec2 current_position;
  Vec2 last_position;

  SnackeSegment() {
    current_position = {GRID_SPACING, GRID_SPACING};
    last_position = {GRID_SPACING, GRID_SPACING};
  };
  void next_segment(SnackeSegment next_one) {
    last_position = current_position;
    current_position = next_one.last_position;
  };
  void next_position(Vec2 next_pos) {
    last_position = current_position;
    current_position = next_pos;
  };
  void draw() {
    DrawCircle(current_position.x, current_position.y, 10, PURPLE);
    // cout << "bruh" << endl;
  };
};

class Snacke {
public:
  Vec2 direction;
  vector<SnackeSegment> body_parts;
  Timer move_time;

  Snacke() {
    SnackeSegment a;
    a.current_position += direction;
    a.type = Head;

    body_parts = {a};
    for (int i = 0; i < 10; i++) {
      SnackeSegment ss;
      body_parts.push_back(ss);
    }
    direction = {GRID_SPACING, 0};
    Timer t;
    t.set_wait_time(0.3);
    move_time = t;
  };
  void move_snacke() {
    Vec2 pos = body_parts[0].current_position;
    body_parts[0].next_position(pos + direction);
    for (int i = 1; i < body_parts.size(); i++) {
      body_parts[i].next_segment(body_parts[i - 1]);
    }
  }
  void update() {
    move_time.update();

    if (move_time.is_finished()) {
      move_snacke();
    }

    Vec2 old_direction = direction;

    const Vec2 UP = {0, -GRID_SPACING};
    const Vec2 DOWN = {0, GRID_SPACING};
    const Vec2 LEFT = {-GRID_SPACING, 0};
    const Vec2 RIGHT = {GRID_SPACING, 0};
    direction = VEC_ZERO;
    if (IsKeyDown(KEY_UP)) {
      direction = direction + UP;
    }
    if (IsKeyDown(KEY_DOWN)) {
      direction = direction + DOWN;
    }
    if (IsKeyDown(KEY_LEFT)) {
      direction = direction + LEFT;
    }
    if (IsKeyDown(KEY_RIGHT)) {
      direction = direction + RIGHT;
    }
    if (((direction * (-1)) == old_direction) || (direction == VEC_ZERO)) {
      direction = old_direction;
    }
  }

  void draw() {
    for (SnackeSegment segment : body_parts) {
      segment.draw();
    }
  };
};
} // namespace snacke

#endif
