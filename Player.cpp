#include <Player.h>
#include <time.h>

Player::Player(){
  symbol = '&';
}

int Player::getHealth(){
  return health;
}

void Player::setHealth(int int1){
  health = int1;
}

void Player::lowerHealth(){
  health--;
}

void Player::takeDamage(){
  lowerHealth();
  damageable = false;
  nodamageStart = time(NULL);
}
