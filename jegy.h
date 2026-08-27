#ifndef JEGY_H_INCLUDED
#define JEGY_H_INCLUDED

#include "seged.h"

class Menetrend;
class Vonal;
class Jarat;

class Jegy
{
protected:
    int JaratID;
    int VonalID;
    std::string FelszallasiHely;
    std::string LeszallasiHely;

public:
    Jegy() {}
    Jegy(int jid, int vid, const std::string s0 = "", const std::string s1 = ""): JaratID(jid), VonalID(vid), FelszallasiHely(s0), LeszallasiHely(s1) {}
    int kocsiszam(const Menetrend& m) const;
    int helyszam(const Menetrend& m) const;
    virtual int ar(const Menetrend& m) const = 0;
    virtual void ReadFromLine(const std::string& s0);
    virtual int WriteToLine(std::ofstream& file) = 0;
    virtual ~Jegy() {}
    virtual Jegy* clone() const = 0;
    Jegy& operator=(const Jegy& j);

    Jarat GetJarat(const Menetrend& m) const;
    Vonal GetVonal(const Menetrend& m) const;

    std::string GetFel() const {return FelszallasiHely;}
    std::string GetLe() const {return LeszallasiHely;}
    int GetJaratID() const {return JaratID;}
    int GetVonalID() const {return VonalID;}
};


//Heterogén kollekció miatt örökléssel:

class DiakJegy: public Jegy
{
public:
    DiakJegy(int jid, int vid, const std::string s0 = "", const std::string s1 = ""): Jegy(jid, vid, s0, s1) {}
    virtual int WriteToLine(std::ofstream& file) override;
    virtual Jegy* clone() const override;
    virtual int ar(const Menetrend& m) const override;
    //virtual ~DiakJegy() override;

};

class FelnottJegy: public Jegy
{
public:
    FelnottJegy(int jid, int vid, const std::string s0 = "", const std::string s1 = ""): Jegy(jid, vid, s0, s1) {}
    virtual int WriteToLine(std::ofstream& file) override;
    virtual Jegy* clone() const override;
    virtual int ar(const Menetrend& m) const override;
    //virtual ~FelnottJegy() override;

};



#endif // JEGY_H_INCLUDED
