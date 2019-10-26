#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int getMax(vector<int> &a)
{
    int n = a.size();
    int mx = a[0];
    for (int i = 1; i < n; i++)
        if (a[i] > mx)
            mx = a[i];
    return mx;
}


void countSort(vector<int> &a, int exp)
{
    int n = a.size();
    int *output = new int[n];
    int i, count[10] = {0};
    for (i = 0; i < n; i++)
        count[ (a[i]/exp)%10 ]++;
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];
    for (i = n - 1; i >= 0; i--) {
        output[count[ (a[i]/exp)%10 ] - 1] = a[i];
        count[ (a[i]/exp)%10 ]--;
    }
    for (i = 0; i < n; i++)
        a[i] = output[i];
    delete[] output;
}


void radixSort(vector<int> &a)
{
    int m = getMax(a);
    for (int exp = 1; m/exp > 0; exp *= 10)
        countSort(a, exp);
}

void PopraviDolje(int i, vector<int> &a, int velicina)
{
    while (!(i >= velicina/2) && (i < velicina)) {
        int veci = 2 * i + 1;
        int dd = 2 * i + 2;
        if ((dd < velicina) && a[dd] > a[veci])
            veci=dd;
        if (a[i] > a[veci])
            return;
        int tmp(a[i]);
        a[i] = a[veci];
        a[veci] = tmp;
        i = veci;
    }
}

void stvoriGomilu(vector<int> &a)
{
    for (int i = a.size(); i >= 0; i--)
        PopraviDolje(i, a, a.size());
}

void PopraviGore(int i, vector<int> &a)
{
    while ((i != 0) && (a[i] > a[(i-1)/2])) {
        int roditelj((i-1)/2);
        int tmp(a[i]);
        a[i] = a[roditelj];
        a[roditelj] = tmp;
        i = roditelj;
    }
}


void umetniUGomilu(vector<int> &a, int umetnuti, int &velicina)
{
    int vel = a.size() - 1;
    if (velicina > vel)
        a.resize(a.size() + 1);
    a[velicina++] = umetnuti;
    PopraviGore(velicina-1, a);
}


int izbaciPrvi(vector<int> &a, int &velicina)
{
    if(velicina==0)
        throw "\nGomila je prazna!\n";
    int tmp(a[0]);
    a[0] = a[--velicina];
    a[velicina] = tmp;
    if (velicina != 0)
        PopraviDolje(0, a, velicina);
    return a[velicina];
}



void gomilaSort1(vector<int> &a) {
    stvoriGomilu(a);
    int vel = a.size();
    for(int i = 0; i < a.size()-2; i++)
        izbaciPrvi(a, vel);
}

void gomilaSort(vector<int> &a) {
    stvoriGomilu(a);
    int k = 1;
    for(int i = a.size() - 1; i >= 1; i--) {
        int tmp = a[0];
        a[0] = a[i];
        a[i] = tmp;
        int velicina = a.size() - k++;
        PopraviDolje(0, a, velicina);
    }
}

int main()
{
    vector<int> a;
    int velicina = 0;
    umetniUGomilu(a,546,velicina);
    umetniUGomilu(a,435435,velicina);
    umetniUGomilu(a,12,velicina);
    umetniUGomilu(a,546,velicina);
    umetniUGomilu(a,675767,velicina);
    umetniUGomilu(a,33,velicina);
    umetniUGomilu(a,555,velicina);
    umetniUGomilu(a,1,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    izbaciPrvi(a,velicina);
    for(int i(0);i<a.size();i++){
        cout<<a[i]<<velicina;
    } 
    return 0;
}
