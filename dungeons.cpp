// ============================================================
//  Dungeon Explorer — a simple text-based adventure in C++
//  Great for beginners: covers structs, functions, loops,
//  conditionals, rand(), and basic game-loop design.
// ============================================================
#include <iostream>
#include <string>
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <limits>    // numeric_limits

// ── Data ────────────────────────────────────────────────────

struct Player {
    std::string name;
    int hp        = 30;
    int maxHp     = 30;
    int attack    = 6;
    int gold      = 0;
    int potions   = 2;
    int level     = 1;
    int xp        = 0;
    int xpToLevel = 10;
};

struct Enemy {
    std::string name;
    int hp;
    int attack;
    int reward;   // gold
    int xp;
};

// ── Helpers ─────────────────────────────────────────────────

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int rollDice(int sides) {          // returns 1 .. sides
    return (rand() % sides) + 1;
}

void printLine() {
    std::cout << "--------------------------------------------\n";
}

void showStatus(const Player& p) {
    printLine();
    std::cout << " " << p.name
              << "  |  HP: " << p.hp << "/" << p.maxHp
              << "  |  Gold: " << p.gold
              << "  |  Potions: " << p.potions
              << "  |  Level: " << p.level
              << "  |  XP: " << p.xp << "/" << p.xpToLevel << "\n";
    printLine();
}

// ── Level-up ────────────────────────────────────────────────

void checkLevelUp(Player& p) {
    if (p.xp < p.xpToLevel) return;

    p.xp       -= p.xpToLevel;
    p.level    += 1;
    p.xpToLevel = p.level * 10;
    p.maxHp    += 5;
    p.hp        = p.maxHp;      // full heal on level-up
    p.attack   += 2;

    std::cout << "\n*** LEVEL UP! You are now level " << p.level
              << "! Attack +" << 2 << ", Max HP +" << 5
              << " (fully healed) ***\n";
}

// ── Combat ──────────────────────────────────────────────────

void fight(Player& p, Enemy e) {
    std::cout << "\nA wild " << e.name << " appears! (HP: " << e.hp << ")\n";

    while (e.hp > 0 && p.hp > 0) {
        showStatus(p);
        std::cout << " Enemy HP: " << e.hp << "\n\n";
        std::cout << " [1] Attack   [2] Use Potion   [3] Run\n> ";

        int choice = 0;
        std::cin >> choice;
        clearInput();

        if (choice == 1) {
            // Player attacks
            int dmg = p.attack + rollDice(4) - 1;   // attack + 0..3 bonus
            e.hp -= dmg;
            std::cout << " You hit the " << e.name << " for " << dmg << " damage!\n";

        } else if (choice == 2) {
            if (p.potions <= 0) {
                std::cout << " No potions left!\n";
            } else {
                int heal = 10 + rollDice(6);
                p.hp = std::min(p.hp + heal, p.maxHp);
                p.potions--;
                std::cout << " You drink a potion and restore " << heal << " HP.\n";
            }

        } else if (choice == 3) {
            if (rollDice(2) == 1) {
                std::cout << " You escaped!\n";
                return;
            } else {
                std::cout << " Couldn't escape!\n";
            }

        } else {
            std::cout << " Unknown command.\n";
        }

        // Enemy attacks back if still alive
        if (e.hp > 0) {
            int eDmg = e.attack + rollDice(4) - 1;
            p.hp -= eDmg;
            std::cout << " The " << e.name << " hits you for " << eDmg << " damage!\n";
        }
    }

    if (p.hp <= 0) return;   // player died — handled in main loop

    // Victory
    std::cout << "\n You defeated the " << e.name
              << "! Got " << e.reward << " gold and " << e.xp << " XP.\n";
    p.gold += e.reward;
    p.xp   += e.xp;
    checkLevelUp(p);
}

// ── Shop ────────────────────────────────────────────────────

