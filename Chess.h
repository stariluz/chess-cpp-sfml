#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED

#include "ChessExceptions.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <list>
#include <assert.h>

using namespace std;
using namespace sf;

//Estructura del Ajedrez
struct Chess{
    // Carga strings estaticos constantes para colocar las direcciones de las texturas de tablero y piezas
    static const string BOARD_SPRITESHEET_FILENAME;
    static const string PIECES_SPRITESHEET_FILENAME;
};

static const string TIMER_FRAME_FILE = "./assets/Marco_reloj.png";
static const string TIMER_HAND_FILE = "./assets/Manecilla.png";

// Metodo para cargar las texturas
static Texture loadResource(string filename){
    Texture texture;
     cout <<filename<<"\n"; // DEV
    if(!texture.loadFromFile(filename)){
        throw ChessImageException();
    }
    return texture;
}
// Estructura que almacena el manejo de cordenadas en el tablero por cada pieza
struct ChessCoord {
    // Declaración de la constante estática de tamaño para la unidad de pixeles
    static const int SIZE;
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

    void setX(char _x) {
        int _valueX = getValueFromX(_x);
        assert(_valueX >= 0 && _valueX <= 8);
        x = _x;
        valueX = _valueX;

    }
    void setY(int _y) {
        assert(_y >= 0 && _y <= 8);
        y = _y;
    }
    void setValueX(int _valueX) {
        assert(_valueX >= 0 && _valueX <= 8);
        valueX = _valueX;
        x = getXFromValue(_valueX);
    }
    static int getValueFromX(char x) {
        return ((int)x - 64);
    }
    static char getXFromValue(int x) {
        return ((char)x + 64);
    }
    Vector2f getScreenPosition() {
        Vector2f result((float) SIZE * (valueX - 1), (float) SIZE * (y - 1));
        return result;
    }

    static ChessCoord getChessPosition(int pxX, int pxY) {
        pxX=pxX/ChessCoord::SIZE+1;
        pxY=pxY/ChessCoord::SIZE+1;
        return ChessCoord(pxX,pxY);
    }
    ChessCoord operator + (const ChessCoord& obj) {
        ChessCoord result;
        result.y = y + obj.y;
        result.valueX = valueX + obj.valueX;
        assert(result.valueX >= 0 && result.valueX <= 8);
        assert(result.y >= 0 && result.y <= 8);
        result.x = getXFromValue(result.valueX);
        return result;
    }
    ChessCoord operator - (const ChessCoord& obj) {
        ChessCoord result;
        result.y = y - obj.y;
        result.valueX = valueX - obj.valueX;
        assert(result.valueX >= 0 && result.valueX <= 8);
        assert(result.y >= 0 && result.y <= 8);
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
    bool operator == (const ChessCoord& obj) {
        return (this->valueX==obj.valueX&&this->y==obj.y);
    }
    bool operator != (const ChessCoord& obj) {
        return (this->valueX!=obj.valueX||this->y!=obj.y);
    }
    friend ostream& operator <<(ostream& os, ChessCoord& coord) {
        os << coord.x << coord.y;
        return os;
    }
    friend istream& operator >>(istream& iStream, ChessCoord& coord) {
        iStream >> coord.x >> coord.y;
        return iStream;
    }
};

// Estructura de tipos de piezas
struct ChessPieceTypes{
    static const int K=0; // King
    static const int Q=1; // Queen
    static const int B=2; // Bishop
    static const int H=3; // Horse
    static const int T=4; // Tower
    static const int P=5; // Pawn

    // Regresa el área del spritesheet en el que se encuentra la pieza con el color y el tipo mandados
    static IntRect getSpriteIntRectByColorType(int color, int pieceType){
        try{
            validate(pieceType,color);
        }catch(PieceTypeException& e){
            cout<<"Exception caught: " << e.what() << endl;
        }catch(PieceColorException& ee){
            cout<<"Exception caught: " << ee.what() << endl;
        }catch(std::bad_alloc eee){
            cout<<"ERROR: " << eee.what();
        }catch(...){
            cout<<"ERROR\n";
        }
        int pieceTypePX=pieceType*ChessCoord::SIZE;
        int colorPX=color*ChessCoord::SIZE;

        return IntRect(pieceTypePX, colorPX, ChessCoord::SIZE, ChessCoord::SIZE);
    }

