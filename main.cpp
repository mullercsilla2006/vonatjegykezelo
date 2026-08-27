#include <iostream>
#include <string.h>

#include "seged.h"
#include "jegy.h"
#include "jarat.h"
#include "vonal.h"
#include "menetrend.h"
#include "gtest_lite.h"
#include "memtrace.h"

#define exitmenu 15

typedef int(*handler)(Menetrend& m);
//menü kezelése
class MenuGroup;

class MenuItem {
private:
    std::string label;
    handler itemProcessor;      // függvény pointer
    char key;
    MenuGroup *childMenu;


public:
    MenuItem(std::string lbl, handler proc, char k, MenuGroup *child = nullptr)
        : label(lbl), itemProcessor(proc), key(k), childMenu(child)  {}

    std::string getLabel() const { return label; }
    int process(Menetrend& m) const { return itemProcessor ? itemProcessor(m) : 0; }
    char getKey() const { return key; }
    MenuGroup* getChild() const { return childMenu; }

    void setChild(MenuGroup* child) { childMenu = child; }
};


class MenuGroup
{
private:
    MenuItem *menupontok[10];
    int cnt;
    MenuGroup *parentmenu;
public:
    static Menetrend mr; //menetrendből ez az egy lesz
    MenuGroup()
    {
        cnt = 0;
        parentmenu = NULL;
    }
    ~MenuGroup()
    {
        for (int i = 0; i < cnt; i++)
            delete menupontok[i];
    }
    void AddMenuItem(std::string label,handler itemProcessor,char key)
    {
        MenuItem *mit = new MenuItem(label,itemProcessor,key);
       // std::cout << "menuitem added"  << "\t" << label << " parent" << parentmenu << std::endl;
      ;
        menupontok[cnt++] = mit;
    }
    MenuItem *FirstItem()
    {
        return menupontok[0];
    }
    int MenuCount()
    {
         return cnt;
    }
    void SetChild(int i,MenuGroup *child)
    {
        menupontok[i]->setChild(child);
        child->parentmenu = this;
    }

    MenuGroup *DoMenu ();

};

MenuGroup* MenuGroup::DoMenu()
{
    int hanyelem = MenuCount();
    while (true)
    {
        system("cls");
        for (int i = 0; i < hanyelem; i++)
        {
            MenuItem *curritem = menupontok[i];
            std::cout << curritem->getKey()  << "\t" << curritem->getLabel() << std::endl;
        }
        std::cout << "\n\nVálasszon a fenti menüpontok közül a megfelelő karakter lenyomásával!";
        char valasz = std::cin.get();
        char kuka;
        while ((kuka = std::cin.get()) != '\n');
        bool talalt = false;
        for (int i = 0; i < hanyelem; i++)
        {
            MenuItem *curritem = menupontok[i];
            if (valasz == curritem->getKey())
            {
                if (parentmenu != NULL && i == 0)
                {
                    return parentmenu;
                }
                else if (curritem->getChild() != NULL)
                {
                    return curritem->getChild();
                }
                else
                {
                    system("cls");
                    int returnval = curritem->process(mr);
                    if (returnval == exitmenu)
                        return NULL;
                }
                talalt = true;
                break;
            }
        }
        if (!talalt)
        {
            std::cout << "\n" << valasz << "-gombhoz nem tartozik funkció. Folytatás gomblenyomással.";
            std::cin.get();
        }
        return this;
    }
}

Menetrend MenuGroup::mr;

int kilepes(Menetrend& m)
{
    std::ofstream m_ofile("menetrend.txt");
    m.WriteToLine(m_ofile); //itt írjuk ki a fájlba
    return exitmenu;}


