#include <iostream>
#include <stdexcept>
using namespace std;

template<typename tip>
class DvostraniRed {
  struct Cvor { 
     tip element; 
     Cvor *sljedeci; 
     Cvor *prethodni; 
     Cvor (const tip &element, Cvor* prevp, Cvor *nextp = nullptr): element(element), prethodni(prevp), sljedeci(nextp) {}
     Cvor(Cvor* prevp = nullptr, Cvor* nextp = nullptr) : prethodni(prevp), sljedeci(nextp) {}
     };
     Cvor* pocetakk;
     Cvor* krajj; 
     int broj_elemenata;
  public:
    DvostraniRed() : pocetakk(nullptr), krajj(nullptr), broj_elemenata(0) {}
    
    DvostraniRed(const DvostraniRed& dl) : pocetakk(nullptr), krajj(nullptr), broj_elemenata(0) {
      Cvor* tmp = dl.pocetakk;
      while(tmp) {
        this->staviNaVrh(tmp->element);
        tmp = tmp->sljedeci;
      }
    }
    
    DvostraniRed& operator=(const DvostraniRed& dl) {
      if(&dl == this) return *this;
      this->brisi();
      Cvor* tmp = dl.pocetakk;
      while(tmp) {
        this->staviNaVrh(tmp->element);
        tmp = tmp->sljedeci;
      }
      return *this;
    }
    
    ~DvostraniRed() {
      int tmp = broj_elemenata;
      for(int i = 0; i < tmp; i++) this->skiniSaVrha();
    }
    
    
    int brojElemenata() const { return broj_elemenata; }
    
    void brisi() {
      int tmp = broj_elemenata;
      for(int i = 0; i < tmp; i++) this->skiniSaVrha();
    }
    
    void staviNaCelo(const tip& el) {
      if(!broj_elemenata) {
        pocetakk = new Cvor(el,nullptr,nullptr);
        krajj = pocetakk;
        broj_elemenata++;
      } else {
          Cvor *tmp = new Cvor(el, nullptr, pocetakk);
          pocetakk->prethodni = tmp;
          pocetakk = tmp;
          broj_elemenata++;
      }
    }
    
    void staviNaVrh(const tip& el) {
      if(!broj_elemenata) {
        pocetakk = new Cvor(el,nullptr,nullptr);
        krajj = pocetakk;
         broj_elemenata++;
      } else {
         Cvor *tmp = new Cvor(el, krajj, nullptr);
          krajj->sljedeci = tmp;
          krajj = tmp;
          broj_elemenata++;
        }
    }
    
    tip skiniSaVrha() {
      if(!broj_elemenata) throw std::logic_error("Prazan red");
      tip vratit = krajj->element;
      Cvor *tmp = krajj;
      krajj = krajj->prethodni;
      if(krajj)
        krajj->sljedeci = nullptr;
      delete tmp;
      broj_elemenata--;
      return vratit;
    }   
    
    tip skiniSaCela() {
      if(!broj_elemenata) throw std::logic_error("Prazan red");
      tip vratit = pocetakk->element;
      Cvor *tmp = pocetakk;
      pocetakk = pocetakk->sljedeci;
      if(pocetakk)
        pocetakk->prethodni = nullptr;
      delete tmp;
      broj_elemenata--;
      return vratit;
    }
    
    tip& celo() {
      if(!broj_elemenata) throw std::logic_error("Prazan red");
      return pocetakk->element;
      
    }
    
    tip& vrh() {
      if(!broj_elemenata) throw std::logic_error("Prazan red");
      return krajj->element;
    }
 
};
template<typename tip>
void testBrojElemenata(DvostraniRed<tip> &red) {
  cout <<red.brojElemenata() << "\n";
    red.staviNaCelo(5);
    cout << "Nakon ubacivanja ima " << red.brojElemenata() << "\n";
    red.brisi();
}
template<typename tip>
void testSkiniSaVrha(DvostraniRed<tip> &red) {
  cout << red.brojElemenata() << "\n";
    red.staviNaVrh(1); cout <<red.brojElemenata() << "\n";
    red.skiniSaVrha();
    cout << red.brojElemenata() << "\n";
}
template<typename tip>
void testSkiniSaCela(DvostraniRed<tip> &red) {
  cout << red.brojElemenata() << "\n";
    red.staviNaCelo(1); cout <<red.brojElemenata() << "\n";
    red.skiniSaCela();
    cout << red.brojElemenata() << "\n";
}
template<typename tip>
void testVrh(DvostraniRed<tip> &red) {
  red.staviNaVrh(200);
    cout << red.vrh();
    red.brisi();
}
template<typename tip>
void testCelo(DvostraniRed<tip> &red) {
  red.staviNaCelo(156);
    cout << red.celo();
    red.brisi();
}
template<typename tip>
void testBrisi(DvostraniRed<tip> &red) {
  for(int i = 5; i < 10; i++) red.staviNaCelo(i);
     cout << red.brojElemenata() << "\n";
     red.brisi();
     cout << "Nakon brisanja ima " << red.brojElemenata() << "\n";
}
template<typename tip>
void testStaviNaVrh(DvostraniRed<tip> &red) {
  for(int i = 1; i < 6; i++) red.staviNaVrh(i);
    for(int i = 1; i < 6; i++) cout << red.skiniSaCela() << " ";
}
template<typename tip>
void testStaviNaCelo(DvostraniRed<tip> &red) {
  for(int i = 10; i < 16; i++) red.staviNaCelo(i);
    for(int i = 10; i < 16; i++) cout << red.skiniSaVrha() << " ";
}


int main() { 
  DvostraniRed<int> red;
  std::cout << "\ntest staviNaCelo\n";
  testStaviNaCelo(red);
  std::cout << "\ntest staviNaVrh\n";
  testStaviNaVrh(red);
  std::cout << "\ntest brojElemenata\n";
  testBrojElemenata(red);
  std::cout << "\ntest vrh\n";
  testVrh(red);
  std::cout << "\ntest celo\n";
  testCelo(red);
  std::cout << "\ntest brisi\n";
  testBrisi(red);
  std::cout << "\ntest skiniSaCela\n";
  testSkiniSaCela(red);
  std::cout << "\ntest skiniSaVrha\n";
  testSkiniSaVrha(red);
  
  return 0;
}