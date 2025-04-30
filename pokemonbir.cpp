#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <vector>

int height = 10, width = 10;
int playerX = 5, playerY = 5;
char map[10][10];
int playerHP = 20;
int playerLevel = 1;
int playerXP = 0;
std::vector<std::string> inventory;

void generateMap() {
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++)
            map[y][x] = '#'; // full grass
}

void drawMap() {
    system("cls");
    std::cout << "HP: " << playerHP
              << " | Level: " << playerLevel
              << " | XP: " << playerXP << "/100\n\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == playerX && y == playerY)
                std::cout << '@';
            else
                std::cout << map[y][x];
        }
        std::cout << '\n';
    }

    std::cout << "\n[i] Inventory\n";
}

void showInventory() {
    std::cout << "\n--- Inventory ---\n";
    if (inventory.empty()) {
        std::cout << "You have no Pokemon.\n";
    } else {
        for (int i = 0; i < inventory.size(); i++) {
            std::cout << i + 1 << ". " << inventory[i] << "\n";
        }
    }
    std::cout << "Press any key to continue...";
    _getch();
}

void gainXP(int amount) {
    playerXP += amount;
    std::cout << "\nYou gained " << amount << " XP!\n";

    while (playerXP >= 100) {
        playerXP -= 100;
        playerLevel++;
        playerHP += 5;
        std::cout << "LEVEL UP! You are now level " << playerLevel << "! HP increased.\n";
    }

    Sleep(1000);
}

void startBattle() {
    std::string pokemons[] = {"Rattata", "Pidgey", "Zubat", "Caterpie"};
    std::string enemy = pokemons[rand() % 4];
    int enemyHP = 10 + rand() % 6;

    std::cout << "\nA wild " << enemy << " appeared!\n";

    while (enemyHP > 0 && playerHP > 0) {
        std::cout << "\n--- Battle ---\n";
        std::cout << enemy << " HP: " << enemyHP << "\n";
        std::cout << "Your HP: " << playerHP << "\n\n";

        std::cout << "[1] Attack\n[2] Throw ball\n[3] Run\nChoice: ";
        char choice = _getch();
        std::cout << choice << "\n";

        if (choice == '1') {
            int playerDMG = 3 + rand() % 4;
            int enemyDMG = 2 + rand() % 3;

            enemyHP -= playerDMG;
            std::cout << "You hit " << enemy << " for " << playerDMG << " damage!\n";

            if (enemyHP > 0) {
                playerHP -= enemyDMG;
                std::cout << enemy << " hit you for " << enemyDMG << " damage!\n";
            }
        }
        else if (choice == '2') {
            int chance = rand() % 100;
            if (chance < 40) {
                std::cout << "You caught " << enemy << "!\n";
                inventory.push_back(enemy);
                gainXP(50); // Bonus XP for catch
                return;
            } else {
                std::cout << "Failed to catch!\n";
                int enemyDMG = 2 + rand() % 3;
                playerHP -= enemyDMG;
                std::cout << enemy << " hit you for " << enemyDMG << " damage!\n";
            }
        }
        else if (choice == '3') {
            int chance = rand() % 100;
            if (chance < 50) {
                std::cout << "You escaped successfully!\n";
                Sleep(1000);
                return;
            } else {
                std::cout << "Failed to escape!\n";
                int enemyDMG = 2 + rand() % 3;
                playerHP -= enemyDMG;
                std::cout << enemy << " hit you for " << enemyDMG << " damage!\n";
            }
        } else {
            std::cout << "Invalid choice!\n";
        }

        if (playerHP <= 0) {
            std::cout << "\nYou fainted... Game over.\n";
            exit(0);
        }

        Sleep(1000);
    }

    if (enemyHP <= 0) {
        std::cout << "\n" << enemy << " fainted!\n";
        gainXP(25); // XP for winning
    }
}

void updateInput() {
    char ch = _getch();
    switch (ch) {
        case 'w': if (playerY > 0) playerY--; break;
        case 's': if (playerY < height - 1) playerY++; break;
        case 'a': if (playerX > 0) playerX--; break;
        case 'd': if (playerX < width - 1) playerX++; break;
        case 'i': showInventory(); return;
    }

    // %30 savaþ þansý
    if (map[playerY][playerX] == '#' && (rand() % 100) < 30) {
        startBattle();
    }
}

int main() {
    srand(time(0));
    generateMap();

    while (true) {
        drawMap();
        updateInput();
    }

    return 0;
}

