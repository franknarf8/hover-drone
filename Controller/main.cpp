/* 
 * File:   main.cpp
 * Author: mn             yyyyy
 *
 * Created on 11 avril 2015, 14:23
 */

#include <cstdlib>
#include "ClavierHandler.h"
#include <iostream>
#include <chrono>
#include <thread>
#include "windows.h"
#include "stdio.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    bool fin = false;
    ClavierHandler clavier(true);
    while (!fin) {
        char c;
        std::cout << "Enter text. Include a dot ('.') in a sentence to exit:" << std::endl;
        do {
            c = getchar();
            putchar(c);
        }
        while (c != '.');
        return 0;
        int key = clavier.ObtenirDerniereToucheAppuyee();
        if (key != 0) {
            std::cout << "key pressed: " << key << std::endl;
        }
        std::cout << "key pressed: " << key << std::endl;
        Sleep(250);
    }
    return 0;
}