    // Validar si la pieza es valida según el tipo y el color
    static void validate(int pieceType, int color){
        if(pieceType<0||pieceType>= 6){
            throw PieceTypeException();
        }else if(color<0||color>=2){
            cout<<"The color isn't valid\n";
            throw PieceColorException();
        }
    }
};

//Estructura de piezas, importante para la creacion y el manejo de cada una
struct ChessPiece
{
    /* Atributos estáticos de piezas */
    static const Texture spriteSheet;
    static int numberOfPieces;
    static const int COLOR_WHITE=0;
    static const int COLOR_BLACK=1;
    static const int TYPE_PAWN=5;

    /* Metodos estáticos de piezas */
    // Implementación del metodo estático para crear piezas, que se encuentra dentro de ChessPiece
    static ChessPiece* createPiece(ChessCoord _position, int pieceType, int color){
        ChessPiece *newPiece=new ChessPiece(_position, pieceType, color);
        numberOfPieces++;
        return newPiece;
    }
    static void operator delete(void *ptr) {
        numberOfPieces--;
        ::operator delete(ptr);
    }

    /* Atributos de piezas*/
    ChessCoord position;
    Sprite sprite;
    int pieceType;

    /* Constructores de piezas */
    ChessPiece()
    {
        cout<<this<<"\n"; // DEV
        position = ChessCoord(0, 0);
    }
    ChessPiece(ChessCoord _position, int _pieceType, int color)
    {
        cout<<"C: "<<this<<"\n"; // DEV
        pieceType = _pieceType;
        setPosition(_position);
        setSprite(pieceType, color);
    }
    ChessPiece(ChessCoord _position, Sprite sprite)
    {
        cout<<"C: "<<this<<"\n"; // DEV
        setPosition(_position);
        setSprite(sprite);
    }

    ////////////// METHODS
    void setPosition(ChessCoord _position)
    {
        position = _position;
        sprite.setPosition(position.getScreenPosition());
    }
    void setSprite(int pieceType, int color)
    {
        sprite = Sprite(spriteSheet,
                        ChessPieceTypes::getSpriteIntRectByColorType(color,pieceType)
                        );
        sprite.setPosition(position.getScreenPosition());
    }
    void setSprite(Sprite _sprite)
    {
        sprite = _sprite;
        sprite.setPosition(position.getScreenPosition());
    }
    friend ostream& operator <<(ostream& os, ChessPiece& piece)
    {
        os << piece.pieceType<<"("<<piece.position<<")";
        return os;
    }

