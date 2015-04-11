//David Lafontaine Felix Vallieres
//1 Novembre 2012
//Fichier CPortSerie.h
//D�claration de la classe CPortSerie pour protocole de domotique sp�cifiquement
#ifndef PORTSERIE_H
#define PORTSERIE_H

#include <Windows.h>
#include <vector>
#include <string>
using std::string;
using std::vector;

//DOCUMENTATION MINIMALE SUR LE PROTOCOLE X10////////////////////
// Param�tres du protocole Baud:9600  DATABITS:8  PARIT�:NONE STOBITS:1
// Commande � envoyer avant le d�but de toute commande STX(0x02)
// Le PowerLinc r�pond par un ACK(0x06), suivi d'un CR(0x0D)=PRET d'un NAK(0x015)=PAS PRET
// Exemple pour envoyer on � l'adresse A1, apr�s avoir envoyer STX et v�rifier que le powerlinc est PRET(CR) 
// 1.0x63 : indique l'envoi d'une commande X10
// 2.0x46 : house code = A
// 3.0x4C : unit code = 1
// 4.0x45 : commande = "on"
// 5.0x41 : repeat count = 1
///////////////////////////////////////////////////////

class PortSerie {
    HANDLE _PortSerie; //Handle permettant d'�crire sur le port 
    DCB _Dcb; // DCB permettant de controler les parametres de la communication s�rie
    COMMTIMEOUTS _Delais; //Controle du d�lais de lecture sur le port
    DWORD NombreOctets; //Nombre Octets � lire par bloc
    DWORD nbCaracteresLus; //le nombre de caract�res effectivement lus


public:
    //Constructeurs et Destructeurs

    PortSerie() {
    }
    PortSerie(const PortSerie& PortSerie);
    PortSerie(int Port);
    ~PortSerie();

private:
    int BaudRate;
    int comDelay;
    int port;
    int delais;
    int attenteLecture;
    int byteSize;
    
    
    //M�thodes D'initialisation du PortSerie
    void CreerPort(int baudrate, int comDelay, int port, int attenteLecture, int byteSize);
    void InitialiserDcb();
    void ReglerDelais(short Delais);

    //M�thode pour capter les erreurs du PortSerie
    void TraiterErreur(string message, DWORD erreur);
    void NettoyerBufferLecture();

public:
    //M�thode d'ecriture de lecture et de nettoyage du port utilis�
    int Envoyer(vector<char> Vals);
    int Envoyer(char Lettre, short Numero, short Requete);
    int Envoyer(short Val);
    char LectureChar();
    short LectureShort();
    unsigned char* LecturePlusieursOctets();

    PortSerie& operator=(const PortSerie& PortSerie) {
        _PortSerie = PortSerie._PortSerie;
        _Dcb = PortSerie._Dcb;
        _Delais = PortSerie._Delais;
        NombreOctets = PortSerie.NombreOctets;
        nbCaracteresLus = PortSerie.nbCaracteresLus;

        return *this;
    }
};
#endif
