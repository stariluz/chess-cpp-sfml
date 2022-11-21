#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED
#include <SFML/Graphics.hpp>
#include <list>
#include <assert.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>

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

/*
    Estructuras de las exepciones
*/
struct ChessImageException : public exception {
    const char* what() throw() {
        return "Couldn't load the resource";
    }
};

struct PieceTypeException : public exception{
    const char* what() throw() {
        return "The piece type isn't valid";
    }
};

struct PieceColorException : public exception{
    const char* what() throw() {
        return "The color isn't valid";
    }
};

//Metodo para cargar las texturas
static Texture loadResource(string filename){
    Texture texture;
    cout << filename<<"\n";
    if(!texture.loadFromFile(filename)){
        cout<<"IMAGE WAS NOT LOADED.";
        throw ChessImageException();
    }
    return texture;
}
//Estructura que almacena el manejo de cordenadas en el tablero por cada pieza
struct ChessCoord {
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
        assert(_valueX > 0 && _valueX <= 8);
        x = _x;
        valueX = _valueX;

    }
    void setY(int _y) {
        assert(_y > 0 && _y <= 8);
        y = _y;
    }
    void setValueX(int _valueX) {
        assert(_valueX > 0 && _valueX <= 8);
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
    void print() {
        cout << *this;
    }
};

//Estructura de tipos de piezas
struct ChessPieceTypes{
    static const int K=0; // King
    static const int Q=1; // Queen
    static const int B=2; // Bishop
    static const int H=3; // Horse
    static const int T=4; // Tower
    static const int P=5; // Pawn

    static IntRect getIntRectOfSpriteSheet(int pieceType, int color){
        try{
            validate(pieceType,color);
        }catch(PieceTypeException& e){
            cout<<"Exception caught: " << e.what() << endl;
        }catch(PieceColorException& ee){
            cout<<"Exception caught: " << ee.what() << endl;
        }catch(...){
            cout<<"ERROR\n";
        }
        int pieceTypePX=pieceType*ChessCoord::SIZE;
        int colorPX=color*ChessCoord::SIZE;
        return IntRect(pieceTypePX, colorPX, ChessCoord::SIZE, ChessCoord::SIZE);
    }
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
    ///////// STATICS
    static const Texture spriteSheet;
    static int numberOfPieces;
    /////// STATIC METHODS
    static ChessPiece* createPiece(ChessCoord _position, int pieceType, int color);
    static void operator delete(void *ptr) {
        numberOfPieces--;
        ::operator delete(ptr);
    }
    //////////////////////


    ///////// ATTRIBUTES
    ChessCoord position;
    Sprite sprite;
    int pieceType;
    //////////////////////

    ///////// CONSTRUCTORS
    ChessPiece()
    {
        cout<<this<<"\n";
        position = ChessCoord(1, 1);
    }
    ChessPiece(ChessCoord _position, int _pieceType, int color)
    {
        cout<<"C: "<<this<<"\n";
        pieceType = _pieceType;
        setPosition(_position);
        setSprite(pieceType, color);
    }
    ChessPiece(ChessCoord _position, Sprite sprite)
    {
        cout<<"C: "<<this<<"\n";
        setPosition(_position);
        setSprite(sprite);
    }
    //////////////////////

