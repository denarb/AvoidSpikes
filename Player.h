#include <time.h>

using namespace std;

class Player{
public:
  Player();
  int getHealth();
  void setHealth(int int1);
  void lowerHealth();
  void raiseHealth();
  void takeDamage();
  void logic();

  int xpos;
  int ypos;
  char symbol;
  bool damageable;
  time_t nodamageStart;

private:
  int health;
};
