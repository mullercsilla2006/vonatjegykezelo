#include <math.h>

#include "seged.h"
#include "jegy.h"
#include "jarat.h"
#include "menetrend.h"
#include "vonal.h"
#include "memtrace.h"

//Egy Vonalban tárolt járatok tömb kapacitásának megduplázása
void Vonal::j_ujraallokal()
{
    //std::cout << JaratokSzama << " jarat\t" << j_kapacitas << " kapacitas\n";
    Jarat** Uj_Jaratok = new Jarat*[j_kapacitas*2];
    for (int i = 0; i < JaratokSzama; i++)
    {
        Uj_Jaratok[i] = Jaratok[i];  // csak pointermásolás
    }
    delete[] Jaratok;
    j_kapacitas *= 2;
    Jaratok = Uj_Jaratok;
}


//Egy Vonalban tárolt megállók & megállóigkm tömb kapacitásának megduplázása
void Vonal::m_ujraallokal()
{
    int* Uj_Megalloigkm = new int[m_kapacitas*2];
    std::string* Uj_megallok = new std::string[m_kapacitas*2];
    for (int i = 0; i < MegallokSzama; i++)
    {
        Uj_Megalloigkm[i] = MegalloigKM[i];
        Uj_megallok[i] = Megallok[i];
    }
    delete[] Megallok;
    delete[] MegalloigKM;
    m_kapacitas *= 2;
    MegalloigKM = Uj_Megalloigkm;
    Megallok = Uj_megallok;
}

//Felvesz egy Vonalba egy új megállót megállónév, és az induláshoz képest megtett kilométerek alapján
//MegalloigKM és Megallok tömb változik
bool Vonal::MegalloHozzaad(const std::string& megallonev, int megalloig)
{
    for (int i = 0; i < MegallokSzama; i++)
    {
        if (Megallok[i] == megallonev)
            return false;
    }
    if (MegallokSzama >= m_kapacitas)
        m_ujraallokal();
    Megallok[MegallokSzama] = megallonev;
    MegalloigKM[MegallokSzama] = megalloig;
    MegallokSzama++;
    return true;
}

//Hozzáad egy járatot a Jaratok tömbhöz
//Jarat pointerek
int Vonal::JaratHozzaad(Jarat* j)
{
    //std::cout << "jarathozzaadba vunk" << JaratokSzama << "jarat\n";
    for (int i = 0; i < JaratokSzama; i++)
    {
        if (Jaratok[i]->GetID() == j->GetID())
            throw "A járat már létezik!";
        //std::cout << i << ". ciklus vege\n";
    }
    if (JaratokSzama >= j_kapacitas)
    {
        //std::cout << "ujraallokalba lepunk\n";
        j_ujraallokal();
    }

    Jaratok[JaratokSzama] = j;
    JaratokSzama++;
    //std::cout << JaratokSzama << " db járat\n";
    return JaratokSzama;
}

void Vonal::VonalPrint()
{
    if (JaratokSzama == 0)
    {
        std::cout << "\nNem indul Járat!";
        return;
    }
    std::cout << "Vonal azonosítója:\t" << VonalID << std::endl;
    for (int i = 0; i < MegallokSzama; i++)
    {
        if (i != 0)
            std::cout << i+1 << ". megálló: " << Megallok[i] << "\t" << MegalloigKM[i] << " km\n";
        else
            std::cout << "1. megálló: " << Megallok[0] << "\n";
    }

    std::cout << "\nIndulások időpontjai:\n";
    for (int j = 0; j < JaratokSzama; j++)
    {
        std::cout << j+1 << ".\t" << Jaratok[j]->GetInd() << " - " << Jaratok[j]->GetInd() + MegalloigPerc(0, MegallokSzama-1) << std::endl;
    }

    int idx;
    std::cout << "Adja meg a kiírni kívánt járat indexét! ";
    bool helyes_inp = false;
    std::cin >> idx;
    while (!helyes_inp)
    {
        if (idx >= 1 && idx <= JaratokSzama)
            helyes_inp = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx;
        }
    }
    Print(GetJarat(idx-1).GetID());
}

//Törlünk id alapján egy Jaratot a tömbből
void Vonal::JaratTorol(int id)
{
    for (int i = 0; i < JaratokSzama; i ++)
    {
        if (Jaratok[i]->GetID() == id)
        {
            delete Jaratok[i];
            if (i != JaratokSzama - 1)
                Jaratok[i] = Jaratok[JaratokSzama-1];
            JaratokSzama -= 1;
            return;
        }
    }
    throw "Hibás Járatazonosító!";
}

