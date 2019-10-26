#include <iostream>
#include <stdexcept>

template<typename tip>
class Iterator;

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
    friend class Iterator<tip>;
};

template<typename tip>
class DvostrukaLista : public Lista<tip> {
  struct Cvor { 
     tip element; 
     Cvor *sljedeci; 
     Cvor *prethodni; 
     Cvor (const tip &element, Cvor* prevp, Cvor *nextp = nullptr): element(element), sljedeci(nextp), prethodni(prevp) {
      //  if ( prethodni != 0) prethodni->sljedeci = this;
        //if (sljedeci != 0) sljedeci->prethodni = this;
        }
     Cvor(Cvor* prevp = nullptr, Cvor* nextp = nullptr) :  sljedeci(nextp), prethodni(prevp) {
      //  if (prethodni != 0 ) prethodni->sljedeci = this;
    //    if (sljedeci != 0) sljedeci->prethodni = this;
         }
     };
     Cvor* pocetakk;
     Cvor* krajj; 
     Cvor* tekuci;
     int broj_elemenata;
  public:
    DvostrukaLista() : pocetakk(nullptr), krajj(nullptr), tekuci(nullptr), broj_elemenata(0) {}
    
    DvostrukaLista(const DvostrukaLista& dl) : pocetakk(nullptr), krajj(nullptr), tekuci(nullptr), broj_elemenata(0) {
      Cvor* tmp = dl.pocetakk;
      while(tmp) {
        this->dodajIza(tmp->element);
        this->sljedeci();
        tmp = tmp->sljedeci;
      }
    }
    
    DvostrukaLista& operator=(const DvostrukaLista& dl) {
      if(&dl == this) return *this;
      int brel = broj_elemenata;
      for(int i = 0; i < brel; i++) this->obrisi();
      broj_elemenata = 0;
      Cvor* tmp = dl.pocetakk;
      while(tmp) {
        this->dodajIza(tmp->element);
        this->sljedeci();
        tmp = tmp->sljedeci;
      }
      return *this;
    }
    
    ~DvostrukaLista() {
      int tmp = broj_elemenata;
      for(int i = 0; i < tmp; i++) this->obrisi();
    }
    
    
    int brojElemenata()  const override { return broj_elemenata; }
    
    
    tip trenutni() const override { if(!broj_elemenata) throw std::range_error("Prazna lista"); return tekuci->element; }
    
    
    tip& trenutni() override { if(!broj_elemenata) throw std::range_error("Prazna lista"); return tekuci->element; }
    
    
    bool prethodni() override {
      if(!broj_elemenata) throw std::range_error("Prazna lista");
      if(tekuci == pocetakk) return false;
      tekuci = tekuci->prethodni;
      return true;
    }
    
    
    bool sljedeci() override {
      if(!broj_elemenata) throw std::range_error("Prazna lista");
      if(tekuci == krajj) return false;
      tekuci = tekuci->sljedeci;
      return true;
    }
    
    
    void pocetak() override { if(!broj_elemenata) throw std::range_error("Prazna lista"); tekuci = pocetakk; }
    
    void kraj() override { if(!broj_elemenata) throw std::range_error("Prazna lista"); tekuci = krajj; }
    
    void obrisi() {
      if(!broj_elemenata) throw std::range_error("Prazna lista");
      if(tekuci == krajj) {
        if(broj_elemenata == 1) {
          delete tekuci;
          tekuci = nullptr;
          krajj = nullptr;
          pocetakk = nullptr;
          broj_elemenata--;
        } else {
          krajj = tekuci->prethodni;
          delete tekuci;
          tekuci = krajj;
          if(tekuci)
            tekuci->sljedeci = nullptr;
          broj_elemenata--;
        }
      } else {
        if(tekuci == pocetakk) {
          pocetakk = pocetakk->sljedeci;
          delete tekuci;
          tekuci = pocetakk;
          if(tekuci)
            tekuci->prethodni = nullptr;
          broj_elemenata--;
        } else {
          auto tmp = tekuci;
          tekuci->prethodni->sljedeci = tekuci->sljedeci;
          tekuci->sljedeci->prethodni = tekuci->prethodni;
          tekuci = tekuci->sljedeci;
          delete tmp;
          broj_elemenata--;
        }
      }
    }
    
    void dodajIspred(const tip& el) override {
      if(!broj_elemenata) {
        pocetakk = new Cvor(el,nullptr,nullptr);
        tekuci = pocetakk;
        krajj = pocetakk;
        broj_elemenata++;
      } else {
        if(tekuci == pocetakk) {
          pocetakk = new Cvor(el, nullptr, tekuci);
          tekuci->prethodni = pocetakk;
          broj_elemenata++;
        } else {
          auto tmp = new Cvor(el, tekuci->prethodni, tekuci);
          tekuci->prethodni->sljedeci = tmp;
          tekuci->prethodni = tmp;
          broj_elemenata++;
        }
      }
    }
    
