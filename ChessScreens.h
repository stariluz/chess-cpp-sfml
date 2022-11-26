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

const IntRect ChessBoard::LIMITS=IntRect(0, 0, ChessCoord::SIZE*8, ChessCoord::SIZE*8);

struct ChessScreen{
    static const int CLICK_ON_BUTTONS=1;
    static const int CLICK_ON_NOTHING=0;
    bool wasRun=false;
    static const int SCREEN_NUMBER;
    virtual int Run(RenderWindow &window){
        return 0;
    }

    virtual void Pause(){
        return;
    }
};

struct ChessGameScreen : public ChessScreen{
    static const int SCREEN_NUMBER;
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

    virtual int Run(RenderWindow &window);

    virtual void Pause(){
        // cout<<"PAUSAAAAAAAAAAAAAAAAAAAA";
        music.pause();
        // timer_thread->wait();
    }

    static const int CLICK_ON_BOARD=2;
    int onClick(Event::MouseButtonEvent mouse){
        /**
            Funcion que busca la acci�n realizada al dar click en la pantalla de juego

            C�digos de regreso:
            CLICK_ON_BUTTONS = 2;
            CLICK_ON BOARD = 1 Usado cuando el click es en el tablero de juego
            CLICK_ON_NOTHING = 0 Usado cuando no se clique� nada interactuable

        */
        if(ChessBoard::LIMITS.contains(mouse.x,mouse.y)){
            if(!ChessGame::onClick(mouse.x, mouse.y)){
                return CLICK_ON_NOTHING;
            }
            return CLICK_ON_BOARD;
        }else{
            return CLICK_ON_NOTHING;
        }
    }
};
struct botton{
    RectangleShape r;
    string name;
    boton(string identidad, RectangleShape rect){
        name=identidad;
        r= rect;
    }
};
struct ChessMenu{
    Font* fuente;
    Text* txt_editor=NULL;
    RectangleShape* option=NULL;
    RenderWindow* window;
    RectangleShape button;
    string name;
    ChessMenu(){
        fuente= new Font();
        fuente->loadFromFile("./assets/Fuente.ttf");
        option = new RectangleShape[4];
        txt_editor = new Text[4];
        for (int i= 0; i<4; i++){
            txt_editor[i]= Text("", *fuente);
        }
        option[0]= RectangleShape({300,60});
        option[1]= RectangleShape({300,60});
        option[2]= RectangleShape({250,70});
        option[3]= RectangleShape({160,50});

        option[0].setFillColor(Color::Black);
        option[1].setFillColor(Color::Black);
        option[2].setFillColor(Color::White);
        option[3].setFillColor(Color::Red);

        option[0].setPosition({100,375});
        option[1].setPosition({100,445});
        option[2].setPosition({180,590});
        option[3].setPosition({890,700});

        txt_editor[0].setColor(Color::White);
        txt_editor[1].setColor(Color::White);
        txt_editor[2].setColor(Color::Black);
        txt_editor[3].setColor(Color::White);

        txt_editor[0].setCharacterSize(50);
        txt_editor[1].setCharacterSize(50);
        txt_editor[2].setCharacterSize(100);
        txt_editor[3].setCharacterSize(55);

        txt_editor[0].setPosition({120,360});
        txt_editor[1].setPosition({120,430});
        txt_editor[2].setPosition({213,545});
        txt_editor[3].setPosition({917,683});

        txt_editor[0].setString("No. de peones");
        txt_editor[1].setString("Tiempo por turno");
        txt_editor[2].setString("JUGAR");
        txt_editor[3].setString("Salir");

    }
    void renderMenu(RenderWindow &window){
        for (int i= 0; i<4; i++){
            window.draw(option[i]);
            window.draw(txt_editor[i]);
        }
    }
};
struct ChessMenuScreen : public ChessScreen{
    static const int SCREEN_NUMBER;
    Music music;
    Event event;
    Texture texture;
    Sprite sprite;
    ChessMenu menu;
    list<botton> option;
    ChessMenuScreen(){
        if (!music.openFromFile("./assets/sounds/MenuMusic.ogg")){
            exit(1);
        }
        if (!texture.loadFromFile("./assets/chess_game2.png"))
        {
            exit(1); //TODO: add exception
        }
        sprite.setTexture(texture);
    }
    virtual int Run(RenderWindow &window){
        if(!wasRun){
            /*
                Code to run when is the first run, and is needed to start some variables just one time
            */
            wasRun=true;
            music.play();
            music.setLoop(true);

        }else{
            /*
                Code to run when the game was paused, and now, will to continue
            */
        }

        bool running = true;

        while (window.isOpen())
        {
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
                            cout<<"Click";
                            return ChessGameScreen::SCREEN_NUMBER;
//                            vector2i posicion_mouse;
//                            posicion_mouse=Mouse::getPosition(window);
//                            for (int i= 0; i<4; i++){
//                                if (option[i]->getGlobalBounds().contains(posicion_mouse.x, posicion_mouse.y)){
//                                    switch(i){
//                                        case 0:
//
//                                        case 1:
//
//                                        case 2:
//
//                                        case 3:
//
//                                        case 4:
//                                            exit(1);
//                                    }
//                                }
//                            }
                        }
                        break;

                }
            }
            window.clear();
            window.draw(sprite);
            menu.renderMenu(window);
            window.display();
        }
    }

    virtual void Pause(){
        music.pause();
    }
};


/* IMPLEMENTATIONS */

int ChessGameScreen::Run(RenderWindow &window){
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
//                    return 0;
                if (event.mouseButton.button == Mouse::Left)
                {
                    int operation=onClick(event.mouseButton);
                    if(operation==CLICK_ON_NOTHING){
                        return ChessMenuScreen::SCREEN_NUMBER;
                    }
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

#endif // CHESSSCREENS_H_INCLUDED