//std string megállónév alapján megkeressük a megálló indexét a tömbben
int Vonal::indexkeres(std::string megallonev)
{
    for (int i = 0; i < MegallokSzama; i++)
    {
        if (Megallok[i] == megallonev)
            return i;
    }
    throw "Hibás megállónév!";
}

//két index alapján adja meg, hogy a kettő megálló között hány km különbség van
int Vonal::Megalloigkm(int i1, int i2)
{
    if (i1 < 0 || i2 < 0 || i1 >= MegallokSzama || i2 >= MegallokSzama)
        throw "Érvénytelen index!";
    return abs(MegalloigKM[i1] - MegalloigKM[i2]);
}

Vonal::~Vonal()
{
    delete[] MegalloigKM;
    delete[] Megallok;
    for (int i = 0; i < JaratokSzama; i++)
        delete Jaratok[i]; //a lefoglalásról nem ez az osztály gondoskodik, de itt szabadítjuk fel
    delete[] Jaratok;
}

Vonal::Vonal(const Vonal& v)
    : VonalID(v.VonalID),
      m_kapacitas(v.m_kapacitas),
      MegallokSzama(v.MegallokSzama),
      JaratokSzama(v.JaratokSzama),
      j_kapacitas(v.j_kapacitas)
{
    // MegalloigKM másolása
    MegalloigKM = new int[m_kapacitas];
    for (int i = 0; i < MegallokSzama; ++i)
        MegalloigKM[i] = v.MegalloigKM[i];

    // Megallok másolása
    Megallok = new std::string[m_kapacitas];
    for (int i = 0; i < MegallokSzama; ++i)
        Megallok[i] = v.Megallok[i];

    // Jaratok másolása (mély másolat klónozással)
    Jaratok = new Jarat*[j_kapacitas];
    for (int i = 0; i < JaratokSzama; ++i)
        Jaratok[i] = new Jarat(*v.Jaratok[i]);
}

//a megtett km és megállók alapján számolja ki a megtett percek számát
int Vonal::MegalloigPerc(int i1, int i2)
{
     int km = Megalloigkm(i1, i2); //hiba esetén már ez a fgv gondoskodik róla
     int megallok = abs(i1-i2) ;
     int osszperc = (km / 1.5) + megallok*2;
     return osszperc;


//feltesszük hogy a vonat állandó 90 km/h = 1.5 km/min sebességgel megy, és minden megállón 2 percet tölt el.
}

//Kiírjuk a Vonal egy járatának és a vonalnak az adatait
void Vonal::Print(int id)
{
    for (int i = 0; i < JaratokSzama; i++)
    {
        if (Jaratok[i]->GetID() == id)
        {
            std::cout << "Járat #" << id << " megállói:\n";
            for (int j = 0; j < MegallokSzama; j++)
            {
                int perc = MegalloigPerc(indexkeres(Megallok[0]) , indexkeres(Megallok[j]));
                int km = Megalloigkm(indexkeres(Megallok[0]) , indexkeres(Megallok[j]));
                std::cout << "\t" << Megallok[j] << ": " << Jaratok[i]->GetInd() + perc << "\t" << km << " kilométer\n";
            }
            return; // ha megtaláltuk és kiírtuk, nem kell tovább keresni
        }
    }
    // ha nem találtuk meg
    throw "Nincs ilyen járatszám!";
}

int Vonal::WriteToLine(std::ofstream &file)
    {
        int sorok = 0;
        file << "VON," << VonalID << "," << JaratokSzama << "," << MegallokSzama << ",";
        for (int j = 0; j < MegallokSzama; j++)
        {
            file << Megallok[j] << "," << MegalloigKM[j];
            if (j < MegallokSzama - 1)  //utolsonal nincs vesszo
                file << ",";
        }
        file << "\n"; //egy sorba kerult az osszes megallo
        for (int j = 0; j < JaratokSzama; j++)
        {
            Jarat *ja = Jaratok[j];
            sorok += ja->WriteToLine(file);
        }
        return sorok + 1; //1 a Vonal sora miatt
    }

//Visszaadja, hogy a két megálló a vonal megállói között van-e, és helyes sorrendben vannak-e
bool Vonal::Jegykeres(std::string fel, std::string le)
{
    int talalt1 = -1;
    int talalt2 = -1;
    for (int i = 0; i < MegallokSzama; i++)
    {
        if (fel == Megallok[i])
            talalt1 = i;
        if (le == Megallok[i])
            talalt2 = i;
    }
    return ((talalt1 != -1) && (talalt2 != -1) && (talalt1 < talalt2));
}

