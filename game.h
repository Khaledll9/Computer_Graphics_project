#ifndef GAME_H
#define GAME_H

enum GameState { START_SCREEN, PLAYING, GAME_OVER };

typedef struct {
    float x, y;       
    float r, g, b;    
    int score;        
    int lives;        
} Player;


typedef struct {
    float x, y;       
    float speed;      
    int type;         
    int active;       
    int points;       
} Collectible;


#define MAX_ITEMS   13
#define BOUNDARY    0.85f
#define PLAYER_STEP 0.032f
#define SPAWN_RATE  110     


extern GameState gameState;
extern Player player;
extern Collectible items[MAX_ITEMS];
extern int spawnTimer;


void initGame();
void resetGame();
void updateGame();
void spawnItem();
void checkCollisions();

#endif
