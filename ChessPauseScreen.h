#ifndef CHESSPAUSESCREEN_H_INCLUDED
#define CHESSPAUSESCREEN_H_INCLUDED


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

struct ChessPauseScreen : public ChessScreen{
    static const string PAUSE_FILENAME;
    static const Texture PauseTexture;
    Sprite sprite;
    Music music;
    Event event;

    ChessPauseScreen(){
        sprite=Sprite(PauseTexture);
        if (!music.openFromFile("./assets/sounds/PauseMusic.ogg")){
            throw BackgroundMusicException();
        }
    }

    virtual void Pause(){
        music.sf::SoundStream::pause();
    }

    virtual int Run(RenderWindow &window){
        if(!wasRun){
            /**
                Codigo a ejecutar solamente en la primer llamada al Run
                El codigo aqui se usa en caso de requerir acciones que se ejecuten 1 sola ocasión en la primer llamada.
            */
            wasRun=true;
            music.setLoop(true);
        }else{
            /**
                Codigo a ejecutar siempre despues de la primera llamada a Run.
                Aquí debe ir todo código que reanude las acciones que el metodo Pause haya detenido.
            */
        }
        music.setVolume(10.f);
        music.play();

        bool running = true;

        while (running)
        {
        /*
            Bucle principal del juego
        */
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
    //                exit(1);
                }
                else if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        // El boton presionado es el click izquierdo
                        return CHESS_GAME_SCREEN_NUMBER;
                    }
                }
            }
            window.clear();

            window.draw(sprite);
            window.display();
        }
        return -1;
    }
};

// Constante para la ubicación de la imagen del menu de pausa
const string ChessPauseScreen::PAUSE_FILENAME="./assets/pause_screen.png";

// Abrir la image del menú de piezas
const Texture ChessPauseScreen::PauseTexture=loadResource(ChessPauseScreen::PAUSE_FILENAME);


#endif // CHESSPAUSESCREEN_H_INCLUDED
