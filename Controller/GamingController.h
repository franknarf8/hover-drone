/* 
 * File:   Controller.h
 * Author: mn             yyyyy
 *
 * Created on 11 avril 2015, 14:42
 */

#ifndef GAMINGCONTROLLER_H
#define	GAMINGCONTROLLER_H
#include "CPortSerie.h"
#include <vector>

using std::vector;

class GamingController 
{
    CPortSerie portSerie;

    GamingController(int baudrate, int comDelay, int port, int attenteLecture, int byteSize) 
    {
        portSerie = CPortSerie(baudrate, comDelay, port, attenteLecture, byteSize);
    }
    
public:
    void LireCommande()
    {
        for(auto i = 0; i < 16; ++i)
        {
           std::cout << portSerie.LectureChar() << std::endl; 
        }
    }
    
    
};

#endif	/* GAMINGCONTROLLER_H */

