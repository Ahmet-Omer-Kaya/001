#include <iostream>
using namespace std;

int main() {
    double sayi1, sayi2;
    char islem;

    cout << "--- Hesap Makinesi ---" << endl;

    cout << "Birinci sayiyi gir: ";
    cin >> sayi1;

    cout << "Ikinci sayiyi gir: ";
    cin >> sayi2;

    cout << "Islem sec (+ , - , /, *): ";
    cin >> islem;

    if (islem == '+') {
        cout << "Toplam: " << (sayi1 + sayi2) << endl;
    }
    else if (islem == '-') {
        cout << "Fark: " << (sayi1 - sayi2) << endl;
    }
    else if (islem == '/') {
        if (sayi2 != 0)
            cout << "Bolum: " << (sayi1 / sayi2) << endl;
        else
            cout << "0'a bolunemez!" << endl;
    }
    else if (islem == '*') {
        cout << "Carpim: " << (sayi1 * sayi2) << endl;
    }
    else {
        cout << "Gecersiz islem!" << endl;
    }

    return 0;
}
