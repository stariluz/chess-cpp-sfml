#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Chess.h"
#include <iostream>
#include <thread>

using namespace std;
using namespace sf;

/// INICIALIZAR DATOS DEL HEADER "CHESS:H"
const string Chess::BOARD_SPRITESHEET_FILENAME="./assets/chess_board.jpg"; /// Constante para la ubicación de la imagen del tablero
const string Chess::PIECES_SPRITESHEET_FILENAME="./assets/pieces_spritesheet.png"; /// Constante para la ubicación del spritesheet de piezas
const int ChessCoord::SIZE=100; /// Constante para la unidad en pixeles
const Texture ChessPiece::spriteSheet=loadResource(Chess::PIECES_SPRITESHEET_FILENAME); /// Abrir el spritesheet de piezas

/// Definición de protocolos
void initPieces(ChessPiece**&, ChessPiece**& );
void MENU();
int main()
{
    MENU();
    sf::Music music;
    if (!music.openFromFile("./assets/sounds/BackgroundMusic.ogg"))
        return EXIT_FAILURE;

    music.play();
    music.setLoop(true);
    //Configuracion de la ventana del juego
    RenderWindow window(VideoMode(WINDOW_HORIZONTAL_SIZE, WINDOW_VERTICAL_SIZE), "Chess");
    //Inisializacion de las piezas blancas y negras
    ChessPiece** whitePieces = new ChessPiece*[8];
    ChessPiece** blackPieces = new ChessPiece*[8];
    //Inicializacion del tablero del juego
    ChessBoard board;

    //Inicializamos un contador
    int current_time = 0;
    Game_Timer s(current_time);
    sf::Thread timer_thread(ref(s));
    timer_thread.launch();

    //TODO: Crear una variable para establecer el tiempo actual y el limite de tiempo

    //Metodo initPieces para las piezas blancas y negras
    initPieces(whitePieces, blackPieces);
    /*
        Bucle principal del juego
        Aqui se manejan los eventos de la ventana
        Eventos del juego como los clicks
        Y el dibujado de la pantalla
    */
    while (window.isOpen())
    {
        //Eventos
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
            {
                music.pause();
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
        //Seccion de dibujado de la pantalla
        window.clear();
            /*
                +Aqui se puede introducir cualquier cosa que se quiera dibujar
            */



        window.draw(board.sprite);
        for (int i = 0; i < 6; i++) {

            window.draw((*whitePieces[i]).sprite);
            window.draw((*blackPieces[i]).sprite);
        }
        //Dibujo del reloj
            /*
                Nota: Este segmento se puede comentar si no se encuentra en uso
                Tambien comente las lineas de arriba del timer para evitar problemas en el rendimiento
            */
        if(s.eneable == true)
        {
            window.draw(s.clock_sprite);
            s.hand_timer_sprite.setOrigin(sf::Vector2f(50,50));
            s.hand_timer_sprite.setPosition(sf::Vector2f(50,50));
            s.hand_timer_sprite.setRotation(s.degrees*s.current_time);
            window.draw(s.hand_timer_sprite);
        }
        window.display();
    }
    return 0;
}

//Metodo para inicializar las piezas
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
//Menu
//struct button{
//    RectangleShape button;
//    Text text;
//    button(){
//    }
//    button(string t, vector2f size, Color bgcolor, Color textcolor){
//        text.setString(t);
//        botton
//    }
//};
void MENU(){
    RenderWindow window(VideoMode(900, 800), "Pantalla_Inicial");
    Texture texture;
    sf::Music musica;
    if (!musica.openFromFile("./assets/sounds/MenuMusic.ogg"))
        exit(1);

    musica.play();
    musica.setLoop(true);
    if (!texture.loadFromFile("./assets/chess-game.jpg"))
        {
            exit(1);
        }
    Sprite sprite;
    sprite.setTexture(texture);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        window.draw(sprite);
        window.display();
    }
}
