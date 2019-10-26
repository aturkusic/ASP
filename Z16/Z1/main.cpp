#include <iostream>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

template<typename tip1, typename tip2>
class Mapa {
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

void testFunkcija1() {
    NizMapa<int, int> nizMapa;
    nizMapa[0] = 1;
    nizMapa[1] = 2;
    nizMapa[2] = 3;
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi(2);
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi();
    cout << nizMapa.brojElemenata();
}

void testFunkcija2() {
    cout << "\n";
    NizMapa<int, string> nizMapa;
    nizMapa[0] = "Arslan";
    nizMapa[1] = "Faris";
    nizMapa[2] = "Medin";
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi(2);
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi();
    cout << nizMapa.brojElemenata();
}

void testFunkcija3() {
    cout << "\n";
    NizMapa<string, string> nizMapa;
    nizMapa["Arslan"] = "Turkusic";
    nizMapa["Medin"] = "Paldum";
    nizMapa["Faris"] = "Music";
    cout << nizMapa["Arslan"] << " " << nizMapa["Medin"] << " " << nizMapa["Faris"] << " ";
    cout << "\n";
    nizMapa.obrisi("Medin");
    cout << nizMapa["Arslan"] << " " << nizMapa["Faris"] << " ";
    cout << "\n";
    nizMapa.obrisi();
    cout << nizMapa.brojElemenata();
}

void testFunkcija4() {
    cout << "\n";
    NizMapa<int, double> nizMapa;
    nizMapa[0] = 1.55687;
    nizMapa[1] = 1.61785;
    nizMapa[2] = 1.99831;
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi(2);
    for(int i = 0; i < nizMapa.brojElemenata(); i++) cout << nizMapa[i] << " ";
    cout << "\n";
    nizMapa.obrisi();
    cout << nizMapa.brojElemenata();
}

void testFunkcija5() {
    cout << "\n";
    NizMapa<string, vector<int>> nizMapa;
    nizMapa["Sarajevo"] = {1, -5, 7, 10, 3};
    nizMapa["Tuzla"] = {1, 0, 5, 4, -7};
    nizMapa["Mostar"] = {10, 9, 7, 10, 15};
    for(int i = 0; i < nizMapa["Sarajevo"].size(); i++) cout << nizMapa["Sarajevo"][i] << " ";
    cout << "\n";
    for(int i = 0; i < nizMapa["Tuzla"].size(); i++) cout << nizMapa["Tuzla"][i] << " ";
    cout << "\n";
    for(int i = 0; i < nizMapa["Mostar"].size(); i++) cout << nizMapa["Mostar"][i] << " ";
    cout << "\n";
    nizMapa.obrisi("Sarajevo");
    for(int i = 0; i < nizMapa["Tuzla"].size(); i++) cout << nizMapa["Tuzla"][i] << " ";
    cout << "\n";
    for(int i = 0; i < nizMapa["Mostar"].size(); i++) cout << nizMapa["Mostar"][i] << " ";
    cout << "\n";
    nizMapa.obrisi();
    cout << nizMapa.brojElemenata();
}


int main() {
    testFunkcija1();
    testFunkcija2();
    testFunkcija3();
    testFunkcija4();
    testFunkcija5();
    return 0;
}