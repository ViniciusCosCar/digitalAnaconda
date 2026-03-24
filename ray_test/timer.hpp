
#include <raylib.h>
#ifndef TIMER
#define TIMER
namespace timer {
enum TimerType { Repeat, Once };

class Timer {
private:
  bool active;
  float time_counted;
  /** in seconds */
  float wait_time;
  bool finished;
  TimerType timer_type;

  void timer_finished() { finished = true; };

public:
  Timer() {
    active = true;
    time_counted = 0;
    wait_time = 1;
    finished = false;
    timer_type = TimerType::Repeat;
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

      case (TimerType::Once):
        finished = false;
        v = finished;
        break;

      case (TimerType::Repeat):
        if (finished) {
          // cout << "bruh" << time_counted << endl;
          reset();
          v = true;
          break;
        }
      }
    }
    return v;
  };
};
} // namespace timer

#endif
