#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int HARITA_BOYUTU = 5;

class Oyuncu {
public:
    int can;
    int posX, posY;
    bool kilicVar;

    Oyuncu() {
        can = 100;
        posX = 2;
        posY = 2;
        kilicVar = false;
    }

    void durumYazdir() {
        canBar();
        cout << "Pozisyon: (" << posX << ", " << posY << ")";
        cout << " | Kilic: " << (kilicVar ? "Var" : "Yok") << endl;
        if (kilicVar) kilicCiz();
    }

    void canBar() {
        int barUzunluk = 20;
        int dolu = (can * barUzunluk) / 200;
        cout << "Can: [";
        for (int i = 0; i < barUzunluk; i++) {
            cout << (i < dolu ? "#" : "-");
        }
        cout << "] (" << can << ")" << endl;
    }

    void kilicCiz() {
        cout << "     //\n";
        cout << "==||====>\n";
        cout << "     \\\\\n";
    }

};

void haritayiCiz(Oyuncu &oyuncu) {
    cout << "\nHarita:\n";
    for (int y = 0; y < HARITA_BOYUTU; y++) {
        for (int x = 0; x < HARITA_BOYUTU; x++) {
            if (x == oyuncu.posX && y == oyuncu.posY)
                cout << "[P]";
            else
                cout << "[ ]";
        }
        cout << endl;
    }
    cout << endl;
}

void hareketEt(Oyuncu &oyuncu, char yon) {
    switch(yon) {
        case 'w': 
            oyuncu.posY--; 
            if (oyuncu.posY < 0) oyuncu.posY = HARITA_BOYUTU - 1;  // Yukarý giderken döngüsel geçiþ
            break;
        case 's': 
            oyuncu.posY++; 
            if (oyuncu.posY >= HARITA_BOYUTU) oyuncu.posY = 0;  // Aþaðý giderken döngüsel geçiþ
            break;
        case 'a': 
            oyuncu.posX--; 
            if (oyuncu.posX < 0) oyuncu.posX = HARITA_BOYUTU - 1;  // Sol giderken döngüsel geçiþ
            break;
        case 'd': 
            oyuncu.posX++; 
            if (oyuncu.posX >= HARITA_BOYUTU) oyuncu.posX = 0;  // Sað giderken döngüsel geçiþ
            break;
        default: 
            cout << "Gecersiz yon!" << endl; 
            break;
    }
}

int rastgeleOdaTuru(int oncekiOda) {
    int yeniOda;
    do {
        yeniOda = rand() % 3; // 0: yaratýk, 1: iksir, 2: kilic
    } while (yeniOda == oncekiOda);
    return yeniOda;
}

void odaOlayi(Oyuncu &oyuncu, int &oncekiOda) {
    int tur = rastgeleOdaTuru(oncekiOda);
    oncekiOda = tur;

    if (tur == 0) {
        cout << "Bir yaratik seni saldirdi!" << endl;
        if (oyuncu.kilicVar && rand() % 2 == 0) {
            cout << "Kilicinla yaratigi yendin! Hic can kaybetmedin!" << endl;
        } else {
            cout << "Yaratik seni yaraladi! -10 can" << endl;
            oyuncu.can -= 10;
        }
    }
    else if (tur == 1) {
        cout << "Bir can iksiri buldun! +10 can" << endl;
        oyuncu.can += 10;
    }
    else if (tur == 2) {
        if (!oyuncu.kilicVar) {
            cout << "Bir kilic buldun ve aldin! Artik savasabilirsin!" << endl;
            oyuncu.kilicVar = true;
        } else {
            cout << "Kilic odasina geldin ama zaten kilicin var." << endl;
        }
    }

    if (oyuncu.can < 0) oyuncu.can = 0;
}

int main() {
    srand(time(0));

    Oyuncu oyuncu;
    char giris;
    int oncekiOda = -1;

    cout << "Zindana hos geldin!" << endl;
    cout << "Yon tuslari: w: yukari, a: sol, s: asagi, d: sag" << endl;

    while (oyuncu.can > 0) {
        haritayiCiz(oyuncu);
        oyuncu.durumYazdir();

        cout << "Yon gir (w/a/s/d): ";
        cin >> giris;

        hareketEt(oyuncu, giris);

        cout << "\nBir odaya girdin...\n";
        odaOlayi(oyuncu, oncekiOda);

        cout << "-----------------------------\n";
    }

    cout << "Canin bitti. Oyun bitti!" << endl;

    return 0;
}
