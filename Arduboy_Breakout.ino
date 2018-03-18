//Don Orrico
//March 10, 2018
//A simple Breakout clone

#include <Arduboy2.h>
#include <ArduboyTones.h>
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

//Variables declared here
int gamestate = 0;
int justpressed = 0;
int ballx = 55;
int bally = 60;
int ballsize = 2;
int ballright = 1;
int balldown = -1;
int paddlewidth = 20;
int paddleheight = 2;
int blockwidth = 7;
int blockheight = 2;
int playerx = 54;
int playery = 62;
int playerscore = 0;
int ballcount = 9;
int level = 1;
int block10[16] = {};
int block13[16] = {};
int block16[16] = {};
int blockout10[16] = {};
int blockout13[16] = {};
int blockout16[16] = {};
int blocksHit = 0;
int ballspeed = 27;       //Delay to slow down ball.  Higher=slower
int highscore = 0;
int nextnote = 0;
#define BLOCK16SPEED 27   // Delay value if none or 1st row hit
#define BLOCK13SPEED 20   // Delay value if 2nd row hit
#define BLOCK10SPEED 13   // Delay value if 3rd(Top) row hit
boolean released = false; // Used for serve function

const int marioTheme[] = {
  NOTE_E6, NOTE_E6, NOTE_E6, NOTE_C6, NOTE_E6, NOTE_G6, NOTE_G5,
  NOTE_C6, NOTE_G5, NOTE_E5, NOTE_A5, NOTE_B5, NOTE_AS5, NOTE_A5, 
  NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6, NOTE_F6, NOTE_G6, NOTE_E6,
  NOTE_C6, NOTE_D6, NOTE_B5, NOTE_C6, NOTE_G5, NOTE_E5, NOTE_A5, 
  NOTE_B5, NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_E6, NOTE_G6, NOTE_A6,
  NOTE_F6, NOTE_G6, NOTE_E6, NOTE_C6, NOTE_D6, NOTE_B5, -1
  };
  
void resetgame() {
        // Place ball on top left of paddle
        bally = playery - 2; 
        ballx = playerx + 1;
        playerscore = 0;
        balldown = -1;
        ballcount = 9;
        // Clear register for which blocks were hit 
        for (int i = 0; i < 16; i ++)
        {
        blockout10[i] = 0;
        blockout13[i] = 0;
        blockout16[i] = 0;   
        } 
        // Clear total # of blocks hit
        blocksHit = 0;
        // Reset Ball speed to slowest
        ballspeed = 27;
        level = 1;
        nextnote = 0;
        // Reset paddlewidth to largest
        paddlewidth = 20;
       }

void setup() {
        arduboy.begin();
        // Seed the random number generator
        arduboy.initRandomSeed();
        // Set the game to 60 frames per second
        arduboy.setFrameRate(60);
        arduboy.clear();
        sound.volumeMode(VOLUME_ALWAYS_HIGH);
}