    bool operator == (const ChessPiece& obj) {
        return (position == obj.position && pieceType == obj.pieceType);
    }
};

// Inicializar el contador de piezas a 0
int ChessPiece::numberOfPieces=0;

/* Estructura de peón */
struct Pawn : public ChessPiece
{
    char icon = 'P';
    Pawn()
    {
        position = ChessCoord(1, 1);
    }
    Pawn(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::P, color);
    }
    Pawn(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

struct ChessPlayer{
    static int numberOfPlayers;
    static ChessPlayer* players;
    static int playerInTurn;
    static int rivalInTurn;

    int numberOfPlayer;
//    ChessPiece** pieces;

    // Inicialización de lista de apuntadores a piezas, que guardará una lista con todas las piezas en juego.
    vector<ChessPiece*> pieces;
    int pieceColor;

    ChessPlayer(){}
    ChessPlayer(int _pieceColor){
        numberOfPlayer = numberOfPlayers++;
        if(_pieceColor == ChessPiece::COLOR_WHITE){
            playerInTurn = numberOfPlayer;
            rivalInTurn = (numberOfPlayer+1)%2;
        }
        pieceColor=_pieceColor;
        // pieces = new ChessPiece*[8];
        initPieces(_pieceColor);
    }

    //Metodo para inicializar las piezas
    void initPieces(int color) {
        int row=1;
        if(numberOfPlayer%2==1){
            row=8;
        }
        for (int i = 0; i < 8; i++) {
            pieces.push_back(
                ChessPiece::createPiece(
                    ChessCoord(i + 1, row), ChessPiece::TYPE_PAWN, color
                )
            );
        }
    }

    /* Metodo para crear jugador y retornarlo */
    static ChessPlayer createPlayer(int _pieceColor){
        ChessPlayer* result=new ChessPlayer(_pieceColor);
        return *result;
    }

    /* Metodo que realiza la busqueda de la pieza a comer */
    static void eatPieceRival(ChessPiece* rivalPiece){
        vector<ChessPiece*>::iterator aux=players[rivalInTurn].pieces.begin();
        for(ChessPiece* piece:players[rivalInTurn].pieces){
            if(piece==rivalPiece){
                players[rivalInTurn].pieces.erase(aux);
                break;
            }
            aux++;
        }
        delete rivalPiece;
    }

    /* Método para realizar la lógica de actualización de turno*/
    static void updatePlayerInTurn(){
        playerInTurn=rivalInTurn;
        rivalInTurn=(rivalInTurn+1)%2;
    }

    // Obtener piezas del jugador en el turno actual en una posición a partir de los pixeles recibidos
    static ChessPiece** getPiecesOfPlayerInTurnAtPosition(int pxX, int pxY){
        ChessCoord coord=ChessCoord::getChessPosition(pxX,pxY);
        ChessPiece** results=new ChessPiece*[2];
        results[0]=NULL;
        results[1]=NULL;
        int counter=0;
        for(
            ChessPiece* piece: players[playerInTurn].pieces
        ){
            if(piece->position==coord){
                // cout<<*piece<<" "; // DEV
                results[counter]=piece;
                counter++;
            }
        }

        return results;
    }

    // Obtener piezas del rival en el turno actual en una posición a partir de los pixeles recibidos
    static ChessPiece** getPiecesOfRivalInTurnAtPosition(int pxX, int pxY){
        ChessCoord coord=ChessCoord::getChessPosition(pxX,pxY);
        ChessPiece** results=new ChessPiece*[2];
        results[0]=NULL;
        results[1]=NULL;
        int counter=0;
        for(
            ChessPiece* piece:players[rivalInTurn].pieces
        ){
            if(piece->position==coord){
                // cout<<*piece<<" "; // DEV
                results[counter]=piece;
                counter++;
            }
        }


        return results;
    }

    // Obtener piezas del jugador en el turno actual en una posición a partir de la coordenada de ajedrez
    static ChessPiece** getPiecesOfPlayerInTurnAtPosition(ChessCoord coord){
        ChessPiece** results=new ChessPiece*[2];
        results[0]=NULL;
        results[1]=NULL;
        int counter=0;
        for(
            ChessPiece* piece:players[playerInTurn].pieces
        ){
            if(piece->position==coord){
                // cout<<*piece<<" "; // DEV
                results[counter]=piece;
                counter++;
            }
        }

        return results;
    }

    // Obtener piezas del rival en el turno actual en una posición a partir de la coordenada de ajedrez
    static ChessPiece** getPiecesOfRivalInTurnAtPosition(ChessCoord coord){
        ChessPiece** results=new ChessPiece*[2];
        results[0]=NULL;
        results[1]=NULL;
        int counter=0;
        for(
            ChessPiece* piece:players[rivalInTurn].pieces
        ){
            if(piece->position==coord){
                // cout<<*piece<<" "; // DEV
                results[counter]=piece;
                counter++;
            }
        }


        return results;
    }

};
int ChessPlayer::numberOfPlayers=0;
int ChessPlayer::playerInTurn=0;
int ChessPlayer::rivalInTurn=1;
ChessPlayer* ChessPlayer::players=NULL;

// Estructura del tablero
struct ChessBoard
{
    /* Atributos del tablero*/
    static const IntRect LIMITS;
    Sprite sprite;
    Texture texture;

    /* Constructores del tablero*/
    ChessBoard() {
        setBoardImage(Chess::BOARD_SPRITESHEET_FILENAME);
    }
    ChessBoard(string boardImageFilename) {
        setBoardImage(boardImageFilename);
    }

    /// Cargar la imagen del tablero
    void setBoardImage(string boardImageFilename){
        try{
            texture=loadResource(boardImageFilename);
            sprite = Sprite(texture);
        }catch(...){
            throw BoardImageException();
            exit(0);
        }
    }
};

#endif // CHESS_H_INCLUDED
