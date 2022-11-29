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

/**
    Estructura del juego, maneja la seleccion de piezas y las mecanicas de seleccion
    incluye el metodo de desplazamiento de pieza
*/
struct ChessGame{
    static int status;
    static ChessPiece* selectedPiece;
    static ChessPiece stateSelectedPiece;
    static list<ChessCoord> avaiableMovements;

    static const int STATUS_IDLE=0; // Estado del juego sin actividad
    static const int STATUS_SELECTED=1; // Estado del juego con selección de una pieza
    static const int STATUS_RELEASED=2; // Estado del juego al terminar de mover una pieza
    static const int STATUS_EATED=3; // Estado del juego al ser comida una pieza
    static const int STATUS_PUT_BACK=4; // Estado del juego al ser comida una pieza

    // Detectar donde ocurrió el click y actuar según qué se clickeo
    static int onClick(int x, int y){
        /**
            Evento de click sobre el tablero de juego

            CODIGOS DE RETORNO:
            STATUS_IDLE = 0 // Estado de inactividad
            STATUS_SELECTED = 1 // Estado de seleccion de pieza
            STATUS_RELEASED = 2 // Estado de colocar pieza
            STATUS_EATED = 3 // Estado de comer pieza
        */

        ChessPiece** playerPiecesInCoord=NULL;
        ChessPiece** rivalPiecesInCoord=NULL;
        ChessPiece* newClickedPiece=NULL;

        playerPiecesInCoord=ChessPlayer::getPiecesOfPlayerInTurnAtPosition(x,y);
        rivalPiecesInCoord=ChessPlayer::getPiecesOfRivalInTurnAtPosition(x,y);


        if(playerPiecesInCoord[1]!=NULL){
            /** Click en una casilla con pieza propia */

            cout<<"CLICK EN CASILLA PROPIA\n"; //DEV

            if(playerPiecesInCoord[0] != selectedPiece){
                newClickedPiece=playerPiecesInCoord[0];
            }else{
                newClickedPiece=playerPiecesInCoord[1];
            }

            /** Se resetea la pieza seleccionada anterior, y se selecciona la nueva pieza clickeada*/
            if(selectedPiece!=NULL){
                restoreMovement();
                selectedPiece=newClickedPiece;
                stateSelectedPiece=*selectedPiece;
                calculateAvaiableMovements(selectedPiece);
                // cout<<"SELECTED NEW\n"; // DEV
                return STATUS_SELECTED;
            }
        }
        else if(playerPiecesInCoord[0]!=NULL && rivalPiecesInCoord[0]!=NULL){
            /** Click en una casilla con pieza rival */

            cout<<"CLICK EN CASILLA RIVAL\n"; //DEV

            /** Se come una pieza rival */
            status = STATUS_EATED;
            selectedPiece = NULL;
            ChessPlayer::eatPieceRival(rivalPiecesInCoord[0]);
            return STATUS_RELEASED;
        }
        else if(playerPiecesInCoord[0]!=NULL){
            /** Click en una casilla sin pieza rival */

            cout<<"CLICK EN CASILLA SIN RIVAL\n"; //DEV

            newClickedPiece=playerPiecesInCoord[0];
            if(selectedPiece!=NULL){
                /** La pieza fue colocada en la misma posición que estaba */
                if(*newClickedPiece==stateSelectedPiece){
                    cout<<"VOLVISTE AL INICIO CARNAL\n"; // DEV
                    restoreMovement();
                    status = STATUS_IDLE;
                    return STATUS_PUT_BACK;
                }


                /** Se resetea la pieza seleccionada anterior, y se selecciona la nueva pieza clickeada*/
                if(newClickedPiece != selectedPiece && selectedPiece!=NULL){
                    cout<<"CAMBIAS PIEZA CRACK\n"; // DEV
                    restoreMovement();
                    selectedPiece=newClickedPiece;
                    stateSelectedPiece=*selectedPiece;
                    calculateAvaiableMovements(selectedPiece);
                    status = STATUS_SELECTED;
                    // cout<<"SELECTED NEW\n"; // DEV
                    return STATUS_SELECTED;
                }


                /** Se coloca la pieza previamente seleccionada */
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
                calculateAvaiableMovements(selectedPiece);
//                cout<<"SELECTED: "<<*selectedPiece<<"\n"; // DEV

                return STATUS_SELECTED;
            }
        }

        status = STATUS_IDLE;
        return STATUS_IDLE;
    }

