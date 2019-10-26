#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
 
 int j = 0;

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
class AVLCvor
{
public:
    AVLCvor()
    {
        lijevo = desno = roditelj = nullptr;
    }
    AVLCvor(const tip1& kljc, const tip2& vrijed/*, BSCvor<tip1, tip2>* l = nullptr, BSCvor<tip1, tip2> *r = nullptr, BSCvor<tip1, tip2> *rodit = nullptr*/)
    {
        kljuc = kljc;
        vrijednost = vrijed;
        /*lijevo = l;
        desno = r;
        roditelj = rodit;*/
    }
    tip1 kljuc;
    tip2 vrijednost;
    int balans = 0;
    AVLCvor<tip1, tip2> *lijevo = 0,*desno = 0, *roditelj = 0;
};

template<typename tip1, typename tip2>
class AVLStabloMapa : public Mapa<tip1, tip2>
{
    AVLCvor<tip1, tip2>* korijen;
    int broj_Elemenata;

    void obrisi2(AVLCvor<tip1, tip2>* r)
    {
        if (r) {
            obrisi2(r->lijevo);
            obrisi2(r->desno);
            delete r;
        }
        korijen = nullptr;

    }
public:
    AVLStabloMapa() : korijen(nullptr), broj_Elemenata(0) {}

    AVLStabloMapa(const AVLStabloMapa& novaMapa) : korijen(nullptr), broj_Elemenata(0)
    {

        AVLCvor<tip1, tip2>* tmp = novaMapa.korijen;
        Preorder(tmp);
    }

    AVLStabloMapa& operator=(const AVLStabloMapa& novaMapa)
    {
        if(this == &novaMapa) return *this;
        this->obrisi();
        korijen = nullptr;
        AVLCvor<tip1, tip2>* tmp = novaMapa.korijen;
        Preorder(tmp);
        return *this;
    }

    void Preorder(AVLCvor<tip1, tip2>* r)
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
        AVLCvor<tip1, tip2>* tmp = new AVLCvor<tip1, tip2>(x, tip2 {});
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
        if(r != nullptr) {
            return r->vrijednost;
        }
        AVLCvor<tip1, tip2>* tmp = new AVLCvor<tip1, tip2>(x, tip2 {});
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
        AVLCvor<tip1, tip2> *p = korijen, *q = 0;
        AVLCvor<tip1, tip2> *tmp = 0,*pp = 0,*rp = 0;
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
            p->desno->roditelj = rp;
        }
        if (q == 0) {
            korijen = rp;
            if(korijen)
                korijen->roditelj = nullptr;
         } else if (p == q->lijevo) {
            q->lijevo = rp;
            if(rp)
            rp->roditelj = q;
        } else {
            q->desno = rp;
            if(rp)
            rp->roditelj = q;
        }
        if(p->roditelj && p->roditelj->lijevo == p) p->roditelj->balans--;
        else if(p->roditelj) p->roditelj->balans++;
        delete p;
        broj_Elemenata--;
    }

    ~AVLStabloMapa()
    {
        obrisi2(korijen);
    }

    void Umetni(AVLCvor<tip1, tip2>* &z)
    {
        AVLCvor<tip1, tip2>* y = 0, *x = korijen;
        while (x != 0) {
            y = x;
            if (z->kljuc < x->kljuc)
                x = x->lijevo;
            else
                x = x->desno;
        }
        bool imaSibling(false);
        if (y == 0) {
            korijen = z;
            broj_Elemenata++;
            imaSibling = true;
        } else {
            if (z->kljuc < y->kljuc) {
                y->lijevo = z;
                z->roditelj = y;
                broj_Elemenata++;
                if(y->desno) {
                    imaSibling = true;
                    y->balans = 0;
                }
            } else {
                y->desno = z;
                z->roditelj = y;
                broj_Elemenata++;
                if(y->lijevo) {
                    imaSibling = true ;
                    y->balans = 0;
                }
            }
        }
        if(!imaSibling)
            azurirajBalans(z);
    }
    
    void azurirajBalans(AVLCvor<tip1, tip2>* &cvor) {
        if(cvor->roditelj == nullptr) return;
        if(!(cvor->lijevo && cvor->desno && cvor->balans == 0)) {
            if(cvor->roditelj->lijevo == cvor) 
               cvor->roditelj->balans++;
            else 
                cvor->roditelj->balans--;
        }
        bool nastaviti(true);
        AVLCvor<tip1, tip2>* r = cvor->roditelj;
        AVLCvor<tip1, tip2>* r1 = cvor;
        if(cvor->balans > 0 && cvor->roditelj->balans < -1) {
            jednostrukaRotacijaUdesno(r1);
            jednostrukaRotacijaUlijevo(r);
            nastaviti = false;
        } 
        else if(cvor->balans < 0 && cvor->roditelj->balans > 1) {
            jednostrukaRotacijaUlijevo(r1);
            jednostrukaRotacijaUdesno(r);
            nastaviti = false;
        }
        else if(cvor->roditelj->balans < -1) {
            jednostrukaRotacijaUlijevo(r);
            nastaviti = false;
        }
        else if(cvor->roditelj->balans > 1) {
            jednostrukaRotacijaUdesno(r);
            nastaviti = false;
        }
        if(nastaviti) 
            azurirajBalans(cvor->roditelj);
        
    }
    
    void jednostrukaRotacijaUlijevo(AVLCvor<tip1, tip2>* &cvor) {
       if(cvor->roditelj && cvor->roditelj->lijevo == cvor) 
            cvor->roditelj->lijevo = cvor->desno;
        else if(cvor->roditelj) 
            cvor->roditelj->desno = cvor->desno;
        AVLCvor<tip1, tip2>* tmp = cvor->desno;
        bool imaDesno(false);
        if(cvor->desno->lijevo) { 
           cvor->desno = tmp->lijevo;
           tmp->lijevo->roditelj = cvor;
           tmp->lijevo = nullptr;
           imaDesno = true;
        }
        tmp->roditelj = cvor->roditelj;
        if(cvor->roditelj)
            cvor->roditelj = tmp;
        else {
            cvor->roditelj = tmp;
            korijen = tmp;
        }
        tmp->lijevo = cvor;
        cvor->balans = 0;
        tmp->balans = 0;
        if(!imaDesno)
            cvor->desno = nullptr;
    }

    void jednostrukaRotacijaUdesno(AVLCvor<tip1, tip2>* &cvor) {
        if(cvor->roditelj && cvor->roditelj->lijevo == cvor) 
            cvor->roditelj->lijevo = cvor->lijevo;
        else if(cvor->roditelj) 
            cvor->roditelj->desno = cvor->lijevo;
        AVLCvor<tip1, tip2>* tmp = cvor->lijevo;
        bool imaLijevo(false);
        if(cvor->lijevo->desno) { 
            cvor->lijevo = tmp->desno;
            tmp->desno->roditelj = cvor;
            tmp->desno = nullptr;
            imaLijevo = true;
        }
        tmp->roditelj = cvor->roditelj;
        if(cvor->roditelj)
            cvor->roditelj = tmp;
        else {
            cvor->roditelj = tmp;
            korijen = tmp;
        }
        tmp->desno = cvor;
        tmp->balans = 0;
        cvor->balans = 0;
        if(!imaLijevo)
            cvor->lijevo = nullptr;
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



int dodajElementeUAVLMapu(AVLStabloMapa<int, int> &mapa)
{
    int zavratit(0);
    int n = 15000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n - 5) zavratit = v;
    }
    return zavratit;
}

