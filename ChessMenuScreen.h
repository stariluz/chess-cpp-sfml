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

struct botton{
    RectangleShape r;
    string name;
    boton(string identidad, RectangleShape rect){
        name=identidad;
        r= rect;
    }
};

struct ChessMenu{
    Font* fuente;
    Text* txt_editor=NULL;
    RectangleShape* option=NULL;
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
        option[1]= RectangleShape({40,42});
        option[2]= RectangleShape({38,40});
        option[3]= RectangleShape({250,80});
        option[4]= RectangleShape({160,50});
        option[5]= RectangleShape({160,50});

        option[0].setFillColor(Color::White);
        option[1].setFillColor(Color::Black);
        option[2].setFillColor(Color::Black);
        option[3].setFillColor(Color::White);
        option[4].setFillColor(Color::Red);
        option[5].setFillColor(Color::White);

        option[0].setPosition({350,357});
        option[1].setPosition({335,355});
        option[2].setPosition({490,357});
        option[3].setPosition({180,600});
        option[4].setPosition({890,700});
        option[5].setPosition({890,100});

        txt_editor[0].setColor(Color::Black);
        txt_editor[1].setColor(Color::White);
        txt_editor[2].setColor(Color::White);
        txt_editor[3].setColor(Color::Black);
        txt_editor[4].setColor(Color::White);
        txt_editor[5].setColor(Color::Black);

        txt_editor[0].setCharacterSize(45);
        txt_editor[1].setCharacterSize(50);
        txt_editor[2].setCharacterSize(50);
        txt_editor[3].setCharacterSize(100);
        txt_editor[4].setCharacterSize(55);
        txt_editor[5].setCharacterSize(55);

        txt_editor[0].setPosition({395,341});
        txt_editor[1].setPosition({356,340});
        txt_editor[2].setPosition({494,339});
        txt_editor[3].setPosition({213,555});
        txt_editor[4].setPosition({917,683});
        txt_editor[5].setPosition({917,183});

        txt_editor[0].setString("Blanco");
        txt_editor[1].setString("<");
        txt_editor[2].setString(">");
        txt_editor[3].setString("JUGAR");
        txt_editor[4].setString("Salir");
        txt_editor[5].setString("?");

    }
    void renderMenu(RenderWindow &window){
        for (int i= 0; i<6; i++){
            window.draw(option[i]);
            window.draw(txt_editor[i]);
        }
    }
};

struct ChessMenuScreen : public ChessScreen
{
    Music music;
    Event event;
    Texture texture;
    Sprite sprite;
    ChessMenu menu;
    list<botton> option;
    ChessMenuScreen()
    {
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
                            return CHESS_GAME_SCREEN_NUMBER;
                            Vector2i posicion_mouse;
                            posicion_mouse=Mouse::getPosition(window);
                            for (int i= 0; i<6; i++){
//                                if (option[i]->getGlobalBounds().Contains(posicion_mouse.x, posicion_mouse.y)){
                                    switch(i){
                                        case 0:
                                            break;

                                        case 1:
                                            break;

                                        case 2:
                                            break;

                                        case 3:

                                            break;

                                        case 4:
                                            exit(1);
                                            break;

                                        case 5:
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
            window.display();
        }
    }

    virtual void Pause(){
        music.sf::SoundStream::pause();
    }

};


#endif // CHESSMENUSCREEN_H_INCLUDED
