#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <raylib.h>
#include <vector>

using namespace std;

struct Vec2 : public Vector2 {

  bool operator==(const Vec2 v1) const {
    bool res = false;
    if ((v1.x == x) && (v1.y == y)) {
      res = true;
    }
    return res;
  };
  bool operator!=(const Vec2 v1) const { return !(v1 == *this); };
  Vec2 operator+=(const Vec2 v) const {
    Vec2 nv;
    nv.x = x + v.x;
    nv.y = y + v.y;
    return nv;
  };

  Vec2 operator-(const Vec2 v) {
    Vec2 nv(x - v.x, y - v.y);
    return nv;
  };
  Vec2 &operator=(Vec2 other) {
    x = other.x;
    y = other.y;
    return *this;
  };

  Vec2 operator+(const Vec2 v) const {
    Vec2 nv = {x + v.x, y + v.y};
    return nv;
  };
  Vec2 operator*(const float v) {
    Vec2 nv = {v * x, v * y};
    return nv;
  };
  Vec2 operator*=(const float v) {
    Vec2 nv = {v * x, v * y};
    return nv;
  };
  Vector2 as_vector2() const {
    Vector2 v = {x, y};
    return v;
  };
  Vec2(Vector2 v) {
    x = v.x;
    y = v.y;
  };
  Vec2() {
    x = 0;
    y = 0;
  }
  Vec2(float x, float y) {
    this->x = x;
    this->y = y;
  }
};

const Vec2 VEC_ZERO = {0, 0};

const int GRID_SPACING = 30;

enum segmentType { Head, Middel, end };
enum timerType { Repeat, Once };

class Timer {
private:
  bool active;
  float time_counted;
  /** in seconds */
  float wait_time;
  bool finished;
  timerType timer_type;

  void timer_finished() { finished = true; };

public:
  Timer() {
    active = true;
    time_counted = 0;
    wait_time = 1;
    finished = false;
    timer_type = timerType::Repeat;
  };
  double get_time() { return time_counted; };
  void start() { active = true; };
  void reset() {
    finished = false;
    time_counted = 0;
    start();
  };
  void update() {
    if (active) {
      time_counted += GetFrameTime();
      // cout << time_counted << endl;
    }

    if (time_counted > wait_time) {
      switch (timer_type) {
      case Once:
        stop();
        break;
      case Repeat:
        timer_finished();
      }
    }
  };

  /** sets the timer's wait time in seconds */
  void set_wait_time(float v) { wait_time = v; };
  Timer &stop() {
    if (active) {

      active = false;
    }

    if (time_counted > wait_time)
      timer_finished();
    return *this;
  };
  bool is_stopped() { return active ? false : true; };
  bool is_finished() {
    bool v = false;
    if (finished) {
      switch (timer_type) {

      case (timerType::Once):
        finished = false;
        v = finished;
        break;

      case (timerType::Repeat):
        if (finished) {
          cout << "bruh" << time_counted << endl;
          reset();
          v = true;
          break;
        }
      }
    }
    return v;
  };
};

class SnackeSegment {
public:
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
    SnackeSegment b;
    b.current_position += (direction * 3);
    SnackeSegment c;
    c.current_position += (direction * 5);

    body_parts = {a, b, c};
    for (int i = 0; i < 50; i++) {
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

    if (IsKeyDown(KEY_UP)) {
      direction = {0, -GRID_SPACING};

    } else if (IsKeyDown(KEY_DOWN)) {
      direction = {0, GRID_SPACING};

    } else if (IsKeyDown(KEY_LEFT)) {
      direction = {-GRID_SPACING, 0};

    } else if (IsKeyDown(KEY_RIGHT)) {
      direction = {GRID_SPACING, 0};
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

int main() {
  InitWindow(1000, 500, "bruh");
  Camera2D camera;
  Vector2 target = {500, 250};
  camera.offset = target;
  camera.zoom = 1.0;
  camera.rotation = 0.0;
  Snacke s;
  Timer t;
  Color cor = GREEN;

  camera.target = static_cast<Vector2>(s.body_parts[0].current_position);
  while (!WindowShouldClose()) {
    // cout << "44" << endl;
    s.update();
    Vec2 v(camera.target);
    v = v + ((s.body_parts[0].current_position - v) * ( GetFrameTime()));
    camera.target = v.as_vector2();
    if (t.is_finished()) {
      cor = RED;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);
    s.draw();
    EndMode2D();

    DrawCircle(100, 100, 10, cor);

    EndDrawing();
  }
  CloseWindow();
}
