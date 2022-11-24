#ifndef CHESSSCREENS_H_INCLUDED
#define CHESSSCREENS_H_INCLUDED
#include "Chess.h"

using namespace std;
using namespace sf;

/// INICIALIZAR DATOS DEL HEADER "CHESS:H"
const string Chess::BOARD_SPRITESHEET_FILENAME="./assets/chess_board.jpg"; /// Constante para la ubicación de la imagen del tablero
const string Chess::PIECES_SPRITESHEET_FILENAME="./assets/pieces_spritesheet.png"; /// Constante para la ubicación del spritesheet de piezas
const int ChessCoord::SIZE=100; /// Constante para la unidad en pixeles
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME); /// Abrir el spritesheet de piezas

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

    ChessGameScreen(){
        if (!music.openFromFile("BackgroundMusic.ogg"))
            cout<<"ERROR";// TODO: exception image

        whitePieces = new ChessPiece*[8];
        blackPieces = new ChessPiece*[8];
//
//        // Reserva de espacio para piezas blancas y negras
//        ChessPiece** whitePieces = new ChessPiece*[8];
//        ChessPiece** blackPieces = new ChessPiece*[8];

        // Cargar posiciones iniciales de las piezas
        initPieces(whitePieces, blackPieces);
        cout<<"SHEEEEEEEEESH";

    }

    //Metodo para inicializar las piezas
    void initPieces(ChessPiece**& whitePieces, ChessPiece**& blackPieces) {
        cout<<"WAKAWAKA E E";
        for (int i = 0; i < 6; i++) {
            whitePieces[i]=ChessPiece::createPiece(
                ChessCoord(i + 1, 1), i, 0
            );
            blackPieces[i]=ChessPiece::createPiece(
                ChessCoord(i + 1, 2), i, 1
            );
            cout<<"White Piece "<<whitePieces[i]<<"\n";
            cout<<"Black Piece "<<blackPieces[i]<<"\n";
        }
    }

    virtual int Run(RenderWindow &window){
        bool running = true;
        music.play();
        while (running)
        {
        /*
            Ciclo de vida de una partida
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

            /// Seccion de dibujado de la pantalla --------->


            window.clear();

            /*
                Aqui se puede introducir cualquier cosa que se quiera dibujar
            */

            window.draw(board.sprite);
            for (int i = 0; i < 6; i++) {
                window.draw((*whitePieces[i]).sprite);
                window.draw((*blackPieces[i]).sprite);
            }

            /// <--------- Final de la seccion de dibujo

            window.display();
        }
        return -1;
    }

};

struct ChessMenuScreen : public ChessScreen{
    int windowHeight;
    int windowWidth;
    virtual int Run(RenderWindow &window){
        bool running = true;
        Event event;
        Texture texture;
        if(!texture.loadFromFile("./assets/Manecilla.png")){
            return -1; /// TODO: exception image
        }
        Sprite sprite(texture);

        while (running)
        {
        /*
            Ciclo de vida de una partida
        */

            //Eventos
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
                        cout<<"CLICK BUAJUAJUA\n";
                        return 1;
                    }
                }
            }

            /// Seccion de dibujado de la pantalla --------->


            window.clear();

            /*
                Aqui se puede introducir cualquier cosa que se quiera dibujar
            */

            window.draw(sprite);

            /// <--------- Final de la seccion de dibujo
            window.display();
        }
        return -1;
    }
};

#endif // CHESSSCREENS_H_INCLUDED
