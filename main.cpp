#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

/// INICIALIZAR DATOS DEL HEADER "CHESS:H"

// Constante para la ubicación de la imagen del tablero
const string Chess::BOARD_SPRITESHEET_FILENAME="./assets/chess_board.jpg";

// Constante para la ubicación del spritesheet de piezas
const string Chess::PIECES_SPRITESHEET_FILENAME="./assets/pieces_spritesheet.png";

// Constante para la unidad en pixeles
const int ChessCoord::SIZE=100;

// Abrir el spritesheet de piezas
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME);

//Variables estaticas para el tamaño de la ventana
static const int WINDOW_HORIZONTAL_SIZE=1067;

//Variables estaticas para el tamaño de la ventana
static const int WINDOW_VERTICAL_SIZE=ChessCoord::SIZE*8;

// Asignación de el numero de las pantallas según el orden
const int ChessMenuScreen::SCREEN_NUMBER=1;
const int ChessGameScreen::SCREEN_NUMBER=2;

int main()
{

    //Configuracion de la ventana del juego
    RenderWindow window(VideoMode(WINDOW_HORIZONTAL_SIZE, WINDOW_VERTICAL_SIZE), "Chess");

    vector<ChessScreen*> screens;
    screens.push_back(new ChessScreen()); // Empty screen, it doesn't do anything
    screens.push_back(new ChessMenuScreen()); // Screen no. 1
    screens.push_back(new ChessGameScreen()); // Screen no. 2
    int currentScreen=1;
    int nextScreen=currentScreen;

    while(currentScreen>=1){
        nextScreen=screens[currentScreen]->Run(window);
        if(nextScreen!=currentScreen){
            screens[currentScreen]->Pause();
            currentScreen=nextScreen;
        }
    }
    return 0;
}
