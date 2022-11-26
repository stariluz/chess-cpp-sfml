#ifndef CHESSSCREENS_H_INCLUDED
#define CHESSSCREENS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include <iostream>
#include <thread>
#include <sstream>
using namespace std;
using namespace sf;


struct ChessScreen{
    bool wasRun=false;
    virtual int Run(RenderWindow &window){
        return 0;
    }

    virtual void Pause(){
        return;
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
        if(!wasRun){
            /*
                Code to run when is the first run, and is needed to start some variables just one time
            */
            wasRun=true;
            music.play();
            music.setLoop(true);
            // Ejecutamos un thread con el timer
            timer_thread->launch();
        }else{
            /*
                Code to run when the game was paused, and now, will to continue
            */
        }
        bool running = true;

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
                    exit(1);
                }
                else if (event.type == Event::MouseButtonPressed)
                {
                    return 0;
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

    virtual void Pause(){
        // cout<<"PAUSAAAAAAAAAAAAAAAAAAAA";
        music.pause();
        // timer_thread->wait();
    }
};

struct ChessMenuScreen : public ChessScreen{
    Music music;
    Event event;
    Texture texture;
    Sprite sprite;

    ChessMenuScreen(){
        if (!music.openFromFile("./assets/sounds/MenuMusic.ogg")){
            exit(1);
        }
    }
    virtual int Run(RenderWindow &window){
        if(!wasRun){
            /*
                Code to run when is the first run, and is needed to start some variables just one time
            */
            wasRun=true;
            music.play();
            music.setLoop(true);

            if (!texture.loadFromFile("./assets/chess_game1.png"))
            {
                exit(1); //TODO: add exception
            }
            sprite.setTexture(texture);
        }else{
            /*
                Code to run when the game was paused, and now, will to continue
            */
        }

        bool running = true;

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

    virtual void Pause(){
        music.pause();
    }
};
//Pasar_a_la_ventana_de_eventos= RenderStates opcion[4];
//for (int i= 0; i<4; i++){
//        option[i]= new RectangleShape({100,50});
//        otipon[i]->setFillColor(color::black);
//        txt_editor[i]= new Text("", *Fuente);
//        txt_editor[i]->setCharacterSize(20);
//}
//txt_editor[0]->setPosition({139,471});
//txt_editor[1]->setPosition({219,471});
//txt_editor[2]->setPosition({120,211});
//txt_editor[3]->setPosition({100,1000});
//
//txt_editor[0]->setString("No. de peones");
//txt_editor[1]->setString("Tiempo por turno");
//txt_editor[2]->setString("JUGAR");
//txt_editor[3]->setString("Salir");
//
//option[0]->setPosition({130,470});
//option[1]->setPosition({215,470});
//option[2]->setPosition({130,210});
//option[3]->setPosition({215,1150});
//
//void editor::Render_Menu(){
//    window->clear();
//    for (int i= 0; i<4; i++){
//        window->daw(*option[i]);
//        window->daw(*txt_editor[i]);
//    }
//    if(window->hasFocus()){
//            Cursor.renderizar(*window);
//    }
//    window->display();
//}
//fuente= new Font;
//fuente->loadFromFile("./assets/Fuente.ttf");
//struct button{
//    Font* fuente;
//    Text* txt_editor[4];
//    RenderWindow* window;
//    RectangleShape* option[4];
//    RectangleShape button;
//    string name;
//};

#endif // CHESSSCREENS_H_INCLUDED
