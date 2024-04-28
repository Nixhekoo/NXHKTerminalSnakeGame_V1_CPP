// =================================================================================== //
// This game was made by Nixhekoo.
// =================================================================================== //

#include <iostream>
#include <ctime>
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include <string>

// =============================== GLOBAL VARIABLES =====================================
// I use global variables because i have problems sending Multidimensional Arrays over
// Functions.

// Grid Size
const int width = 40;
const int height = 20;
char Grid[height][width];
int Item[height][width];
// Player Position
int playerWidth;
int playerHeight;
// Symbols
char symbolFood = '*';
char symbolPlayerHead = '@';
char symbolPlayerBody = 'O';
char symbolPlayerTail = 'o';
char symbolBarrier = '#';
char symbolEmpty = ' ';
// Alive
bool alive = true;
// Keys
char lastKeyPress;
char keyPress;
std::string userInput;
// Score
int foodCollected = 1;
int score = 0;
int highscore = 0;
int gameScorePerFood = 1;
// Game Speed
int gameSpeed = -1;
// Food Location
int foodHeight = -1;
int foodWidth = -1;
// Cause of Death
std::string menuMessage = "=> Welcome to TSG (Terminal Snake Game) developed by Nixhekoo!\n=> Enjoy your stay and make sure to frequently check new versions on GitHub! (github.com/nixhekoo)\n\n";


// Clear Screen Function
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
COORD restartCursorPosition = {0, 0};
void clearConsole(){
    SetConsoleCursorPosition(hConsole, restartCursorPosition);
}

void restartGame(){
    alive = true;
    playerHeight = round(height / 2);
    playerWidth = round(width / 2);
    Grid[playerHeight][playerWidth] = symbolPlayerHead;
    Item[playerHeight][playerWidth] = 1;
    foodCollected = 1;
    score = 0;
    lastKeyPress = ' ';
}

void gameEndMessage(std::string type, std::string gameEndMessage){
    clearConsole();
    system("cls");
    menuMessage = gameEndMessage;
    if(score > highscore){
        highscore = score;
    }
    if(type == "death"){
        std::cout << gameEndMessage;
        std::cout << "Your score: " << score << "\nYour Highscore: " << highscore << "\n> ";
        Sleep(2000);
        system("pause");
    }
    alive = false;
}

// =================================== PRE GAME =========================================

void gameVariableSetup(){
    std::string userInput;
    int temporary;
    bool validInput = false;
    do{
        system("cls");
        std::cout << menuMessage;
        std::cout << "=============================\n";
        std::cout << "Enter a game speed (100-300): \n";
        std::cout << "(Higher = Faster)" << '\n' << "> ";
        std::cin >> userInput;

        // Check if input contains non-numeric characters
        validInput = true;
        for (char c : userInput) {
            if (!isdigit(c)) {
                validInput = false;
                break;
            }
        }

        if (validInput) {
            temporary = std::stoi(userInput); // Convert valid input to integer
            if (temporary > 300){
                temporary = 300;
            } else if (temporary < 100){
                temporary = 100;
            }
            gameScorePerFood = temporary;
            gameSpeed = (0 - temporary) + 300;
        }
    } while (!validInput);
    system("cls");
}

void gameGridBackgroundSetup(){
    // Grid
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){

            // Sets Borders
            if(h == 0 || h == (height - 1) || w == 0 || w == (width - 1)){
                Grid[h][w] = symbolBarrier;
                Item[h][w] = -2;
            }
            else{
                Grid[h][w] = symbolEmpty;
            }
        }
    }
}

// =================================== GAME LOOP ========================================

void gameFoodSetup(){
    if(foodHeight == -1 && foodWidth == -1){
        srand(time(NULL));
        bool foodCreated = false;
        do{
            int tempH = (rand() % (height - 2)) + 1;
            int tempW = (rand() % (width - 2)) + 1;
            if(Item[tempH][tempW] == 0){
                Item[tempH][tempW] = -1;
                foodCreated = true;
                foodHeight = tempH;
                foodWidth = tempW;
            }
        }while(!foodCreated);
    }
}

