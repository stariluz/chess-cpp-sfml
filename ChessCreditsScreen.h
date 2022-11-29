#ifndef CHESSCREDITSSCREEN_H_INCLUDED
#define CHESSCREDITSSCREEN_H_INCLUDED


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

struct ChessCreditsScreen : public ChessScreen{
    static const string CREDITS_FILENAME;
    static const Texture texture;
    Sprite sprite;
    Music music;
    Event event;

    ChessCreditsScreen(){
        sprite=Sprite(texture);
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
                        return CHESS_MENU_SCREEN_NUMBER;
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
const string ChessCreditsScreen::CREDITS_FILENAME="./assets/credits.png";

// Abrir la image del menú de piezas
const Texture ChessCreditsScreen::texture=loadResource(ChessCreditsScreen::CREDITS_FILENAME);

#endif // CHESSCREDITSSCREEN_H_INCLUDED