    ////////////// METHODS
    void setPosition(ChessCoord _position)
    {
        position = _position;
        sprite.setPosition(position.getScreenPosition());
    }
    void setSprite(int pieceType, int color)
    {
        sprite = Sprite(spriteSheet,
                        ChessPieceTypes::getIntRectOfSpriteSheet(pieceType,color)
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
    //////////////////////
};
int ChessPiece::numberOfPieces=0;
/*Estructuras dedicadas a cada tipo de pieza*/
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

struct Tower : public ChessPiece
{
    char icon = 'T';
    Tower ()
    {
        position = ChessCoord(1, 1);
    }
    Tower(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::T, color);
    }
    Tower(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

struct Horse : public ChessPiece
{
    char icon = 'H';
    Horse ()
    {
        position = ChessCoord(1, 1);
    }
    Horse(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::H, color);
    }
    Horse(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

struct Bishop : public ChessPiece
{
    char icon = 'B';
    Bishop ()
    {
        position = ChessCoord(1, 1);
    }
    Bishop(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::B, color);
    }
    Bishop(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

struct Queen : public ChessPiece
{
    char icon = 'Q';
    Queen ()
    {
        position = ChessCoord(1, 1);
    }
    Queen(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::Q, color);
    }
    Queen(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

struct King : public ChessPiece
{
    char icon = 'K';
    King ()
    {
        position = ChessCoord(1, 1);
    }
    King(ChessCoord position, int color)
    {
        this->position = position;
        setSprite(ChessPieceTypes::K, color);
    }
    King(ChessCoord position, Sprite sprite)
    {
        this->position = position;
        setSprite(sprite);
    }
};

//Estructura del tablero
struct ChessBoard
{
    /* Atributos */
    static list<ChessPiece*> piecesOnBoard;
    Sprite sprite;
    Texture texture;
    string imageFilename;
    char empty_icon = ' ';//  Caracter vacio para los espacios vacios en el tablero
    char** matrix_board; //  Matriz del tablero

    ChessBoard() {
        setBoardImage(Chess::BOARD_SPRITESHEET_FILENAME);
    }
    ChessBoard(string boardImageFilename) {
        setBoardImage(imageFilename);
    }

    void setBoardImage(string boardImageFilename){
        try{
            this->texture=loadResource(boardImageFilename);
            this->imageFilename = boardImageFilename;
            sprite = Sprite(texture);
        }catch(...){
            /// DEV TODO: Add exception of image
            cout<<"FATAL ERROR";
            getchar();
            exit(0);
        }
    }
    static ChessPiece* getPieceAtPosition(int pxX, int pxY){
        ChessCoord coord=ChessCoord::getChessPosition(pxX,pxY);
        // DEV_COMMENT //cout<<"COORD_PX: "<<pxX<<","<<pxY<<"\nCOORD: "<<coord<<"\n";
        for(list<ChessPiece*>::iterator piece=ChessBoard::piecesOnBoard.begin(); piece!=ChessBoard::piecesOnBoard.end(); piece++){
            if((*piece)->position==coord){
                    cout<<**piece<<" ";
                return &**piece;
            }
        }
        return NULL;
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
//??
list<ChessPiece*> ChessBoard::piecesOnBoard=list<ChessPiece*>();
//Aqui se implementa el metodo Crear piezas, que se encuentra dentro de ChessPiece en forma de un metodo estatico
ChessPiece* ChessPiece::createPiece(ChessCoord _position, int pieceType, int color){
    ChessPiece *newPiece=new ChessPiece(_position, pieceType, color);
    numberOfPieces++;
    ChessBoard::piecesOnBoard.push_back(newPiece);
    return newPiece;
}

/*
    Estructura del juego, maneja la seleccion de piezas y las mecanicas de seleccion
        incluye el metodo de desplazamiento de pieza
*/
struct ChessGame{
    static string status;
    static ChessPiece* selectedPiece;
    static void onClick(int x, int y){
        cout << "\nButton pressed" << endl;
        cout << "mouse x: " << x << endl;
        cout << "mouse y: " << y << endl;
        if(status=="iddle"||status=="selected"){
            selectedPiece=ChessBoard::getPieceAtPosition(x,y);
            if(selectedPiece&&status=="selected"){
                status="iddle";
                selectedPiece=NULL;
                cout<<"NO SELECTED\n";
            }else if(selectedPiece){
                status="selected";
                cout<<"SELECTED: "<<*selectedPiece<<"\n";
            }else{
                status="iddle";
                cout<<"NO SELECTED\n";
            }
        }
    }
    static void dragPiece(int pxX, int pxY){
        if(!selectedPiece)return;
        //cout<<*selectedPiece<<"\n";
        selectedPiece->setPosition(ChessCoord::getChessPosition(pxX, pxY));
    }
};
//un string que se usa para declarar el tipo de estado de ideal en el que no se encuentra seleccionada ninguna pieza
string ChessGame::status="iddle";
//Establece en ChessGame la pieza selecionada como nula
ChessPiece* ChessGame::selectedPiece=NULL;
//Variables estaticas para el tamaño de la ventana
static const int WINDOW_HORIZONTAL_SIZE=ChessCoord::SIZE*8;
static const int WINDOW_VERTICAL_SIZE=ChessCoord::SIZE*8;

/*
    Esta estructura llevara el manejo del reloj del juego
*/
struct Game_Timer
{
/*Atributos del Timer*/
    // Variables con rutas de la imagen del Timer
    Texture clock_texture, hand_timer_texture;
    Sprite clock_sprite,hand_timer_sprite;

    //A
    bool eneable;
    int seconds, minuts,limit_seconds,limit_minuts;
/*Constructores*/
    Game_Timer()
    {
        eneable = false;
        init_Timer();
    }

/*Metodos del Timer*/
    //Inicializa los valores por defecto del Timer
    void init_Timer()
    {
        clock_texture = loadResource(TIMER_FRAME_FILE);
        hand_timer_texture = loadResource(TIMER_HAND_FILE);

        clock_sprite.setTexture(clock_texture);
        hand_timer_sprite.setTexture(hand_timer_texture);

        seconds = 0;
        minuts = 0;

        limit_seconds = 0;
        limit_minuts = 0;

        on_timer();
    }

    //Aqui se maneja la ejecuccion del hilo cunado se invoca el laucher
    operator()()
    {
        cout<<"Iniciando contador"<<endl;

        on_timer();

        setMinutsLimit(1);
        setSecondsLimit(0);

        run();
    }
    //Metodos de control para el Timer, si se ecnuentra ncendido o apagado
    void on_timer()
    {
        eneable = true;
    }
    void off_timer()
    {
        eneable = false;
    }
    //En este metodo se ejecuta todas las acciones del timer
    void run ()
    {

        int limit_time = convert_Time_Limit();
        cout<<"Limite de tiempo:"<<limit_time<<endl;

        int current_time = 0;

        while(limit_time>current_time){
            if(eneable==true)
            {
                Sleep(1000);
                if(seconds == 60){
                    seconds = 0;
                    minuts += 1;
                }
                seconds +=1;
                cout<<"Segundos:"<<seconds<<endl;
                current_time = (minuts*60) + seconds;
                cout<<"Tiempo actual:"<<current_time<<endl;
            }else{
                //No hacer nada
            }
        }
        //Termino del turno
        cout<<"Cambiando turno"<<endl;
        reset_Timer();
    }
    //Esta funcion convierte los limites de tiempo a una unidad unica para su manejo
    int convert_Time_Limit()
    {
        int sum;
        sum = limit_seconds + (limit_minuts*60);
        return sum;
    }
    //Metodo para configurar los calores de segundos y minutos a su valor por defecto
    void reset_Timer()
    {
        cout<<"Se ha reseteado el Timer"<<endl;
        seconds = 0;
        minuts = 0;
    }
    //Metodos para configurar el limite de tiempo en minutos y segundos
    void setMinutsLimit(int new_minuts_limit){
        limit_minuts = new_minuts_limit;
    }

    void setSecondsLimit(int new_seconds_limit){
        limit_seconds = new_seconds_limit;
    }
    //Funcion amiga para el operador <<
    friend ostream& operator <<(ostream& os, Game_Timer& _game_timer)
    {
        os << _game_timer.minuts <<":"<<_game_timer.seconds;
        return os;
    }
};

#endif // CHESS_H_INCLUDED
