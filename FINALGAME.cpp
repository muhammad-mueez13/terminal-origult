// -------------------- HEADER FILES -----------------------------

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <string>

using namespace std;

// -------------------------- Constants --------------------
const int width = 70;
const int height = 35;
const int max_bullets = 15;
const int max_enemies = 6;
const int max_e_bullets = 12;

// -------------------- Global Variables ----------------------

int pX = 35, pY = 22, pHealth = 100, score = 0, level = 1;
bool isFlashing = false;
int flashTimer = 0;
int itemX, itemY;
bool itemActive = false;

int eX[max_enemies], eY[max_enemies], eHP[max_enemies];
bool eActive[max_enemies];
int eTimer[max_enemies];

int bX[max_bullets], bY[max_bullets];
bool bActive[max_bullets];

int ebX[max_e_bullets], ebY[max_e_bullets];
bool ebActive[max_e_bullets];



// --- Function Declarations ---


void title();
void gotoxy(int x, int y);
void setColor(int color);

void ResetEnemy(int i);

void SpawnEnergy();

void ShowLevelScreen();

void DrawPlayer(int x, int y, bool erase);
void DrawEnemy(int x, int y, bool erase);

void UpdateUI();

void HandleBullets();
void HandleEnemies();
void HandleInput();

void MainMenu(); 
void StartGame(); 
void ShowInstructions(); 

// --- Main Function ---
int main() 
{
    srand(static_cast<unsigned int>(time(0)));
    
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

    MainMenu(); 
    
    return 0;
}

// ------------------- Main Menu Logic -------------------

void MainMenu() {
    int selected = 0;
    string options[] = {"S T A R T", "I N S T R U C T I O N S", "E X I T"};
    system("cls");
    title();
    while (true) 
    {
        
    
        
        
    
        setColor(8); 
        
        gotoxy(width / 2 - 15, height / 2 + 3);
        
        cout << "________________________________";
        
        for (int i = 0; i < 3; i++) 
        {
            gotoxy(width / 2 - 15, height / 2 + 5 + (i * 2));
        
            if (selected == i) 
            {
                setColor(11);
                cout << "  >>  " << options[i] << "  <<  ";
            } 
            else
            {
                setColor(7);
                cout << "      " << options[i] << "          ";
            }
        }
        
        setColor(8);
        gotoxy(width / 2 - 15, height / 2 + 11);
        
        cout << "________________________________";
        setColor(10);
        
        gotoxy(width / 2 - 10, height / 2 + 13);
        cout << "[ USE ARROWS & ENTER ]";

        char key = _getch();
        if (key == 72) 
        {
            
            selected = (selected - 1 + 3) % 3;
        } 
        
        else if (key == 80) 
        
        {
            selected = (selected + 1) % 3;
        
        } 
        
        else if (key == 13) 
        { 
          
            if (selected == 0) 
            
                StartGame();
            
            else if (selected == 1) 
                
                ShowInstructions();
            
            else if (selected == 2) 
            
                exit(0);
        }
    }
}

void ShowInstructions() 
{
    system("cls");

    title();

    setColor(14);

    gotoxy(10, height / 2 + 3);

    cout << "--- SYSTEM MISSION LOG ---";

    setColor(15);

    gotoxy(10, height / 2 + 5);

    cout << "PILOT: You are the last defense against the Grid Virus.";

    gotoxy(10, height / 2 + 7);

    cout << "CONTROLS:";
    
    setColor(10);
    
    cout << "\n\t\t[ ARROWS ]  Navigational Thrusters";
    cout << "\n\t\t[ SPACE  ]  Fire";
    cout << "\n\t\t[  (+)   ]  Energy Core Repair";
    
    gotoxy(10, height / 2 + 13);
    setColor(8);
    cout << "PRESS ANY KEY TO RETURN TO MAIN...";
    _getch();
}

void StartGame() 
{
    pHealth = 100; 
    score = 0; 
    level = 1; 
    pX = 35; 
    pY = 22;
    for (int i = 0; i < max_bullets; i++) bActive[i] = false;
    for (int i = 0; i < max_e_bullets; i++) ebActive[i] = false;
    for (int i = 0; i < max_enemies; i++) eActive[i] = false;

    ShowLevelScreen();

    while (pHealth > 0) 
    {
        UpdateUI();
        HandleInput();
        HandleEnemies();
        HandleBullets();

        if (score >= 100 && level == 1) 
        { 
            level = 2; 
            ShowLevelScreen(); 
        }
        
        else if (score >= 300 && level == 2) 
        { 
            level = 3; 
            ShowLevelScreen(); 
        }
        Sleep(30);
    }

    system("cls");
    setColor(12);
    gotoxy(width/2 - 10, height/2);
    cout << "!!! CONNECTION LOST !!!";
    gotoxy(width/2 - 8, height/2 + 2);
    setColor(15);
    cout << "FINAL SCORE: " << score;
    gotoxy(width/2 - 12, height/2 + 5);
    setColor(8);
    cout << "Press any key to reboot...";
    _getch();
}




