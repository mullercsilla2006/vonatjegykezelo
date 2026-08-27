#include "seged.h"
#include "jegy.h"
#include "jarat.h"
#include "vonal.h"
#include "menetrend.h"
#include "memtrace.h"


Jegy& Jegy::operator=(const Jegy& j)
{
    if (this != &j)
        {FelszallasiHely = j.FelszallasiHely;
        LeszallasiHely = j.LeszallasiHely;
        JaratID = j.JaratID;
        VonalID = j.VonalID;}
    return *this;
}

void Jegy::ReadFromLine(const std::string& s0)
{
    FelszallasiHely = split(s0, ',', 2);
    LeszallasiHely = split(s0, ',', 3);
    //std::cout << "Jegybeolvasás ";
}

Jegy* DiakJegy::clone() const
{
    return new DiakJegy(*this);
}

Jegy* FelnottJegy::clone() const
{
    return new FelnottJegy(*this);
}

//Vonalid alapján visszaadja a Vonalat, amire vették a jegyet
Vonal Jegy::GetVonal(const Menetrend& m) const
{
     for (int i = 0; i < m.GetVonalakSzama(); i++)
    {
        if (VonalID == m.GetVonal(i).GetID())
            return m.GetVonal(i);
    }
    throw "Hibás Vonalazonosító!";
}

//Jaratid alapján visszaadja a Járatot, amire vették a jegyet
Jarat Jegy::GetJarat(const Menetrend& m) const
{
    Vonal v = GetVonal(m);
    for (int i = 0; i < v.GetJaratokSzama(); i++)
    {
        if (JaratID == v.GetJarat(i).GetID())
            return v.GetJarat(i);
    }
    throw "Hibás Járatazonosító!";
}

//Jarat foglalt helyei alapján
int Jegy::kocsiszam(const Menetrend& m) const
{
    return GetJarat(m).GetFoglalt() / 100 + 1; //feltételezzük, hogy egy kocsiban 100 hely van
}

//Jarat foglalt helyei alapján
int Jegy::helyszam(const Menetrend& m) const
{
    return GetJarat(m).GetFoglalt() % 100; //feltételezzük, hogy egy kocsiban 100 hely van
}

//A felszállási és leszállási hely közti km különbség alapján
int DiakJegy::ar(const Menetrend& m) const
{
    int i1 = GetVonal(m).indexkeres(GetFel());
    int i2 = GetVonal(m).indexkeres(GetLe());
    return GetVonal(m).Megalloigkm(i1, i2) * 9; //feltesszük, hogy km-enként 9 ft
}

//A felszállási és leszállási hely közti km különbség alapján
int FelnottJegy::ar(const Menetrend& m) const
{
    int i1 = GetVonal(m).indexkeres(GetFel());
    int i2 = GetVonal(m).indexkeres(GetLe());
    return GetVonal(m).Megalloigkm(i1, i2) * 11; //km-enként 11 ft
}


int DiakJegy::WriteToLine(std::ofstream& file)
{
    file << "\t\tDJG," << FelszallasiHely << "," << LeszallasiHely << "\n";
    return 0;
}

int FelnottJegy::WriteToLine(std::ofstream& file)
{
    file << "\t\tFJG," << FelszallasiHely << "," << LeszallasiHely << "\n";
    return 0;
}
