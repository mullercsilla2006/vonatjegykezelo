#ifndef SEGED_H_INCLUDED
#define SEGED_H_INCLUDED


#include <iostream>

class Datum
{
private:
    int ev;
    int honap;
    int nap;
public:
    Datum(int e = 0, int h = 0, int n = 0): ev(e), honap(h), nap(n) {}
    Datum& operator=(const Datum& d);
    bool operator==(Datum& d) const {return (ev == d.ev && honap == d.honap && nap == d.nap);}
    void ReadFromLine(const std::string& s0);
    virtual ~Datum() {}
    friend std::ostream& operator<<(std::ostream& os, const Datum& d);

};

class Idopont
{
private:
    int ora;
    int perc;
public:
    Idopont(int o = 0, int p = 0): ora(o%24), perc(p%60) {}
    Idopont& operator=(const Idopont& i);
    bool operator==(const Idopont&i) const {return (ora==i.ora && perc==i.perc);}
    Idopont& operator+(const Idopont& i);
    Idopont& operator+(int p);
    void ReadFromLine(const std::string& s0);
    friend std::ostream& operator<<(std::ostream& os, const Idopont& ido);

    virtual ~Idopont() {}
};


std::string split(std::string szoveg, char c, int hanyadik);
void var();





#endif // SEGED_H_INCLUDED
