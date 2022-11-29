#ifndef CHESSSCREENS_H_INCLUDED
#define CHESSSCREENS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include "ChessExceptions.h"
#include "ChessTimer.h"
#include <iostream>

using namespace std;
using namespace sf;

const int CHESS_MENU_SCREEN_NUMBER=1;
const int CHESS_GAME_SCREEN_NUMBER=2;

struct ChessScreen{
    static const int CLICK_ON_NOTHING=0;
    static const int CLICK_ON_BUTTONS=1;
    bool wasRun=false;
    virtual int Run(RenderWindow &window){
        wasRun=true;
        return 0;
    }

    virtual void Pause(){
        return;
    }
};

struct ChessPauseScreen : public ChessScreen{
};

#endif // CHESSSCREENS_H_INCLUDED
