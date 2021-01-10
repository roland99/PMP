#include <Adafruit_NeoPixel.h>

#define PIXELPIN       4

// nr of leds
#define NUMPIXELS      24


#define BUTTON_TAP     3
#define BUTTON_RST     2


// Initializare neopixel
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

// A bunch of variables
int playerIndex = -1;
int playerIndexTrail = -1;
int playerDirection = 1;
float nextMove = 0;
float currentPlayerSpeed = 15; //for real life: 150
unsigned long countdown  = 0;
int enemyIndex = -1;
int pointIndex = -1;
int score = 0;
int bestScore = 0;
int lastScore = 0;
bool gameOver = false;

void setup()
{
  
  pinMode( BUTTON_TAP, INPUT );
  pinMode( BUTTON_RST, INPUT);
  
  
  pixels.begin();
  pixels.setBrightness(50);

  // 2 seconds until start
  countdown = millis() + 2; // for real life: + 2000
}

void loop()
{
  
  if (digitalRead(BUTTON_TAP) ==  HIGH){
    Click();
  }
  
  if (digitalRead(BUTTON_RST) ==  HIGH){
    Restart();
  }

  
  if ( gameOver )
    return;

  // put enemy and point on circle
  SetLevel();

  // get ready
  if ( countdown > millis() )
  {
    pixels.show();
    return;
  }

  DisplayPlayer();

   // shoe the pixels
  pixels.show();
  
  
}

// Clear the level, pixels  black
void ClearLevel()
{
  for(int i=0;i<NUMPIXELS;i++)
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 

  pixels.show();
}

//  best score in yelow, less than all time best -> red the rest
void BestScore()
{
  // Best score in yellow
  for(int i=0;i<NUMPIXELS;i++)
  {
    if ( i < bestScore )
      pixels.setPixelColor(i, pixels.Color(255,155,0)); // green
    else
      pixels.setPixelColor(i, pixels.Color(0,0,0)); 
  }

  // last score is less than best is in red
  if ( lastScore < bestScore )
  {
    for(int i=0;i<lastScore;i++)
      pixels.setPixelColor(i, pixels.Color(255,0,0)); // red
  }
  pixels.show();
}


void GameOver()
{
  // 1. turning red from the enemy pos
  int a = enemyIndex;
  int b = enemyIndex;
  
  for(int i=0;i<NUMPIXELS/2;i++)
  {
      pixels.setPixelColor(a, pixels.Color(255,0,0)); 
      pixels.setPixelColor(b, pixels.Color(255,0,0)); 

      a = ( a + 1) % NUMPIXELS;
      b--;
      if ( b == -1 )
        b = NUMPIXELS;

      pixels.show();
      delay(20);
  }

  delay(100);

  // set black from enemy
  a = enemyIndex;
  b = enemyIndex;
  
  for(int i=0;i<NUMPIXELS/2;i++)
  {
      pixels.setPixelColor(a, pixels.Color(0,0,0)); // Black
      pixels.setPixelColor(b, pixels.Color(0,0,0)); // Black

      a = ( a + 1) % NUMPIXELS;
      b--;
      if ( b == -1 )
        b = NUMPIXELS;

        pixels.show();
        delay(20);
  }

  // display best score
  delay(100);
  BestScore();
}

// Seting enemy and point position
void SetLevel()
{
  // If enemyindex =  -1 => reseted
  // new enemy
  if ( enemyIndex < 0 )
  {
    // first time putting enemy in midle
    if ( playerIndex < 0 )
    {
      enemyIndex = NUMPIXELS / 2;
    }
    else  //when to pass to next level
    {
      enemyIndex = random(0, NUMPIXELS);

      while ( abs(enemyIndex - playerIndex ) < ( NUMPIXELS / 4 ) )
        enemyIndex = random(0, NUMPIXELS);
    }
  }
  // If  point position=-1 => reseted
  // new point pos
  if ( pointIndex < 0 )
  {
      pointIndex = random(0, NUMPIXELS);

      // point in (enemy,player)
      while ( abs(pointIndex - playerIndex ) < 7 || ( abs(pointIndex - enemyIndex ) < 7 ) )
        pointIndex = random(0, NUMPIXELS);
  }

  pixels.setPixelColor(enemyIndex, pixels.Color(255,0,0));
  pixels.setPixelColor(pointIndex, pixels.Color(255,255,0));
}

// move the player, collect point and die
void DisplayPlayer()
{
  
  if ( nextMove < millis() )
  {
    nextMove = millis() + currentPlayerSpeed;

    // Tail of the player
    if ( playerIndexTrail >= 0 )
        pixels.setPixelColor(playerIndexTrail, pixels.Color(0, 0, 0));

    if ( playerIndex >= 0)
    {
      pixels.setPixelColor(playerIndex, pixels.Color(0, 100, 0));
      playerIndexTrail = playerIndex;
    }

    // player move
    playerIndex += playerDirection;

    // overflow
    if ( playerIndex < 0 )
      playerIndex = NUMPIXELS - 1;
    else if ( playerIndex == NUMPIXELS )
      playerIndex = 0;
    
    pixels.setPixelColor(playerIndex, pixels.Color(0, 255, 0));

    // hit point => next level => place coin, increase speed
    if ( playerIndex == pointIndex )
    {
      enemyIndex = -1;
      pointIndex = -1;
      score++;
      currentPlayerSpeed = constrain( currentPlayerSpeed - 1, 5, 15 ); // for real life: (currentPlayerSpeed - 10, 50, 150)
      ClearLevel();
      pixels.setPixelColor(playerIndex, pixels.Color(0, 255, 0));
    }
    
    // hit enemy => die => show score
    else if ( playerIndex == enemyIndex )
    {
      lastScore = score;
      if ( score >= bestScore )
        bestScore = score;
        
      GameOver();
      
      gameOver = true;
      enemyIndex = -1;
      pointIndex = -1;
      playerIndex = -1;
    }
  }
}

// BUTTON_TAP
void Click()
{
  // after the preparation
  if ( countdown > millis() )
    return;

  // change direction
  playerDirection = -playerDirection;
}
  
// BUTTON_RST
void Restart()
{
    
    // Reset the game, if game over => start game else stop current game
    gameOver = !gameOver;
    if ( gameOver )
    {
      enemyIndex = -1;
      pointIndex = -1;
      playerIndex = -1;
      currentPlayerSpeed = 15;  //for real life: 150
      ClearLevel();
    }
    else
    {
      ClearLevel();
      score = 0;
      currentPlayerSpeed = 15;
      countdown = millis() + 2;
    }
}