void gotoxy(int x, int y) 
{
    COORD coord = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color) 
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void ResetEnemy(int i) 
{
    eX[i] = 2 + rand() % (width - 6);
    eY[i] = 1;
    eActive[i] = true;
    eTimer[i] = 0;
    eHP[i] = level * 2; 
}

void SpawnEnergy() 
{
    itemX = 5 + rand() % (width - 10);
    itemY = 5 + rand() % (height - 10);
    itemActive = true;
}

void ShowLevelScreen() 
{
    system("cls");
    title();
    setColor(14); 
    gotoxy(width / 2 - 10, height / 2 + 2);
    cout << "==========================";
    gotoxy(width / 2 - 10, height / 2 + 3);
    cout << "     INITIATING LEVEL " << level;
    gotoxy(width / 2 - 10, height / 2 + 4);
    cout << "==========================";
    Sleep(2000);
    system("cls");
    setColor(4);
    for (int i = 0; i <= width; i++) 
    { 
        gotoxy(i, 0); 
        cout << "#"; 
        gotoxy(i, height + 1); 
        cout << "#";
    }
    
    for (int i = 0; i <= height + 1; i++) 
    { 
        gotoxy(0, i); 
        cout << "#"; 
        gotoxy(width, i); 
        cout << "#"; 
    }
    
    for (int i = 0; i < max_enemies; i++) 
    
    ResetEnemy(i);
    
    SpawnEnergy();
}

void DrawPlayer(int x, int y, bool erase) 
{
    gotoxy(x, y);
    if (erase) 
    { 
        cout << "   "; 
        gotoxy(x, y + 1); 
        cout << "   "; 
    } 
    
    else 
    {
        if (isFlashing) 
            setColor(12); 
        
        else 
            setColor(10);
        
            cout << " ^ "; 
            gotoxy(x, y + 1); 
            cout << "[H]";
    }
}

void DrawEnemy(int x, int y, bool erase) 
{
    if (y < 1 || y >= height) 
    return;
    
    gotoxy(x, y);
    
    if (erase) 
    { 
        cout << "   "; 
        gotoxy(x, y + 1); 
        cout << "   "; 
    } 
    
        else 
        { 
        
            setColor(13); 
            cout << "[X]"; 
        
            gotoxy(x, y + 1); 
            cout << "v v"; 
        }
}

void UpdateUI() 
{
    gotoxy(75, 5); 
    
    setColor(15);
    
    cout << "SCORE: " << score << "  ";
    
    gotoxy(75, 6); 
    cout << "LEVEL: " << level;
   
    gotoxy(75, 8); 
    cout << "HEALTH: " << pHealth << "%  ";
   
    gotoxy(75, 9);
    
    for(int i = 0; i < 10; i++) 
    
    {
        if (i < pHealth / 10) 
        { 
            setColor(10); 
            cout << "I"; 
        }
        
        else 
        { 
            setColor(8); 
            cout << "I"; 
        }
    }
}

void HandleInput() 
{
    DrawPlayer(pX, pY, true);
    
    if (GetAsyncKeyState(VK_LEFT) && pX > 1) 
    pX--;
    
    if (GetAsyncKeyState(VK_RIGHT) && pX < width - 4) 
    pX++;
    
    if (GetAsyncKeyState(VK_UP) && pY > 1) 
    pY--;
    
    if (GetAsyncKeyState(VK_DOWN) && pY < height - 2) 
    pY++;
    
    if (GetAsyncKeyState(VK_SPACE)) 
    {
        
        static DWORD lastShot = 0;
        
        if (GetTickCount() - lastShot > 180) 
        {
            for (int i = 0; i < max_bullets; i++) 
            
            {
              
                if (!bActive[i]) 
                {
                    bActive[i] = true; 
                    bX[i] = pX + 1; 
                    bY[i] = pY - 1;
                    lastShot = GetTickCount(); 
                    break;
                }
            }
        }
    }
    DrawPlayer(pX, pY, false);
    if (itemActive && abs(pX - itemX) < 2 && abs(pY - itemY) < 2) 
    {
        pHealth = min(100, pHealth + 30); 
        itemActive = false;
        
        gotoxy(itemX, itemY); 
        cout << "   ";
    }
    
    if (itemActive) 
    { 
        gotoxy(itemX, itemY); 
        setColor(11); 
        cout << "[+]"; 
    }
}

