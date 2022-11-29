#ifndef CHESSGAMESCREEN_H_INCLUDED
#define CHESSGAMESCREEN_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include "ChessMenuScreen.h"
#include "ChessExceptions.h"
#include "ChessTimer.h"

using namespace std;
using namespace sf;


/// INICIALIZAR DATOS DEL HEADER "CHESS:H"

// Constante para la ubicación de la imagen del tablero
const string Chess::BOARD_SPRITESHEET_FILENAME="./assets/chess_board.png";

// Constante para la ubicación del spritesheet de piezas
const string Chess::PIECES_SPRITESHEET_FILENAME="./assets/pieces_spritesheet.png";

// Constante para los limites de la pantalla
const IntRect ChessBoard::LIMITS=IntRect(0, 0, ChessCoord::SIZE*8, ChessCoord::SIZE*8);

// Implementación de la constante estática de tamaño para la unidad de pixeles
const int ChessCoord::SIZE=100;

// Abrir el spritesheet de piezas
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME);

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
    static const int STATUS_PUT_BACK=4; // Estado del juego regresar una pieza a su lugar

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

        ChessPiece** playerPiecesInCoord =
            ChessPlayer::getPiecesOfPlayerInTurnAtPosition(x,y);

        ChessPiece** rivalPiecesInCoord =
            ChessPlayer::getPiecesOfRivalInTurnAtPosition(x,y);

        ChessPiece* newClickedPiece=NULL;



        if(playerPiecesInCoord[1]!=NULL){
            /** Tengo seleccionada una pieza y el click es
                en una casilla con pieza propia
            */

            cout<<"CLICK EN CASILLA PROPIA\n"; //DEV

            if(playerPiecesInCoord[0] != selectedPiece){
                newClickedPiece=playerPiecesInCoord[0];
            }else{
                newClickedPiece=playerPiecesInCoord[1];
            }

            /** Se resetea la pieza seleccionada anterior,
                y se selecciona la nueva pieza clickeada
            */
            if(selectedPiece != NULL){
                restoreMovement();
                selectedPiece=newClickedPiece;
                stateSelectedPiece=*selectedPiece;
                calculateAvaiableMovements(selectedPiece);
                // cout<<"SELECTED NEW\n"; // DEV
                return STATUS_SELECTED;
            }
        }
        else if(playerPiecesInCoord[0] != NULL && rivalPiecesInCoord[0] != NULL){
            /** Click en una casilla con pieza rival */

            cout<<"CLICK EN CASILLA RIVAL\n"; //DEV

            /** Se come una pieza rival */
            status = STATUS_EATED;
            selectedPiece = NULL;
            ChessPlayer::eatPieceRival(rivalPiecesInCoord[0]);
            return STATUS_RELEASED;
        }
        else if(playerPiecesInCoord[0] != NULL){
            /** Click en una casilla sin pieza rival */

            cout<<"CLICK EN CASILLA PROPIA\n"; //DEV

            newClickedPiece=playerPiecesInCoord[0];

            if(selectedPiece != NULL){
                if(*newClickedPiece==stateSelectedPiece){
                    /** La pieza seleccionada fue colocada en la misma
                        posición que estaba
                    */
                    cout<<"VOLVISTE AL INICIO CARNAL\n"; // DEV
                    restoreMovement();
                    status = STATUS_IDLE;
                    return STATUS_PUT_BACK;
                }


                if(newClickedPiece != selectedPiece && selectedPiece != NULL){
                    /** Se resetea la pieza seleccionada anterior, y se
                        selecciona la nueva pieza clickeada
                    */
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
            else{
                /** Se selecciona la pieza clickeada */
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
//        cout<<"AVAIABLE MOVEMENTS: "; // DEV
//        for(ChessCoord movement:avaiableMovements){
//            cout<<movement<<", ";
//        } // DEV
//        cout<<"\n"; // DEV
    }

    // Posiciona la pieza sobre la casilla de ajedrez en la que se encuentran los pixeles recibidos.
    static void dragPiece(int pxX, int pxY){
        if(selectedPiece == NULL )return;

        ChessCoord coordToMove=ChessCoord::getChessPosition(pxX, pxY);
//        cout<<"SEARCH MOVEMENT:";
        for(ChessCoord movement:avaiableMovements){
//            cout<<movement<<", ";
            if(movement==coordToMove){
                selectedPiece->setPosition(coordToMove);
                return;
            }
        }
//        cout<<"\n";
    }

    static void restoreMovement(){
        if(selectedPiece == NULL)return;

        cout<<"VAN PA FUERA MI LOCO\n"; // DEV
        avaiableMovements.clear();
        *selectedPiece = stateSelectedPiece;
        selectedPiece = NULL;
        status = STATUS_IDLE;
    }
};

// Estado inicial de el juego, nada está pasando así que es idle
int ChessGame::status=ChessGame::STATUS_IDLE;

// Inicializa en ChessGame la pieza selecionada como nula
ChessPiece* ChessGame::selectedPiece=NULL;

// Inicializa en ChessGame un estado por default
ChessPiece ChessGame::stateSelectedPiece;

// Inicializa en ChessGame una lista vacia de movimientos disponibles
list<ChessCoord> ChessGame::avaiableMovements;


struct ChessGameButtons{
    RectangleShape pauseButton;

    ChessGameButtons(){

        pauseButton=RectangleShape({80,80});
        pauseButton.setFillColor(Color::Transparent);
        pauseButton.setPosition({949,682});

    }
    void renderButtons(RenderWindow &window){
        window.draw(pauseButton);
    }
};


struct ChessGameScreen : public ChessScreen{
    Music music;

    ChessBoard board;
    Event event;

    int save_time;
    int current_time;
    Game_Timer *timer=NULL;
    Thread *timer_thread=NULL;
    ChessGameButtons buttons;

    ChessGameScreen(){
        if (!music.openFromFile("./assets/sounds/BackgroundMusic.ogg")){
            throw BackgroundMusicException();
        }
    }

    // Declaracion de la funcion Run
    virtual int Run(RenderWindow &window);

    virtual void Pause(){
        // cout<<"PAUSAAAAAAAAAAAAAAAAAAAA";
        music.sf::SoundStream::pause();
        ChessGame::restoreMovement();
        save_time=current_time;
        timer->stop();
    }

    static const int CLICK_ON_PAUSE=3;
    static const int CLICK_ON_BOARD=2;
    int onClick(Event::MouseButtonEvent mouseEvent){
        /**
            Funcion que busca la accion realizada al dar click en la pantalla de juego

            Codigos de regreso:
            CLICK_ON_BUTTONS = 2 // Usado cuando se da click a los botones de salir y de pausa
            CLICK_ON_BOARD = 1 // Usado cuando el click es en el tablero de juego
            CLICK_ON_NOTHING = 0 // Usado cuando no se clique� nada interactuable

        */
        int operation=0;
        if(ChessBoard::LIMITS.contains(mouseEvent.x, mouseEvent.y)){
            operation=ChessGame::onClick(mouseEvent.x, mouseEvent.y);
            if(operation==ChessGame::STATUS_IDLE){
                return CLICK_ON_NOTHING;
            }
            else if(operation==ChessGame::STATUS_RELEASED||operation==ChessGame::STATUS_EATED){
                ChessPlayer::updatePlayerInTurn();
            }
            return CLICK_ON_BOARD;
        }else if(buttons.pauseButton.getGlobalBounds().contains(mouseEvent.x, mouseEvent.y)){
            return CLICK_ON_PAUSE;
        }else{
            return CLICK_ON_NOTHING;
        }
    }
};
//ChessPlayer* ChessGameScreen::players=NULL;

/* IMPLEMENTATIONS */

int ChessGameScreen::Run(RenderWindow &window){
    if(!wasRun){
        /**
            Codigo a ejecutar solamente en la primer llamada al Run
            El codigo aqui se usa en caso de requerir acciones que se ejecuten 1 sola ocasión en la primer llamada.
        */
        wasRun=true;
        music.setLoop(true);

        // Reserva de espacio para los jugadores y sus piezas
        ChessPlayer::players=new ChessPlayer[2];
        if(MAIN_PLAYER_COLOR==ChessPiece::COLOR_WHITE){
            ChessPlayer::players[0]=ChessPlayer::createPlayer(ChessPiece::COLOR_BLACK);
            ChessPlayer::players[1]=ChessPlayer::createPlayer(ChessPiece::COLOR_WHITE);

        }else{
            ChessPlayer::players[0]=ChessPlayer::createPlayer(ChessPiece::COLOR_WHITE);
            ChessPlayer::players[1]=ChessPlayer::createPlayer(ChessPiece::COLOR_BLACK);

        }
        // Ejecutamos un thread con el timer

        //Inicializamos un contador
        current_time=0;
        timer=new Game_Timer(current_time, MinuteSecond(MINUTES_PER_TURN,0), ChessPlayer::updatePlayerInTurn);
        timer_thread=new Thread(
            ref( *timer )
        );
    }else{
        /**
            Codigo a ejecutar siempre despues de la primera llamada a Run.
            Aquí debe ir todo código que reanude las acciones que el metodo Pause haya detenido.
        */
    }
    music.setVolume(10.f);
    music.play();

    timer_thread->launch();

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

                    int operation=onClick(event.mouseButton);
                    if(operation == CLICK_ON_NOTHING){ // DEV
                        return CHESS_MENU_SCREEN_NUMBER;
                    }else if(operation==CLICK_ON_PAUSE){
                        return CHESS_PAUSE_SCREEN_NUMBER;
                    }
                }
            }
            else if (event.type == Event::MouseMoved && ChessGame::status==ChessGame::STATUS_SELECTED)
            {
                ChessGame::dragPiece(event.mouseMove.x, event.mouseMove.y);
            }
        }

        window.clear();

        window.draw(board.sprite);
        for(ChessPiece* piece:ChessPlayer::players[ChessPlayer::rivalInTurn].pieces){
            window.draw(piece->sprite);
        }
        for(ChessPiece* piece:ChessPlayer::players[ChessPlayer::playerInTurn].pieces){
            window.draw(piece->sprite);
        }
        buttons.renderButtons(window);

        window.draw(timer->clock_sprite);
        window.draw(timer->hand_timer_sprite);

        /*
            Aqui se puede introducir cualquier cosa que se quiera dibujar
        */
        window.display();
    }
    return -1;
}


#endif // CHESSGAMESCREEN_H_INCLUDED