void loop() {
        // Prevent the Arduboy from running too fast
        if(!arduboy.nextFrame()) {
                return;
        }
        // Refresh screen
        arduboy.clear();
        
        // Game code here
        switch( gamestate ) {
                case 0:
                        // Title screen
                        arduboy.setCursor(20, 15);
                        arduboy.setTextSize(2);
                        arduboy.print("BREAKOUT");
                        arduboy.setTextSize(1);
                        arduboy.setCursor(20,45);
                        arduboy.print("Press 'A' Button");
                        arduboy.setCursor(40,55);
                        arduboy.print("to start");
                        //Change the gamestate
                        if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
                                justpressed = 1;
                                gamestate = 1;
                        }        
                        break;
                case 1:
                        // Gameplay screen
                        arduboy.setCursor(0, 0);
                        arduboy.print("Lv:");
                        arduboy.print(level);
                        arduboy.setCursor(50, 0);
                        arduboy.print("Sc:");
                        arduboy.print(playerscore);
                        arduboy.setCursor(105, 0);
                        arduboy.print("Ba:");
                        arduboy.print(ballcount);
                        delay(ballspeed);
                                                
                        // Draw the ball
                        arduboy.fillRect(ballx, bally, ballsize, ballsize, WHITE);

                        // Draw the player's paddle
                        arduboy.fillRect(playerx, playery, paddlewidth, paddleheight, WHITE);
                        
                        // Check if block was already hit
                        // Only draw blocks not hit and record coordinates                        
                        for (int i = 0; i < 16; i ++)
                        {                                               
                          if(blockout10[i] == 0){
                          arduboy.fillRect(i*8, 10, blockwidth, blockheight, WHITE);
                          }
                          block10[i] = i * 8;
                          
                          if(blockout13[i] == 0){                     
                          arduboy.fillRect(i*8, 13, blockwidth, blockheight, WHITE);
                          }
                          block13[i] = i * 8;
                          
                          if(blockout16[i] == 0){
                          arduboy.fillRect(i*8, 16, blockwidth, blockheight, WHITE); 
                          }
                          block16[i] = i * 8;                                                 
                        } 
                        
                        // Hold ball until served (button released) 
                        if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
                                justpressed = 1;
                                released = true;
                        }
                        
                        if(released){         
                          // Move the ball right
                          if(ballright == 1) {
                                ballx = ballx + 2;
                          }
                        
                          // Move the ball left
                          if(ballright == -1) {
                                ballx = ballx - 2;
                          }
                        
                          // Move the ball down
                          if(balldown == 1) {
                                bally = bally + 2;
                          }
                        
                          // Move the ball up
                          if(balldown == -1) {
                                bally = bally - 2;
                          }
                        }
                             
                        // Reflect the ball off of the left of the screen
                          if(ballx <= 0) {
                                ballright = 1;
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1 and (released)){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                           }
                        
                         // Reflect the ball off of the right of the screen
                           if(ballx + ballsize >= 127) {
                                ballright = -1;
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                            }
                        
                         // Reflect the ball off of the top of the screen
                            if(bally <= 0) {
                                balldown = 1;
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }            
                            }
                                                
                         // If the player presses left and the paddle is not touching the left
                            if(arduboy.pressed(LEFT_BUTTON) and playerx > 0) {
                                playerx = playerx - 3;
                                if(!released){
                                  ballx = playerx + 1;
                                }
                            }
                        
                         // If the player presses right and the paddle is not touching the right
                            if(arduboy.pressed(RIGHT_BUTTON) and playerx + paddlewidth < 127) {
                                playerx = playerx + 3;
                                if(!released){
                                  ballx = playerx + 1;
                                }
                            }
                        
                        // If the ball moves off of the screen to the bottom
                            if(bally > 63) {
                                // Move the ball back to the player paddle to serve new ball
                                ballx = playerx + 1;
                                bally = playery - 2;
                                // Reduce ballcount by 1
                                ballcount = ballcount - 1; 
                                // Change direction of ball                            
                                balldown = -1;         
                                // Reset serve counter
                                released = false;
                                justpressed = 0;
                                // Reset ball speed to slowest
                                ballspeed = 27;                                                      
                            }
                        
                        // Check if the player is out of balls
                            if(ballcount == 0) {
                                gamestate = 3;        
                            }
                        
                        // If the ball makes contact with the players paddle, bounce in direction of bricks
                        if(bally + ballsize == playery and playerx <= ballx + ballsize and playerx + paddlewidth >= ballx) {
                                balldown = -1;
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1 and (released)){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                        }                          
                        
                        //If the ball makes contact with a block then erase block and add score
                        for (int i = 0; i < 16; i ++)
                        {
                        if((bally == 10 + blockheight or bally + ballsize == 10 or bally + 1 == 10 + blockheight or bally + 1 + ballsize == 10) and ballx + ballsize >= block10[i] and ballx <= block10[i] + blockwidth and blockout10[i] == 0) {
                          
                          // Scores 100 points per top row block hit
                          playerscore = playerscore + 100;
                          
                          // Draw black block in place of white block to erase
                          arduboy.fillRect(i*8, 10, blockwidth, blockheight, BLACK);
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                          
                          // Register hit block as hit
                          blockout10[i] = 1;
                          
                          // Increase hit counter by 1
                          blocksHit = blocksHit + 1;

                          // Speed ball to top row speed
                          if(ballspeed == BLOCK13SPEED or ballspeed == BLOCK16SPEED){
                          ballspeed = BLOCK10SPEED;                                                                          
                          }                                                                    
                          
                          // Change direction of ball to reflect
                          if(bally + ballsize == 10) {
                            balldown = -1;
                          }
                          else {
                            balldown = 1;
                          }                                               
                        }
                       
                        if((bally == 13 + blockheight or bally + ballsize == 13 or bally + 1 == 13 + blockheight or bally + 1 + ballsize == 13) and ballx + ballsize >= block13[i] and ballx <= block13[i] + blockwidth and blockout13[i] == 0) {
                          
                          // Scores 50 points per middle row block hit
                          playerscore = playerscore + 50;
                          
                          // Draw black block in place of white block to erase
                          arduboy.fillRect(i*8, 13, blockwidth, blockheight, BLACK);
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                          
                          // Register hit block as hit
                          blockout13[i] = 1;
                          
                          // Increase hit counter by 1
                          blocksHit = blocksHit + 1;
                           
                          // Speed ball to middle row speed
                          if(ballspeed == BLOCK16SPEED){
                          ballspeed = BLOCK13SPEED;                                                                          
                          }
                          
                          // Change direction of ball to reflect
                          if(bally + ballsize == 13) {
                            balldown = -1;
                          }
                          else {
                            balldown = 1;
                          }                          
                        }
                        
                        if((bally == 16 + blockheight or bally + ballsize == 16 or bally + 1 == 16 + blockheight or bally + 1 + ballsize == 16) and ballx + ballsize >= block16[i] and ballx <= block16[i] + blockwidth and blockout16[i] == 0) {
                          
                          // Scores 50 points per middle row block hit
                          playerscore = playerscore + 10;
                          
                          // Draw black block in place of white block to erase
                          arduboy.fillRect(i*8, 16, blockwidth, blockheight, BLACK);
                                
                                //Play next note of song for each hit
                                if(marioTheme[nextnote] != -1){
                                sound.tone(marioTheme[nextnote],75);
                                nextnote = nextnote + 1;
                                }
                                else {
                                nextnote = 0;
                                }
                          
                          // Register hit block as hit
                          blockout16[i] = 1; 
                          
                          // Increase hit counter by 1
                          blocksHit = blocksHit + 1;
                                                                        
                          // Change direction of ball to reflect
                          if(bally + ballsize == 16) {
                            balldown = -1;
                          }
                          else {
                            balldown = 1;
                          }
                        }
                        }
                        
                        //Check if all blocks have been hit
                          if(blocksHit == 48){
                            gamestate = 2;
                          }
                            
                        break;
                        
                case 2:
                        // Proceed to next level
                        arduboy.setCursor(17,28);
                        arduboy.print("Level ");
                        arduboy.print(level);
                        arduboy.print(" Complete");
                        
                        // Advance Level, Reduce Paddle Size, Clear blocks hit, Change the gamestate
                        if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
                                justpressed = 1;
                                
                                // Advance Level
                                level = level + 1;

                                // Reduce Paddle Size but not smaller than 6
                                if(paddlewidth > 5){
                                paddlewidth = paddlewidth - 3;
                                }                               
                                
                                // Clear blocks hit
                                for (int i = 0; i < 16; i ++)
                                {
                                blockout10[i] = 0;
                                blockout13[i] = 0;
                                blockout16[i] = 0;   
                                }
                                
                                // Clear total blocks hit
                                blocksHit = 0;
                                
                                // Place ball on top left of paddle
                                ballx = playerx + 1;
                                bally = playery - 2;
                                
                                // Reset ball speed to slowest
                                ballspeed = 27;
                                balldown = -1;
                                
                                // Change game state
                                gamestate = 1;
                        }        
                        
                        break;
                        
                case 3:
                        //vGame over screen
                        
                        // Check if score beats high score
                        if(playerscore > highscore){
                          highscore = playerscore;
                        }
                        
                        // Display High Score and Score
                        arduboy.setCursor(18, 0);
                        arduboy.setTextSize(1);
                        arduboy.print("HIGH SCORE: ");
                        arduboy.print(highscore);
                        arduboy.setCursor(13, 18);
                        arduboy.setTextSize(2);
                        arduboy.print("GAME OVER");
                        arduboy.setTextSize(1);
                        arduboy.setCursor(35, 55);
                        arduboy.print("SCORE: ");
                        arduboy.print(playerscore);
                        
                        // Change the gamestate to reset game to play again
                        if(arduboy.pressed(A_BUTTON) and justpressed == 0) {
                                justpressed = 1;
                                resetgame();                                
                                gamestate = 0;
                        }        
                        
                        break;
                                         
            }
        
        // Check if the button is being held down
        if(arduboy.notPressed(A_BUTTON)) {
                justpressed = 0;
        }

        // Display new screen update
        arduboy.display();
}