    static void calculateAvaiableMovements(ChessPiece* selectedPiece){
        ChessCoord aux;
        avaiableMovements.clear();
        avaiableMovements.push_back(selectedPiece->position);
//        cout<<"SELECTED: "<<selectedPiece->position<<" "<<selectedPiece->position.x<<"\n"; // DEV
        if(ChessPlayer::playerInTurn==1&&selectedPiece->position.y>1){
//            cout<<"PLAYER 1: "; //DEV
            aux=selectedPiece->position-ChessCoord(0,1);
//            cout<<aux<<" "; // DEV
            if(ChessPlayer::getPiecesOfPlayerInTurnAtPosition(aux)[0] == NULL
               && ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] == NULL){
                avaiableMovements.push_back(aux);
            }

            if(selectedPiece->position.valueX>1){
                aux=selectedPiece->position-ChessCoord(1,1);
//                cout<<"L"<<aux<<" "; // DEV
                if(ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] != NULL){
                    avaiableMovements.push_back(aux);
                }
            }

            if(selectedPiece->position.valueX<8){
                aux=selectedPiece->position+ChessCoord(1,0)-ChessCoord(0,1);
//                cout<<"R"<<aux<<" "; // DEV
                if(ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] != NULL){
                    avaiableMovements.push_back(aux);
                }
            }
        }
        else if(ChessPlayer::playerInTurn==0&&selectedPiece->position.y<8){
//            cout<<"PLAYER 2: "; // DEV
            aux=selectedPiece->position+ChessCoord(0,1);
//            cout<<aux<<" "; // DEV
            if(ChessPlayer::getPiecesOfPlayerInTurnAtPosition(aux)[0] == NULL
               && ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] == NULL){
                avaiableMovements.push_back(aux);
            }

            if(selectedPiece->position.valueX>1){
                aux=selectedPiece->position-ChessCoord(1,0)+ChessCoord(0,1);
//                cout<<"L"<<aux<<" "; // DEV
                if(ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] != NULL){
                    avaiableMovements.push_back(aux);
                }
            }

            if(selectedPiece->position.valueX<8){
                aux=selectedPiece->position+ChessCoord(1,1);
//                cout<<"R"<<aux<<" "; // DEV
                if(ChessPlayer::getPiecesOfRivalInTurnAtPosition(aux)[0] != NULL){
                    avaiableMovements.push_back(aux);
                }
            }
        }
        cout<<"AVAIABLE MOVEMENTS: "; // DEV
        for(ChessCoord movement:avaiableMovements){
            cout<<movement<<", ";
        } // DEV
        cout<<"\n"; // DEV
    }

    // Posiciona la pieza sobre la casilla de ajedrez en la que se encuentran los pixeles recibidos.
    static void dragPiece(int pxX, int pxY){
        if(selectedPiece == NULL )return;
        ChessCoord coordToMove=ChessCoord::getChessPosition(pxX, pxY);
        cout<<"SEARCH MOVEMENT:";
        for(ChessCoord movement:avaiableMovements){
            cout<<movement<<", ";
            if(movement==coordToMove){
                selectedPiece->setPosition(coordToMove);
                return;
            }
        }
        cout<<"\n";
    }

    static void restoreMovement(){
        if(selectedPiece==NULL)return;
        cout<<"VAN PA FUERA MI LOCO\n";
        avaiableMovements.clear();
        *selectedPiece = stateSelectedPiece;
        selectedPiece = NULL;
        status=STATUS_IDLE;
    }
};

// Estado inicial de el juego, nada está pasando así que es idle
int ChessGame::status=ChessGame::STATUS_IDLE;

// Inicializa en ChessGame la pieza selecionada como nula
ChessPiece* ChessGame::selectedPiece=NULL;

ChessPiece ChessGame::stateSelectedPiece;

list<ChessCoord> ChessGame::avaiableMovements;

struct ChessGameScreen : public ChessScreen{
    static const int SCREEN_NUMBER;
    Music music;
    int windowHeight;
    int windowWidth;

    ChessBoard board;
    Event event;

    int save_time;
    int current_time;
    Game_Timer *timer=NULL;
    Thread *timer_thread=NULL;

    ChessGameScreen(){
        if (!music.openFromFile("./assets/sounds/BackgroundMusic.ogg"))
            throw BackgroundMusicException();
            cout<<"ERROR";// TODO: exception image

//        whitePieces = new ChessPiece*[8];
//        blackPieces = new ChessPiece*[8];


        // Reserva de espacio para los jugadores y sus piecas
        ChessPlayer::players=new ChessPlayer[2];
        // TODO: Solicitar al menú el color del jugador 1, para crearlos en ese orden
        ChessPlayer::players[0]=ChessPlayer::createPlayer(ChessPiece::COLOR_WHITE);
        ChessPlayer::players[1]=ChessPlayer::createPlayer(ChessPiece::COLOR_BLACK);

        //Inicializamos un contador
        current_time=0;
        timer=new Game_Timer(current_time, MinuteSecond(0,10), ChessPlayer::updatePlayerInTurn);
        timer_thread=new Thread(
            ref( *timer )
        );
        //TODO: Crear una variable para establecer el tiempo actual y el limite de tiempo
    }