void HandleEnemies() 
{
    if (isFlashing && ++flashTimer > 3) 
    { 
        isFlashing = false; 
        flashTimer = 0; 
    }
    
    for (int i = 0; i < (1 + level); i++)
    {
    
        if (eActive[i]) 
        {
            DrawEnemy(eX[i], eY[i], true);
            if (++eTimer[i] % (9 - level) == 0) 
            eY[i]++;
            
            if (rand() % (80 / level) == 0) 
            {
                for (int j = 0; j < max_e_bullets; j++) 
                {
                    if (!ebActive[j]) { ebActive[j] = true; 
                        ebX[j] = eX[i] + 1; 
                        ebY[j] = eY[i] + 2; 
                        break; 
                    }
                }
            }
            if (eY[i] >= height - 1) 
            ResetEnemy(i);
            
            else
            DrawEnemy(eX[i], eY[i], false);
            
            if (abs(eX[i] - pX) < 3 && abs(eY[i] - pY) < 2) 
            {
                pHealth -= 20; 
                isFlashing = true;
                
                DrawEnemy(eX[i], eY[i], true); 
                ResetEnemy(i);
            }
        }
    }
}

void HandleBullets() 
{
    for (int i = 0; i < max_bullets; i++) 
    {
        if (bActive[i]) 
        {
            gotoxy(bX[i], bY[i]); 
            cout << " ";
            
            if (--bY[i] < 1) bActive[i] = false;
           
            else 
            {
                for (int k = 0; k < max_e_bullets; k++) 
                {
                    if (ebActive[k] && bX[i] == ebX[k] && (bY[i] == ebY[k] || bY[i] == ebY[k] - 1)) 
                    {
                       
                        bActive[i] = false; 
                        ebActive[k] = false;
                        
                        gotoxy(ebX[k], ebY[k]); 
                        cout << " "; 
                        break;
                    }
                }
                if (bActive[i]) 
                {
                    gotoxy(bX[i], bY[i]); 
                    setColor(14); 
                    cout << "|"; 
                    
                    for (int j = 0; j < max_enemies; j++) 
                    {
                        if (eActive[j] && abs(bX[i] - (eX[j] + 1)) < 2 && abs(bY[i] - eY[j]) < 2) 
                        {
                            bActive[i] = false; 
                            
                            gotoxy(bX[i], bY[i]); 
                            cout << " ";
                            
                            if (--eHP[j] <= 0) 
                            { 
                                score += 5; 
                                DrawEnemy(eX[j], eY[j], true); 
                                ResetEnemy(j); 
                            }
                        }
                    }
                }
            }
        }
    }
    for (int i = 0; i < max_e_bullets; i++) 
    {
        if (ebActive[i])
        {
            gotoxy(ebX[i], ebY[i]); 
            cout << " ";
            
            if (++ebY[i] >= height) ebActive[i] = false;
            else 
            {
                gotoxy(ebX[i], ebY[i]); 
                setColor(12); 
                cout << "o";
                
                if (abs(ebX[i] - (pX + 1)) < 2 && abs(ebY[i] - pY) < 2) 
                {
                    pHealth -= 10; 
                    isFlashing = true; 
                    ebActive[i] = false;
                    gotoxy(ebX[i], ebY[i]); 
                    
                    cout << " ";
                }
            }
        }
    }
}


void title() 
{
    setColor(15);
    cout << "==========================================================================" << endl;
    setColor(11);
    cout << "\t  _______ ______ _____  __  __ _____ _   _          _      " << endl;
    cout << "\t |__   __|  ____|  __ \\|  \\/  |_   _| \\ | |   /\\   | |     " << endl;
    cout << "\t    | |  | |__  | |__) | \\  / | | | |  \\| |  /  \\  | |     " << endl;
    cout << "\t    | |  |  __| |  _  /| |\\/| | | | | . ` | / /\\ \\ | |     " << endl;
    cout << "\t    | |  | |____| | \\ \\| |  | |_| |_| |\\  |/ ____ \\| |____ " << endl;
    cout << "\t    |_|  |______|_|  \\_\\_|  |_|_____|_| \\_/_/    \\_\\______|" << endl;
    
    setColor(10);
    cout << "\t      ____  _____ _____  _____ _    _ _      _______ " << endl;
    cout << "\t     / __ \\|  __ \\_   _|/ ____| |  | | |    |__   __|" << endl;
    cout << "\t    | |  | | |__) || | | |  __| |  | | |       | |   " << endl;
    cout << "\t    | |  | |  _  / | | | | |_ | |  | | |       | |   " << endl;
    cout << "\t    | |__| | | \\ \\_| |_| |__| | |__| | |____   | |   " << endl;
    cout << "\t     \\____/|_|  \\_\\_____\\_____|\\____/|______|  |_|   " << endl << endl;
    
    setColor(15);
    gotoxy(0, 13);
    cout << "\n==========================================================================" << endl;
}

