#include <iostream>

template<typename tip>
class Stek {
    struct Cvor {
        tip info;
        Cvor *sljedeci;
        Cvor (const tip &info, Cvor *sljedeci):
                info(info), sljedeci(sljedeci) {}
    };
    Cvor *vrhh;
    int broj_elemenata;
public:

    Stek() : vrhh(nullptr), broj_elemenata(0) {}

    Stek(const Stek &s) : broj_elemenata(s.broj_elemenata) {
        Cvor *q, *p(s.vrhh);
         vrhh = nullptr;
         while(p != nullptr) {
             Cvor *n(new Cvor(p->info,0));
             if (vrhh == nullptr)
             vrhh = n;
             else
             q->sljedeci = n;
             p = p->sljedeci;
             q = n;
         }
    }
    
    ~Stek() {
        this->brisi();
    }
    
    Stek& operator=(const Stek &s) {
        if(this == &s)
            return *this;
        if(broj_elemenata) this->brisi();
        Cvor *q, *p(s.vrhh);
         vrhh = nullptr;
         broj_elemenata = s.broj_elemenata;
         while(p != nullptr) {
             Cvor *n(new Cvor(p->info,0));
             if (vrhh == nullptr)
             vrhh = n;
             else
             q->sljedeci = n;
             p = p->sljedeci;
             q = n;
         }
    
        return *this;
    }

    void brisi() {
        if(broj_elemenata) {
            int tmp(broj_elemenata);
            for(int i = 0; i < tmp; i++) {
                Cvor *pok = vrhh->sljedeci;
                delete vrhh;
                vrhh = pok;
                broj_elemenata--;
            }
        }
    }

    void stavi(const tip& el) {
        Cvor *tmp = new Cvor(el,vrhh);
        vrhh = tmp;
        broj_elemenata++;
    }

    tip skini() {
        if(!broj_elemenata) throw std::range_error("Prazan stek");
        tip tmp = vrhh->info;
        Cvor *pok = vrhh->sljedeci;
        delete vrhh;
        broj_elemenata--;
        vrhh = pok;
        return tmp;
    }

    tip& vrh() {
        if(!broj_elemenata) throw std::range_error("Prazan stek");
        return vrhh->info;
    }

    int brojElemenata() { return broj_elemenata; }

};

void TestStavi() {
    Stek<int> s;
    for(int i = 0; i < 10; i++) {
        s.stavi(i);
    }
     for(int i = 0; i < 10; i++) {
        std::cout << s.skini() << " ";
    }
}

void TestBrojElemenata() {
    Stek<int> s;
    for(int i = 0; i < 10; i++) {
        s.stavi(i);
    }
    std::cout << "Ocekivan izlaz funkcije je 10, vas je " << s.brojElemenata() << "\n";
}

void TestSkini() {
    Stek<int> s;
    for(int i = 0; i < 10; i++) {
        s.stavi(i);
    }
    s.skini();
    std::cout << "Ocekivan izlaz funkcije je 8, vas je " << s.skini() << "\n";
}

void TestVrh() {
    Stek<int> s;
    s.stavi(1); s.stavi(15);
    std::cout << "Ocekivan izlaz je 15, 15 vas je " << s.vrh() <<  ", " << s.vrh();
}

void TestBrisi() {
    Stek<int> s;
    for(int i = 0; i < 10; i++) {
        s.stavi(i);
    }
    std::cout << "Prije brisanja ima " << s.brojElemenata() << " elemenata a nakon brisanja ima ";
    s.brisi();
    std::cout << s.brojElemenata();
}

int main() {
TestBrisi();
TestSkini();
TestBrojElemenata();
TestStavi();
TestVrh();
    

return 0;
}