void visitShop(Player& p) {
    printLine();
    std::cout << " SHOP\n";
    printLine();
    std::cout << " Your gold: " << p.gold << "\n\n";
    std::cout << " [1] Potion (8 gold)  — restore HP in battle\n";
    std::cout << " [2] Sharpen Sword (15 gold) — +3 attack permanently\n";
    std::cout << " [3] Leave shop\n> ";

    int choice = 0;
    std::cin >> choice;
    clearInput();

    if (choice == 1) {
        if (p.gold >= 8) { p.gold -= 8; p.potions++; std::cout << " Bought a potion!\n"; }
        else              std::cout << " Not enough gold.\n";
    } else if (choice == 2) {
        if (p.gold >= 15) { p.gold -= 15; p.attack += 3; std::cout << " Sword sharpened! Attack +3.\n"; }
        else               std::cout << " Not enough gold.\n";
    } else {
        std::cout << " You leave the shop.\n";
    }
}

// ── Random encounter ────────────────────────────────────────

Enemy randomEnemy(int depth) {
    // enemy strength scales with dungeon depth
    int tier = std::min(depth, 5);

    Enemy enemies[5] = {
        {"Slime",    4 + tier*2,  2 + tier,   3 + tier,   3 + tier},
        {"Goblin",   6 + tier*2,  3 + tier,   5 + tier,   5 + tier},
        {"Skeleton", 8 + tier*2,  4 + tier,   7 + tier,   7 + tier},
        {"Orc",     12 + tier*2,  5 + tier,  10 + tier,  10 + tier},
        {"Troll",   18 + tier*2,  7 + tier,  15 + tier,  15 + tier},
    };

    return enemies[rand() % 5];
}

// ── Main game loop ───────────────────────────────────────────

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    std::cout << "============================================\n";
    std::cout << "         DUNGEON EXPLORER\n";
    std::cout << "============================================\n";
    std::cout << " Enter your hero's name: ";

    Player player;
    std::getline(std::cin, player.name);
    if (player.name.empty()) player.name = "Hero";

    std::cout << "\n Welcome, " << player.name
              << "! Descend into the dungeon and survive!\n";

    int depth = 0;   // floors explored

    while (player.hp > 0) {
        showStatus(player);
        std::cout << "\n You are on floor " << depth + 1 << ".\n\n";
        std::cout << " [1] Explore deeper\n";
        std::cout << " [2] Rest (free, once per floor — risky!)\n";
        std::cout << " [3] Visit shop\n";
        std::cout << " [4] Quit\n> ";

        int choice = 0;
        std::cin >> choice;
        clearInput();

        if (choice == 1) {
            depth++;
            int roll = rollDice(6);

            if (roll <= 4) {                       // 4/6 chance: fight
                Enemy e = randomEnemy(depth);
                fight(player, e);

            } else if (roll == 5) {                // 1/6 chance: treasure
                int found = 5 + rollDice(10);
                std::cout << "\n You found a chest with " << found << " gold!\n";
                player.gold += found;

            } else {                               // 1/6 chance: nothing
                std::cout << "\n The corridor is empty. You move on.\n";
            }

        } else if (choice == 2) {
            // Resting heals but may attract an enemy
            if (rollDice(2) == 1) {
                int heal = 5 + rollDice(6);
                player.hp = std::min(player.hp + heal, player.maxHp);
                std::cout << "\n You rest and recover " << heal << " HP.\n";
            } else {
                std::cout << "\n An enemy interrupts your rest!\n";
                fight(player, randomEnemy(depth));
            }

        } else if (choice == 3) {
            visitShop(player);

        } else if (choice == 4) {
            std::cout << "\n You flee the dungeon. Goodbye!\n";
            break;

        } else {
            std::cout << " Invalid choice.\n";
        }
    }

    // Game over / final score
    printLine();
    if (player.hp <= 0) {
        std::cout << " " << player.name << " has fallen in the dungeon...\n";
    }
    std::cout << " === FINAL SCORE ===\n";
    std::cout << "  Floors explored : " << depth  << "\n";
    std::cout << "  Gold collected  : " << player.gold << "\n";
    std::cout << "  Level reached   : " << player.level << "\n";
    printLine();

    return 0;
}
