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
        sf::SoundBuffer buffer3;
        sf::Sound sonido3;

        if(buffer3.loadFromFile("ChessImageExc.ogg")){
            sonido3.setBuffer(buffer3);
            sonido3.setVolume(50);
            sonido3.play();
        }

        return "No se pudo cargar el recurso";
    }
};

struct PieceTypeException : public exception{
    const char* what() throw() {
        sf::SoundBuffer buffer;
        sf::Sound sonido;

        if(buffer.loadFromFile("PieceTypeExc.ogg")){
            sonido.setBuffer(buffer);
            sonido.setVolume(50);
            sonido.play();
        }

        return "El tipo de pieza no es valido";
    }
};

struct PieceColorException : public exception{
    const char* what() throw() {
        sf::SoundBuffer buffer2;
        sf::Sound sonido2;

        if(buffer2.loadFromFile("PieceColorExc.ogg")){
            sonido2.setBuffer(buffer2);
            sonido2.setVolume(50);
            sonido2.play();
        }

        return "El color no es valido";
    }
};

struct BoardImageException : public exception{
    const char* what() throw() {
        SoundBuffer buffer3;
        Sound sonido3;

        if(buffer3.loadFromFile("BoardImageExc.ogg")){
            sonido3.setBuffer(buffer3);
            sonido3.setVolume(50);
            sonido3.play();
        }

        return "Error: La imagen no carga";
    }
};

struct InvalidMovement : public exception{
    const char* what() throw() {
        SoundBuffer buffer4;
        Sound sonido4;

        if(buffer4.loadFromFile("InvalidMovementExc.ogg")){
            sonido4.setBuffer(buffer4);
            sonido4.setVolume(50);
            sonido4.play();
        }

        return "Error: Movimiento invalido";
    }
};

struct BackgroundMusicException : public exception{
    const char* what() throw() {
        return "Error: La musica de fondo no carga";
    }
};

struct MenuMusicException : public exception{
    const char* what() throw() {
        return "Error: La musica del menu no carga";
    }
};





#endif // CHESSEXCEPTIONS_H_INCLUDED
