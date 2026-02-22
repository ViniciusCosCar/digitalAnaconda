#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <vector>
#include <time.h>

using namespace std;

const int TotalLayers = 3;
const int ScreenWidth = 10;
const int ScreenHeight = 10;

class Timer
{
public:
  double wait_time_seconds;
  int time_out_events;


  bool is_time_out_event(){
    if (time_out_events > 0){
      time_out_events -=1;
      return true;
    }
    return false;
  }

  void update()
  {
    time(&current_time);
    if (difftime(current_time,start_time) >= wait_time_seconds)
    {
      start_time = current_time;
      time_out_events += 1;
    }
  }
  Timer()
  {
    wait_time_seconds = 2;
  }

private:
  time_t start_time;
  time_t current_time;
};

class position
{
public:
  int x;
  int y;
  void set_x(int value) { x = value; }
  void set_y(int value) { y = value; }

  position()
  {
    x = 0;
    y = 0;
  }

  position(int v_x, int v_y)
  {
    x = v_x;
    y = v_y;
  }
};

class Snacke
{
public:
  position head_position;
  int size;
  vector<position> body_parts_position;
  Snacke()
  {
    head_position = position(10, 10);
    body_parts_position.clear();
    body_parts_position.insert(body_parts_position.begin(), head_position);
  }

  ~Snacke() { body_parts_position.clear(); }
};

class GameScreen
{
public:
  char data[TotalLayers][ScreenWidth][ScreenHeight];
  GameScreen()
  {

    for (int x = 0; x < ScreenWidth; x++)
    {
      for (int y = 0; y < ScreenHeight; y++)
      {
        data[1][x][y] = '+';
      }
    }
  }
  void draw()
  {

    for (int y = 0; y < ScreenHeight; y++)
    {
      for (int x = 0; x < ScreenWidth; x++)
      {
        cout << data[1][x][y];
      }
      cout << "\n";
    }
    cout << endl;
  }
};
class Game
{
public:
  bool quit;
  GameScreen screen;
  Snacke player_one;
  void update()
  {
    system("clear");
    player_one;
    screen.draw();
  }
  Game()
  {
    quit = false;
  }
  void quit_game()
  {
    quit = true;
  }
};

void pb() { std::cout << "bruh bbb\n"; }
void pa() { std::cout << "bruh aaa\n"; }

const char UP[5] = "^[[A";
const char DOWN[5] = "^[[B";
const char LEFT[5] = "^[[D";
const char RIGHT[5] = "^[[C";

void get_intput(char i[5])
{
  cin >> i;
}

int main()
{

  time_t start_time;
  time(&start_time);
  time_t end_time;
  time(&end_time);

  double time_dif = difftime(start_time, end_time);
  char dir[5];

  thread t(get_intput, dir);
  Timer clock;

  Game game;
  while (!game.quit)
  {
    clock.update();
    if (clock.is_time_out_event())
    {
      game.update();
    }

    if (dir[0] == 'q')
    {
      game.quit_game();
    }
    if (t.joinable())
      t.join();
  }

  return 0;
}
