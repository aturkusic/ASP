#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdlib>
#include <tuple>

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

template<typename tip1, typename tip2>
class HashMapa : public Mapa<tip1, tip2>
{
    //true obrisan, false uredu
    std::tuple<tip1, tip2, bool> **elementi;
    int max_velicina, broj_Elemenata;
    unsigned int (*pokNaFun)(tip1, unsigned int);
public:
    HashMapa() : elementi(new std::tuple<tip1, tip2, bool>*[1000] {}), max_velicina(1000), broj_Elemenata(0), pokNaFun(nullptr) {}

    HashMapa(const HashMapa<tip1, tip2> &novaMapa) : elementi(new std::tuple<tip1, tip2, bool>*[novaMapa.max_velicina] {}), max_velicina(novaMapa.max_velicina), broj_Elemenata(novaMapa.broj_Elemenata), pokNaFun(novaMapa.pokNaFun)
    {
        for(int i = 0; i < max_velicina; i++) {
            if(novaMapa.elementi[i] != nullptr) {
                elementi[i] = new std::tuple<tip1, tip2, bool>(*novaMapa.elementi[i]);
            }
        }
    }

    HashMapa& operator=(const HashMapa<tip1, tip2> &novaMapa)
    {
        if(this != &novaMapa) {
            this->obrisi();
            delete[] elementi;
            elementi = new std::tuple<tip1, tip2, bool>*[novaMapa.max_velicina] {};
            max_velicina = novaMapa.max_velicina;
            broj_Elemenata = novaMapa.broj_Elemenata;
            pokNaFun = novaMapa.pokNaFun;
            for(int i = 0; i < max_velicina; i++) {
                if(novaMapa.elementi[i] != nullptr) {
                    elementi[i] = new std::tuple<tip1, tip2, bool>(*novaMapa.elementi[i]);
                }
            }
        }
        return *this;
    }

    tip2 operator[] (tip1 vrijednost) const
    {
        if(!pokNaFun) throw "Nema hash funkcije, postavite hash funkciju";
        unsigned int indeks = pokNaFun(vrijednost, max_velicina);
        int i(0);
        do {
            if (elementi[indeks] != nullptr && std::get<0>(*elementi[indeks]) == vrijednost && !std::get<2>(*elementi[indeks]))
                return std::get<1>(*elementi[indeks]);
            else {
                i++;
                indeks = (indeks + 1) % max_velicina;
            }
        } while (!(elementi[indeks] == nullptr) && i != max_velicina);
        return tip2{};
    }

    tip2& operator[] (tip1 vrijednost)
    {
        if(!pokNaFun) throw "Nema hash funkcije, postavite hash funkciju";
        unsigned int indeks = pokNaFun(vrijednost, max_velicina);
        int i(0);
        do {
            if (elementi[indeks] != nullptr && std::get<0>(*elementi[indeks]) == vrijednost && !std::get<2>(*elementi[indeks]))
                return std::get<1>(*elementi[indeks]);
            else {
                i++;
                indeks = (indeks + 1) % max_velicina;
            }
        } while (!(elementi[indeks] == nullptr) && i != max_velicina);
        if(broj_Elemenata < max_velicina) {
            int j  = pokNaFun(vrijednost, max_velicina);
            i = 0;
            return dodajElement(vrijednost, j, i);
        } else {
            std::tuple<tip1, tip2, bool> **novi = new std::tuple<tip1, tip2, bool>*[max_velicina * 2] {};
            for(int i = 0; i < max_velicina; i++) {
                if(elementi[i] != nullptr) {
                    int j  = pokNaFun(std::get<0>(*elementi[i]), max_velicina * 2);
                    int k = 0;
                    do {
                        if (!novi[j]) {
                            novi[j] = new std::tuple<tip1, tip2, bool>(std::get<0>(*elementi[i]), tip2{}, false);
                            break;
                        } else {
                            k++;
                            j = (j + 1) % (max_velicina * 2);
                        }
                    } while (k < max_velicina * 2);
                    delete elementi[i];
                    elementi[i] = nullptr;
                }

            }
            delete[] elementi;
            max_velicina *= 2;
            elementi = novi;
            int indexZaVratit = pokNaFun(vrijednost, max_velicina);
            i = 0;
            return dodajElement(vrijednost, indexZaVratit, i);
        }
    }

    tip2& dodajElement(tip1 vrijednost, int j, int i)
    {
        do {
            if (!elementi[j] || std::get<2>(*elementi[j])) {
                delete elementi[j];
                elementi[j] = new std::tuple<tip1, tip2, bool>(vrijednost, tip2{}, false);
                broj_Elemenata++;
                return std::get<1>(*elementi[j]);
            } else {
                i++;
                j = (j + 1) % max_velicina;
            }
        } while (i < max_velicina);
    }

    int brojElemenata() const
    {
        return broj_Elemenata;
    }

    void obrisi()
    {
        for(int i = 0; i < max_velicina; i++) {
            delete elementi[i];
            elementi[i] = nullptr;
        }
        broj_Elemenata = 0;
    }

    void obrisi(const tip1& kljuc)
    {
        if(!pokNaFun) throw "Nema hash funkcije, postavite hash funkciju";
        unsigned int indeks = pokNaFun(kljuc, max_velicina);
        int i(0);
        do {
            if (elementi[indeks] != nullptr && std::get<0>(*elementi[indeks]) == kljuc && !std::get<2>(*elementi[indeks])) {
                std::get<2>(*elementi[indeks]) = true;
                broj_Elemenata--;
                return;
            } else {
                i++;
                indeks = (indeks + 1) % max_velicina;
            }
        } while (!(elementi[indeks] == nullptr) && i != max_velicina);
        throw "Nije pronadjen element";
    }

