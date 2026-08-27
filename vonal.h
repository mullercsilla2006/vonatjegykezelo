#ifndef VONAL_H_INCLUDED
#define VONAL_H_INCLUDED

#include <iostream>
#include <fstream> // Needed for ofstreamű

#include "seged.h"
#include "jegy.h"
#include "jarat.h"



class Vonal
{
private:
    int VonalID;

    int m_kapacitas = 10;


    int MegallokSzama = 0;
    std::string *Megallok;
    int *MegalloigKM;

    int JaratokSzama = 0;
    Jarat** Jaratok;

    int j_kapacitas = 10;

    void j_ujraallokal(); //jaratok újraallokálása (kapacitás megduplázása)
    void m_ujraallokal();  //megallok újraallokálása (kapacitás megduplázása)

public:
    Vonal(int id = 0): VonalID(id)  //kezdetben nincs felvéve megálló és járat
    {
        MegalloigKM = new int[m_kapacitas];
        Megallok = new std::string[m_kapacitas];
        Jaratok = new Jarat*[j_kapacitas];
        for (int i = 0; i < j_kapacitas; ++i)
        Jaratok[i] = nullptr;
    }
    Vonal(const Vonal& v);
    bool MegalloHozzaad(const std::string& megallonev, int megalloig);

    int JaratHozzaad(Jarat* j);
    void JaratTorol(int id);

    int indexkeres(std::string megallonev);
    bool Jegykeres(std::string fel, std::string le);
    int Jaratkeres(int id);
    int Megalloigkm(int i1, int i2);
    int MegalloigPerc(int i1, int i2);
    void ReadFromLine(std::ifstream& file);
    int WriteToLine(std::ofstream &file);
    void VonalPrint();
    void JaratokPrint();
    void Print(int id);
    virtual ~Vonal();

    int GetMegallokSzama() {return MegallokSzama;}
    int GetJaratokSzama() {return JaratokSzama;}
    std::string GetMegallo(int i) {return Megallok[i];}
    Jarat& GetJarat(int i) {return *Jaratok[i];}
    const Jarat& GetJarat(int i) const { return *Jaratok[i];}
    int GetID() {return VonalID;}
};

//menüből meghívott függvények
int Print_Jarat(Menetrend& m);
int Jarat_hozzaad(Menetrend& m);
int Jarat_torol(Menetrend& m);
int Jegy_kiad(Menetrend& m);





#endif // VONAL_H_INCLUDED
