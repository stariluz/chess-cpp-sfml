#ifndef CHESSSCREENS_H_INCLUDED
#define CHESSSCREENS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include <iostream>
#include <thread>
using namespace std;
using namespace sf;


struct ChessScreen{
    virtual int Run(RenderWindow &window){
        return 0;
    }

    virtual int Pause(RenderWindow &window){
        return 0;
    }
};

struct ChessGameScreen : public ChessScreen{
    Music music;
    int windowHeight;
    int windowWidth;
    ChessPiece** whitePieces;
    ChessPiece** blackPieces;
    ChessBoard board;
    Event event;

    int current_time;
    Game_Timer *s=NULL;
    Thread *timer_thread=NULL;

    ChessGameScreen(){
        if (!music.openFromFile("./assets/sounds/BackgroundMusic.ogg"))
            cout<<"ERROR";// TODO: exception image

        // Reserva de espacio para piezas blancas y negras
        whitePieces = new ChessPiece*[8];
        blackPieces = new ChessPiece*[8];

        // Cargar posiciones iniciales de las piezas
        initPieces(whitePieces, blackPieces);


        //Inicializamos un contador
        current_time=0;
        s=new Game_Timer(current_time);
        timer_thread=new Thread(ref(*s));
        //TODO: Crear una variable para establecer el tiempo actual y el limite de tiempo
    }

    //Metodo para inicializar las piezas
    void initPieces(ChessPiece**& whitePieces, ChessPiece**& blackPieces) {
        for (int i = 0; i < 6; i++) {
            whitePieces[i]=ChessPiece::createPiece(
                ChessCoord(i + 1, 1), i, 0
            );
            blackPieces[i]=ChessPiece::createPiece(
                ChessCoord(i + 1, 2), i, 1
            );
        }
    }

    virtual int Run(RenderWindow &window){
        bool running = true;
        music.play();

        // Ejecutamos un thread con el timer
        timer_thread->launch();

        while (running)
        {
        /*
            Bucle principal del juego
            Aqui se manejan los eventos de la ventana
            Eventos del juego como los clicks
            Y el dibujado de la pantalla
        */

            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                else if (event.type == Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == Mouse::Left)
                    {
                        ChessGame::onClick(event.mouseButton.x, event.mouseButton.y);
                    }
                }
                else if (event.type == Event::MouseMoved && ChessGame::status=="selected")
                {
                    ChessGame::dragPiece(event.mouseMove.x, event.mouseMove.y);
                }
            }

            window.clear();

            /// Seccion de dibujo en la pantalla --------->
            /*
                Aqui se puede introducir cualquier cosa que se quiera dibujar
            */

            window.draw(board.sprite);
            for (int i = 0; i < 6; i++) {

                window.draw((*whitePieces[i]).sprite);
                window.draw((*blackPieces[i]).sprite);
            }

            if(s->eneable == true)
            {
                window.draw(s->clock_sprite);
                s->hand_timer_sprite.setOrigin(sf::Vector2f(50,50));
                s->hand_timer_sprite.setPosition(sf::Vector2f(50,50));
                s->hand_timer_sprite.setRotation(s->degrees*s->current_time);
                window.draw(s->hand_timer_sprite);
            }

            /// <--------- Final de la seccion de dibujo
            window.display();
        }
        return -1;
    }

};

struct ChessMenuScreen : public ChessScreen{
    virtual int Run(RenderWindow &window){
        bool running = true;
        Event event;
        Texture texture;
        if (!texture.loadFromFile("./assets/chess-game.jpg"))
        {
            exit(1); //TODO: add exception
        }
        Sprite sprite;
        sprite.setTexture(texture);
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                    case (Event::Closed):
                        window.close();
                        break;
                    case (Event::MouseButtonPressed):
                        if (event.mouseButton.button == Mouse::Left)
                        {
                            cout<<"CLICK BUAJUAJUA\n";
                            return 1;
                        }
                        break;

                }
            }
            window.clear();
            window.draw(sprite);
            window.display();
        }
    }
};

#endif // CHESSSCREENS_H_INCLUDED
