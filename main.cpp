#include <ncurses.h>
#include <Enemy.h>
#include <Player.h>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <string>
#include <time.h>


using namespace std;

void print();
void init();
void input();
void logic();
void collision();
void clearscreen();
void HUD();
int clamp(int value, int upperLimit, int lowerLimit);
int clamp(int value, int limit, char uvsl);

bool running;
int WIDTH = 30;
int HEIGHT = 20;
int count;
int score;
bool bottomWin;
vector<Enemy> enemies;

Player player;

int main(){
  initscr();
  noecho();
  keypad(stdscr,true);
  nodelay(stdscr, TRUE);
  init();

  while(running){
    logic();
    print();
    input();
    usleep(10000); //increase to make game slower
  }

  nodelay(stdscr, FALSE);
  clearscreen();
  mvprintw(10,10,"Game over, press any key to exit");
  refresh();
  getch();
  endwin();

  return 0;
}

void init(){
  player.setHealth(10);
  player.xpos = 1;
  player.ypos = 1;
  player.damageable = true;
  for(int i = 0; i < 15; i++){
    Enemy tempEnemy;
    tempEnemy.xpos = i+1;
    tempEnemy.ypos = i+3;
    enemies.push_back(tempEnemy);
  }
  /*
  old method of adding enemies
  Enemy enemy1;
  enemy1.ypos = 10;
  enemy1.xpos = 10;
  enemies.push_back(enemy1);
  Enemy enemy2;
  enemy2.ypos = 15;
  enemy2.xpos = 5;
  enemies.push_back(enemy2);
  */
  count = 0;
  score = 0;
  bottomWin = true;
  running = true;

}

void print(){
  clearscreen();
  //enemies
  for(int i = 0; i < enemies.size(); i++){
    Enemy tempEnemy = enemies.at(i);
    mvaddch(tempEnemy.ypos,tempEnemy.xpos,tempEnemy.symbol);
  }
  //gameboard and player
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      if(i == 0){
        mvaddch(i,j,'#');
        //first is row, second is column
      }
      else if(i == player.ypos && j == player.xpos){
        mvaddch(i,j,player.symbol);
      }
      else if(j == 0 || j == WIDTH -1){
        mvaddch(i,j,'#');
      }
      else if(i == HEIGHT -1){
        mvaddch(i,j,'#');
      }
    }
  }
  HUD();

  refresh();
}

void clearscreen(){
  //clear board
  for(int i = 0; i < HEIGHT; i++){
    for(int j = 0; j < WIDTH; j++){
      mvaddch(i,j,' ');
    }
  }
}

void HUD(){
  string str1 = "Health: " + to_string(player.getHealth());
  mvprintw(21,20,str1.c_str());
  str1 = "Score: " + to_string(score);
  mvprintw(22,21,str1.c_str());
//  str1 = "Enemy 1 at (" + to_string(enemies.at(0).xpos) + "," + to_string(enemies.at(0).ypos) + ")";
//  mvprintw(23,20,str1.c_str());
//  str1 = "Player at (" + to_string(player.xpos) + "," + to_string(player.ypos) + ")";
//  mvprintw(24,20,str1.c_str());
}

void input(){
  int ch = getch();
  if(ch == KEY_UP){
    player.ypos--;
  }
  else if(ch == KEY_DOWN){
    player.ypos++;
  }
  else if(ch == KEY_LEFT){
    player.xpos--;
  }
  else if(ch == KEY_RIGHT){
    player.xpos++;
  }
  else if(ch == KEY_BACKSPACE){
    player.takeDamage();
  }
  else if(ch == KEY_END){
    running = false;
    endwin();
  }
//  player.xpos = clamp(player.xpos,1,WIDTH - 1);
//  player.ypos = clamp(player.ypos,1,HEIGHT-1);
}

int clamp(int value, int upperLimit, int lowerLimit){
  if(value < lowerLimit){
    return lowerLimit;
  }
  else if(value > upperLimit){
    return upperLimit;
  }
  return value;
}

void logic(){
  if(count % 25 == 0){
    for(int i = 0; i < enemies.size(); i++){
      Enemy tempEnemy = enemies.at(i);
      if(tempEnemy.right){
        tempEnemy.xpos++;
      }
      else{
        tempEnemy.xpos--;
      }
      if(tempEnemy.xpos >= 28){
        tempEnemy.right = false;
      }
      if(tempEnemy.xpos <= 2){
        tempEnemy.right = true;
      }
      enemies.at(i) = tempEnemy;
    }
  }
  count++;
  if(count == 100){
    count = 0;
  }
  if(player.ypos == 1 && !bottomWin){
    score++;
    bottomWin = true;
  }
  if(player.ypos == 18 && bottomWin){
    score++;
    bottomWin = false;
  }
  if(player.getHealth() == 0){
    running = false;
  }
  if(!player.damageable){
    time_t currentTime = time(NULL);
    if(currentTime - player.nodamageStart > 1){
      player.damageable = true;
    }
  }
  collision();
}

void collision(){
  for(int i = 0; i < enemies.size(); i++){
    if(player.xpos == enemies.at(i).xpos && player.ypos == enemies.at(i).ypos && player.damageable){
      player.takeDamage();
    }
  }
}
