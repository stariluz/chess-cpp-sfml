#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <iostream>
using namespace std;
using namespace sf;

struct ChessImageException :public exception {
    const char* what() throw() {
        return "Couldn't load the resourse";
    }
};

struct ChessImages {
    static Texture loadResource(string filename)/* throw (ChessImageException) */{
        Texture texture;
        cout << filename;
        texture.loadFromFile(filename);
        return texture;
    }
};

struct ChessCoord {
    static const int SIZE = 100;
    int valueX, y;
    char x;
    ChessCoord() {
        x = 'A';
        valueX = 1;
        y = 1;
    }
    ChessCoord(char x, int y) {
        setX(x);
        setY(y);
    }
    ChessCoord(int x, int y) {
        setValueX(x);
        setY(y);
    }

    void setX(char x) {
        int valueX = getValueFromX(x);
        assert(valueX>0&&valueX<=8);
        this->x = x;
        this->valueX = valueX;

    }
    void setY(int y) {
        assert(y > 0 && y <= 8);
        this->y = y;
    }
    void setValueX(int valueX) {
        assert(valueX > 0 && valueX <= 8);
        this->valueX = valueX;
        this->x = getXFromValue(valueX);
    }
    static int getValueFromX(char x) {
        return ((int)x - 64);
    }
    static char getXFromValue(int x) {
        return ((char)x + 64);
    }
    Vector2f getScreenPosition() {
        Vector2f result(SIZE * (valueX - 1), SIZE * (y - 1));
        return result;
    }
    ChessCoord operator + (const ChessCoord& obj) {
        ChessCoord result;
        result.y = y + obj.y;
        result.valueX = valueX + obj.valueX;
        assert(result.valueX > 0 && result.valueX <= 8);
        assert(result.y > 0 && result.y <= 8);
        result.x = getXFromValue(result.valueX);
        return result;
    }
    ChessCoord operator - (const ChessCoord& obj) {
        ChessCoord result;
        result.y = y - obj.y;
        result.valueX = valueX - obj.valueX;
        assert(result.valueX > 0 && result.valueX <= 8);
        assert(result.y > 0 && result.y <= 8);
        result.x = getXFromValue(result.valueX);
        return result;
    }
    ChessCoord operator += (const ChessCoord& obj) {
        *this = *this + obj;
        return *this;
    }
    ChessCoord operator -= (const ChessCoord& obj) {
        *this = *this - obj;
        return *this;
    }
    friend ostream& operator <<(ostream& os, ChessCoord& coord) {
        os << coord.x << ", " << coord.y;
        return os;
    }
    friend istream& operator >>(istream& iStream, ChessCoord& coord) {
        iStream >> coord.x >> coord.y;
        return iStream;
    }
    void print() {
        cout << *this;
    }
};

struct ChessPiece
{

    static Texture piecesSpriteSheet;
    ChessCoord position;
    Sprite sprite;
    ChessPiece()
    {
        position = ChessCoord(1, 1);
        setSprite(Sprite());
    }

    ChessPiece(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
    void setSprite(Sprite _sprite) {
        sprite = _sprite;
        sprite.setPosition(position.getScreenPosition());
    }
};
//Estructura del tablero
struct ChessBoard
{
    /* Atributos */
    Sprite sprite;
    Texture texture;
    string imageFilename;
    char empty_icon = ' ';//  Icono vacio para los espacios vacios en el tablero
    char** matrix_board; //  Matriz del tablero

    ChessBoard() {
        setBoardImage((string&)"assets/chess_board.jpg");
    }
    ChessBoard(string& boardImageFilename) {
        setBoardImage(imageFilename);
    }
    void setBoardImage(string& boardImageFilename) {
        this->texture = ChessImages::loadResource(boardImageFilename);
        this->imageFilename = boardImageFilename;
        sprite = Sprite(texture);
    }
    /* Metodos del tablero */
    //  Este metodo inicializa la matirz del tablero, apartando espacio en la memoria
    void init_board()
    {
        matrix_board = new char* [8];
        for (int i = 0; i < 8; i++)
        {
            matrix_board[i] = new char[8];
            for (int j = 0; j < 8; j++)
            {
                matrix_board[i][j] = empty_icon;
            }
        }
    }
    void end_board() {
        for (int i = 0; i < 8; i++)
        {
            delete matrix_board[i];
        }

    }
    // Este metodo imprime el tablero en consola
    void print_board()
    {
        string string_board;
        for (int i = 0; i < 8; i++)
        {
            //Produce una cadena vacia
            string_board = "";
            //Coloca todos los caracteres de las filas del tablero
            for (int j = 0; j < 8; j++)
            {
                string_board += matrix_board[i][j];
                string_board += " ";
            }
            //imprimer la primera fila
            cout << string_board << endl;
        }
    }

};
#endif // CHESS_H_INCLUDED
