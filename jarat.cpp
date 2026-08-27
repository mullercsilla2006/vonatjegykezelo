#include "seged.h"
#include "jegy.h"
#include "jarat.h"
#include "vonal.h"
#include "memtrace.h"


//kapacitás megduplázása
void Jarat::ujraallokal()
{
    Jegy** UjJegyek = new Jegy*[kapacitas*2];
    for (int i = 0; i < FoglaltHelyek; i++)
        UjJegyek[i] = EladottJegyek[i];

    kapacitas *= 2;
    delete[] EladottJegyek;
    EladottJegyek = UjJegyek;
}


int Jarat::JegyHozzaad(const Jegy& j)
{
    if (FoglaltHelyek >= kapacitas)
    {
        ujraallokal();
    }
    EladottJegyek[FoglaltHelyek] = j.clone();
    return FoglaltHelyek++;
}

Jarat::~Jarat()
{
    for (int i = 0; i < FoglaltHelyek; i ++)
        delete EladottJegyek[i];
    delete[] EladottJegyek;
}

Jarat& Jarat::operator=(const Jarat& j)
{
    if (this == &j) return *this;

    JaratID = j.JaratID;
    FoglaltHelyek = j.FoglaltHelyek;
    kapacitas = j.kapacitas;
    indulas = j.indulas;
    datum = j.datum;

    for (int i = 0; i < FoglaltHelyek; ++i) {
        delete EladottJegyek[i];
    }
    delete[] EladottJegyek;

    EladottJegyek = new Jegy*[kapacitas];
    for (int i = 0; i < FoglaltHelyek; ++i) {
        EladottJegyek[i] = j.EladottJegyek[i]->clone();
    }

    return *this;

}

Jarat::Jarat(const Jarat& j)
{
    JaratID = j.JaratID;
    FoglaltHelyek = j.FoglaltHelyek;
    kapacitas = j.kapacitas;
    indulas = j.indulas;
    datum = j.datum;
    EladottJegyek = new Jegy*[kapacitas];
    for (int i = 0; i < FoglaltHelyek; i++)
    {
        EladottJegyek[i] = j.EladottJegyek[i]->clone();
    }
}

int Jarat::WriteToLine(std::ofstream &file) //visszaadja a fájlbaírt sorok számát
{
    file << "\tJAR," << JaratID << "," << indulas << "," << datum << "," << FoglaltHelyek << "\n";
    for (int i = 0; i < FoglaltHelyek ; i++)
    {
        EladottJegyek[i]->WriteToLine(file);
    }
    return FoglaltHelyek + 1; //ennyi sort írt a fájlba
}

void Jarat::ReadFromLine(std::ifstream& file, int vid)
{
    std::string buff;
    std::getline(file, buff);
    JaratID = atoi(split(buff, ',', 2).c_str());
    //std::cout << "idopontsplit: " << split(buff, ',', 3) << "\n";
    indulas.ReadFromLine(split(buff, ',', 3));
    //std::cout << "datumsplit: " << split(buff, ',', 4) << "\n";
    datum.ReadFromLine(split(buff, ',', 4));
    int jegyekdb = atoi(split(buff, ',', 5).c_str());
    for (int i = 0; i < jegyekdb; i++)
    {
        std::string buff2;
        std::getline(file, buff2);
        if (buff2.find("DJG") != std::string::npos)
        {
            DiakJegy j(JaratID, vid);
            j.ReadFromLine(buff2);
            JegyHozzaad(j);
            //std::cout << "jegy hozzaadva\n";
        }
        else if (buff2.find("FJG") != std::string::npos)
        {
            FelnottJegy j(JaratID, vid);
            j.ReadFromLine(buff2);
            JegyHozzaad(j);
            //std::cout << "jegy hozzaadva\n";
        }
        //std::cout << i << ".\n";
    }
    //std::cout << "Járatbeolvasás ";
}