int dodajElementeUBinarnuMapu(BinStabloMapa<int, int> &mapa)
{
    int zavratit(0);
    int n = 15000;
    for(int i = 0; i < n; i++) {
        int v = rand() % 100000;
        mapa[v] = 5;
        if(i == n - 5) zavratit = v;
    }
    return zavratit;
}

int main() {
//Malo jaci test performansi AVLStabloMape gdje se vidi 
  //da li je zaista O(log n)
AVLStabloMapa <int, int> m;
int vel(2000000);
int progress = 1000;
for (int i(0); i<vel; i++) {
    m[rand()%(2*vel)-vel] = i;
    if (i==progress) { 
       cout<<"Dodano "<<i<<endl;
       progress *= 2;
    }
}
for (int i(-vel); i<vel; i++) m[i] = i;
for (int i(-vel); i<vel; i++) { if (m[i] != i) { cout << i << " NOT "; break; } m.obrisi(i);
}
cout << "OK";
    // BinStabloMapa<int, int> Bmapa;
    // AVLStabloMapa<int, int> AVLmapa;
    // int prvi, drugi, ukvrijeme;
    // clock_t vrijeme1, vrijeme2;
    // prvi = dodajElementeUBinarnuMapu(Bmapa);
    // drugi = dodajElementeUAVLMapu(AVLmapa);
    // vrijeme1 = clock();
    // Bmapa[100001] = 1;
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme dodavanja novog elementa u binarnu mapu: " << ukvrijeme << " ms(/100)\n";
    // vrijeme1 = clock();
    // AVLmapa[100001] = 1;
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme dodavanja novog elementa u AVL mapu: " << ukvrijeme << " ms(/100)\n";

    // vrijeme1 = clock();
    // AVLmapa[drugi] = 3333;
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme izvrsavanja trazenja elemetna u AVL mapi: " << ukvrijeme << " ms(/100)\n";
    // vrijeme1 = clock();
    // Bmapa[prvi] = 3333;
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme izvrsavanja trazenja elemetna u binarnoj mapi: " << ukvrijeme << " ms(/100)\n";
    
    // vrijeme1 = clock();
    // AVLmapa.obrisi(drugi);
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme izvrsavanja brisanja elemetna u AVL mapi: " << ukvrijeme << " ms(/100)\n";
    // vrijeme1 = clock();
    // Bmapa.obrisi(prvi);
    // vrijeme2 = clock();
    // ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 100000);
    // cout << "Vrijeme izvrsavanja brisanja elemetna u binarnoj mapi: " << ukvrijeme << " ms(/100)\n";

    return 0;

}
