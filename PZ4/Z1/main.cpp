#include <iostream>

using namespace std;

template<typename tip>
class Red  {
private:
    struct cvor{
        tip element;
        cvor *sljedeci;
        cvor(const tip& element, cvor* sljedeci) : element(element), sljedeci(sljedeci) {};
    };
    cvor *pocetakk, *krajj;
    int broj_elemenata;
public:
    Red() : pocetakk(nullptr), krajj(nullptr), broj_elemenata(0) {}

    Red(const Red &lista) : pocetakk(nullptr), krajj(nullptr), broj_elemenata(0) {
        cvor *p(lista.pocetakk);
        while(p) {
            this->stavi(p->element);
            p = p->sljedeci;
        }
    }

    ~Red() {
        int tmp(broj_elemenata);
        for(int i = 0; i < tmp; i++) this->skini();
    }

    Red<tip> &operator=(const Red &lista) {
        if (&lista==this)
            return *this;
        this->brisi();
        broj_elemenata = 0;
        cvor *p(lista.pocetakk);
        while(p) {
            this->stavi(p->element);
            p = p->sljedeci;
        }
        return *this;
    }


    int brojElemenata() const { return broj_elemenata; }

    tip& celo() {
        if(!broj_elemenata) throw std::range_error("Prazan red");
        return pocetakk->element;
    }

    tip skini() {
        if(!broj_elemenata) throw std::range_error("Prazan red");
                cvor *tmp = pocetakk->sljedeci;
                tip vrati = pocetakk->element;
                delete pocetakk;
                pocetakk = tmp;
                broj_elemenata--;
                return vrati;
        }

    void stavi(const tip& el) {
        if(!broj_elemenata) {
            pocetakk = new cvor(el, nullptr);
            broj_elemenata++;
            krajj = pocetakk;
        } else {
                cvor *tmp = new cvor(el, nullptr);
                krajj->sljedeci = tmp;
                krajj = tmp;
                broj_elemenata++;
            } 
    }

    void brisi() {
        int tmp(broj_elemenata);
        for(int i = 0; i < tmp; i++) this->skini();
    }


};

template<typename tip>
void testbrojElemenata(Red<tip> &red) {
    cout <<red.brojElemenata() << "\n";
    red.stavi(5);
    cout << "Nakon ubacivanja ima " << red.brojElemenata() << "\n";
    red.brisi();
}
template<typename tip>
void testBrisi(Red<tip> &red) {
     for(int i = 5; i < 10; i++) red.stavi(i);
     cout << red.brojElemenata() << "\n";
     red.brisi();
     cout << "Nakon brisanja ima " << red.brojElemenata() << "\n";
}
template<typename tip>
void testStavi(Red<tip> &red){
    for(int i = 1; i < 6; i++) red.stavi(i);
    for(int i = 1; i < 6; i++) cout << red.skini() << " ";
}
template<typename tip>
void testSkini(Red<tip> &red) {
    cout << red.brojElemenata() << "\n";
    red.stavi(1); cout <<red.brojElemenata() << "\n";
    red.skini();
    cout << red.brojElemenata() << "\n";
}
template<typename tip>
void testCelo(Red<tip> &red) {
    red.stavi(156);
    cout << red.celo();
    red.brisi();
}

int main() {
    Red<int> red;
    cout << "\ntest skini\n";
    testSkini(red);
    cout << "\ntest stavi\n";
    testStavi(red);
    cout << "\ntest celo\n";
    testCelo(red);
    cout << "\ntest brisi\n";
    testBrisi(red);
    cout << "\ntest brojElemenata\n";
    testbrojElemenata(red);
}
