#include <iostream>
#include <stdexcept>
#include <algorithm> 

using namespace std;

template<typename tip>
class Lista {
  public:
    Lista () {};
    virtual ~Lista() {} ;
    virtual int brojElemenata() const = 0;
    virtual tip trenutni() const = 0;
    virtual tip& trenutni() = 0;
    virtual bool prethodni() = 0;
    virtual bool sljedeci() = 0;
    virtual void pocetak() = 0;
    virtual void kraj() = 0;
    virtual void obrisi() = 0;
    virtual void dodajIspred(const tip& el) = 0;
    virtual void dodajIza(const tip& el) = 0;
    virtual tip &operator[](int) = 0;
    virtual tip operator[](int) const = 0;
};

template<typename tip>
class NizLista : public Lista<tip>  {
    tip *niz;
    int kapacitet, velicina , trnt;
   
  public:
    NizLista() : niz(new tip[1000]), kapacitet(1000), velicina(0), trnt(0) {}
    
    NizLista(const NizLista<tip> &v) : niz(new tip[v.kapacitet]), kapacitet(v.kapacitet), velicina(v.velicina), trnt(v.trnt)  {
        std::copy(v.niz, v.niz + v.velicina, niz);
    }
    
    NizLista &operator =(const NizLista<tip> &v) {
     tip *novi_prostor(new tip[v.kapacitet]);
     std::copy(v.niz, v.niz + v.velicina, novi_prostor);
     delete[] niz;
     kapacitet = v.kapacitet; trnt = v.trnt; velicina = v.velicina; niz = novi_prostor;
     return *this;
    }
    
    int brojElemenata() const { return velicina; }
    
    tip& trenutni()  { return niz[trnt]; }
    
    tip trenutni() const { return niz[trnt]; }
    
    bool sljedeci() {
        if(!velicina) throw std::range_error("Prazan");
        if(trnt == velicina-1) return false; 
        trnt++; 
        return true; 
        
    }
    
    bool prethodni() {
        if(!velicina) throw std::range_error("Prazan");
        if(!trnt) return false; 
        trnt--; 
        return true; 
        
    }
    
    void pocetak() { if(!velicina) throw std::range_error("Prazan"); trnt = 0; }
    
    void kraj() { if(!velicina) throw std::range_error("Prazan");  trnt = velicina - 1; }
    
    void obrisi() {
        if(!velicina) throw std::range_error("Prazan");
        if(trnt == velicina - 1) {
            velicina --; trnt--;
        } else {
          for(int i = trnt; i < velicina - 1; i++) {
            niz[i] = niz[i+1];
        }
        velicina--;
        }
    }
    
    void dodajIspred(const tip& el) {
        if(velicina == kapacitet) {
            tip *niz1 = new tip[kapacitet * 2];
            kapacitet *= 2;
            std::copy(niz, niz + velicina, niz1);
            delete[] niz;
            niz = niz1;
        }
        if(!velicina) { 
            niz[0] = el;
            trnt = 0;
            velicina++;

        } else {
            for(int i = velicina - 1; i >= trnt; i--) {
                niz[i+1] = niz[i];
            }
            niz[trnt] = el;
            trnt++;
            velicina++;
        }
    }
    
    void dodajIza(const tip& el) {
        if(velicina == kapacitet) {
            tip *niz1 = new tip[kapacitet * 2];
            kapacitet *= 2;
            std::copy(niz, niz + velicina, niz1);
            delete[] niz;
            niz = niz1;
        }
        if(!velicina) { 
            niz[0] = el;
            trnt = 0;
            velicina++;
        }
        else if(trnt == velicina - 1) {
            niz[trnt + 1] = el;
            velicina++;
         } else {
             for(int i = velicina - 1; i > trnt; i--) {
                niz[i+1] = niz[i];
            }
        niz[trnt + 1] = el;
        velicina++;
         }
    }
    
    tip &operator[](int i) {
        if(i < 0 || i > velicina - 1) throw std::range_error("Greska");
        return niz[i];
    }
   
    tip operator[](int i) const {
        if(i < 0 || i > velicina - 1) throw std::range_error("Greska");
        return niz[i];
    }
    
    ~NizLista() {
        delete[] niz;
    }
    
};