int Vonal::Jaratkeres(int id) //tombben az indexet adja vissza
{
    for (int i = 0; i < JaratokSzama; i++)
    {
        if (id == Jaratok[i]->GetID())
            return i;
    }
    return -1; //ha nincs ilyen járat
}

void Vonal::ReadFromLine(std::ifstream& file)
{
    std::string buff;
    std::getline(file, buff);
    VonalID = atoi(split(buff, ',', 2).c_str());
    int jaratokdb = atoi(split(buff, ',', 3).c_str());
    int megallokdb = atoi(split(buff, ',', 4).c_str());
    for (int i = 0; i < megallokdb; i++)
    {
        std::string mnev = split(buff, ',', 5 + 2*i);
        int mkm = atoi(split(buff, ',', 6 + 2*i).c_str());
        MegalloHozzaad(mnev, mkm);
    }

    for (int j = 0; j < jaratokdb; j++)
    {
        Jarat *ja = new Jarat();
        ja->ReadFromLine(file, VonalID);
        JaratHozzaad(ja);
        //std::cout << j << ". \n";
    }
    //std::cout << "Vonalbeolvasás\n";
}
void Vonal::JaratokPrint()
{
    for (int i = 0; i < JaratokSzama; i++)
    {
        std::cout << i+1 << ".\t" << Jaratok[i]->GetInd() << std::endl;
    }
}

int Print_Jarat(Menetrend& m)
{
    m.Vonallista();
    if (m.GetVonalakSzama() == 0)
    {
        std::cout << "\nNincs felvéve vonal!";
        var();
        return 0;
    }
    int idx;
    std::cout << "Adja meg a kiírni kívánt Vonal indexét! ";
    std::cin >> idx;
    bool helyes_inp = false;
    while (!helyes_inp)
    {
        if (idx >= 1 && idx <= m.GetVonalakSzama())
            helyes_inp = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx;
        }
    }
    system("cls");
    m.GetVonal(idx-1).VonalPrint();
    var();
    return 0;
}



int Jarat_hozzaad(Menetrend& m)
{
    m.Vonallista();
    if (m.GetVonalakSzama() == 0)
    {
        std::cout << "\nNincs Vonal, amihez felvehetne járatot, kérjük először Vonalat vegyen fel!";
        var();
        return 0;
    }
    int idx;
    std::cout << "Adja meg a vonal indexét, amelyikhez járatot szeretne felvenni! ";
    std::cin >> idx;
    bool helyes_inp = false;
    while (!helyes_inp)
    {
        if (idx >= 1 || idx <= m.GetVonalakSzama())
            helyes_inp = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx;
        }
    }
    Vonal& v = m.GetVonal(idx-1);

    int id;
    std::cout << "\nAdja meg a járat azonosítóját! ";
    std::cin >> id;

    std::string d_str, ip_str;
    std::cout << "\nAdja meg az indulás dátumát \"év.hónap.nap.\" formátumban! ";
    std::cin >> d_str;

    std::cout << "\nAdja meg az indulás időpontját \"óra:perc\" formátumban! ";
    std::cin >> ip_str;

    Datum date;
    date.ReadFromLine(d_str);
    Idopont ind;
    ind.ReadFromLine(ip_str);

    Jarat* uj = new Jarat(id, ind, date);
    v.JaratHozzaad(uj);
    std::cout << "\nJárat sikeresen felvéve!";
    var();
    return 0;
}


int Jarat_torol(Menetrend& m)
{
    m.Vonallista();
    std::cout << "Adja meg a vonal indexét! ";
    bool helyes_inp1 = false;
    int idx1;
    std::cin >> idx1;
    while (!helyes_inp1)
    {
        if (idx1 >= 1 || idx1 <= m.GetVonalakSzama())
            helyes_inp1 = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx1;
        }
    }
    if (m.GetVonal(idx1-1).GetJaratokSzama() == 0)
    {
        std::cout << "\nNincs felvéve Járat!";
        var();
        return 0;
    }
    m.GetVonal(idx1-1).JaratokPrint();
    int idx2;
    std::cout << "\nAdja meg a törölni kívánt járat indexét!";
    std::cin >> idx2;
    bool helyes_inp2 = false;
    while (!helyes_inp2)
    {
        if (idx2 >= 1 || idx2 <= m.GetVonalakSzama())
            helyes_inp2 = true;
        else
        {
            std::cout << "\nHelyes sorszámot adjon meg!";
            std::cin >> idx2;
        }
    }
    m.GetVonal(idx1-1).JaratTorol(m.GetVonal(idx1-1).GetJarat(idx2-1).GetID());
    std::cout << "\nJárat sikeresen törölve!";
    var();
    return 0;
}

