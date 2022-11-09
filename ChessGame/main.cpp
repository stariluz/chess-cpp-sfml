#include <SFML/Graphics.hpp>
#include "Chess.h"
#include <iostream>
using namespace std;
using namespace sf;

Texture
void initPieces(ChessPiece* &whitePieces, ChessPiece* &blackPieces) {
    for (int i = 0; i <6; i++) {
        whitePieces[i] = ChessPiece(
            ChessCoord(i+1, 1),
            Sprite(ChessPiece::piecesSpriteSheet,
                IntRect(i * ChessCoord::SIZE, 0, ChessCoord::SIZE, ChessCoord::SIZE)
            )
        );
        blackPieces[i] = ChessPiece(
            ChessCoord(i+1, 2),
            Sprite(ChessPiece::piecesSpriteSheet,
               IntRect(i* ChessCoord::SIZE, ChessCoord::SIZE, ChessCoord::SIZE, ChessCoord::SIZE)
            )
        );
    }
}

int main()
{
    RenderWindow window(VideoMode(800, 800), "Chess");
    ChessPiece* whitePieces = new ChessPiece[8];
    ChessPiece* blackPieces = new ChessPiece[8];
    ChessBoard board;
    Texture texture;
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
