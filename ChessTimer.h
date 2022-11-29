#ifndef CHESSTIMER_H_INCLUDED
#define CHESSTIMER_H_INCLUDED


#include "ChessExceptions.h"
#include <SFML/Graphics.hpp>
#include <assert.h>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <mutex>

using namespace std;
using namespace sf;

struct MinuteSecond{
    int minute, second;
    MinuteSecond(int _minute, int _second){
        minute=_minute;
        second=_second;
    }
};

/*
    Esta estructura llevara el manejo del reloj del juego
*/
struct Game_Timer
{
    /*Atributos del Timer*/
    // Variables con rutas de la imagen del Timer
    Texture clock_texture, hand_timer_texture;
    Sprite clock_sprite,hand_timer_sprite;

    bool eneable;
    int seconds, minuts;
    int limit_seconds, limit_minuts;
    int current_time;
    int limit_time;
    int running=false;
    void (*callback)();
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
        current_time = current_time_external;
        init_Timer();
    }

    Game_Timer(int &current_time_external, MinuteSecond limitTime, void _callback())
    {
        eneable = false;

        clock_texture = loadResource(TIMER_FRAME_FILE);
        hand_timer_texture = loadResource(TIMER_HAND_FILE);

        clock_sprite.setTexture(clock_texture);
        hand_timer_sprite.setTexture(hand_timer_texture);

        current_time = current_time_external;
        init_Timer();
        setMinutsLimit(limitTime.minute);
        setSecondsLimit(limitTime.second);
        callback=_callback;
    }
    /* Metodos del Timer*/

    //Inicializa los valores por defecto del Timer
    void init_Timer()
    {

        minuts=current_time/60;
        seconds=current_time%60;

        degrees = 0;
        on_timer();
    }

    //Aqui se maneja la ejecuccion del hilo cunado se invoca el laucher
    operator()()
    {
        mutex mtx;
        mtx.lock();
        cout<<"Iniciando contador"<<endl;
        cout<<"CURRENT TIME: "<<current_time<<"\n";
        on_timer();

        limit_time = convert_Time_Limit();
        degrees = 360/limit_time;

        run();
        mtx.unlock();
    }

    //Metodos de control para el Timer, si se ecnuentra ncendido o apagado
    void on_timer()
    {
        running = true;
    }
    void off_timer()
    {
        running = false;
    }

    //En este metodo se ejecuta todas las acciones del timer
    void run ()
    {
        cout<<"Limite de tiempo:"<<limit_time<<endl;
        cout<<degrees<<endl;
        running=true;
        while(limit_time>current_time && running){
            updateTimerSprite();
            Sleep(1000);
            if(seconds == 60){
                seconds = 0;
                minuts += 1;
            }
            seconds +=1;

            cout<<"Segundos:"<<seconds<<endl;
            current_time = (minuts*60) + seconds;
            cout<<"Tiempo actual:"<<current_time<<endl;
        }
        //Termino del turno
        if(running){
            cout<<"Cambiando turno"<<endl;
            callback();
            reset_Timer();
        }
        running=false;
    }

    void updateTimerSprite(){
        hand_timer_sprite.setOrigin(sf::Vector2f(50,50));
        hand_timer_sprite.setPosition(sf::Vector2f(50,50));
        hand_timer_sprite.setRotation( degrees * current_time );
    }

    void stop(){
        running=false;
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
        assert(limit_minuts >= 0 && new_minuts_limit <= 60);
    }

    void setSecondsLimit(int new_seconds_limit){
        limit_seconds = new_seconds_limit;
        assert(limit_seconds >= 0 && new_seconds_limit <= 60);
    }

    //Funcion amiga para el operador <<
    friend ostream& operator <<(ostream& os, Game_Timer& _game_timer)
    {
        os << _game_timer.minuts <<":"<<_game_timer.seconds;
        return os;
    }
};


#endif // CHESSTIMER_H_INCLUDED
