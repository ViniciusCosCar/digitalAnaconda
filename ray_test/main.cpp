#include <ctime>
#include <iostream>
#include <raylib.h>

#include "consts.hpp"
#include "snacke.hpp"
#include "timer.hpp"
#include "vec2.hpp"

using namespace std;
using namespace timer;
using namespace snacke;

class Game {

}

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
    v = v + ((s.body_parts[0].current_position - v) * (GetFrameTime() / 1.2));
    camera.target = v.as_vector2();
    if (t.is_finished()) {
      cor = RED;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(camera);
    s.draw();
    DrawCircle(100, 100, 10, cor);
    EndMode2D();

    EndDrawing();
  }
  CloseWindow();
}
