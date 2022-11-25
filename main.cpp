#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
static const int WINDOW_HORIZONTAL_SIZE=ChessCoord::SIZE*8;

//Variables estaticas para el tamaño de la ventana
static const int WINDOW_VERTICAL_SIZE=ChessCoord::SIZE*8;

int main()
{
    
    // music.setLoop(true);
    //Configuracion de la ventana del juego
    RenderWindow window(VideoMode(WINDOW_HORIZONTAL_SIZE, WINDOW_VERTICAL_SIZE), "Chess");

    vector<ChessScreen*> screens;
    screens.push_back(new ChessMenuScreen()); // 0
    screens.push_back(new ChessGameScreen()); // 1
    int screenNumber=0;

    while(screenNumber>=0){
        screenNumber=screens[screenNumber]->Run(window);
    }
    return 0;
}

//struct button{
//    RectangleShape button;
//    Text text;
//    button(){
//    }
//    button(string t, vector2f size, Color bgcolor, Color textcolor){
//        text.setString(t);
//        botton
//    }
//};

    // sf::Music musica;
    // if (!musica.openFromFile("./assets/sounds/MenuMusic.ogg"))
    //     exit(1);

    // musica.play();
    // musica.setLoop(true);