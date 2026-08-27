#ifndef JARAT_H_INCLUDED
#define JARAT_H_INCLUDED

#include "seged.h"

class Jegy;

class Jarat
{
private:
    int JaratID;
    int FoglaltHelyek;
    int kapacitas;
    Jegy** EladottJegyek;
    Idopont indulas;
    Datum datum;

    void ujraallokal(); //(kapacitás megduplázása)
public:
    Jarat(): JaratID(0), FoglaltHelyek(0), kapacitas(10), indulas(), datum()
        {
            EladottJegyek = new Jegy*[kapacitas];
        }
    Jarat(const int id, const Idopont& ind, const Datum& d): JaratID(id), FoglaltHelyek(0), kapacitas(10), indulas(ind), datum(d)
        {
            EladottJegyek = new Jegy*[kapacitas];
        }
    //kezdetben nincs eladva jegy
    Jarat(const Jarat& j);

    int GetID() const {return JaratID;}
    Idopont GetInd() const {return indulas;}
    Datum GetDatum() const {return datum;}
    int GetFoglalt() const {return FoglaltHelyek;}

    Jarat& operator=(const Jarat& j);
    int JegyHozzaad(const Jegy& j);
    int WriteToLine(std::ofstream& file);
    void ReadFromLine(std::ifstream& file, int vid);
    virtual ~Jarat();
};





#endif // JARAT_H_INCLUDED