    void definisiHashFunkciju(unsigned int (*pokNaFun1)(tip1, unsigned int))
    {
        pokNaFun = pokNaFun1;
    }

    ~HashMapa()
    {
        for(int i = 0; i < max_velicina; i++) {
            delete elementi[i];
            elementi[i] = nullptr;
        }
        delete[] elementi;
    }

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

    void Preorder(BSCvor<tip1, tip2>* r)
    {
        if (r != 0) {
            this->dodaj(r->kljuc) = r->vrijednost;
            Preorder(r->lijevo);
            Preorder(r->desno);
        }
    }



    tip2 operator[] (tip1 x) const
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
        tip2 tmp{};
        return tmp;
    }

    tip2& dodaj(tip1 x)
    {
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

    int brojElemenata() const
    {
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
class NizMapa : public Mapa<tip1,tip2>
{
    pair<tip1,tip2>** parovi;
    int max_velicina = 1000;
    int broj_elemenata;
public:
    NizMapa() : parovi(new pair<tip1,tip2>*[1000] {}), broj_elemenata(0) {}

    NizMapa(const NizMapa &nova_mapa) : parovi(new pair<tip1,tip2>*[nova_mapa.max_velicina] {}), max_velicina(nova_mapa.max_velicina), broj_elemenata(nova_mapa.broj_elemenata)
    {
        for(int i = 0; i < broj_elemenata; i++)
            parovi[i] = new pair<tip1, tip2>(*nova_mapa.parovi[i]);
    }

    NizMapa& operator=(const NizMapa &nova_mapa)
    {
        if(&nova_mapa != this) {
            this->obrisi();
            delete[] parovi;
            parovi = new pair<tip1,tip2>*[nova_mapa.max_velicina] {};
            max_velicina = nova_mapa.max_velicina;
            broj_elemenata = nova_mapa.broj_elemenata;
            for(int i = 0; i < broj_elemenata; i++) parovi[i] = new pair<tip1, tip2>(*nova_mapa.parovi[i]);
        }
        return *this;
    }

    ~NizMapa()
    {
        for(int i = 0; i < broj_elemenata; i++) {
            delete parovi[i];
        }
        delete[] parovi;
    }

    tip2& operator[] (tip1 vrijednost) override
    {
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

    tip2 operator[] (tip1 vrijednost) const override
    {
        for(int i = 0; i < broj_elemenata; i++) {
            if(parovi[i]->first == vrijednost) {
                return parovi[i]->second;
            }
        }
        tip2 tmp{};
        return tmp;
    }

    int brojElemenata() const override
    {
        return broj_elemenata;
    }

    void obrisi() override
    {
        for(int i = 0; i < broj_elemenata; i++) {
            delete parovi[i];
            parovi[i] = nullptr;
        }
        broj_elemenata = 0;
    }

    void obrisi(const tip1& kljuc) override
    {
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

unsigned int hashfunckija(int x, unsigned int max)
{
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = ((x >> 16) ^ x) * 0x45d9f3b;
    x = (x >> 16) ^ x;
    return x % max;
}

int dodajElementeUHashMapu(HashMapa<int, int> &mapa)
{
    int zavratit(0);
    int n = 14000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n/2) zavratit = v;
    }
    return zavratit;
}

int dodajElementeUBinarnuMapu(BinStabloMapa<int, int> &mapa)
{
    int zavratit(0);
    int n = 14000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n/2) zavratit = v;
    }
    return zavratit;
}

int dodajElementeUNizMapu(NizMapa<int, int> &mapa)
{
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
    HashMapa<int,int> Hmapa;
    Hmapa.definisiHashFunkciju(hashfunckija);
    int prvi, drugi, treci, ukvrijeme;
    clock_t vrijeme1, vrijeme2;
    prvi = dodajElementeUBinarnuMapu(Bmapa);
    drugi = dodajElementeUNizMapu(Nmapa);
    treci = dodajElementeUHashMapu(Hmapa);
    vrijeme1 = clock();
    Bmapa[100001] = 1;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme dodavanja novog elementa u binarnu mapu: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Hmapa[100001] = 1;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme dodavanja novog elementa u hash mapu: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Nmapa[100001] = 1;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme dodavanja novog elementa u niz mapu: " << ukvrijeme << " ms(/100)\n";

    vrijeme1 = clock();
    Hmapa[treci] = 3333;
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja trazenja elemetna u hash mapi: " << ukvrijeme << " ms(/100)\n";
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
    
    vrijeme1 = clock();
    Bmapa.obrisi(prvi);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja brisanja elemetna u binarnoj mapi: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Hmapa.obrisi(treci);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja brisanja elemetna u hash mapi: " << ukvrijeme << " ms(/100)\n";
    vrijeme1 = clock();
    Nmapa.obrisi(drugi);
    vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    cout << "Vrijeme izvrsavanja brisanja elementa u niz mapi: " << ukvrijeme << " ms(/100)\n";
    return 0;
}

// hash mapa bi trebala biti brza i pri brisanju i pri dodavanju i pri trazenju ali zbog vjerovatno velikog broja kolizija i lose hash funkcije ovdje se cini da nije
// da bi ovo popravio trebalo bi napraviti bolju hash funkciju kao i mjenjanje velicine hash mape cim se ona napuni 1% ali ja to nisam sada radio(a nije tesko ni uraditi)
// druge dvije sam komentarisao u prethodnoj zadaci