template<typename tip>
class JednostrukaLista : public Lista<tip> {
private:
    struct cvor{
        tip element;
        cvor *sljedeci;
        cvor(const tip& element, cvor* sljedeci) : element(element), sljedeci(sljedeci) {};
    };
    cvor *pocetakk, *krajj;
    cvor *prije_trenutnog;
    cvor *trenutnii;
    int broj_elemenata;
    void PomjeriPrijeTrenutnog() {
        if(trenutnii == pocetakk || broj_elemenata < 2) prije_trenutnog = nullptr;
        else {
             prije_trenutnog = pocetakk;
            while(prije_trenutnog->sljedeci != trenutnii) {
                prije_trenutnog = prije_trenutnog->sljedeci;
            }
        }
    }
public:
    JednostrukaLista() : pocetakk(nullptr), krajj(nullptr), prije_trenutnog(nullptr), trenutnii(nullptr), broj_elemenata(0) {}

    JednostrukaLista(const JednostrukaLista &lista) : pocetakk(nullptr), krajj(nullptr), prije_trenutnog(nullptr), trenutnii(nullptr), broj_elemenata(0) {
        cvor *p(lista.pocetakk);
        while(p != 0) {
            this->dodajIza(p->element);
            this->sljedeci();
            p = p->sljedeci;
        }
    }

    ~JednostrukaLista() {
        int tmp(broj_elemenata);
        for(int i = 0; i < tmp; i++) this->obrisi();
    }

    JednostrukaLista<tip> &operator=(const JednostrukaLista &lista) {
        if (&lista==this)
            return *this;
        if(broj_elemenata > 1)
            this->kraj();
        cvor *p = lista.pocetakk;
        int tmp(broj_elemenata);
        for(int i = 0; i < tmp; i++) this->obrisi();
        broj_elemenata = 0;
        while(p != 0) {
            this->dodajIza(p->element);
            this->sljedeci();
            p = p->sljedeci;
        }
        return *this;
    }


    int brojElemenata() const { return broj_elemenata; }

    tip& trenutni() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        return trenutnii->element;
    }

    tip trenutni() const {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        return trenutnii->element;
    }

    bool prethodni() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        if(!prije_trenutnog) return false;
        trenutnii = prije_trenutnog;
        PomjeriPrijeTrenutnog();
        return true;
    }

    bool sljedeci() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        if(!trenutnii->sljedeci) return false;
        trenutnii = trenutnii->sljedeci;
        PomjeriPrijeTrenutnog();
        return true;
    }

    void pocetak() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        trenutnii = pocetakk;
        PomjeriPrijeTrenutnog();
    }

    void kraj() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        trenutnii = krajj;
        PomjeriPrijeTrenutnog();
    }

    void obrisi() {
        if(!broj_elemenata) throw std::range_error("Prazna lista");
        if(trenutnii == krajj) {
            delete trenutnii;
            trenutnii = prije_trenutnog;
            krajj = prije_trenutnog;
            if(krajj) krajj->sljedeci = nullptr;
            if(!trenutnii) pocetakk = nullptr;
            broj_elemenata--;
            PomjeriPrijeTrenutnog();
            if(!prije_trenutnog) pocetakk = nullptr;
        } else {
            if(prije_trenutnog) {
                prije_trenutnog->sljedeci = trenutnii->sljedeci;
                delete trenutnii;
                broj_elemenata--;
                trenutnii = prije_trenutnog->sljedeci;
            } else {
                pocetakk = trenutnii->sljedeci;
                delete trenutnii;
                trenutnii = pocetakk;
                broj_elemenata--;
            }
        }
    }

    void dodajIspred(const tip& el) {
        if(!broj_elemenata) {
            pocetakk = new cvor(el, nullptr);
            broj_elemenata++;
            trenutnii = pocetakk;
            krajj = pocetakk;
        } else {
            if(pocetakk == trenutnii) {
                pocetakk = new cvor(el, trenutnii);
                broj_elemenata++;
                PomjeriPrijeTrenutnog();
            } else {
                cvor *tmp = new cvor(el, trenutnii);
                prije_trenutnog->sljedeci = tmp;
                prije_trenutnog = tmp;
                broj_elemenata++;
            }
        }
    }

    void dodajIza(const tip& el) {
        if(!broj_elemenata) {
            pocetakk = new cvor(el, nullptr);
            broj_elemenata++;
            trenutnii = pocetakk;
            krajj = pocetakk;
        } else {
            if(krajj == trenutnii) {
                krajj = new cvor(el, nullptr);
                trenutnii->sljedeci = krajj;
                broj_elemenata++;
            } else {
                cvor *tmp = new cvor(el, trenutnii->sljedeci);
                trenutnii->sljedeci = tmp;
                broj_elemenata++;
            }
        }
    }

    tip &operator[](int n) {
        if(n < 0 || n > broj_elemenata - 1) throw std::range_error("Izvan opsega");
        cvor *tmp(pocetakk);
        for(int i = 0; i < n; i++){
            tmp = tmp->sljedeci;
        }
        return tmp->element;
    }
    
    tip operator[](int n) const {
        if(n < 0 || n > broj_elemenata - 1) throw std::range_error("Izvan opsega");
        cvor *tmp(pocetakk);
        for(int i = 0; i < n; i++){
            tmp = tmp->sljedeci;
        }
        return tmp->element;
    }


};


