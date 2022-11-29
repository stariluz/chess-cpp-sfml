#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include "ChessGameScreen.h"
#include "ChessMenuScreen.h"
#include "ChessPauseScreen.h"
#include "ChessCreditsScreen.h"
#include "ChessExceptions.h"
#include <iostream>

using namespace std;
using namespace sf;


//Variables estaticas para el tamaño de la ventana
static const int WINDOW_HORIZONTAL_SIZE=1067;

//Variables estaticas para el tamaño de la ventana
static const int WINDOW_VERTICAL_SIZE=ChessCoord::SIZE*8;

int main()
{

    //Configuracion de la ventana del juego
    RenderWindow window(VideoMode(WINDOW_HORIZONTAL_SIZE, WINDOW_VERTICAL_SIZE), "Chess");

    ChessScreen** screens=new ChessScreen*[5];
    screens[0]=new ChessScreen(); // Pantalla vacía, no hace nada
    screens[1]=new ChessMenuScreen(); // Pantalla 1
    screens[2]=new ChessGameScreen(); // Pantalla 2
    screens[3]=new ChessPauseScreen(); // Pantalla 3
    screens[4]=new ChessCreditsScreen(); // Pantalla 4

    int currentScreen=1;
    int nextScreen=currentScreen;

    while(currentScreen>=1){
        nextScreen=screens[currentScreen]->Run(window);
        if(nextScreen!=currentScreen){
            screens[currentScreen]->Pause();
            currentScreen=nextScreen;
        }
    }
    for(int i=0; i<=4; i++){
        delete screens[i];
    }
    delete[] screens;
    return 0;
}
