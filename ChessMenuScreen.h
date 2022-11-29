#ifndef CHESSMENUSCREEN_H_INCLUDED
#define CHESSMENUSCREEN_H_INCLUDED

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include "Chess.h"
#include "ChessScreens.h"
#include "ChessExceptions.h"
#include "ChessTimer.h"
#include <sstream>

using namespace std;
using namespace sf;

static int MAIN_PLAYER_COLOR=ChessPiece::COLOR_WHITE;
static int MINUTES_PER_TURN=1;

struct ChessMenu{
    Font* fuente;
    Text* txt_editor=NULL;
    RectangleShape* option=NULL;
    Text minutesText;
    Text colorText;
    RenderWindow* window;
    RectangleShape button;
    string name;
    ChessMenu(){
        fuente = new Font();
        fuente->loadFromFile("./assets/Fuente.ttf");
        option = new RectangleShape[6];
        txt_editor = new Text[6];
        for (int i= 0; i<6; i++){
            txt_editor[i]= Text("", *fuente);
        }
        option[0]= RectangleShape({140,40});
        option[1]= RectangleShape({40,40});
        option[2]= RectangleShape({40,40});
        option[3]= RectangleShape({250,80});
        option[4]= RectangleShape({140,60});
        option[5]= RectangleShape({80,80});

        option[0].setFillColor(Color::Transparent);
        option[1].setFillColor(Color::Transparent);
        option[2].setFillColor(Color::Transparent);
        option[3].setFillColor(Color::Transparent);
        option[4].setFillColor(Color::Transparent);
        option[5].setFillColor(Color::Transparent);

        option[0].setPosition({350,357});
        option[1].setPosition({390,460});
        option[2].setPosition({490,460});
        option[3].setPosition({180,600});
        option[4].setPosition({885,700});
        option[5].setPosition({945,32});

        minutesText.setFont(*fuente);
        minutesText.setColor(Color::Black);
        minutesText.setCharacterSize(50);
        minutesText.setPosition({450,435});
        minutesText.setString("1");


        colorText.setFont(*fuente);
        colorText.setColor(Color::Black);
        colorText.setCharacterSize(50);
        colorText.setPosition({375,336});
        colorText.setString("Blanco");


    }
    void renderMenu(RenderWindow &window){
        for (int i= 0; i<6; i++){
            window.draw(option[i]);
        }
//        window.draw(minutesShape);
//        window.draw(minutesText);
    }
    void updateColor(){
        string text="Blanco";
        if(MAIN_PLAYER_COLOR==ChessPiece::COLOR_BLACK){
            text="Negro";
        }
        cout<<"TEXT: "<<text<<"\n";
        colorText.setString(text);
    }
    void updateMinutes(){
        string text=to_string(MINUTES_PER_TURN);
        cout<<"TEXT: "<<text<<"\n";
        minutesText.setString(text);
    }
};

struct ChessMenuScreen : public ChessScreen
{
    static const int STATUS_PLAY=1;
    static const int STATUS_PAUSE=2;
    Music music;
    Event event;
    Texture texture;
    Sprite sprite;
    ChessMenu menu;
//    list<botton> option;
    int (*actions[6])(ChessMenu &);

    ChessMenuScreen()
    {
        actions[0]=ActionChangeColor;
        actions[1]=ActionSubstractTime;
        actions[2]=ActionAddTime;
        actions[3]=ActionPlay;
        actions[4]=ActionClose;
        actions[5]=ActionPause;
        if (!music.openFromFile("./assets/sounds/MenuMusic.ogg")){
            throw MenuMusicException();
            exit(1);
        }
        if (!texture.loadFromFile("./assets/Chess_Game.png"))
        {
            throw ChessImageException();
            exit(1);
        }
        sprite.setTexture(texture);
    }
    virtual int Run(RenderWindow &window){
        if(!wasRun){
            /**
                Codigo a ejecutar solamente en la primer llamada al Run
                El codigo aqui se usa en caso de requerir acciones que se ejecuten 1 sola ocasión en la primer llamada.
            */
            wasRun=true;
            music.setLoop(true);

        }else{
            /**
                Codigo a ejecutar siempre despues de la primera llamada a Run.
                Aquí debe ir todo código que reanude las acciones que el metodo Pause haya detenido.
            */
        }
        music.play();
        music.setVolume(40.f);
        bool running = true;

        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                switch(event.type)
                {
                    case (Event::Closed):
                        window.close();
                        break;
                    case (Event::MouseButtonPressed):
                        if (event.mouseButton.button == Mouse::Left)
                        {
//                            return CHESS_GAME_SCREEN_NUMBER;
                            Vector2i posicion_mouse;
                            posicion_mouse=Mouse::getPosition(window);
                            for (int i= 0; i<6; i++){
                                if (menu.option[i].getGlobalBounds().contains(posicion_mouse.x, posicion_mouse.y)){
                                    int operation=actions[i](menu);
                                    if(operation==STATUS_PAUSE){
                                        return CHESS_PAUSE_SCREEN_NUMBER;
                                    }else if(operation==STATUS_PLAY){
                                        return CHESS_GAME_SCREEN_NUMBER;
                                    }
                                    break;
                                }
                            }
                        }
                        break;

                }
            }
            window.clear();
            window.draw(sprite);
            menu.renderMenu(window);
            window.draw(menu.minutesText);
            window.draw(menu.colorText);
            window.display();
        }
    }

    virtual void Pause(){
        music.sf::SoundStream::pause();
    }

    static int ActionPlay(ChessMenu& menu){
        cout<<"PLAY\n";
        return STATUS_PLAY;
    }
    static int ActionClose(ChessMenu& menu){
        exit(1);
    }
    static int ActionAddTime(ChessMenu& menu){
        if(MINUTES_PER_TURN<5){
            MINUTES_PER_TURN++;
            menu.updateMinutes();
        }
        cout<<"ADD\n";
    }
    static int ActionSubstractTime(ChessMenu& menu){
        if(MINUTES_PER_TURN>1){
            MINUTES_PER_TURN--;
            menu.updateMinutes();
        }
        cout<<"SUBSTRACT\n";
    }
    static int ActionChangeColor(ChessMenu& menu){
        if(MAIN_PLAYER_COLOR==ChessPiece::COLOR_WHITE){
            MAIN_PLAYER_COLOR=ChessPiece::COLOR_BLACK;
        }else{
            MAIN_PLAYER_COLOR=ChessPiece::COLOR_WHITE;
        }
        menu.updateColor();

        cout<<"CHANGECOLOR: "<<MAIN_PLAYER_COLOR<<"\n";
    }
    static int ActionPause(ChessMenu& menu){
        cout<<"PAUSE\n";
        return STATUS_PAUSE;
    }
};


#endif // CHESSMENUSCREEN_H_INCLUDED
