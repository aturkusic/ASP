#include <iostream>

int fib2_0 (int n, int prvi = 1, int drugi = 1) {
    int tmp = drugi;
    if (n<=2) return tmp;
    int zbir = prvi + drugi;
    tmp = zbir;
    return fib2_0(n - 1, drugi, zbir);
}

int nzd(int x, int y) {
    if(!y) return x;
    return nzd(y, x%y);
}

int main() {
    std::cout << nzd(100, 10);
}
