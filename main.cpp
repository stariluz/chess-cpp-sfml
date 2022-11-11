#include <SFML/Graphics.hpp>
#include "Chess.h"
#include <iostream>
using namespace std;
using namespace sf;

/// INICIALIZAR DATOS DEL HEADER "CHESS:H"
const string Chess::BOARD_SPRITESHEET_FILENAME="./assets/chess_board.jpg"; /// Constante para la ubicación de la imagen del tablero
const string Chess::PIECES_SPRITESHEET_FILENAME="./assets/pieces_spritesheet.png"; /// Constante para la ubicación del spritesheet de piezas
const int ChessCoord::SIZE=100; /// Constante para la unidad en pixeles
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME); /// Abrir el spritesheet de piezas


/// Definición de protocolos
void initPieces(ChessPiece**&, ChessPiece**& );

int main()
{

    RenderWindow window(VideoMode(WINDOW_HORIZONTAL_SIZE, WINDOW_VERTICAL_SIZE), "Chess");
    ChessPiece** whitePieces = new ChessPiece*[8];
    ChessPiece** blackPieces = new ChessPiece*[8];
    ChessBoard board;

    initPieces(whitePieces, blackPieces);

    while (window.isOpen())
    {
        Event event;
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
        window.draw(board.sprite);
        for (int i = 0; i < 6; i++) {

            window.draw((*whitePieces[i]).sprite);
            window.draw((*blackPieces[i]).sprite);
        }

        window.display();
    }

    return 0;
}

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
