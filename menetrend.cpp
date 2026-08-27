

#include "seged.h"
#include "jarat.h"
#include "vonal.h"
#include "jegy.h"
#include "menetrend.h"



//kapacitás duplázása
void Menetrend::ujraallokal()
{
    Vonal **Uj_Vonalak = new Vonal*[kapacitas*2];
    for (int i = 0; i < VonalakSzama; i++)
    {
        Uj_Vonalak[i] = new Vonal(*Vonalak[i]);
        delete Vonalak[i];
    }
    kapacitas *= 2;
    delete[] Vonalak;
    Vonalak = Uj_Vonalak;
}

Menetrend::Menetrend(const Menetrend& m)
{
    kapacitas = m.kapacitas;
    VonalakSzama = m.VonalakSzama;
    Vonalak = new Vonal*[kapacitas];
    for (int i = 0; i < VonalakSzama; i++)
        Vonalak[i] = new Vonal(*m.Vonalak[i]);
}

//Tömbbe felveszünk egy elemet
//visszaadja a vonalak számát
int Menetrend::VonalHozzaad(Vonal* v)
{
    //hiba csak akkor lehet, ha már felvett vonalat próbál a felhasználó felvenni, ezt máshol kezeljük
    if (VonalakSzama >= kapacitas)
        ujraallokal();
    Vonalak[VonalakSzama] = v;
    VonalakSzama++;
    return VonalakSzama;
}

//Az összes Vonal között megkeressük az érkezõ és induló vonatokat, majd kiírjuk azokat
//visszaadja az első megtalált járat azonosítóját (tesztelés miatt), ha nincs 0
int Menetrend::Megallokeres(std::string megallonev, Datum date)
{
    int elso_jid = 0;
    std::cout << megallonev << ":\n";
    for (int i = 0; i < VonalakSzama; i++)
    {
        for (int j = 0; j < Vonalak[i]->GetMegallokSzama(); j++)
        {
            if (Vonalak[i]->GetMegallo(j) == megallonev)
            {
                int idx = Vonalak[i]->indexkeres(megallonev);
                for (int k = 0; k < Vonalak[i]->GetJaratokSzama(); k++)
                {
                    if (Vonalak[i]->GetJarat(k).GetDatum() == date)
                    {
                        int jaratid = Vonalak[i]->GetJarat(k).GetID();
                        if (elso_jid == 0) elso_jid = jaratid;

                        Idopont erkezik = Vonalak[i]->GetJarat(k).GetInd() + Vonalak[i]->MegalloigPerc(0, idx);
                        if (j != 0 && j != Vonalak[i]->GetMegallokSzama() - 1)
                            std::cout << "Járatszám: " << jaratid << "\térkezik: " << erkezik << "\tindul: " << erkezik+2;
                        else if (j == 0)
                            std::cout << "Járatszám: " << jaratid << "\tindul: " << erkezik;
                        else
                            std::cout << "Járatszám: " << jaratid << "\térkezik: " << erkezik+2;
                        std::cout << "\n";
                    }
                }
            }
        }
    }
    return elso_jid;
}

Menetrend::~Menetrend()
{
    for (int i = 0; i < VonalakSzama; i++)
        delete Vonalak[i]; //ugyan a lefoglalásról nem ez az osztály gondoskodik, itt szabadítjuk fel
    delete[] Vonalak;
}

int Menetrend::Vonalkeres(int id) //tombben az indexet adja vissza
{
    for (int i = 0; i < VonalakSzama; i++)
    {
        if (id == GetVonal(i).GetID())
            return i;
    }
    return -1; //helytelen id esetén
}

int Menetrend::WriteToLine(std::ofstream &file)
{
    int sorok = 0;
    file  << "MEN," << VonalakSzama << "\n";
    for (int i =  0; i < VonalakSzama; i++)
    {
        Vonal *v = Vonalak[i];
        sorok += v->WriteToLine(file);
    }
    return sorok + 1; //1 a Menetrend miatt
}

