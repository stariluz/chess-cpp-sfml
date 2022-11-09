#include <SFML/Graphics.hpp>
#include "Chess.h"
#include <iostream>
using namespace std;
using namespace sf;

const string Chess::BOARD_SPRITESHEET_FILENAME="assets/chess_board.jpg";
const string Chess::PIECES_SPRITESHEET_FILENAME="assets/pieces_spritesheet.png";
const int ChessCoord::SIZE=100;
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME);

void initPieces(ChessPiece*&, ChessPiece*&);

int main()
{

    RenderWindow window(VideoMode(800, 800), "Chess");
    ChessPiece* whitePieces = new ChessPiece[8];
    ChessPiece* blackPieces = new ChessPiece[8];

    ChessBoard board;

    Sprite test(ChessPiece::spriteSheet);
    initPieces(whitePieces, blackPieces);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(board.sprite);

        for (int i = 0; i < 6; i++) {
            window.draw(whitePieces[i].sprite);
            window.draw(blackPieces[i].sprite);
        }
        ChessCoord pawn('A', 2), horse('B',1);

        window.display();
    }

    return 0;
}

void initPieces(ChessPiece*& whitePieces, ChessPiece*& blackPieces) {
    for (int i = 0; i < 6; i++) {
        whitePieces[i] = ChessPiece(
            ChessCoord(i + 1, 1), i, 0
        );
        blackPieces[i] = ChessPiece(
            ChessCoord(i + 1, 2), i, 1
        );
    }
}
