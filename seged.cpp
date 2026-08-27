#include <iostream>
#include <fstream>
#include <cstring>
#include <string>

#include "seged.h"
#include "memtrace.h"


Datum& Datum::operator=(const Datum& d)
{
    if (&d != this)
        {ev = d.ev;
        honap = d.honap;
        nap = d.nap;}
    return *this;
}

void Datum::ReadFromLine(const std::string& s0)
{
    try
    {
        ev = atoi(split(s0, '.', 1).c_str());
        honap = atoi(split(s0, '.', 2).c_str());
        nap = atoi(split(s0, '.', 3).c_str());
    }
    catch(const char* msg)
    {
        throw std::invalid_argument("Hibás dátum formátum! (Pl. 2025.05.17. várt)");
    }

}

Idopont& Idopont::operator=(const Idopont& i)
{
    if (&i != this)
        {ora = i.ora;
        perc = i.perc;}
    return *this;
}

Idopont& Idopont::operator+(const Idopont& i)
{
    ora += i.ora;
    perc += i.perc;
    if (perc >= 60)
    {
        ora += 1;
        perc = perc%60;
    }
    if (ora >= 24)
        ora = ora%24;
    return *this;
}

void Idopont::ReadFromLine(const std::string& s0)
{
    {
        try {
            ora = atoi(split(s0, ':', 1).c_str());
            perc = atoi(split(s0, ':', 2).c_str());
        }
        catch (const char* msg) {
            throw std::invalid_argument("Hibás időpont formátum! (Pl. 12:34 várt)");
        }
    }
}

Idopont& Idopont::operator+(int p)
{
    perc += p;
    if (perc >= 60)
    {
        ora += perc/60;
        perc = perc%60;
    }
    if (ora >= 24)
        ora = ora%24;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Idopont& ido) {
    if (ido.ora < 10) os << '0';
    os << ido.ora << ':';
    if (ido.perc < 10) os << '0';
    os << ido.perc;
    return os;
}

std::ostream& operator<<(std::ostream& os, const Datum& d)
{
    os << d.ev << '.' << d.honap << '.' << d.nap << '.';
    return os;
}

std::string split(std::string szoveg, char c, int hanyadik)
{
    std::string eredmeny = "";
    int x = 1;
    for (size_t i = 0; i < szoveg.length(); i++)
    {
        if (x == hanyadik && szoveg[i] != c)
            eredmeny += szoveg[i];
        else if (szoveg[i] == c)
            x++;
        else if (x > hanyadik)
            break;
    }
    if (eredmeny == "")
        throw "Sikertelen Split!";
    return eredmeny;
}
 void var()
 {
     std::cout << "\nTovábblépés gomblenyomással! ";
     std::cin.get();
 }