    void dodajIza(const tip& el) override {
      if(!broj_elemenata) {
        pocetakk = new Cvor(el,nullptr,nullptr);
        tekuci = pocetakk;
        krajj = pocetakk;
         broj_elemenata++;
      } else {
        if(tekuci == krajj) {
          krajj = new Cvor(el, tekuci, nullptr);
          tekuci->sljedeci = krajj;
          broj_elemenata++;
        } else {
          auto tmp = new Cvor(el, tekuci, tekuci->sljedeci);
          tekuci->sljedeci->prethodni = tmp;
          tekuci->sljedeci = tmp;
          broj_elemenata++;
        }
      }
    }
    tip &operator[](int n) override {
      if(n < 0 || n > broj_elemenata - 1) throw std::range_error("Izvan granica");
      if(n <= broj_elemenata - 1 - n) {
        Cvor *tmp = pocetakk;
        for(int i = 0; i < n; i++) tmp = tmp->sljedeci;
        return tmp->element;
      } else {
        Cvor *tmp = krajj;
        for(int i = 0; i < broj_elemenata - 1 - n; i++) tmp = tmp->prethodni;
        return tmp->element;
      }
    }
    tip operator[](int n) const override {
      if(n < 0 || n > broj_elemenata - 1) throw std::range_error("Izvan granica");
      if(n <= broj_elemenata - 1 - n) {
        Cvor *tmp = pocetakk;
        for(int i = 0; i < n; i++) tmp = tmp->sljedeci;
        return tmp->element;
      } else {
        Cvor *tmp = krajj;
        for(int i = 0; i < broj_elemenata - 1 - n; i++) tmp = tmp->prethodni;
        return tmp->element;
      }
    }
    friend class Iterator<tip>;
};

template<typename tip>
class Iterator{
    const DvostrukaLista<tip> *lista;
    typename DvostrukaLista<tip>::Cvor *trenutniLista;
  public:
    Iterator(const Lista<tip> &lista) {
      DvostrukaLista<tip> &pok = (DvostrukaLista<tip> &)lista;
      this->lista = &pok;
      trenutniLista = pok.pocetakk;
    }
    
    Iterator(const DvostrukaLista<tip> &lista) : lista(&lista), trenutniLista(lista.pocetakk) {}
    
    tip brojElemenata() const {
        if (lista != 0)
            return lista->brojElemenata();
    }
    
    tip trenutni()  {
        if (trenutniLista)
            return trenutniLista->element;
    }
    
    bool prethodni() {
            if(!trenutniLista == lista->pocetakk) return false;
            trenutniLista = trenutniLista->prethodni;
            return true;
    }
    
     bool sljedeci() {
            if(trenutniLista == lista->krajj) return false;
            trenutniLista = trenutniLista->sljedeci;
            return true;
    }
    
    void pocetak() {
      if(lista)
          trenutniLista = lista->pocetakk;
    }
    
    void kraj() {
      if(lista)
         trenutniLista = lista->krajj;
    }
    
};


template<typename Tip>
Tip dajMaksimum(const Lista<Tip>& n) {
    Iterator<Tip> it(n);
    it.pocetak();
    Tip max = it.trenutni();
    for(int i = 0; i < it.brojElemenata(); i++) {
        if(it.trenutni() > max) max = it.trenutni();
        it.sljedeci();
    }
    return max;
}


template <typename Tip>
void test_brojelemenata(DvostrukaLista<Tip> &l) {
    std::cout<<l.brojElemenata()<<std::endl;
    l.dodajIza(5);
    std::cout<<l.brojElemenata()<<std::endl;
}

template <typename Tip>
void test_trenutniconst( DvostrukaLista<Tip> &l) {
    std::cout<<l.brojElemenata()<<std::endl;
    l.dodajIza(5);
    std::cout<<l.brojElemenata()<<std::endl;
}
template <typename Tip>
void test_trenutniNeconst(DvostrukaLista<Tip> &l) {
    l.trenutni() = 500;
    std::cout<<l.trenutni()<<std::endl;
}

template <typename Tip>
void test_sljedeci (DvostrukaLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.sljedeci();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_prethodni (DvostrukaLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.prethodni();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_pocetak(DvostrukaLista<Tip> &l) {

    std::cout<<l.trenutni()<<std::endl;
    l.pocetak();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_kraj(DvostrukaLista<Tip> &l) {
    std::cout<<l.trenutni()<<std::endl;
    l.kraj();
    std::cout<<l.trenutni()<<std::endl;

}
template <typename Tip>
void test_dodajiza(DvostrukaLista<Tip> &l) {
    for ( int i = 1; i <10; i++) l.dodajIza(i);
}


template <typename Tip>
void test_dodajispred(DvostrukaLista<Tip> &l) {
    for ( int i = 1; i < 100; i+=2) l.dodajIza(i);
}
template <typename Tip>
void test_obrisi(DvostrukaLista<Tip> &l) {
    for ( int i = 0; i < l.brojElemenata()/3; i++) l.obrisi();
}


int main() {
DvostrukaLista<int> lista;
test_dodajispred(lista);
test_dodajiza(lista);
test_brojelemenata(lista);
test_trenutniconst(lista);
test_trenutniNeconst(lista);
test_sljedeci(lista);
test_prethodni(lista);
test_pocetak(lista);
test_kraj(lista);
DvostrukaLista<int> nlista2;
nlista2 = lista;
test_obrisi(lista);
DvostrukaLista<int> nova(lista);
for (int i = 0; i < lista.brojElemenata(); i++) std::cout<<lista[i]<<" ";
for (int i = 0; i < nlista2.brojElemenata(); i++) std::cout<<nlista2[i]<<" ";
for (int i = 0; i < nova.brojElemenata(); i++) std::cout<<nova[i]<<" ";
for (int i = 0; i < nlista2.brojElemenata(); i++) nlista2[i] = 6;
for (int i = 0; i < nlista2.brojElemenata(); i++) std::cout<<nlista2[i]<<" ";


}