bool Menetrend::ReadFromLine(std::ifstream& file) //igazat ad, ha mindent sikerült beolvasni
{
    std::string buff;
    std::getline(file, buff);
    int vonalakdb = atoi(split(buff,',',2).c_str());
    //std::cout << "Menetrend beolva\n";
    for (int i = 0; i < vonalakdb; i++)
    {
        Vonal *v = new Vonal();
        v->ReadFromLine(file);
        VonalHozzaad(v);
    }
    return true;
}

void Menetrend::VonalTorol(int id)
{
    for (int i = 0; i < VonalakSzama; i++)
    {
        if (Vonalak[i]->GetID() == id)
        {
            delete Vonalak[i];

            if (i != VonalakSzama - 1)
            {
                Vonalak[i] = Vonalak[VonalakSzama - 1];
            }
            VonalakSzama--;
            return;
        }
    }
}

int Megallo_listaz(Menetrend& m)
{
    std::string megallo, buff;
    std::cout << "Adja meg a kívánt dátumot \"év.hónap.nap.\" formátumban! ";
    std::cin >> buff;

    Datum date;
    date.ReadFromLine(buff);

    std::cout << "\n\nAdja meg a megálló nevét! ";
    std::cin >> megallo;
    if (m.Megallokeres(megallo, date) == 0)
    {
        std::cout << "\nNem találtunk a keresésnek megfelelő járatot!";
    }
    var();
    return 0;
}

void Menetrend::Vonallista()
{
    for (int i = 0; i < GetVonalakSzama(); i++)
    {
        std::cout << i+1 << ".\t" << GetVonal(i).GetID() << "\t" << GetVonal(i).GetMegallo(0) << "-->" << GetVonal(i).GetMegallo(GetVonal(i).GetMegallokSzama()-1) << std::endl;
    }
}
int Vonal_hozzaad(Menetrend& m)
{
    int id;
    std::cout << "Adja meg a Vonal azonosítóját! ";
    std::cin >> id;
    bool helyes_inp = false;
    while (!helyes_inp)
    {
        bool letezik = false;
        for (int i = 0; i < m.GetVonalakSzama(); i++)
        {
            if (m.GetVonal(i).GetID() == id)
            {
                letezik = true;
                break;
            }
        }
        if (letezik)
        {
            std::cout << "\nA Vonal már létezik! Adjon meg egy helyes vonalazonosítót!";
            std::cin >> id;
        }
        else
            helyes_inp = true;
    }

    Vonal *v = new Vonal(id);
    int megallokdb;
    std::cout << "Adja meg a megállók számát! ";
    std::cin >> megallokdb;

    for (int i = 0; i < megallokdb; i++)
    {
        bool helyes_inp2 = false;
        while (!helyes_inp2)
        {
            std::string megallonev;
            int km = 0;

            std::cout << i + 1 << ". megálló neve: ";
            std::cin >> megallonev;

            if (i != 0)
            {
                std::cout << i + 1 << ". megállóig a kilométerek száma (indulástól): ";
                std::cin >> km;
            }
            if (v->MegalloHozzaad(megallonev, km))
            {
                helyes_inp2 = true;
            }
            else
            {
                std::cout << "\nA megálló már szerepel a megállók között, adjon meg helyeset!\n";
            }
        }
    }
    m.VonalHozzaad(v);
    std::cout << "\nVonal sikeresen hozzáadva!";
    var();
    return 0;
}


int Vonal_torol(Menetrend& m)
{
    if (m.GetVonalakSzama() == 0)
    {
        std::cout << "Nincsen törölhető Vonal!\n";
        var();
        return 0;
    }
    m.Vonallista();
    int idx;
    std::cout << "\nAdja meg a törölni kívánt vonal sorszámát! ";
    std::cin >> idx;
    bool helyes_inp = false;
    while (!helyes_inp)
    {
        if (idx >= 1 && idx <=m.GetVonalakSzama())
            helyes_inp = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx;
        }
    }
    m.VonalTorol(m.GetVonal(idx-1).GetID());
    std::cout << "\nVonal sikeresen törölve!";
    var();
    return 0;
}