void playerMovement(){
    keyPress = ' ';
    for(int i = 0; i <= gameSpeed; i+=50){
        Sleep(50);
        if (_kbhit()){
            keyPress = _getch();
        }
    }

    // Check for keybinds
    if(keyPress != ' '){
        if((keyPress == 'w') || (keyPress == 'a') || (keyPress == 's') || (keyPress == 'd') || (keyPress == 'x')){
            if(lastKeyPress != ' '){
                if(foodCollected > 1){
                    if(lastKeyPress == 'w' && keyPress != 's' || lastKeyPress == 's' && keyPress != 'w' || lastKeyPress == 'a' && keyPress != 'd' || lastKeyPress == 'd' && keyPress != 'a'){
                        lastKeyPress = keyPress;
                    }
                }
                else{
                    lastKeyPress = keyPress;
                }
            }
            else{
                lastKeyPress = keyPress;
            }
            if(keyPress == 'x'){
                gameEndMessage("forced", "=> Welcome to TSG (Terminal Snake Game) developed by Nixhekoo!\n=> Enjoy your stay and make sure to frequently check new versions on GitHub! (github.com/nixhekoo)\n\n");
                alive = false;
            }
        }
    }

    // Move Player
    if(lastKeyPress == 'w'){
        playerHeight--;
    }
    else if(lastKeyPress == 's'){
        playerHeight++;
    }
    else if(lastKeyPress == 'a'){
        playerWidth--;
    }
    else if(lastKeyPress == 'd'){
        playerWidth++;
    }

    // Check if collided with food
    if (Item[playerHeight][playerWidth] == -1){
        foodCollected++;
        score+= gameScorePerFood;
        foodHeight = -1;
        foodWidth = -1;
    }

    // Checks if player is touching edge
    if(playerHeight == 0 || playerHeight == (height - 1) || playerWidth == 0 || playerWidth == (width - 1)){
        gameEndMessage("death", "You Died by ramming into the wall!\nBe careful next time!\n\n");
    }

    // Check for collision to myself
    if (Item[playerHeight][playerWidth] < foodCollected && Item[playerHeight][playerWidth] > 1){
        gameEndMessage("death", "You Died by ramming into yourself!\nWatch out next time!\n\n");
    }

    // Manipulate Item Array
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            if(Item[h][w] > 0){
                Item[h][w]++;
            }
            if(Item[h][w] > foodCollected){
                Item[h][w] = 0;
            }
        }
    }

    Grid[playerHeight][playerWidth] = symbolPlayerHead;
    Item[playerHeight][playerWidth] = 1;
}

void updateGrid(){
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            if(Item[h][w] == -2){
                Grid[h][w] = symbolBarrier;
            }
            if(Item[h][w] == -1){
                Grid[h][w] = symbolFood;
            }
            if(Item[h][w] == 0){
                Grid[h][w] = symbolEmpty;
            }
            if(Item[h][w] == 1){
                Grid[h][w] = symbolPlayerHead;
            }
            if(Item[h][w] > 1 && Item[h][w] < foodCollected){
                Grid[h][w] = symbolPlayerBody;
            }
            if(Item[h][w] > 1 && Item[h][w] == foodCollected){
                Grid[h][w] = symbolPlayerTail;
            }
        }
    }
}

void drawGrid(){
    clearConsole();
    for(int h = 0; h < height; h++){
        for(int w = 0; w < width; w++){
            std::cout << Grid[h][w];
        }
        if(h != height - 1){
            std::cout << '\n';
        }
    }
    std::cout << "\n\nKeybinds:" << "\n[WASD] Movement" << "\n[X] Return to Menu\n\nScore: " << score << "\nHighscore: " << highscore << "\n";
}

// ===================================== WIN ============================================

void winCheck(){
    int winscore = ((height - 2) * (width - 2) - 1);
    if(foodCollected == winscore){
        clearConsole();
        system("cls");
        std::cout << "YOU WIN!\nYour score: " << score << "\nCongratulations!\n\n> ";
        Sleep(4000);
        system("pause");
        alive = false;
    }
}

// ===================================== MAIN ===========================================

int main(){

    while(1 == 1){
        restartGame();
        gameVariableSetup();
        gameGridBackgroundSetup();
        gameFoodSetup();
        drawGrid();

        while(alive){
            playerMovement();
            gameFoodSetup();
            updateGrid();
            drawGrid();
            winCheck();
        }
    }
}