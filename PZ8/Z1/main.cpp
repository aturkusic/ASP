#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

template<typename tip1, typename tip2>
class Mapa
{
    public:
    Mapa() {}
    virtual tip2 operator[] (tip1 vrijednost) const = 0;
    virtual tip2& operator[] (tip1 vrijednost)  = 0;
    virtual int brojElemenata() const = 0;
    virtual void obrisi() = 0;
    virtual void obrisi(const tip1& kljuc) = 0;
    virtual ~Mapa() {};
};


template <typename tip1, typename tip2>
class BSCvor
{
    public:
    BSCvor()
    {
        lijevo = desno = roditelj = nullptr;
    }
    BSCvor(const tip1& kljc, const tip2& vrijed, BSCvor* l = nullptr, BSCvor *r = nullptr, BSCvor *rodit = nullptr)
    {
        kljuc = kljc;
        vrijednost = vrijed;
        lijevo = l;
        desno = r;
        roditelj = rodit;
    }
    tip1 kljuc;
    tip2 vrijednost;
    BSCvor<tip1, tip2> *lijevo,*desno, *roditelj;
};

template<typename tip1, typename tip2>
class BinStabloMapa : public Mapa<tip1, tip2>
{
    BSCvor<tip1, tip2>* korijen;
    int broj_Elemenata;
    
    void obrisi2(BSCvor<tip1, tip2>* r)
    {
        if (r) {
            obrisi2(r->lijevo);
            obrisi2(r->desno);
            delete r;
        }
        korijen = nullptr;

    }
    public:
    BinStabloMapa() : korijen(nullptr), broj_Elemenata(0) {}

    BinStabloMapa(const BinStabloMapa& novaMapa) : korijen(nullptr), broj_Elemenata(0)
    {
        
        BSCvor<tip1, tip2>* tmp = novaMapa.korijen;
        Preorder(tmp);
    }

    BinStabloMapa& operator=(const BinStabloMapa& novaMapa)
    {
        if(this == &novaMapa) return *this;
        this->obrisi();
        korijen = nullptr;
        BSCvor<tip1, tip2>* tmp = novaMapa.korijen;
        Preorder(tmp);
        return *this;
    }
    
    void Preorder(BSCvor<tip1, tip2>* r) {
        if (r != 0)
        {
            this->dodaj(r->kljuc) = r->vrijednost;
            Preorder(r->lijevo);
            Preorder(r->desno);
        }
    }



    tip2 operator[] (tip1 x) const {
        auto r = korijen;
        while (r != 0 && x != r->kljuc)
        {
            if (x < r->kljuc)
                r = r->lijevo;
            else
                r = r->desno;
        }
        if(r != nullptr)
            return r->vrijednost;
        tip2 tmp{};
        return tmp;
    }

    tip2& dodaj(tip1 x) {
        BSCvor<tip1, tip2>* tmp = new BSCvor<tip1, tip2>(x, tip2 {});
        this->Umetni(tmp);
        return tmp->vrijednost;
    }


    tip2& operator[] (tip1 x)
    {
        auto r = korijen;
        while (r != 0 && x != r->kljuc) {
            if (x < r->kljuc)
                r = r->lijevo;
            else
                r = r->desno;
        }
        if(r != nullptr)
            return r->vrijednost;
        BSCvor<tip1, tip2>* tmp = new BSCvor<tip1, tip2>(x, tip2 {});
        this->Umetni(tmp);
        return tmp->vrijednost;
    }

    int brojElemenata() const {
        return broj_Elemenata;
    }

    void obrisi()
    {
        obrisi2(korijen);
        broj_Elemenata = 0;
    }

    void obrisi(const tip1& x)
    {
        BSCvor<tip1, tip2> *p = korijen, *q = 0;
        BSCvor<tip1, tip2> *tmp = 0,*pp = 0,*rp = 0;
        while (p !=0 && x != p->kljuc) {
            q = p;
            if (x < p->kljuc)
                p = p->lijevo;
            else
                p = p->desno;
        }
        if (p == 0)
            throw std::logic_error("Nije pronadjen");
        if (p->lijevo == 0)
            rp = p->desno;
        else if (p->desno == 0)
            rp = p->lijevo;
        else {
            pp = p;
            rp = p->lijevo;
            tmp = rp->desno;
            while (tmp != 0) {
                pp = rp;
                rp = tmp;
                tmp = rp->desno;
            }
            if (pp != p) {
                pp->desno = rp->lijevo;
                rp->lijevo = p->lijevo;
                pp->roditelj = rp;
                rp->roditelj = q;
                if(pp->desno)
                    pp->desno->roditelj = pp;
            }
            rp->desno = p->desno;
        }
        if (q == 0)
            korijen = rp;
        else if (p == q->lijevo)
            q->lijevo = rp;
        else
            q->desno = rp;
        delete p;
        broj_Elemenata--;
    }

    ~BinStabloMapa()
    {
        obrisi2(korijen);
    }

    void Umetni(BSCvor<tip1, tip2>* &z)
    {
        BSCvor<tip1, tip2>* y = 0, *x = korijen;
        while (x != 0) {
            y = x;
            if (z->kljuc < x->kljuc)
                x = x->lijevo;
            else
                x = x->desno;
        }
        if (y == 0) {
            korijen = z;
            broj_Elemenata++;
        } else {
            if (z->kljuc < y->kljuc) {
                y->lijevo = z;
                z->roditelj = y;
                broj_Elemenata++;
            } else {
                y->desno = z;
                z->roditelj = y;
                broj_Elemenata++;
            }
        }
    }

};

