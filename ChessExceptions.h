#ifndef CHESSEXCEPTIONS_H_INCLUDED
#define CHESSEXCEPTIONS_H_INCLUDED


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <iostream>

using namespace std;
using namespace sf;
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
        SoundBuffer buffer3;
        Sound sonido3;

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





#endif // CHESSEXCEPTIONS_H_INCLUDED
