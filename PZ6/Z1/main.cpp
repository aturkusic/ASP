#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

template <typename Tip>
void selection_sort(Tip* niz, int vel) {
    Tip min;
    int index_min;
    for(int i = 0; i < vel; i++) {
        min = niz[i];
        index_min = i;
        for(int j = i + 1; j < vel; j++) {
           if(niz[j] < min) {
               min = niz[j];
               index_min = j;
           }
        }
        Tip tmp = niz[i];
        niz[i] = niz[index_min];
        niz[index_min] = tmp;
    }
}

template <typename Tip>
void bubble_sort(Tip* niz, int vel) {
    for(int i = 0; i < vel; i++) {
        for(int j = 0; j < vel - i - 1; j++) {
            if(niz[j] > niz[j + 1]) {
                Tip tmp = niz[j];
                niz[j] = niz[j +1];
                niz[j + 1] = tmp;
            }
        }
    }
}


template<typename Tip>
void merge(Tip* niz, int l, int p, int q, int u) {
    int i(0), j(q-l), k(l);
    Tip *pomocni = new Tip[u + 1 - l]; 
    for(int m = 0; m <= u - l; m++) {
        pomocni[m] = niz[l + m];
    }
    while(i <= p - l && j <= u - l) {
        if(pomocni[i] < pomocni[j]) {
            niz[k] = pomocni[i];
            i++;
        } else {
            niz[k] = pomocni[j];
            j++;
        }
        k++;
    }
    while(i <= p - l) {
        niz[k] = pomocni[i];
        k++;
        i++;
    }
    while(j <= u - l) {
        niz[k] = pomocni[j];
        k++;
        j++;
    }
    delete[] pomocni;
}

template <typename Tip>
void merge_sortPomocni(Tip* niz, int poc, int kraj) {
    if(kraj > poc) {
       int p = (poc + kraj - 1) / 2;
       int q = p + 1;
       merge_sortPomocni(niz, poc, p);
       merge_sortPomocni(niz, q, kraj);
       merge(niz, poc, p, q, kraj);
    }
}

template <typename Tip>
void merge_sort(Tip* niz, int vel) {
    merge_sortPomocni(niz, 0, vel - 1);
}

template <typename Tip>
void Zamjeni(Tip*niz, const int i, const int j) {
    Tip priv = niz[i];
    niz[i] = niz[j];
    niz[j] = priv;
}

template <typename Tip>
int Particija(Tip *niz, const int prvi, const int zadnji) {
     Tip pivot = niz[prvi];
    int p = prvi + 1;
    while (p <= zadnji) {
       if (niz[p] <= pivot)
           p++;
       else
           break;
       }
    for (int i = p+1; i <= zadnji; i++) {
        if (niz[i] < pivot) {
            Zamjeni(niz, p, i);
            p++;
        }
    }
    Zamjeni(niz, prvi, p-1);
    return p-1;
}

template <typename Tip>
void quick_sortPomocni(Tip * niz, const int prvi, const int zadnji) {
    if (prvi < zadnji) {
    int j = Particija(niz, prvi, zadnji);
    quick_sortPomocni(niz, prvi, j - 1);
    quick_sortPomocni(niz, j + 1, zadnji);
 }

}

template <typename Tip>
void quick_sort(Tip* niz, int vel) {
    quick_sortPomocni(niz, 0, vel - 1);
}

void ucitaj(std::string filename, int*& niz, int &vel) {
    std::ifstream ulazni_tok(filename);
    int broj;
    std::vector<int> v;
    while(ulazni_tok >> broj) {
        v.push_back(broj);
    }
    int *niz1 = new int[v.size()];
    std::copy(v.begin(), v.end(), niz1);
    niz = niz1; vel = v.size();
}

void generisi(std::string filename, int vel) {
    std::ofstream tok(filename);
    for(int i = 0; i < vel; i++) tok << rand() << " "; 
}

template<typename Tip>
bool daLiJeSortiran(Tip *niz,int vel) {
    for(int i = 0; i < vel - 1; i++) {
        if(niz[i] > niz[i + 1]) return false;
    }
    return true;
}

int main() {
    int* niz;
    int vel, n, ukvrijeme;
    ucitaj("dat", niz, vel);
    cout << "Odaberite jedan od sledecih metoda sortiranja: \n 1. Bubble sort\n 2. Selection sort \n 3. Merge sort \n 4. Quick sort \n";
    cin >> n;
    clock_t vrijeme1 = clock();
    if(n == 1) {
        bubble_sort(niz, vel);
    } else if (n == 2) {
        selection_sort(niz, vel);
    } else if (n == 3) {
        merge_sort(niz, vel);
    }  else if (n == 4) {
        quick_sort(niz, vel);
    }
    clock_t vrijeme2 = clock();
    ukvrijeme = (vrijeme2 - vrijeme1) / (CLOCKS_PER_SEC / 1000);
    cout << "Vrijeme izvrsavanja: " << ukvrijeme << " ms\n";
    if(daLiJeSortiran(niz, vel)) cout << "Niz je sortiran\n";
    else cout << "Niz nije sortiran\n";
    std::ofstream tok("sortiran");
    for(int i = 0; i < vel; i++) tok << niz[i] << " ";
    
    return 0;
}