int Jegy_kiad(Menetrend& m)
{
    bool talalt = false;
    std::string fel, le;
    std::cout << "Felszállási hely: ";
    std::cin >> fel;
    std::cout << "\nLeszállási hely: ";
    std::cin >> le;

    std::string buff;
    std::cout << "\nAdja meg az utazás dátumát év.hó.nap. formátumban! " << std::endl;
    std::cin >> buff;
    Datum date;
    date.ReadFromLine(buff);

    int idx_fel = -1;
    int idx_le = -1;
    for (int i = 0; i < m.GetVonalakSzama(); i++)
    {
        if (m.GetVonal(i).Jegykeres(fel, le))
        {
            for (int j = 0; j < m.GetVonal(i).GetJaratokSzama(); j++)
            {
                if (m.GetVonal(i).GetJarat(j).GetDatum() == date)
                {
                    talalt = true;
                    idx_fel = m.GetVonal(i).indexkeres(fel);
                    idx_le = m.GetVonal(i).indexkeres(le);
                    Idopont indul = m.GetVonal(i).GetJarat(j).GetInd();
                    Idopont erkezik = m.GetVonal(i).GetJarat(j).GetInd() + m.GetVonal(i).MegalloigPerc(idx_fel, idx_le);
                    std::cout << "Vonalazonosító: " << m.GetVonal(i).GetID() << "\tJáratazonosító: " << m.GetVonal(i).GetJarat(j).GetID();
                    std::cout << ".\tindul: " << indul << "\térkezik: " << erkezik << "\n";
                }
            }
        }
    }
    if (talalt)
    {
        int v_id, j_id, tipus, j_idx, v_idx;
        std::cout << "\nAdja meg a kívánt induláshoz tartozó VONALazonosítót! ";
        std::cin >> v_id;
        bool helyes_inp1 = false;
        while (!helyes_inp1)
        {
            if (m.Vonalkeres(v_id) != -1)
            {
                helyes_inp1 = true;
                v_idx = m.Vonalkeres(v_id);
            }
            else
            {
                std::cout << "\nHelytelen input! Adjon meg helyeset!";
                std::cin >> v_id;
            }
        }
        std::cout << "\nAdja meg a kívánt induláshoz tartozó JÁRATazonosítót! ";
        std::cin >> j_id;
        bool helyes_inp2 = false;
        while (!helyes_inp2)
        {
            if (m.GetVonal(v_idx).Jaratkeres(j_id) != -1)
            {
                helyes_inp2 = true;
                j_idx = m.GetVonal(v_idx).Jaratkeres(j_id);
            }
            else
            {
                std::cout << "\nHelytelen input! Adjon meg helyeset!";
                std::cin >> j_id;
            }
        }
        std::cout << "\nVálassza ki a jegy típusát\t1 Diákjegy\t2 Felnőttjegy: ";
        std::cin >> tipus;
        bool helyes_inp3 = false;
        while (!helyes_inp3)
        {
            if (tipus == 1 || tipus == 2)
                helyes_inp3 = true;
            else
            {
                std::cout << "\nHelytelen input! Adjon meg helyeset!";
                std::cin >> tipus;
            }
        }

        Jegy *d = nullptr;
        if (tipus == 1) //diakjegyet valasztott
            d = new DiakJegy(j_id, v_id, fel, le);
        else //felnott
            d = new FelnottJegy(j_id, v_id, fel, le);
        m.GetVonal(v_idx).GetJarat(j_idx).JegyHozzaad(*d);

        system("cls");
        idx_fel = m.GetVonal(v_idx).indexkeres(fel);
        idx_le = m.GetVonal(v_idx).indexkeres(le);
        Idopont ind = m.GetVonal(v_idx).GetJarat(j_idx).GetInd();
        int percek = m.GetVonal(v_idx).MegalloigPerc(idx_fel, idx_le);

        std::cout << "Vonal azonosítója: " << v_id << "\tJárat azonosítója: " << j_id << "\n";
        std::cout << "Vonat indul: " << date << " " << ind << "\térkezik: " << ind + percek << "\tösszesen: " << percek << " perc\n";
        std::cout << d->kocsiszam(m) << ". kocsi\t" << d->helyszam(m) << ". hely\n";
        std::cout << "Jegy ára: " << d->ar(m) << "\n";
        delete d; //a foglalásokat a clone() függvények végzik (Jegyhozzaadban), itt nem kell
        var();
    }
    else
    {
        std::cout << "Nincs a keresésnek megfelelő Járat!";
    }
    var();
    return 0;
}