template<typename tip1, typename tip2>
class NizMapa : public Mapa<tip1,tip2> {
    pair<tip1,tip2>** parovi;
    int max_velicina = 1000;
    int broj_elemenata;
public:
    NizMapa() : parovi(new pair<tip1,tip2>*[1000]{}), broj_elemenata(0) {}

    NizMapa(const NizMapa &nova_mapa) : parovi(new pair<tip1,tip2>*[nova_mapa.max_velicina]{}), max_velicina(nova_mapa.max_velicina), broj_elemenata(nova_mapa.broj_elemenata) {
        for(int i = 0; i < broj_elemenata; i++) 
            parovi[i] = new pair<tip1, tip2>(*nova_mapa.parovi[i]);
    }

    NizMapa& operator=(const NizMapa &nova_mapa) {
        if(&nova_mapa != this) {
            this->obrisi();
            delete[] parovi;
            parovi = new pair<tip1,tip2>*[nova_mapa.max_velicina]{};
            max_velicina = nova_mapa.max_velicina; 
            broj_elemenata = nova_mapa.broj_elemenata;
            for(int i = 0; i < broj_elemenata; i++) parovi[i] = new pair<tip1, tip2>(*nova_mapa.parovi[i]);
        }
        return *this;
    }

    ~NizMapa(){
        for(int i = 0; i < broj_elemenata; i++) {
            delete parovi[i];
        }
        delete[] parovi;
    }

    tip2& operator[] (tip1 vrijednost) override {
        for(int i = 0; i < broj_elemenata; i++) {
            if(parovi[i]->first == vrijednost) {
                return parovi[i]->second;
            }
        }
        // ako nije pronaslo element
        if(broj_elemenata < max_velicina) {
            parovi[broj_elemenata] = new pair<tip1, tip2>();
            parovi[broj_elemenata]->first = vrijednost;
            return parovi[broj_elemenata++]->second;
        } else { //ako predje limit maxelemenata elemenata
            max_velicina *= 2;
            pair<tip1,tip2>** tmp = new pair<tip1,tip2>*[max_velicina]();
            copy(parovi, parovi + broj_elemenata, tmp);
            this->obrisi();
            delete[] parovi;
            parovi = tmp;
            parovi[broj_elemenata] = new pair<tip1, tip2>();
            parovi[broj_elemenata]->first = vrijednost;
            return parovi[broj_elemenata++]->second;
        }

    }

    tip2 operator[] (tip1 vrijednost) const override {
        for(int i = 0; i < broj_elemenata; i++) {
            if(parovi[i]->first == vrijednost) {
                return parovi[i]->second;
            }
        }
        tip2 tmp{};
        return tmp;
    }

    int brojElemenata() const override { return broj_elemenata; }

    void obrisi() override {
        for(int i = 0; i < broj_elemenata; i++) {
            delete parovi[i];
            parovi[i] = nullptr;
        }
        broj_elemenata = 0;
    }

    void obrisi(const tip1& kljuc) override {
        bool pronadjen(false);
        for(int i = 0; i < broj_elemenata; i++) {
            if(parovi[i]->first == kljuc) {
                pronadjen = true;
                delete parovi[i];
                parovi[i] = nullptr;
                broj_elemenata--;
                break;
            }
        }
        if(!pronadjen) throw std::logic_error("Nije pronadjen");
        remove(parovi, parovi + broj_elemenata + 1, nullptr);
    }

};

int dodajElementeUBinarnuMapu(BinStabloMapa<int, int> &mapa) {
    int zavratit(0);
    int n = 14000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n/2) zavratit = v; 
    }
    return zavratit;
}

int dodajElementeUNizMapu(NizMapa<int, int> &mapa) {
    int zavratit(0);
    int n = 14000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n/2) zavratit = v; 
    }
    return zavratit;
}


int main()
{
    BinStabloMapa<int, int> Bmapa;
    NizMapa<int, int> Nmapa;
    int prvi, drugi, ukvrijeme;
    clock_t vrijeme1, vrijeme2;
    prvi = dodajElementeUBinarnuMapu(Bmapa);
    drugi = dodajElementeUNizMapu(Nmapa);
    vrijeme1 = clock();
    Bmapa[100001] = 1;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme dodavanja novog elementa u mapu: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Nmapa[100001] = 1;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme dodavanja novog elementa u mapu: " << ukvrijeme << " ms(/100)\n";
    
    vrijeme1 = clock();
    Bmapa[prvi] = 3333;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja trazenja elemetna u binarnoj mapi: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Nmapa[drugi] = 3333;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja trazenja elementa u niz mapi: " << ukvrijeme << " ms(/100)\n";
    
    /*nakon obavljenih nekoliko testiranja utvrdio sam da je ubacivanje novog elementa brze u niz dok je broj elemenata manji u mapama ali sa porastom broja elemenata
    ubacivanje u niz ce za neki broj postati sporije od ubcvanja u binarno stablo, pretraga elemenata je uvjek brza za binarno stablo u odnosu na nizmapu*/
    
    return 0;
}
