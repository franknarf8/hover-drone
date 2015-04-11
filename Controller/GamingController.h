/* 
 * File:   Controller.h
 * Author: mn             yyyyy
 *
 * Created on 11 avril 2015, 14:42
 */

#ifndef GAMINGCONTROLLER_H
#define	GAMINGCONTROLLER_H
#include "PortSerie.h"
#include <vector>
#include <iostream>

using std::vector;

class GamingController 
{
private:
    PortSerie portSerie;
public:
    GamingController(int baudrate, int comDelay, int port, int attenteLecture, int byteSize) 
    {
        portSerie = PortSerie(baudrate, comDelay, port, attenteLecture, byteSize);
    }
    
    void LireCommande()
    {
        for(auto i = 0; i < 16; ++i)
        {
           std::cout << portSerie.LectureChar() << std::endl; 
        }
    }
    
    
};

#endif	/* GAMINGCONTROLLER_H */