int main()
{
    system("chcp 65001");
    //a menu mukodesehez definialni kell, a felhasznalotol varja az inputokat
    //#define MENU


    //#define CPORTA

    #ifdef MENU
    MenuGroup maingroup;
    MenuGroup submenu1;
    MenuGroup submenu2;

    maingroup.AddMenuItem("Menetrendkezelői funkciók", NULL, '1');
    maingroup.AddMenuItem("Felhasználói funkciók", NULL, '2');
    maingroup.AddMenuItem("Kilépés", kilepes, 'x');

    submenu1.AddMenuItem("<<Vissza", NULL, 'b');
    submenu1.AddMenuItem("Vonal felvétele", Vonal_hozzaad,'1');
    submenu1.AddMenuItem("Járat felvétele", Jarat_hozzaad, '2');
    submenu1.AddMenuItem("Vonal törlése", Vonal_torol, '3');
    submenu1.AddMenuItem("Járat törlése", Jarat_torol, '4');
    submenu1.AddMenuItem("Kilépés", kilepes, 'x');

    submenu2.AddMenuItem("<<Vissza", NULL, 'b');
    submenu2.AddMenuItem("Egy járat adatai",Print_Jarat,'1');
    submenu2.AddMenuItem("Megállóra érkező/induló vonatok", Megallo_listaz, '2');
    submenu2.AddMenuItem("Jegyvásárlás", Jegy_kiad, '3');
    submenu2.AddMenuItem("Kilépés", kilepes, 'x');

    maingroup.SetChild(0,&submenu1);
    maingroup.SetChild(1, &submenu2);

    MenuGroup *currgroup = &maingroup;

    std::ifstream menetrend_file("menetrend.txt");
    if (MenuGroup::mr.ReadFromLine(menetrend_file))
    {
        while (true )
        {
            currgroup = currgroup->DoMenu();
            if (!currgroup) break;
        }
    }

    #endif // MENU

    #ifdef CPORTA
    Menetrend m;
    TEST(fajlbeolvas, sikeres)
    {
        std::ifstream file1("hazi.txt");
        EXPECT_TRUE(m.ReadFromLine(file1));
        file1.close();
    }END

    TEST(vonalkeres, id)
    {
        EXPECT_EQ(1, m.Vonalkeres(333));
    }END

    TEST(erkezo_indulo_jaratok, elso)
    {
        Datum d(2026, 1, 24);
        EXPECT_EQ(127,m.Megallokeres("Balatonfüred", d));

    }END

    TEST(megallok_keresese, igaz)
    {
        EXPECT_TRUE(m.GetVonal(0).Jegykeres("Pécs", "Balatonfüred"));
    }END

    TEST(vonalakszama, vonaltorles)
    {
        EXPECT_NO_THROW(m.VonalTorol(333));
        EXPECT_EQ(1, m.GetVonalakSzama());
    }END

    Vonal *v2 = new Vonal(459);
    TEST(vonalhozzaad, vonalakszama)
    {
        EXPECT_EQ(2, m.VonalHozzaad(v2));
        EXPECT_EQ(1, m.Vonalkeres(459));
    }END

    TEST(megallohozzad, megallokszama)
    {
        EXPECT_TRUE(v2->MegalloHozzaad("Győr", 0));
        v2->MegalloHozzaad("Sopron", 90);
        EXPECT_EQ(2, v2->GetMegallokSzama());
        EXPECT_EQ(1, v2->indexkeres("Sopron"));
        EXPECT_FALSE(v2->MegalloHozzaad("Sopron", 120));
    }END

    TEST(megalloigkm, megalloigperc)
    {
        EXPECT_EQ(90, v2->Megalloigkm(0, 1));
        EXPECT_EQ(60+2, v2->MegalloigPerc(0, 1));
    }END

    Datum date(2025, 7, 9), date2(2026,1,14);
    Idopont ind(15, 54), ind2(21, 12);
    Jarat *ja = new Jarat(774, ind, date);
    Jarat* ja2 = new Jarat(884, ind2, date2);
    TEST(Jarathozzaad_jarattorol, jaratokszama)
    {
        EXPECT_EQ(1, v2->JaratHozzaad(ja));
        EXPECT_EQ(2, v2->JaratHozzaad(ja2));
        EXPECT_EQ(0, v2->Jaratkeres(774));
        v2->JaratTorol(774);
        EXPECT_EQ(1, v2->GetJaratokSzama());
    }END

    DiakJegy dj(774, 459, "Győr", "Sopron");
    FelnottJegy fj(884, 459, "Győr", "Sopron");
    FelnottJegy fj2(884, 459, "Győr", "Sopron");
    TEST(jegyhozzaad, jegyekszama_arak)
    {
        ja2->JegyHozzaad(dj);
        ja2->JegyHozzaad(fj);
        ja2->JegyHozzaad(fj2);
        EXPECT_EQ(3, ja2->GetFoglalt());
        EXPECT_EQ(90*9, dj.ar(m));
        EXPECT_EQ(90*11, fj.ar(m));
    }END

    TEST(kocsiszam _helyszam, Jarat_Vonal)
    {
        EXPECT_EQ(1, fj2.kocsiszam(m));
        EXPECT_EQ(3, fj2.helyszam(m));

        Jarat j = fj.GetJarat(m);
        Vonal v = fj.GetVonal(m);
        EXPECT_EQ(884, j.GetID());
        EXPECT_EQ(459, v.GetID());
    }END

    Vonal *vo = new Vonal(456);
    for (int i = 0; i < 10; i++)
        {
            Vonal* v = new Vonal(i);
            m.VonalHozzaad(v);
        }
    Idopont ip(10,45);
    Datum d(2025,7,6);
    for (int j = 0; j < 15; j++)
    {
        //std::cout << vo->GetJaratokSzama() << "járat\n";
       // std::cout << j << ". jarathozzaad\n";
        Jarat *ja = new Jarat(j, ip, d);
        vo->JaratHozzaad(ja);
    }
    m.VonalHozzaad(vo);

    std::string Megallonevek[] = {"Budapest-Keleti","Kőbánya-Kispest","Monor","Cegléd","Szolnok","Törökszentmilós","Mezőtúr","Gyomaendrőd","Békéscsaba","Lőkösháza","Arad"};
    int megalloigkm[] = {0, 30, 60, 90, 120, 150, 180, 210, 240, 270, 300};
    for (int i = 0; i < 11; i++)
    {
        vo->MegalloHozzaad(Megallonevek[i], megalloigkm[i]);
    }
    TEST(ujraallokacio, dintomb)
    {
        vo->Print(2);
        std::cout << "\n";
        vo->JaratokPrint();
        EXPECT_EQ(11, vo->GetMegallokSzama());
        EXPECT_EQ(13, m.GetVonalakSzama());
        EXPECT_EQ(15, vo->GetJaratokSzama());
    }END


    TEST(idopont, hozzaad)
    {
        Idopont i0(16, 34);
        Idopont i1(2, 0);
        Idopont res1(16, 40);
        Idopont res2(18, 40);
        EXPECT_EQ(res1, i0+6);
        EXPECT_EQ(res2, i0+i1);
    }END

    TEST(sorbololvas, seged)
    {
        Idopont i0(16, 34);
        Idopont i1;
        i1.ReadFromLine("16:34");
        EXPECT_EQ(i0, i1);
        Datum d0(2006,1,24);
        Datum d1;
        d1.ReadFromLine("2006.01.24.");
        EXPECT_EQ(d0, d1);

    }END

    TEST(fajlkiir, sorokszama)
    {
        std::ofstream file2("hazi2.txt");
        EXPECT_EQ(36, m.WriteToLine(file2));
        file2.close();
    }END

    #endif // CPORTA



}
