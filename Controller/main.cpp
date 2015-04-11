/* 
 * File:   main.cpp
 * Author: mn             yyyyy
 *
 * Created on 11 avril 2015, 14:23
 */

#include <cstdlib>
#include "ClavierHandler.h"
#include <iostream>

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    bool fin = false;
    ClavierHandler clavier(true);
    while(!fin)
    {
        int key = clavier.ObtenirDerniereToucheAppuyee();
        if(key != NUMPADNull)
        {
            std::cout << "key pressed: " << key << std::endl;
        }
    }
    return 0;
}