    /* Declaracion de la funcion Run*/
    virtual int Run(RenderWindow &window);

    virtual void Pause(){
        // cout<<"PAUSAAAAAAAAAAAAAAAAAAAA";
        music.sf::SoundStream::pause();
        ChessGame::restoreMovement();
        save_time=current_time;
        timer->stop();
        // timer_thread->wait();
    }

    /* Metodo que realiza toda la lógica necesaria para el cambio de turno */
//    void changeOfTurn(){
//        ChessPlayer::updatePlayerInTurn();
//        current_time=0;
//        timer=new Game_Timer(current_time, MinuteSecond(0,10), ref(changeOfTurn));
//    }
    static const int CLICK_ON_BOARD=2;
    int onClick(Event::MouseButtonEvent mouse){
        /**
            Funcion que busca la accion realizada al dar click en la pantalla de juego

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
            else if(operation==ChessGame::STATUS_RELEASED||operation==ChessGame::STATUS_EATED){
                ChessPlayer::updatePlayerInTurn();
            }
            return CLICK_ON_BOARD;
        }else{
            cout << "NO CLICKEASTE NAAAAAAAAAAAAAA";
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
        option = new RectangleShape[6];
        txt_editor = new Text[6];
        for (int i= 0; i<6; i++){
            txt_editor[i]= Text("", *fuente);
        }
        option[0]= RectangleShape({140,40});
        option[1]= RectangleShape({40,42});
        option[2]= RectangleShape({38,40});
        option[3]= RectangleShape({250,80});
        option[4]= RectangleShape({160,50});
        option[5]= RectangleShape({160,50});

        option[0].setFillColor(Color::White);
        option[1].setFillColor(Color::Black);
        option[2].setFillColor(Color::Black);
        option[3].setFillColor(Color::White);
        option[4].setFillColor(Color::Red);
        option[5].setFillColor(Color::White);

        option[0].setPosition({350,357});
        option[1].setPosition({335,355});
        option[2].setPosition({490,357});
        option[3].setPosition({180,600});
        option[4].setPosition({890,700});
        option[5].setPosition({890,100});

        txt_editor[0].setColor(Color::Black);
        txt_editor[1].setColor(Color::White);
        txt_editor[2].setColor(Color::White);
        txt_editor[3].setColor(Color::Black);
        txt_editor[4].setColor(Color::White);
        txt_editor[5].setColor(Color::Black);

        txt_editor[0].setCharacterSize(45);
        txt_editor[1].setCharacterSize(50);
        txt_editor[2].setCharacterSize(50);
        txt_editor[3].setCharacterSize(100);
        txt_editor[4].setCharacterSize(55);
        txt_editor[5].setCharacterSize(55);

        txt_editor[0].setPosition({395,341});
        txt_editor[1].setPosition({356,340});
        txt_editor[2].setPosition({494,339});
        txt_editor[3].setPosition({213,555});
        txt_editor[4].setPosition({917,683});
        txt_editor[5].setPosition({917,183});

        txt_editor[0].setString("Blanco");
        txt_editor[1].setString("<");
        txt_editor[2].setString(">");
        txt_editor[3].setString("JUGAR");
        txt_editor[4].setString("Salir");
        txt_editor[5].setString("?");

    }
    void renderMenu(RenderWindow &window){
        for (int i= 0; i<6; i++){
            window.draw(option[i]);
            window.draw(txt_editor[i]);
        }
    }
};

struct ChessMenuScreen : public ChessScreen
{
    static const int SCREEN_NUMBER;
    Music music;
    Event event;
    Texture texture;
    Sprite sprite;
    ChessMenu menu;
    list<botton> option;
    ChessMenuScreen()
    {
        if (!music.openFromFile("./assets/sounds/MenuMusic.ogg")){
            throw MenuMusicException();
            exit(1);
        }
        if (!texture.loadFromFile("./assets/Chess_Game.png"))
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
                            for (int i= 0; i<6; i++){
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

                                        case 5:
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

struct ChessPauseScreen : public ChessScreen{
};

struct ChessCreditsScreen : public ChessScreen{
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
    }else{
        /**
            Codigo a ejecutar siempre despues de la primera llamada a Run.
            Aquí debe ir todo código que reanude las acciones que el metodo Pause haya detenido.
        */
    }

    music.play();
    timer_thread->launch();

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
                    if(operation == CLICK_ON_NOTHING){
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
        for(ChessPiece* piece:ChessPlayer::players[ChessPlayer::rivalInTurn].pieces){
            window.draw(piece->sprite);
        }
        for(ChessPiece* piece:ChessPlayer::players[ChessPlayer::playerInTurn].pieces){
            window.draw(piece->sprite);
        }

        window.draw(timer->clock_sprite);
        window.draw(timer->hand_timer_sprite);

        window.display();
    }
    return -1;
}

#endif // CHESSSCREENS_H_INCLUDED
