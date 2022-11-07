#include <iostream>
using namespace std;

/*Estructuras usadas en el juego*/
//Estructura para las piezas
struct Piece 
{
    int pos_x;
    int pos_y;

    Piece()
    {

    }
};

struct Pawn : public Piece 
{
    char icon = 'P';
};

struct King : public Piece 
{
    char icon = 'K';
};

struct Queen : public Piece 
{
    char icon = 'Q';
};

struct Tower : public Piece 
{
    char icon = 'T';
};

struct Bishop : public Piece 
{
    char icon = 'B';
};

struct Horse : public Piece 
{
    char icon = 'H';
};

//Estructura del jugador
struct Player
{
    //A cada jugador se le asigna una lista de piezas
    Piece* player_pieces;

    Player()
    {

    }
    
    void init_player_pieces()
    {

    }

};

//Estructura del tablero
struct Board
{   
    /*Atributos*/
    //  Icono vacio para los espacios vacios en el tablero
    char empty_icon = ' ';
    //  Matriz del tablero
    char** matrix_board;

    /*Metodos del tablero*/
    //  Este metodo inicializa la matirz del tablero, apartando espacio en la memoria
    void init_board()
    {   
        matrix_board= new char*[8];
        for(int i = 0; i < 8 ;i++)
        {
            matrix_board[i] = new char[8];
            for (int j = 0; j < 8; j++)
            {
                matrix_board[i][j] = empty_icon;
            }
        }
    }
    void end_board(){
        for (int i = 0; i < 8; i++)
        {
            delete matrix_board[i];
        }
        
    }
    // Este metodo imprime el tablero en consola
    void print_board()
    {
        string string_board;
        for(int i = 0; i < 8 ;i++)
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
            cout<<string_board<<endl;
        }
    }

};

//Protocolo de funciones
void init_game(Board, Player, Player);
void end_game(Board, Player, Player);
void exit();

int main()
{
    Board board;
    Player player1,player2;
    board.print_board();
    
    return 0;
}


/*Funcion es del main */
//  Funcion para inicializar los jugadores
void init_game(Board bd, Player p1, Player p2){
    bd.init_board();
    p1.init_player_pieces();
    p2.init_player_pieces();
}
//  Funcion para terminar la partida
void end_game(Board bd, Player p1, Player p2){
    bd.end_board();
    delete [] bd.matrix_board;
    delete [] p1.player_pieces;
    delete [] p2.player_pieces;
}