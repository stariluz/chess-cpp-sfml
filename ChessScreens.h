#ifndef CHESSSCREENS_H_INCLUDED
#define CHESSSCREENS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
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

/*
    Estructura del juego, maneja la seleccion de piezas y las mecanicas de seleccion
    incluye el metodo de desplazamiento de pieza
*/
struct ChessGame{
    static int status;
    static ChessPiece* selectedPiece;
    static ChessPiece stateSelectedPiece;

    static const int STATUS_IDLE=0; // Estado del juego sin actividad
    static const int STATUS_SELECTED=1; // Estado del juego con selección de una pieza
    static const int STATUS_RELEASED=2; // Estado del juego al terminar de mover una pieza
    static const int STATUS_EATED=3; // Estado del juego al ser comida una pieza

    // Detectar donde ocurrió el click y actuar según qué se clickeo
    static int onClick(int x, int y){
        /**
            Evento de click sobre el tablero de juego

            CODIGOS DE RETORNO:
            STATUS_IDLE = 0
            STATUS_SELECTED = 1
            STATUS_RELEASED = 2
            STATUS_EATED = 3
        */

        // cout << "\nButton pressed" << endl; // DEV
        // cout << "mouse x: " << x << endl; // DEV
        // cout << "mouse y: " << y << endl; // DEV
        if (status==STATUS_IDLE || status==STATUS_SELECTED)
        {

            ChessPiece** playerPiecesInCoord=NULL;
            ChessPiece** rivalPiecesInCoord=NULL;
            ChessPiece* newClickedPiece=NULL;

            playerPiecesInCoord=ChessPlayer::getPiecesOfPlayerInTurnAtPosition(x,y);
            rivalPiecesInCoord=ChessPlayer::getPiecesOfRivalInTurnAtPosition(x,y);


            if(playerPiecesInCoord[1]!=NULL){
                /** Click en una casilla con pieza propia */
                // cout<<"2 PIECES"; //DEV
                if(playerPiecesInCoord[0] != selectedPiece){
                    newClickedPiece=playerPiecesInCoord[0];
                }else{
                    newClickedPiece=playerPiecesInCoord[1];
                }

                /** Se resetea la pieza seleccionada anterior, y se selecciona la nueva pieza clickeada*/
                if(selectedPiece!=NULL && status==STATUS_SELECTED){
                    // TODO: Error de movimiento inválido
                    *selectedPiece=stateSelectedPiece;
                    selectedPiece=newClickedPiece;
                    stateSelectedPiece=*selectedPiece;
                    // cout<<"SELECTED NEW\n"; // DEV
                    return STATUS_SELECTED;
                }
            }
            else if(playerPiecesInCoord[0]!=NULL&&rivalPiecesInCoord[0]!=NULL){
                /** Click en una casilla con pieza rival */

                /** Se come una pieza rival */
                status = STATUS_IDLE;
                ChessPlayer::eatPieceRival(rivalPiecesInCoord[0]);
                return STATUS_EATED;
            }
            else if(playerPiecesInCoord[0]!=NULL){
                /** Click en una casilla sin pieza rival */

                // cout<<"1 PIECE"; // DEV
                newClickedPiece=playerPiecesInCoord[0];

                /** Se coloca la pieza seleccionada*/
                if(selectedPiece!=NULL && status==STATUS_SELECTED)
                {
                    status = STATUS_IDLE;
                    selectedPiece = NULL;
                    // cout<<"RELEASED\n"; // DEV
                    return STATUS_RELEASED;
                }

                /** Se selecciona la pieza clickeada */
                else{
                    selectedPiece=newClickedPiece;
                    stateSelectedPiece=*selectedPiece;
                    status = STATUS_SELECTED;
                    cout<<"SELECTED: "<<*selectedPiece<<"\n"; // DEV
                    return STATUS_SELECTED;
                }
            }

        }
        status = STATUS_IDLE;
        return STATUS_IDLE;
    }

    // Posiciona la pieza sobre la casilla de ajedrez en la que se encuentran los pixeles recibidos.
    static void dragPiece(int pxX, int pxY){
        if(selectedPiece == NULL )return;
        selectedPiece->setPosition(ChessCoord::getChessPosition(pxX, pxY));
    }
};

// Estado inicial de el juego, nada está pasando así que es idle
int ChessGame::status=ChessGame::STATUS_IDLE;

// Inicializa en ChessGame la pieza selecionada como nula
ChessPiece* ChessGame::selectedPiece=NULL;

ChessPiece ChessGame::stateSelectedPiece;

struct ChessGameScreen : public ChessScreen{
    static const int SCREEN_NUMBER;
    Music music;
    int windowHeight;
    int windowWidth;
//    ChessPiece** whitePieces;
//    ChessPiece** blackPieces;

    ChessBoard board;
    Event event;

    int current_time;
    Game_Timer *s=NULL;
    Thread *timer_thread=NULL;

    ChessGameScreen(){
        if (!music.openFromFile("./assets/sounds/BackgroundMusic.ogg"))
            throw BackgroundMusicException();
            cout<<"ERROR";// TODO: exception image

//        // Reserva de espacio para piezas blancas y negras
//        whitePieces = new ChessPiece*[8];
//        blackPieces = new ChessPiece*[8];
//
//        // Cargar posiciones iniciales de las piezas
//        initPieces(whitePieces, blackPieces);
        ChessPlayer::players=new ChessPlayer[2];
        ChessPlayer::players[0]=ChessPlayer::createPlayer(ChessPiece::COLOR_WHITE);
        ChessPlayer::players[1]=ChessPlayer::createPlayer(ChessPiece::COLOR_BLACK);

        //Inicializamos un contador
        current_time=0;
        s=new Game_Timer(current_time);
        timer_thread=new Thread(ref(*s));
        //TODO: Crear una variable para establecer el tiempo actual y el limite de tiempo
    }

