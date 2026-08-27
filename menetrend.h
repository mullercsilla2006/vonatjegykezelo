#ifndef MENETREND_H_INCLUDED
#define MENETREND_H_INCLUDED

#include "vonal.h"


class Menetrend
{
private:
    Vonal** Vonalak;
    int VonalakSzama = 0;
    int kapacitas = 10;

    void ujraallokal();
public:
    Menetrend() //kezdetben nincs benne vonal
    {
        Vonalak = new Vonal*[kapacitas];
    }
    Menetrend(const Menetrend& m);
    int VonalHozzaad(Vonal* v);
    int Vonalkeres(int id);
    void VonalTorol(int id);
    void Vonallista();
    virtual ~Menetrend();
    int Megallokeres(std::string megallonev, Datum date); //visszaadja az első megtalált járat azonosítóját (tesztelés miatt)

    int GetVonalakSzama() const {return VonalakSzama;}
    Vonal& GetVonal(int i) const {return *Vonalak[i];}
    int WriteToLine(std::ofstream &file);
    bool ReadFromLine(std::ifstream& file);
};

//menüből meghívott függvények
int Megallo_listaz(Menetrend& m);
int Vonal_hozzaad(Menetrend& m);
int Vonal_torol(Menetrend& m);


#endif // MENETREND_H_INCLUDED