template <typename Tip>
void test_brojelemenata(const NizLista<Tip> &l) {

    std::cout<<l.brojElemenata()<<std::endl;

}

template <typename Tip>
void test_trenutniconst(const NizLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_trenutniNeconst(NizLista<Tip> &l) {

    l.trenutni() = 500;
    std::cout<<l.trenutni()<<std::endl;

}

template <typename Tip>
void test_sljedeci (NizLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.sljedeci();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_prethodni (NizLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.prethodni();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_pocetak(NizLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.pocetak();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_kraj(NizLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.kraj();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_dodajiza(NizLista<Tip> &l) {
    for ( int i = 1; i <10; i++) l.dodajIza(i);
}


template <typename Tip>
void test_dodajispred(NizLista<Tip> &l) {
    for ( int i = 1; i < 100; i+=2) l.dodajIza(i);
}
template <typename Tip>
void test_obrisi(NizLista<Tip> &l) {
    for ( int i = 0; i < l.brojElemenata()/3; i++) l.obrisi();
}



template <typename Tip>
void test_brojelemenata2(const JednostrukaLista<Tip> &l) {
    std::cout<<l.brojElemenata()<<std::endl;
}
template <typename Tip>
void test_trenutniconst2(const JednostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_trenutniNeconst2(JednostrukaLista<Tip> &l) {
    l.trenutni() = 300;
    std::cout<<l.trenutni()<<std::endl;

}

template <typename Tip>
void test_sljedeci2(JednostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.sljedeci();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_prethodni2(JednostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.prethodni();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_pocetak2(JednostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.pocetak();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_kraj2(JednostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.kraj();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_dodajiza2(JednostrukaLista<Tip> &l) {
    for ( int i = 10; i < 20; i++) l.dodajIza(i);
}

template <typename Tip>
void test_dodajispred2(JednostrukaLista<Tip> &l) {
    for ( int i = 100; i < 100; i+=2) l.dodajIza(i);
}
template <typename Tip>
void test_obrisi2(JednostrukaLista<Tip> &l) {
    for ( int i = 0; i < l.brojElemenata()/3; i++) l.obrisi();
}

int main() {

NizLista<int> lista;
test_dodajispred(lista);
test_dodajiza(lista);
test_brojelemenata(lista);
test_trenutniconst(lista);
test_trenutniNeconst(lista);
test_sljedeci(lista);
test_prethodni(lista);
test_pocetak(lista);
test_kraj(lista);
NizLista<int> nlista2;
nlista2 = lista;
test_obrisi(lista);
NizLista<int> nova(lista);
for (int i = 0; i < lista.brojElemenata(); i++) std::cout<<lista[i]<<" ";
for (int i = 0; i < nlista2.brojElemenata(); i++) std::cout<<nlista2[i]<<" ";
for (int i = 0; i < nova.brojElemenata(); i++) std::cout<<nova[i]<<" ";
for (int i = 0; i < nlista2.brojElemenata(); i++) nlista2[i] = 6;
for (int i = 0; i < nlista2.brojElemenata(); i++) std::cout<<nlista2[i]<<" ";


std::cout<<std::endl;
// JEDNOSTRUKA TESTOVI

JednostrukaLista<int> jlista;
test_dodajispred2(jlista);
test_dodajiza2(jlista);
test_brojelemenata2(jlista);
test_trenutniconst2(jlista);
test_trenutniNeconst2(jlista);
test_sljedeci2(jlista);
test_prethodni2(jlista);
test_pocetak2(jlista);
test_kraj2(jlista);
JednostrukaLista<int> jedn_2;
jedn_2= jlista;
test_obrisi2(jlista);
JednostrukaLista<int> jnova(jlista);
for (int i = 0; i < jlista.brojElemenata(); i++) std::cout<<jlista[i]<<" ";
for (int i = 0; i < jedn_2.brojElemenata(); i++) std::cout<<jedn_2[i]<<" ";
for (int i = 0; i < jnova.brojElemenata(); i++) std::cout<<jnova[i]<<" ";
for (int i = 0; i < nlista2.brojElemenata(); i++) nlista2[i] = 6;
for (int i = 0; i < nlista2.brojElemenata(); i++) std::cout<<nlista2[i]<<" ";

return 0;
}
