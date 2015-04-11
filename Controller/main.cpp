/* 
 * File:   main.cpp
 * Author: mn             yyyyy
 *
 * Created on 11 avril 2015, 14:23
 */

#include <cstdlib>
#include "GamingController.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv)
{
    GamingController controller(9600, 2, 2, 1, 8);
    while (true) 
    {
        controller.LireCommande();
    }

    return 0;
}

