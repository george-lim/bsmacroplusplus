#include <iostream>

class Action {
  enum { MouseClick, MouseDrag, KeyPress } type;
  double x, y, x2, y2;
  std::string keyName;
  unsigned int movePoints, &time, delay, duration;
public:
  Action(double x, double y, unsigned int &time, unsigned int delay = 500, unsigned int duration = 200) : type(MouseClick), x(x), y(y), time(time), delay(delay), duration(duration) {}
  Action(double x, double y, double x2, double y2, unsigned int movePoints, unsigned int &time, unsigned int delay = 500, unsigned int duration = 200) : type(MouseDrag), x(x), y(y), x2(x2), y2(y2), movePoints(movePoints), time(time), delay(delay), duration(duration) {}
  Action(std::string keyName, unsigned int &time, unsigned int delay = 500, unsigned int duration = 200) : type(KeyPress), keyName(keyName), time(time), delay(delay), duration(duration) {}
  friend std::ostream &operator<<(std::ostream &out, const Action& action) {
    action.time += action.delay;
    switch (action.type) {
      case MouseClick:
      out << "{ \"Timestamp\": " << action.time << ", \"X\": " << action.x << ", \"Y\": " << action.y << ", \"Delta\": 0, \"EventType\": \"MouseDown\" }, ";
      action.time += action.duration;
      out << "{ \"Timestamp\": " << action.time << ", \"X\": " << action.x << ", \"Y\": " << action.y << ", \"Delta\": 0, \"EventType\": \"MouseUp\" }, ";
      break;
      case MouseDrag: {
        double xCurrent = action.x;
        double yCurrent = action.y;
        double deltaX = (action.x2 - action.x) / action.movePoints;
        double deltaY = (action.y2 - action.y) / action.movePoints;
        unsigned int deltaTime = action.duration / action.movePoints;
        unsigned int finalTime = action.time + action.duration;
        out << "{ \"Timestamp\": " << action.time << ", \"X\": " << action.x << ", \"Y\": " << action.y << ", \"Delta\": 0, \"EventType\": \"MouseDown\" }, ";
        for (unsigned int i = 0; i < action.movePoints; i++) {
          xCurrent += deltaX;
          yCurrent += deltaY;
          action.time += deltaTime;
          out << "{ \"Timestamp\": " << action.time << ", \"X\": " << xCurrent << ", \"Y\": " << yCurrent << ", \"Delta\": 0, \"EventType\": \"MouseMove\" }, ";
        }
        action.time = finalTime;
        out << "{ \"Timestamp\": " << action.time << ", \"X\": " << action.x2 << ", \"Y\": " << action.y2 << ", \"Delta\": 0, \"EventType\": \"MouseUp\" }, ";
        break;
      }
      case KeyPress:
      out << "{ \"Timestamp\": " << action.time << ", \"KeyName\": \"" << action.keyName << "\", \"EventType\": \"KeyDown\" }, ";
      action.time += action.duration;
      out << "{ \"Timestamp\": " << action.time << ", \"KeyName\": " << action.keyName << ", \"EventType\": \"KeyUp\" }, ";
    }
    return out;
  }
};

int main() {
  unsigned int time = 0;
  const std::string name = "Demo"; // Specify new macro name
  const Action events[] = { // Provide a list of actions to print
    {0, 0, time, 0}, // Click at (0,0) with no initial delay and default duration
    {0, 0, 50.01, 50.01, 100, time, 500, 2000}, // Drag from (0,0) to (50.01,50.01) over 100 move actions, with 500ms delay and 2000ms duration
    {"Esc", time}, // Type 'ESC' (equivalent to pressing back button) using default delay and duration
 };

  // Print Macro to standard out
  std::cout << "{ \"TimeCreated\": \"00010101T000000\", \"Name\": \"" << name << "\", \"FileName\": \"\", \"Events\": [ ";
  for (auto event : events) std::cout << event;
  std::cout << "], \"LoopType\": \"UntilStopped\", \"LoopNumber\": 1, \"LoopTime\": 0, \"LoopInterval\": 0, \"Acceleration\": 1.0, \"PlayOnStart\": false, \"RestartPlayer\": false, \"RestartPlayerAfterMinutes\": 60, \"ShortCut\": \"\" }";
  return 0;
}
