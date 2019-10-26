#include<iostream>
#include<vector>
using namespace std;

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

void pretraga(Stek<std::vector<int>>& s, int trazeni) {
    int brojac = s.brojElemenata() - 1;
    std::vector<int> tmp;
    if(!s.brojElemenata()) {
        std::cout << "Nema elementa\n";
        return;
    }
    else {
        tmp = s.vrh();
        if(!(tmp.size() && tmp[tmp.size() - 1] < trazeni)) {
            int dno = 0;
            int vrh = tmp.size() - 1;
            while(vrh >= dno) {
                int srednji = (dno + vrh) / 2;
                if(tmp[srednji] == trazeni) {
                    std::cout << srednji << " " << brojac;
                    return;
                }
                else if(tmp[srednji] > trazeni) vrh = srednji - 1;
                else dno = srednji + 1;
            }
        }
        s.skini();
        pretraga(s, trazeni);
        s.stavi(tmp);
    }
}

void testPretraga1() {
    Stek<std::vector<int>> s;
    s.stavi({1,2,3,4,5});
    s.stavi({10,11,13,14,15});
    s.stavi({21,22,23,24,25});
    
    std::cout << "\nOcekivan izlaz je 0 1 a vas je "; pretraga(s,10);
}

void testPretraga2() {
    Stek<std::vector<int>> s;
    s.stavi({1,2,3,4,5});
    s.stavi({10,11,13,14,15});
    s.stavi({21,22,23,24,25});
    
    std::cout << "\nOcekivan izlaz je Nema elementa a vas je ";pretraga(s,17);
}
void testPretraga3() {
    Stek<std::vector<int>> s;
    s.stavi({1,2,3,4,5});
    s.stavi({10,11,13,14,15});
    s.stavi({21,22,23,24,25});
    
    std::cout << "\nOcekivan izlaz je 0 0 a vas je "; pretraga(s,1);
}

int main() {
    testPretraga1();
    testPretraga2();
    testPretraga3();
    return 0;
}