    virtual int Run(RenderWindow &window);

    virtual void Pause(){
        // cout<<"PAUSAAAAAAAAAAAAAAAAAAAA";
        music.sf::SoundStream::pause();
        // timer_thread->wait();
    }

    static const int CLICK_ON_BOARD=2;
    int onClick(Event::MouseButtonEvent mouse){
        /**
            Funcion que busca la acci�n realizada al dar click en la pantalla de juego

            Codigos de regreso:
            CLICK_ON_BUTTONS = 2 Usado cuando se da click a los botones de salir y de pausa
            CLICK_ON BOARD = 1 Usado cuando el click es en el tablero de juego
            CLICK_ON_NOTHING = 0 Usado cuando no se clique� nada interactuable

        */
        int operation=0;
        if(ChessBoard::LIMITS.contains(mouse.x,mouse.y)){
            operation=ChessGame::onClick(mouse.x, mouse.y);
            if(operation==ChessGame::STATUS_IDLE){
                return CLICK_ON_NOTHING;
            }
            else if(operation==ChessGame::STATUS_RELEASED){
                ChessPlayer::updatePlayerInTurn();
            }
            return CLICK_ON_BOARD;
        }else{
            return CLICK_ON_NOTHING;
        }
    }
};
//ChessPlayer* ChessGameScreen::players=NULL;

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
        fuente = new Font();
        fuente->loadFromFile("./assets/Fuente.ttf");
        option = new RectangleShape[8];
        txt_editor = new Text[4];
        for (int i= 0; i<4; i++){
            txt_editor[i]= Text("", *fuente);
        }
        option[0]= RectangleShape({200,60});
        option[1]= RectangleShape({100,60});
        option[2]= RectangleShape({250,70});
        option[3]= RectangleShape({160,50});

        option[0].setFillColor(Color::White);
        option[1].setFillColor(Color::White);
        option[2].setFillColor(Color::White);
        option[3].setFillColor(Color::Red);

        option[0].setPosition({100,375});
        option[1].setPosition({100,445});
        option[2].setPosition({180,590});
        option[3].setPosition({890,700});

        txt_editor[0].setColor(Color::Black);
        txt_editor[1].setColor(Color::Black);
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

        txt_editor[0].setString("Black");
        txt_editor[1].setString("<");
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
            throw MenuMusicException();
            exit(1);
        }
        if (!texture.loadFromFile("./assets/chess-game5.png"))
        {
            throw ChessImageException();
            exit(1);
        }
        sprite.setTexture(texture);
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
        music.play();
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
                            return ChessGameScreen::SCREEN_NUMBER;
                            Vector2i posicion_mouse;
                            posicion_mouse=Mouse::getPosition(window);
                            for (int i= 0; i<4; i++){
//                                if (option[i]->getGlobalBounds().Contains(posicion_mouse.x, posicion_mouse.y)){
                                    switch(i){
                                        case 0:
                                            break;

                                        case 1:
                                            break;

                                        case 2:
                                            break;

                                        case 3:

                                            break;

                                        case 4:
                                            exit(1);
                                            break;
                                    }
                                }
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
        music.sf::SoundStream::pause();
    }
};


/* IMPLEMENTATIONS */

int ChessGameScreen::Run(RenderWindow &window){
    if(!wasRun){
        /**
            Codigo a ejecutar solamente en la primer llamada al Run
            El codigo aqui se usa en caso de requerir acciones que se ejecuten 1 sola ocasión en la primer llamada.
        */
        wasRun=true;
        music.setLoop(true);
        // Ejecutamos un thread con el timer
        timer_thread->launch();
    }else{
        /**
            Codigo a ejecutar siempre despues de la primera llamada a Run.
            Aquí debe ir todo código que reanude las acciones que el metodo Pause haya detenido.
        */
    }

    music.play();

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
//                exit(1);
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
            else if (event.type == Event::MouseMoved && ChessGame::status==ChessGame::STATUS_SELECTED)
            {
                ChessGame::dragPiece(event.mouseMove.x, event.mouseMove.y);
            }
        }

        window.clear();

        /*
            Aqui se puede introducir cualquier cosa que se quiera dibujar
        */

        window.draw(board.sprite);
        for(int i=0; i<8; i++){
            try{
                window.draw((*ChessPlayer::players[0].pieces[i]).sprite);
                window.draw((*ChessPlayer::players[1].pieces[i]).sprite);
            }catch(...){
                cout<<"ERROOOOOOOOOOOOOOOOOR: NO HAY SPRITE\n";
            }
        }

        if(s->eneable == true)
        {
            window.draw(s->clock_sprite);
            s->hand_timer_sprite.setOrigin(sf::Vector2f(50,50));
            s->hand_timer_sprite.setPosition(sf::Vector2f(50,50));
            s->hand_timer_sprite.setRotation(s->degrees*s->current_time);
            window.draw(s->hand_timer_sprite);
        }

        window.display();
    }
    return -1;
}

#endif // CHESSSCREENS_H_INCLUDED
