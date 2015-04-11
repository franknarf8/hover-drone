//David Lafontaine Felix Vallieres
//1 Novembre 2012
//Fichier CPortSerie.cpp
//D�finition de la classe CPortSerie
#include "CPortSerie.h"
#include <conio.h>
#include <iostream>
using std::cout;
using std::endl;

CPortSerie::CPortSerie(int baudrate, int comDelay, int port, int attenteLecture, int byteSize)
{
    //Appel de Fonctions pour Cr�er ouvrir et Initialiser le PortS�rie
   
    this->BaudRate = baudrate;
    this->comDelay = comDelay;
    this->port = port;
    this->attenteLecture = attenteLecture;
    this->byteSize = byteSize;
    CreerPort(port);
    InitialiserDcb();
    ReglerDelais(comDelay);
    NombreOctets = 0;
    nbCaracteresLus = 0;
}

CPortSerie::CPortSerie(const CPortSerie& PortSerie)
{
    *this = PortSerie;
}

void CPortSerie::CreerPort(int Port)
{
    //Affectation du Handle et Cr�ation du Port
    _PortSerie = CreateFile("COM" + Port, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    if (_PortSerie == INVALID_HANDLE_VALUE) {
        TraiterErreur("dans CreateFile", GetLastError());
    }
}

void CPortSerie::InitialiserDcb()
{
    //Parametres de Communication
    if (!GetCommState(_PortSerie, &_Dcb)) {
        TraiterErreur("Dans GetCommState", GetLastError());
    }
    //Modification des parametres principaux pour un protocole X10
    _Dcb.BaudRate = BaudRate;
    _Dcb.ByteSize = byteSize;
    _Dcb.Parity = NOPARITY;
    _Dcb.StopBits = ONESTOPBIT;

    //application des nouveaux parametres
    if (!SetCommState(_PortSerie, &_Dcb)) {
        TraiterErreur("dans SetCommState", GetLastError());
    }
}

void CPortSerie::ReglerDelais(short Delais)
{
    // mets tous les champs de la structure � z�ro
    memset(&_Delais, 0, sizeof ( _Delais));

    // d�lai total de lecture
    _Delais.ReadIntervalTimeout = 0;
    _Delais.ReadTotalTimeoutMultiplier = 0;
    _Delais.ReadTotalTimeoutConstant = Delais;
    _Delais.WriteTotalTimeoutMultiplier = 0;
    _Delais.WriteTotalTimeoutConstant = 0;

    if (!SetCommTimeouts(_PortSerie, &_Delais)) {
        TraiterErreur("dans SetCommTimeouts", GetLastError());
    }
}

int CPortSerie::Envoyer(vector<char> Vals)
{
    DWORD caracteresEcrits;
    WriteFile(_PortSerie, &Vals, Vals.size(), &caracteresEcrits, 0); //Eciture sur le port serie des octet un par un
    if (Vals.size() == caracteresEcrits) {
        return 0;
    }
    return 1;
}

int CPortSerie::Envoyer(char Lettre, short Numero, short Requete)
{
    DWORD caracteresEcrits;
    WriteFile(_PortSerie, ("" + Lettre + Numero + Requete), 5, &caracteresEcrits, 0); //Eciture sur le port serie
    if (3 == caracteresEcrits) {
        return 0;
    }
    return 1;
}

int CPortSerie::Envoyer(short Val)
{
    DWORD caracteresEcrits;
    WriteFile(_PortSerie, &Val, 2, &caracteresEcrits, 0); //Eciture sur le port serie
    if (3 == caracteresEcrits) {
        return 0;
    }
    return 1;
}

char CPortSerie::LectureChar()
{
    char Buffer;

    if (!ReadFile(_PortSerie, &Buffer, 1, &nbCaracteresLus, NULL)) {
        TraiterErreur("dans ReadFile", GetLastError());
    }
    Sleep(attenteLecture);
    return Buffer;
}

short CPortSerie::LectureShort()
{
    short Buffer;

    if (!ReadFile(_PortSerie, &Buffer, 2, &nbCaracteresLus, NULL)) {
        TraiterErreur("dans ReadFile", GetLastError());
    }
    Sleep(attenteLecture);
    return Buffer;
}

void CPortSerie::NettoyerBufferLecture()
{
    PurgeComm(_PortSerie, PURGE_RXCLEAR);
}

void CPortSerie::TraiterErreur(string message, DWORD erreur)
{
    cout << "Erreur no " << erreur << " dans " << message << endl;
    ExitProcess(1);
}

CPortSerie::~CPortSerie()
{
    if (!CloseHandle(_PortSerie))//Fermeture du port 
    {
        TraiterErreur("Erreur dans CLOSEHANDLE", GetLastError());
    }
}
