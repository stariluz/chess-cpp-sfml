#ifndef CHESS_H_INCLUDED
#define CHESS_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
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
    Estructuras de las exepciones personalizadas
*/
struct ChessImageException : public exception {
    const char* what() throw() {
        return "Couldn't load the resource";
        sf::SoundBuffer buffer3;
        sf::Sound sonido3;

        if(!buffer3.loadFromFile("ChessImageExc.ogg")){
           cout << "Error " << endl;
        }

        sonido3.setBuffer(buffer3);
        sonido3.setVolume(50);
        sonido3.play();
    }
};

struct PieceTypeException : public exception{
    const char* what() throw() {
        return "The piece type isn't valid";
        sf::SoundBuffer buffer;
        sf::Sound sonido;

        if(!buffer.loadFromFile("PieceTypeExc.ogg")){
           cout << "Error " << endl;
        }

        sonido.setBuffer(buffer);
        sonido.setVolume(50);
        sonido.play();
    }
};

struct PieceColorException : public exception{
    const char* what() throw() {
        return "The color isn't valid";
        sf::SoundBuffer buffer2;
        sf::Sound sonido2;

        if(!buffer2.loadFromFile("PieceColorExc.ogg")){
           cout << "Error " << endl;
        }

        sonido2.setBuffer(buffer2);
        sonido2.setVolume(50);
        sonido2.play();
    }
};

struct BoardImageException : public exception{
    const char* what() throw() {
        return "Error: The image doesn´t load";
        sf::SoundBuffer buffer3;
        sf::Sound sonido3;

        if(!buffer3.loadFromFile("BoardImageExc.ogg")){
           cout << "Error " << endl;
        }

        sonido3.setBuffer(buffer3);
        sonido3.setVolume(50);
        sonido3.play();
    }
};

struct BackgroundMusicException : public exception{
    const char* what() throw() {
        return "Error: The Background Music doesn´t load";
    }
};

struct MenuMusicException : public exception{
    const char* what() throw() {
        return "Error: The Menu Music doesn´t load";
    }
};

// Metodo para cargar las texturas
static Texture loadResource(string filename){
    Texture texture;
    // DEV cout <<filename<<"\n";
    if(!texture.loadFromFile(filename)){
        throw ChessImageException();
    }
    return texture;
}

// Estructura que almacena el manejo de cordenadas en el tablero por cada pieza
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
    void print() {
        cout << *this;
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


    // Regresa el área de pieces spritesheet en el que se encuentra la pieza con el color y el tipo mandados
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
    static ChessPiece* createPiece(ChessCoord _position, int pieceType, int color);
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
    static ChessPlayer *players;
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
        int position=1;
        if(numberOfPlayer%2==1){
            position=8;
        }
        for (int i = 0; i < 8; i++) {
            pieces.push_back(
                ChessPiece::createPiece(
                    ChessCoord(i + 1, position), ChessPiece::TYPE_PAWN, color
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
            ChessPiece* piece: ChessPlayer::players[ChessPlayer::playerInTurn].pieces
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
            ChessPiece* piece: ChessPlayer::players[ChessPlayer::rivalInTurn].pieces
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
            ChessPiece* piece: ChessPlayer::players[ChessPlayer::playerInTurn].pieces
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
            ChessPiece* piece: ChessPlayer::players[ChessPlayer::rivalInTurn].pieces
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
    static list<ChessPiece*> piecesOnBoard;
    static const IntRect LIMITS;
    Sprite sprite;
    Texture texture;
    string imageFilename;

    /* Constructores del tablero*/
    ChessBoard() {
        setBoardImage(Chess::BOARD_SPRITESHEET_FILENAME);
    }
    ChessBoard(string boardImageFilename) {
        setBoardImage(imageFilename);
    }

    /// Cargar la imagen del tablero
    void setBoardImage(string boardImageFilename){
        try{
            this->texture=loadResource(boardImageFilename);
            this->imageFilename = boardImageFilename;
            sprite = Sprite(texture);
        }catch(...){
            throw BoardImageException();
            exit(0);
        }
    }

    // Obtener la pieza en una posición a partir de los pixeles recibidos
    static ChessPiece* getPieceAtPosition(int pxX, int pxY){
        ChessCoord coord=ChessCoord::getChessPosition(pxX,pxY);
        // cout<<"COORD_PX: "<<pxX<<","<<pxY<<"\nCOORD: "<<coord<<"\n"; // DEV

        for(
            list<ChessPiece*>::iterator piece=ChessBoard::piecesOnBoard.begin();
            piece!=ChessBoard::piecesOnBoard.end();
            piece++
        ){
            if((*piece)->position==coord){
                    cout<<**piece<<" ";
                return &**piece;
            }
        }
        return NULL;
    }
};

// Inicialización de lista de apuntadores a Piezas, que guardará una lista con todas las piezas en juego.
list<ChessPiece*> ChessBoard::piecesOnBoard=list<ChessPiece*>();

// Implementación del metodo estático para crear piezas, que se encuentra dentro de ChessPiece
ChessPiece* ChessPiece::createPiece(ChessCoord _position, int pieceType, int color){
    ChessPiece *newPiece=new ChessPiece(_position, pieceType, color);
    numberOfPieces++;
//    ChessBoard::piecesOnBoard.push_back(newPiece);
    return newPiece;
}

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
    int current_time;
    int limit_time;

    float degrees;

    /* Constructores */
    Game_Timer()
    {
        eneable = false;
        init_Timer();
    }
    Game_Timer(int &current_time_external)
    {
        eneable = false;
        init_Timer();
        current_time = current_time_external;
    }

    /* Metodos del Timer*/

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

        limit_time=0;

        degrees = 0;
        on_timer();
    }

    //Aqui se maneja la ejecuccion del hilo cunado se invoca el laucher
    operator()()
    {
        cout<<"Iniciando contador"<<endl;

        on_timer();

        setMinutsLimit(1);
        setSecondsLimit(0);

        limit_time = convert_Time_Limit();
        degrees = 360/limit_time;

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
        cout<<"Limite de tiempo:"<<limit_time<<endl;
        cout<<degrees<<endl;

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
