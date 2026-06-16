#include <iostream>
#include <Windows.h>
#include <string> 
#include <limits> // tik del std::cin Ignore
#include <random> // Idomu pabandyti, C++ standartai (C++20/23)
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <fstream>      // save/load

//del highscore visi sitie
#include <vector>
#include <algorithm>
#include <ctime>


void PagrindinisMeniu();
void Judejimas();
void Karas(); //karas
void Monstras(); //sukurti monstra karo metu
void Resursai(); //Rinkti resursus is kuriu galima gaminti ginklus
void Inventorius(); //perziureti inventoriu arba ginklus bei ju gaminima
void Pasauliai(); // keliauti i kita pasauli, kas gali siek tiek pakeisti zaidima



void KawaiiMenu();
void VeikejoPasirinkimas();//Pradzioje pasirenkama kokie bus veikejo bonusai
void Maistas();//Judejimas -> 1 = Maistas
void Miegas(); //Judejimas -> 3 = Miegas
void levelUP();

//Funkcijos pridetos 06-16
void Save();
void Load();

void AddHighscore();
std::string GetDateTime();
int SkaiciuotiTaskus();
void Highscore();

void EksportuotiHTML();

bool kawaii = false; //Viena karta parodyti katino meniu
bool veikPasirinkimas = false; // viena karta pasirenki koks veikejas busi
bool MaistoApdorojimas = false; // Turint tinkamas priemones, labai sumazeja apsinuodijimas maistu
bool MaistoPaieska = false; //gali tik karta per diena ieskoti maisto
bool Miegutis = false; //gali tik karta per diena bandyti pailseti
bool Monstriukas = false; // naudojama Monstras() sukurimo funkcijoje
bool AplinkosApieska = false;


//::::::::::::Inventoriaus daiktai ir resursai skirti gaminti sarvus ir ginklus::::::::::::::::::::::::::
int ITEMviela{ 0 }, ITEMGelezis{ 0 }, ITEMstrypas{ 0 }, ITEMlipniJuosta{ 0 };
bool ITEMsuvirinimas = false;
bool ITMviela = false;
bool ITMlazda = false;
bool ITMsalmas = false;
bool ITMsarvai = false;
//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


//++++++++++++++++++++++++++++++++++++Keliones po skirtingus pasaulius++++++++++++++++++++++++++++++++++++
int PasaulioEjimai = 0;          // kiek ejimu praleista siame pasaulyje
int PasaulioKeitimoLimitas = 2;  // min ejimu kiekis
bool GalimaKeistiPasauli = false;
std::string PasauliuVardai[4] = { "Apleistas miestas", "Miskas", "Dykuma", "Soi6" };

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++++++++++++++++++++++++++++++++++++
// Dienos/nakties ciklas ir veiksmų skaitiklis
bool Diena = true;
int VeiksmaiDienoje = 0;
const int MaxVeiksmaiPerDiena = 3;
int IsgyventaDienu = 0;
//++++++++++++++++++++++++++++++++++++



std::string MonstroPavadinimas[] = { "Uzkrestas zmogus", "Skeletas", "Smygolas", "Zombis", "Vampyras", "Piktas suo!", "Minedas" }; //7
std::string DabartinisMonstras = " ";
std::string ZaidejoKlase = " "; //Funkcijoje karo langas naudojama, Zaidejo klases: mutantas, veteranas, isgyventojas, civilis

int MonstruVarduSkaicius = 6;

// Pagrindiniai kintamieji   IsgyventaDienu{ 0 },
int Gyvybes{ 100 }, GyvybesDabar{ 0 }, GyvybesMax{ 120 }, Alkis{ 100 }, Energija{ 100 }, Patirtis{ 0 }, Lygis{ 0 }, KitasPatirtiesLygis{ 100 };
int MonstroGyvybes{ 30 }, MonstroPatirtis{ 1 }, MonstroLygis{ 1 }, ZaidejoAtaka, MonstroAtaka{ 4 }, Gydymas{ 0 };
int ZaidejoZala = 6 + Lygis;
int Pasaulis = 0;//0 = miestas, 1 = miskas, 2 = dykuma, 3 = pelke, 4 = Soi6Suvarnabhumi


int veikejoPasirinkimas{ 0 }; // veikejas bus mutantas, veteranas, isgyventojas ar paprastas be pasikeitimu
int a;//Naudojamas visiems cin>>a;
int b;//pasirinkimas
int c;//maisto ivykis
int d;//Poilsio pasirinkimas
int e;//Monstras() %30 + 1
int f;//Monstras() %3 + 1
int g;
int h;
//panaudoti int: a,b,c,d,e,f,g,h

//#include <random> testas, C++ standartai (C++20/23)
//VeikejoPasirinkimas() if cin>>1
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dist(2, 5);
int randomNumber = dist(gen);

//VeikejoPasirinkimas() if cin>>1
std::uniform_int_distribution<> distTwo(2, 5);
//randomNumber = distTwo(rd);
//int randomNumber = distTwo(rd);


struct HighscoreEntry
{
    std::string date;
    int score;

    int Gyvybes;
    int Alkis;
    int Energija;
    int Patirtis;
    int Lygis;
};


int main()
{

    srand(time(0)); // reikia random seed, kad nesikartotu tie patys random skaiciai
    if (kawaii == false) { KawaiiMenu(); Patirtis = 0; }
    //++++++++++++++++++++++++++++++++++++
    while (Gyvybes > 0 && Alkis > 0 && Energija > 0)
    {
        //++++++++++++++++++++++++++++++++++++
        // Dienos/Nakties ciklas
        if (VeiksmaiDienoje >= 3) // 3 veiksmai per dieną
        {
            Diena = !Diena;       // pasikeičia laikas: diena <-> naktis
            VeiksmaiDienoje = 0;  // reset veiksmai
            IsgyventaDienu++;     // diena praejo
            std::cout << "\n*** " << (Diena ? "Diena" : "Naktis") << " prasideda! ***\n\n";
            Sleep(2000);
        }
        //++++++++++++++++++++++++++++++++++++

        //c = rand() % 100 + 1; // Naudojama Maistas()   
        //e = rand() % 30 + 1;
        //f = rand() % 3 + 1;
        //g = rand() % 7 + 1;   // naudojama KaroLangas() montro pavadinimas random
        //h = rand() % 100 + 1; // Naudojama Judejimo() dalyje pasirinkimas 2 ieskoti resursu

        if (veikPasirinkimas == false) { VeikejoPasirinkimas(); }

        // Pagrindinis meniu ar veiksmas
        PagrindinisMeniu();
    }

    //++++++++++++++++++++++++++++++++++++






    return 0;
}


void KawaiiMenu()
{
    kawaii = true;
    Sleep(3000);
    std::cout << "********Zaidimas********\n";
    std::cout << " |*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*| \n\n\n";
    std::cout << "      /|_/| <3 \n";
    std::cout << "      >^,^< \n";
    std::cout << "       / | \n";
    std::cout << "      (___)_// \n";
    std::cout << " |*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*|*| \n\n\n";
    Sleep(2000);
    system("cls");

}
void VeikejoPasirinkimas()
{
    veikPasirinkimas = true;
PasirinkimoPradzia:
    std::cout << "Pasirink kas esi siam pasaulyje !\n" << std::endl;
    std::cout << "Spausk 1 jei per sunkus pasirinkimas" << std::endl;
    std::cout << "Spausk 2 jei esi stiprus mutantas" << std::endl;
    std::cout << "Spausk 3 jei esi patyres veteranas" << std::endl;
    std::cout << "Spausk 4 jei esi vikrus isgyventojas" << std::endl;
    std::cout << "Spausk 5 jei pagalbos nereikia\n" << std::endl;



    std::cin >> veikejoPasirinkimas;
JeiNumeris1: // jei nesinori rinktis tada griztama cia ir automatiskai bus pasirenkama

    if (veikejoPasirinkimas == 1)
    {
        veikejoPasirinkimas = randomNumber;
        std::cout << "veikejoPasirinkimas = " << veikejoPasirinkimas << std::endl;
        Sleep(2000);
        GyvybesDabar = Gyvybes;
        goto JeiNumeris1;
    }
    else if (veikejoPasirinkimas == 2)
    {
        //Mutantas
        Gyvybes = Gyvybes + 15;
        GyvybesMax = GyvybesMax + 10;
        GyvybesDabar = Gyvybes;
        ZaidejoKlase = "Mutantas";
        PagrindinisMeniu();
    }
    else if (veikejoPasirinkimas == 3)
    {
        //Veteranas
        Patirtis = Patirtis + 70;
        GyvybesDabar = Gyvybes;
        ZaidejoKlase = "Veteranas";
        PagrindinisMeniu();
    }
    else if (veikejoPasirinkimas == 4)
    {
        //Isgyventojas
        Alkis = Alkis + 20;
        Energija = Energija + 20;
        GyvybesDabar = Gyvybes;
        ZaidejoKlase = "Isgyventojas";
        PagrindinisMeniu();
    }
    else if (veikejoPasirinkimas == 5)
    {
        //Be papildomo pranasumo
        GyvybesDabar = Gyvybes;
        ZaidejoKlase = "Civilis";
        PagrindinisMeniu();
    }
    else
    {
        std::cout << "Skaicius turi buti nuo 1 iki 5 !!!\n" << std::endl;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        system("cls");
        goto PasirinkimoPradzia;
    }
}


void PagrindinisMeniu()
{
    //int a;
    system("cls");
    //std::cout << "Norint testi spauskite 'Enter' ";
    //std::cin.ignore();
    std::cout << "Add highscore: 2" << "    <--\n\n";
	std::cout << "Highscore: 3" << "    <--\n\n";
    std::cout << "Save: 4 " << "    <--\n\n";
    std::cout << "Load: 5 " << "    <--\n\n";
    std::cout << "Gaminti ginklus arba sarvus: 6 " << "    <--\n\n";
    std::cout << "Pradeti nauja zaidima: 7 " << "    <--\n\n";
    std::cout << "Instrukcijos: 8 " << "    <--\n\n";
    std::cout << "Iseiti:9 " << "   <--\n\n";
    std::cout << "IsgyventaDienu: " << IsgyventaDienu << "\n\n";
    std::cout << PasauliuVardai[Pasaulis] << "\n\n";





    std::cout << "Lygis: " << Lygis << "\n\n";
    std::cout << "Gyvybes: " << GyvybesDabar << "       Patirtis: " << Patirtis << "     Energija: " << Energija << "     Alkis: " << Alkis << "\n";
    std::cout << "Spauskite bet koki kita skaiciu, kurio nera meniu sarase, norint testi " << std::endl;
    std::cin >> a;

    if (a == 2)
    {
        AddHighscore();

        std::cout << "Highscore pridetas!\n";
        Sleep(1500);
        return;
    }
    else if (a == 3)
    {
        Highscore();
        return;
    }
    else if (a == 4)
    {
        Save();
    }
    else if (a == 5)
    {
		Load();
    }
    else if (a == 6)
    {
        //Inventorius
        Inventorius();
    }
    else if (a == 7)
    {
        //Naujas zaidimas
        std::cout << "NAUJAS ZAIDIMAS!!! " << "\n\n";
        Sleep(2000);
        //std::cin >> a;
        //std::cin.ignore();
        PagrindinisMeniu();
    }
    else if (a == 8)
    {
        //instrukcijos
        std::cout << "INSTRUKCIJOS !!!" << "\n\n";
        Sleep(2000);
        //std::cin >> a;
        //std::cin.ignore();
        PagrindinisMeniu();
    }
    else if (a == 9)
    {
        exit(0);
    }

    else
    {
        std::cin.ignore();
        Judejimas();
    }
}

void Judejimas()
{
JudejimoPradzia:


    //Pergales tekstas ir salyga(25 dienos isgyventos)
    if (IsgyventaDienu == 25)
    {
        system("cls");
        std::cout << "LAIMEJOTE ZAIDIMA, SVEIKINIMAI!!!!!!" << std::endl;
        Sleep(4000);
        exit(0);
    }
    // rodyti diena/naktis
    std::cout << "\n======= ";
    if (Diena)
    {
        std::cout << "DIENA";
    }
    else
    {
        std::cout << "NAKTIS";
    }
    std::cout << " =======\n";

    // rodyti veiksmo pasirinkimus
    if (!MaistoPaieska)
    {
        std::cout << "Ieskoti maisto: 1 \n";
    }

    if (!AplinkosApieska)
    {
        std::cout << "Apieskoti aplinka ir rinkti resursus: 2 \n";
    }

    if (!Miegutis)
    {
        std::cout << "Pasilseti: 3 \n";
    }

    std::cout << "Isvykti is cia: 4 \n";
    std::cout << "Grizti i pagrindini meniu: 5 \n";
    std::cout << "Praleisti diena/nakti: 6 \n";

    std::cin >> b;

    // slaptas dialogas, negalima kartoti veiksmo
    if ((b == 1 && MaistoPaieska) || (b == 3 && Miegutis))
    {
        std::cout << " nespank tik ! slysk is cia ! \n\n\n";
        PagrindinisMeniu();
        return;
    }

    // kiekvienas veiksmas
    if (b == 1 && !MaistoPaieska)
    {
        Maistas();
    }
    else if (b == 2 && !AplinkosApieska)
    {
        Resursai();
    }
    else if (b == 3 && !Miegutis)
    {
        Miegas();
    }
    else if (b == 4)
    {
        Pasauliai();
        goto JudejimoPradzia;
    }
    else if (b == 5)
    {
        PagrindinisMeniu();
        return;
    }
    else if (b == 6)
    {
        // praleidzia diena/nakti
        VeiksmaiDienoje = MaxVeiksmaiPerDiena; // priverstinai paleidzia dienos pabaiga
        std::cout << "\nPraleista diena/nakti...\n";
        Sleep(1500);
    }
    else
    {
        goto JudejimoPradzia;
    }

    // padidinti veiksmų skaitiklį, nebent diena praleista (6)
    if (b != 6)
    {
        VeiksmaiDienoje++;
    }

    // diena/naktis ciklas
    if (VeiksmaiDienoje >= MaxVeiksmaiPerDiena)
    {
        Diena = !Diena; // sukeisti diena/nakti
        VeiksmaiDienoje = 0;
        IsgyventaDienu++; // padidinti išgyventų dienų skaitiklį

        std::cout << "\n*** Laikas pasikeite! ";
        if (Diena)
        {
            std::cout << "Diena prasideda ***\n";
        }
        else
        {
            std::cout << "Naktis prasideda ***\n";
        }

        // reset daily actions
        MaistoPaieska = false;
        Miegutis = false;
        AplinkosApieska = false;

        Sleep(2000);
    }



    if (Alkis > 100)
    {
        Alkis = 100;
    }

    if (Energija > 100)
    {
        Energija = 100;
    }

    goto JudejimoPradzia;
}





void Maistas()
{
    VeiksmaiDienoje++; //++++++++++++++++++++++++++++++++++++
    int c = rand() % 100 + 1;


    MaistoPaieska = true;

    if (c <= 20) // geri konservai
    {
        std::cout << "Radai neaiskius 3 konservus, taciau juos suvalgius pajutai, kad net akyse sviesiau pasidare ! \n\n" << std::endl;
        Sleep(2000);
        Gyvybes = Gyvybes + 5;
        Alkis = Alkis + 10;
        Energija = Energija + 5;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
    else if (c <= 40) // blogi konservai
    {
        std::cout << "Radai neaiskius 3 konservus, taciau juos suvalgius labai supykino ! \n\n" << std::endl;
        Sleep(2000);
        if (MaistoApdorojimas == false)
        {
            Gyvybes = Gyvybes - 20;
            Alkis = Alkis + 10;
            Energija = Energija - 20;
            std::cin.clear();
            std::cin.ignore(INT_MAX, '\n');
        }
    }
    else if (c <= 60) // Zuikis
    {
        std::cout << "Tau pavyko sumedzioti zuiki, taciau tai issekino paskutines jegas \n\n" << std::endl;
        Sleep(2000);
        Gyvybes = Gyvybes + 15;
        Alkis = Alkis + 35;
        Energija = Energija - 30;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
    else if (c <= 75) //energetinis, isgerus kofeino NEGALIMA MIEGOTI
    {
        std::cout << "Tau pavyko rasti kelis energetinius gerimus, po 150 mg kofeino! \n\n" << std::endl;
        Sleep(2000);
        Energija = Energija + 30;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
        Miegutis = true;
    }
    else if (c <= 100) //nepasiseke rasti maisto :(
    {
        std::cout << "Veltui isvaistytos jegos, reikia ieskoti maisto kitur \n\n" << std::endl;
        Sleep(2000);
        Energija = Energija - 10;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    }
    Sleep(2000);
    system("cls");
    PagrindinisMeniu();
}
void Miegas()
{
    VeiksmaiDienoje++; //++++++++++++++++++++++++++++++++++++

    d = rand() % 3 + 1;
    if (Miegutis == false)
    {
        Miegutis = true;
        switch (d) //Switch ir case panaudojimas isskyrus default
        {
        case 1:
        {
            std::cout << "Pagaliau pavyko normaliai pasnausti, jegos grizta... \n\n" << std::endl;
            Sleep(3000);
            Gyvybes = Gyvybes + 10;
            Alkis = Alkis - 20;
            Energija = Energija + 30;
            //system("cls");
            Judejimas();
            break;
        }
        case 2:
        {
            std::cout << "Uzmigti nesigavo, taciau poilsis vistiek nors kiek pastiprino... \n\n" << std::endl;
            Sleep(3000);
            Gyvybes = Gyvybes + 5;
            Alkis = Alkis - 20;
            Energija = Energija + 15;
            //system("cls");
            Judejimas();
            break;
        }
        case 3:
        {
            std::cout << "Staiga isgirsti kaip arteja neaiskus garsas ! Kazkas artinasi !" << std::endl;
            Sleep(2000);
            std::cout << "Staigiai pasoki ir kupinas adrenalino pasiruosi kovai!\n\n" << std::endl;
            Sleep(2000);
            Energija = Energija + 10;
            std::cout << "KARAS!!!!!!!!!!!" << std::endl;
            Monstras();
            //std::string tempName = MonstroPavadinimas[rand() % MonstruVarduSkaicius];
            //std::cout << "A" << tempName << "! /n;
            //DabartinisMonstras = tempName;
            Karas();
            Sleep(1000);
            break;
        }
        }

    }
}
void Resursai()
{
    int h = rand() % 100 + 1;
    VeiksmaiDienoje++; //++++++++++++++++++++++++++++++++++++

    if (h <= 5)
    {
        std::cout << "Jus radote gelezies, taciau pasidaro sunku gabenti !" << std::endl;
        ITEMGelezis++;
        Sleep(2000);
        Energija -= 20;
        std::cout << "Tempiant gelezies likucius buvote uzpultas monstro !" << std::endl;
        Sleep(2000);
        Karas();
    }
    else if (h <= 20 && h > 5)
    {
        std::cout << "Jus radote vielos, gal tai pravers ! Taciau suviniojant susizalojote" << std::endl;
        ITEMviela++;
        Sleep(4000);
        Energija -= 5;
        GyvybesDabar -= 5;

    }
    else if (h <= 60 && h > 20)
    {
        std::cout << "Jus nieko neradote, demotivuojantis jegu svaistymas !" << std::endl;
        Sleep(4000);
        Energija -= 15;
    }
    else if (h <= 70 && h > 60)
    {
        std::cout << "Jus radote strypa, bus reikalingas !" << std::endl;
        ITEMstrypas++;
        Sleep(4000);

    }
    else if (h <= 90 && h > 70)
    {
        std::cout << "Jus radote lipnios juostos, paciu laiku! " << std::endl;
        ITEMlipniJuosta++;
        Sleep(4000);
    }
    else
    {
        std::cout << "Jus radote suvirinimo prietaisa, taciau toks svarbus irankis yra saugomas !" << std::endl;
        ITEMsuvirinimas = true;
        Sleep(4000);
        Karas();
    }
    AplinkosApieska = true;
    //goto JudejimoPradzia;
    system("cls");



    PagrindinisMeniu();
}

void Pasauliai()
{
    system("cls");


    int senasPasaulis = Pasaulis;
    Pasaulis = rand() % 4; // 0–3

    std::cout << "Keliaujate i kita pasauli...\n";
    Sleep(3000);

    std::cout << "Pasaulis: " << PasauliuVardai[Pasaulis] << "\n\n";


    int roll = rand() % 3; // 0,1,2

    if (roll == 0)
    {
        GyvybesDabar += 5;
        std::cout << "+5 Gyvybiu\n";
    }
    else if (roll == 1)
    {
        Energija += 5;
        std::cout << "+5 Energijos\n";
    }
    else
    {
        Patirtis += 5;
        std::cout << "+5 Patirties\n";

    }
    IsgyventaDienu++;
    //if (GyvybesDabar > GyvybesMax) GyvybesDabar = GyvybesMax;
    //if (Energija > 100) Energija = 100;

    Sleep(3000);
}




void Karas()
{
    //int g = rand() % MonstruVarduSkaicius; // duoda monstrui varda
    int h = rand() % 100 + 1;


    //++++++++++++++++++++++++++++++++++++
    // Dienos/nakties efektai kovoje
    int efektoAtaka = MonstroAtaka;
    int efektoGynyba = ZaidejoZala;

    if (!Diena) // naktį monstras stipresnis
        efektoAtaka += 2;
    else // dieną žaidėjas gauna lengvą pagerinima
        efektoGynyba += 1;
    //++++++++++++++++++++++++++++++++++++

    if (Monstriukas == true)
    {
        Sleep(500);
        system("cls");

        std::cout << "Zaidejas: " << ZaidejoKlase << "         Zaidejo gyvybes: " << GyvybesDabar << "       Zaidejo lygis: " << Lygis << "      Zaidejo patirtis: " << Patirtis << std::endl;
        std::cout << "                                                                        " << std::endl;
        std::cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||" << std::endl;
        std::cout << "                                                                        " << std::endl;
        std::cout << "Monstras: " << DabartinisMonstras << "         Monstro gyvybes: " << MonstroGyvybes << "       MonstroLygis: " << MonstroLygis << "      Monstro patirtis: " << MonstroPatirtis << std::endl;

        if (GyvybesDabar <= 0)
        {
            GyvybesDabar = 0;
            system("cls");
            std::cout << "Jus negyvas! \n Auksciausias lygys pasiektas: " << Lygis << " buvote nuzudytas " << DabartinisMonstras << std::endl;
            Sleep(2000);
            exit(0);
        }
        else if (MonstroGyvybes <= 0)
        {
            MonstroGyvybes = 0;
            levelUP();
            return;
        }
        else if (GyvybesDabar >= 1 && MonstroGyvybes >= 1)
        {
            std::cout << "\n";
            std::cout << "1. Atakuoti\n";
            std::cout << "2. Blokuoti\n";
            std::cout << "3. BEGTI!\n";
            std::cout << "\n";
            std::cin >> ZaidejoAtaka;
            Sleep(1000);

            if (ZaidejoAtaka == 1)
            {
                // Ataka
                std::cout << "Puolant... padarete " << efektoGynyba << " zalos monstrui: " << DabartinisMonstras << std::endl;
                MonstroGyvybes -= efektoGynyba;
                Sleep(1500);

                if (MonstroGyvybes >= 1)
                {
                    std::cout << "\n";
                    std::cout << "Monstras atakuoja.. \n";
                    GyvybesDabar -= efektoAtaka;
                    std::cout << "Just smarkiai nukentejote " << efektoAtaka << " Gyvybiu liko: " << GyvybesDabar << std::endl;
                    Sleep(1500);
                }
            }
            else if (ZaidejoAtaka == 2)
            {
                // Blokavimas
                std::cout << "Blokavimas \n";
                int i = rand() % 100 + 1;
                if (i >= 50)
                {
                    std::cout << "Ataka pavyko uzblokuoti \n";
                    Gydymas = Lygis * 10 / 2;
                    std::cout << "Buvo pagydytas tasku: " << Gydymas << std::endl;
                    GyvybesDabar += Gydymas;
                    Sleep(2000);
                }
                else
                {
                    std::cout << "Nepavyko blokuoti atakos! \n";
                    GyvybesDabar -= efektoAtaka;
                    std::cout << "Jus buvote pasmeigtas i nugara del " << efektoAtaka << " dabartines gyvybes " << GyvybesDabar << std::endl;
                    Sleep(2000);
                }
            }
            else if (ZaidejoAtaka == 3)
            {
                // Begti
                std::cout << "Bandote isvengti ir begti\n";
                int x = rand() % 100 + 1;
                if (x >= 50)
                {
                    std::cout << "Pavyko pabegti !\n";
                    Sleep(2000);
                    PagrindinisMeniu();
                    return;
                }
                else
                {
                    std::cout << "Nepavyko pabegti !\n";
                    std::cout << "Monstras jus ziauriai suzalojo ! \n";
                    GyvybesDabar -= efektoAtaka + 20;
                    std::cout << "Jus praradote: " << efektoAtaka + 20 << " dabar gyvybiu liko: " << GyvybesDabar << std::endl;
                    Sleep(2000);
                }
            }
            else
            {
                std::cout << "Neteisingas skaicius! \n";
                Sleep(1000);
            }

            // Po veiksmo patikrinti gyvybes
            if (GyvybesDabar <= 0)
            {
                GyvybesDabar = 0;
                system("cls");
                std::cout << "Jus negyvas! \n Auksciausias lygys pasiektas: " << Lygis << " buvote nuzudytas " << DabartinisMonstras << std::endl;
                Sleep(2000);
                exit(0);
            }
            else if (MonstroGyvybes <= 0)
            {
                MonstroGyvybes = 0;
                levelUP();
                return;
            }

            // Tęsti kovą
            Karas();
        }
    }
    else
    {
        Monstras();
    }
}



void levelUP()
{
    Patirtis = Patirtis + MonstroPatirtis;
    std::cout << "Jus nugalejote" << std::endl;
    Sleep(1000);
    std::cout << " uzmusant monstra gaunate: " << MonstroPatirtis << " patirties tasku" << std::endl;

    Sleep(2000);
    if (Patirtis >= KitasPatirtiesLygis)
    {
        Lygis++;
        KitasPatirtiesLygis = KitasPatirtiesLygis * 3 / 2;
        GyvybesDabar = GyvybesDabar + 20;
        GyvybesMax = GyvybesDabar;
        std::cout << "Pasiektas kitas zaidejo lygis! " << Lygis << std::endl;
        std::cout << "Tasku lygis pakilo 20 ! " << GyvybesDabar << std::endl;
        Sleep(4000);
        Monstriukas = false;
        PagrindinisMeniu();
    }
    Sleep(4000);
    Monstriukas = false;
    PagrindinisMeniu();
}
/*
void Monstras()
{
    int e = rand() % 30 + 1;
    int f = rand() % 3 + 1;

    MonstroLygis += f + Lygis;
    MonstroGyvybes = e + MonstroLygis + IsgyventaDienu*2;

    MonstroAtaka = f + 2 + MonstroLygis + IsgyventaDienu;

    if (MonstroGyvybes == 0)
    {
        Monstras();
    }
    if (MonstroPatirtis == 0)
    {
        Monstras();
    }
    Monstriukas = true;
    Karas();
}
*/



/* SENESNE DALIS
void Monstras()
{
    // ===== MONSTRO SUKURIMAS =====

    // Atsitiktinai parenkamas monstras vardas
    int vardoI = rand() % MonstruVarduSkaicius;
    DabartinisMonstras = MonstroPavadinimas[vardoI];

    // Atsitiktiniai baziniai parametrai
    int bazinisHP = rand() % 30 + 20;      // 20–49 bazinės gyvybės
    int lygioBonusas = rand() % 3 + 1;  // +1–3 lygio bonusas

    // Monstro lygis priklauso nuo žaidėjo ir išgyventų dienų
    MonstroLygis = Lygis + lygioBonusas + IsgyventaDienu / 2;

    // Monstro gyvybės
    MonstroGyvybes = bazinisHP + MonstroLygis * 2;

    // Monstro ataka
    MonstroAtaka = 3 + MonstroLygis;

    // Patirtis už monstro nugalėjimą
    MonstroPatirtis = 5 + MonstroLygis * 2;

    Monstriukas = true;

    // Pradedame kovą
    Karas();
}
*/
void Monstras()
{
    int vardoI = rand() % MonstruVarduSkaicius;
    DabartinisMonstras = MonstroPavadinimas[vardoI];

    int bazinisHP = rand() % 30 + 20;
    int lygioBonusas = rand() % 3 + 1;

    MonstroLygis = Lygis + lygioBonusas + IsgyventaDienu / 2;

    MonstroGyvybes = bazinisHP + MonstroLygis * 2;
    MonstroAtaka = 3 + MonstroLygis;

    MonstroPatirtis = 5 + MonstroLygis * 2;

    if (Pasaulis == 0) // Apleistas miestas
    {
        if (DabartinisMonstras == "Uzkrestas zmogus" ||
            DabartinisMonstras == "Skeletas" ||
            DabartinisMonstras == "Zombis" ||
            DabartinisMonstras == "Minedas")
        {
            MonstroGyvybes += 5;
            MonstroAtaka += 2;

            std::cout << "[Apleistas miestas] "
                << DabartinisMonstras
                << " sustiprintas! +5 HP, +2 Ataka\n";
            Sleep(1500);
        }
    }
    else if (Pasaulis == 1) // Miskas
    {
        if (DabartinisMonstras == "Skeletas" ||
            DabartinisMonstras == "Vampyras" ||
            DabartinisMonstras == "Piktas suo!")
        {
            MonstroAtaka += 3;

            std::cout << "[Miskas] "
                << DabartinisMonstras
                << " gauna +3 Ataka\n";
            Sleep(1500);
        }

        if (DabartinisMonstras == "Zombis" ||
            DabartinisMonstras == "Uzkrestas zmogus")
        {
            MonstroAtaka -= 3;
            MonstroGyvybes -= 20;

            std::cout << "[Miskas] "
                << DabartinisMonstras
                << " susilpnejo! -20 HP, -3 Ataka\n";
            Sleep(1500);
        }
    }
    else if (Pasaulis == 2) // Dykuma
    {
        if (DabartinisMonstras == "Minedas")
        {
            MonstroGyvybes += 20;
            MonstroAtaka += 20;

            std::cout << "[Dykuma] Minedas gauna +20 HP ir +20 Ataka!\n";
            Sleep(1500);
        }
        else
        {
            MonstroGyvybes -= 15;

            std::cout << "[Dykuma] "
                << DabartinisMonstras
                << " praranda 15 HP\n";
            Sleep(1500);
        }
    }
    else if (Pasaulis == 3) // Soi6
    {
        GyvybesDabar -= 20;

        std::cout << "[Soi6] Jus iskart praradote 20 gyvybiu!\n";
        std::cout << "Dabar gyvybiu: " << GyvybesDabar << std::endl;
        Sleep(1500);
    }

    // INVENTORIAUS BONUSAI

    if (ITMviela)
    {
        if (DabartinisMonstras == "Skeletas" ||
            DabartinisMonstras == "Zombis")
        {
            MonstroGyvybes -= 15;
            MonstroAtaka -= 2;

            std::cout << "[Lazda su viela] "
                << DabartinisMonstras
                << " nukencia nuo spygliu! -15 HP, -2 Ataka\n";
            Sleep(1500);
        }
    }

    if (ITMlazda)
    {
        if (DabartinisMonstras == "Piktas suo!")
        {
            MonstroGyvybes -= 20;
            MonstroAtaka -= 5;

            std::cout << "[Zaisliukas is lazdos] "
                << "Piktas suo issiblasko! -20 HP, -5 Ataka\n";
            Sleep(1500);
        }
    }

    if (ITMsalmas)
    {
        if (DabartinisMonstras == "Zombis" ||
            DabartinisMonstras == "Vampyras")
        {
            MonstroAtaka -= 4;

            std::cout << "[Metalinis salmas] "
                << DabartinisMonstras
                << " sunkiau suzeidzia jus! -4 Ataka\n";
            Sleep(1500);
        }
    }

    if (ITMsarvai)
    {
        if (DabartinisMonstras == "Minedas")
        {
            MonstroAtaka = 1;

            std::cout << "[Metaliniai sarvai] "
                << "Minedo nagai neprasiskverbia! Ataka sumazinta iki 1\n";
            Sleep(1500);
        }

        GyvybesDabar += 5;

        std::cout << "[Metaliniai sarvai] +5 Gyvybes pries kova\n";
        Sleep(1000);
    }

    if (MonstroGyvybes < 1)
    {
        MonstroGyvybes = 1;
    }

    if (MonstroAtaka < 1)
    {
        MonstroAtaka = 1;
    }

    Monstriukas = true;

    Karas();
}

void Inventorius()
{
    system("cls");

InventoriausPradzia:

    std::cout << "Jusu turima gelezis: " << ITEMGelezis << std::endl;
    std::cout << "Jusu turimos lipnios juostos: " << ITEMlipniJuosta << std::endl;
    std::cout << "Jusu turimi strypai: " << ITEMstrypas << std::endl;
    std::cout << "Jusu turima viela: " << ITEMviela << std::endl;
    std::cout << "      " << std::endl;
    std::cout << "      " << std::endl;

    if (ITEMsuvirinimas == true || ITEMlipniJuosta == true)
    {
        std::cout << "Jau galite sutvirtinti asmenini inventoriu suvirinimu" << std::endl;
        Sleep(1500);
    }
    else if (ITEMsuvirinimas == false && ITEMlipniJuosta == false)
    {
        std::cout << "Apieskok aplinka ir surask inventoriu su kuriuo gali GAMINTI ir SUTVIRTINTI" << std::endl;
        Sleep(4000);
        system("cls");
        PagrindinisMeniu();
        return;
    }

    std::cout << "\n=== PAGAMINTI DAIKTAI ===\n";

    if (ITMviela)
        std::cout << "[TURITE] Lazda su viela (+ pries Skeleta ir Zombi)\n";

    if (ITMlazda)
        std::cout << "[TURITE] Zaisliukas is lazdos (+ pries Pikta suni)\n";

    if (ITMsalmas)
        std::cout << "[TURITE] Metalinis salmas (+ pries Zombi ir Vampyra)\n";

    if (ITMsarvai)
        std::cout << "[TURITE] Metaliniai sarvai (+ pries Mineda)\n";

    std::cout << "\n";

    if (ITMviela == false)
    {
        std::cout << "Spausti 1 norint pagaminti lazda su viela (+ ataka pries negyvelius) (1 vnt. Strypas | 2 vnt. viela)" << std::endl;
    }

    if (ITMlazda == false)
    {
        std::cout << "Spausti 2 norint pagaminti zaisliuka is lazdos (+ nukeksminamas piktas suo) (1 vnt. Strypas)" << std::endl;
    }

    if (ITMsalmas == false)
    {
        std::cout << "Spausti 3 norint pagaminti metalini salma ir kaklo apsauga (+ gyvybes, - ataka zombio bei vampyro) (1 vnt. viela | 1 vnt. gelezis)" << std::endl;
    }

    if (ITMsarvai == false)
    {
        std::cout << "Spausti 4 norint pagaminti pilno kuno sarvus (+ imunitetas nuo Minedo, + gyvybes, - energija) (2 vnt. gelezis | 2 vnt. viela)" << std::endl;
    }

    std::cout << "Norint gristi i pagrindini meniu spausti 5" << std::endl;

    std::cin >> a;

    if (a == 1 && ITMviela == false)
    {
        if (ITEMstrypas >= 1 && ITEMviela >= 2)
        {
            std::cout << "Pagaminote lazda su viela!" << std::endl;

            ITEMstrypas -= 1;
            ITEMviela -= 2;

            ITMviela = true;

            GyvybesDabar += 30;

            std::cout << "+30 gyvybiu!" << std::endl;
            Sleep(1500);
        }
        else
        {
            std::cout << "Nepakanka resursu!" << std::endl;
            Sleep(1500);
            goto InventoriausPradzia;
        }
    }
    else if (a == 2 && ITMlazda == false)
    {
        if (ITEMstrypas >= 1)
        {
            std::cout << "Lazda pagaminta!" << std::endl;

            ITEMstrypas -= 1;

            ITMlazda = true;

            GyvybesDabar += 20;

            std::cout << "+20 gyvybiu!" << std::endl;
            Sleep(1500);
        }
        else
        {
            std::cout << "Nepakanka resursu!" << std::endl;
            Sleep(1500);
            goto InventoriausPradzia;
        }
    }
    else if (a == 3 && ITMsalmas == false)
    {
        if (ITEMviela >= 1 && ITEMGelezis >= 1)
        {
            std::cout << "Makaules apsauga pagaminta!" << std::endl;

            ITEMviela -= 1;
            ITEMGelezis -= 1;

            ITMsalmas = true;

            GyvybesDabar += 40;

            std::cout << "+40 gyvybiu!" << std::endl;
            Sleep(1500);
        }
        else
        {
            std::cout << "Nepakanka resursu!" << std::endl;
            Sleep(1500);
            goto InventoriausPradzia;
        }
    }
    else if (a == 4 && ITMsarvai == false)
    {
        if (ITEMviela >= 2 && ITEMGelezis >= 2)
        {
            std::cout << "Metaliniai sarvai pagaminti, Minedo nagams nera tikimybes praeiti pro juos!" << std::endl;

            ITEMviela -= 2;
            ITEMGelezis -= 2;

            ITMsarvai = true;

            GyvybesDabar += 50;
            Energija -= 10;

            std::cout << "+50 gyvybiu!" << std::endl;
            std::cout << "-10 energijos!" << std::endl;

            Sleep(1500);
        }
        else
        {
            std::cout << "Nepakanka resursu!" << std::endl;
            Sleep(1500);
            goto InventoriausPradzia;
        }
    }
    else if (a == 5)
    {
        PagrindinisMeniu();
        return;
    }
    else
    {
        system("cls");
        goto InventoriausPradzia;
    }

    Sleep(1000);
    system("cls");
    goto InventoriausPradzia;
}
/*
void Inventorius()
{


    system("cls");
InventoriausPradzia:
    std::cout << "Jusu turima gelezis: " << ITEMGelezis << std::endl;
    std::cout << "Jusu turimos lipnios juostos: " << ITEMlipniJuosta << std::endl;
    std::cout << "Jusu turimi strypai: " << ITEMstrypas << std::endl;
    std::cout << "Jusu turima viela: " << ITEMviela << std::endl;
    std::cout << "      " << std::endl;
    std::cout << "      " << std::endl;
    if (ITEMsuvirinimas == true || ITEMlipniJuosta == true)
    {
        std::cout << " Jau galite sutvirtinti asmenini inventoriu suvirinimu " << std::endl;
        Sleep(1500);
    }
    else if (ITEMsuvirinimas == false && ITEMlipniJuosta == false)
    {
        std::cout << " Apieskok aplinka ir surask inventoriu su kuriuo gali GAMINTI ir SUTVIRTINTI " << std::endl;
        Sleep(4000);
        system("cls");
        PagrindinisMeniu();
    }

    if (ITMviela == false)
    {
        std::cout << "Spausti 1 norint pagaminti lazda su viela(+ ataka pries negyvelius) (1 vnt. Strypas | 2 vnt. viela )" << std::endl; //+ ataka pries Skeletas, zombis
    }
    if (ITMlazda == false)
    {
        std::cout << "Spausti 2 norint pagaminti zaisliuka is lazdos(+ nukeksminamas piktas suo) (1 vnt. Strypas)" << std::endl;
    }
    if (ITMsalmas == false)
    {
        std::cout << "Spausti 3 norint pagaminti metalini salma ir kaklo apsauga(+ gyvybes,- ataka zombio bei vampyro ) (1 vnt. viela | 1 vnt. gelezis)" << std::endl;
    }
    if (ITMsarvai == false)
    {
        std::cout << "Spausti 4 norint pagaminti pilno kuno sarvus(+ imunitetas nuo Minedo, + gyvybes, - energija) (2 vnt. gelezis | 2 vnt. viela )" << std::endl;
    }
    std::cout << "Norint gristi i pagrindini meniu spausti 5 " << std::endl;

    std::cin >> a;
    if (a == 1 && ITMviela == false)
    {
        if (ITEMstrypas >= 1 && ITEMviela >= 2) //Lazda su viela
        {
            std::cout << "Pagaminote lazda su viela!" << std::endl;
            ITEMstrypas -= 1;
            ITEMviela -= 2;
            ITMviela = true;
            GyvybesDabar += 30;
        }
        else
        {
            goto InventoriausPradzia;
        }

    }
    else if (a == 2 && ITMlazda == false) // lazda
    {
        if (ITEMstrypas >= 1)
        {
            std::cout << "Lazda pagaminta " << std::endl;
            ITEMstrypas -= 1;
            ITMlazda = true;
            GyvybesDabar += 20;
        }
        else
        {
            goto InventoriausPradzia;
        }

    }
    else if (a == 3 && ITMsalmas == false) //Salmas
    {

        if (ITEMviela >= 1 && ITEMGelezis >= 1)
        {
            std::cout << "Makaules apsauga pagaminta" << std::endl;
            ITEMstrypas -= 1;
            ITEMGelezis -= 1;
            ITMsalmas = true;
            GyvybesDabar += 40;
        }
        else
        {
            goto InventoriausPradzia;
        }

    }
    else if (a == 4 && ITMsarvai == false) //sarvai nuo Minedo ir kitu monstru
    {
        if (ITEMviela >= 2 && ITEMGelezis >= 2)
        {
            std::cout << "Metaliniai sarvai pagaminti, Minedo nagams nera tikimybes praeiti pro juos" << std::endl;
            ITEMstrypas -= 2;
            ITEMGelezis -= 2;
            ITMsarvai = true;
            GyvybesDabar += 50;
        }
        else
        {
            goto InventoriausPradzia;
        }
    }
    else if (a == 5)
    {
        PagrindinisMeniu();
    }
    else
    {
        system("cls");
        goto InventoriausPradzia;
    }
    Sleep(1000);
    system("cls");
    goto InventoriausPradzia;

}
*/

void Save()
{
    int lauk;

    std::cout << "Iveskite issaugojimo numeri (1,2,3...): ";
    std::cin >> lauk;

    std::string failas = std::to_string(lauk) + ".json";

    std::ofstream file(failas.c_str());

    if (!file.is_open())
    {
        std::cout << "Nepavyko issaugoti!" << std::endl;
        Sleep(2000);
        PagrindinisMeniu();
        return;
    }

    file << Gyvybes << std::endl;
    file << GyvybesDabar << std::endl;
    file << GyvybesMax << std::endl;
    file << Alkis << std::endl;
    file << Energija << std::endl;
    file << Patirtis << std::endl;
    file << Lygis << std::endl;
    file << KitasPatirtiesLygis << std::endl;
    file << IsgyventaDienu << std::endl;
    file << Pasaulis << std::endl;

    file << ITEMviela << std::endl;
    file << ITEMGelezis << std::endl;
    file << ITEMstrypas << std::endl;
    file << ITEMlipniJuosta << std::endl;

    file << ITEMsuvirinimas << std::endl;
    file << ITMviela << std::endl;
    file << ITMlazda << std::endl;
    file << ITMsalmas << std::endl;
    file << ITMsarvai << std::endl;

    file << kawaii << std::endl;
    file << veikPasirinkimas << std::endl;
    file << MaistoApdorojimas << std::endl;
    file << MaistoPaieska << std::endl;
    file << Miegutis << std::endl;
    file << Monstriukas << std::endl;
    file << AplinkosApieska << std::endl;

    file << ZaidejoKlase << std::endl;

    
    file.flush();// Priverstinai iraso visus duomenis i faila

    
    file.close();// Uzdaro faila

    std::cout << "Issaugota i faila: " << failas << std::endl;
    Sleep(2000);

	PagrindinisMeniu();
}


void Load()
{
    int lauk;

    std::cout << "Iveskite skaiciu kad uzkrauti pasauli: ";
    std::cin >> lauk;

    std::string failas = std::to_string(lauk) + ".json";

    std::ifstream file(failas);

    if (!file.is_open())
    {
        std::cout << "Tokio pasaulio nera!" << std::endl;
        Sleep(2000);
        return;
    }

    file >> Gyvybes;
    file >> GyvybesDabar;
    file >> GyvybesMax;
    file >> Alkis;
    file >> Energija;
    file >> Patirtis;
    file >> Lygis;
    file >> KitasPatirtiesLygis;
    file >> IsgyventaDienu;
    file >> Pasaulis;

    file >> ITEMviela;
    file >> ITEMGelezis;
    file >> ITEMstrypas;
    file >> ITEMlipniJuosta;

    file >> ITEMsuvirinimas;
    file >> ITMviela;
    file >> ITMlazda;
    file >> ITMsalmas;
    file >> ITMsarvai;

    file >> kawaii;
    file >> veikPasirinkimas;
    file >> MaistoApdorojimas;
    file >> MaistoPaieska;
    file >> Miegutis;
    file >> Monstriukas;
    file >> AplinkosApieska;

    file.ignore();
    std::getline(file, ZaidejoKlase);

    file.close();

    std::cout << "Sekmingai uzkrautas pasaulis: " << lauk << std::endl;
    Sleep(2000);
	PagrindinisMeniu();
}


std::string GetDateTime()
{
    time_t now = time(0);

    tm localTime;
    localtime_s(&localTime, &now);

    char buffer[100];

    strftime(buffer, sizeof(buffer),
        "%Y-%m-%d %H:%M:%S",
        &localTime);

    return buffer;

}

//Funkcijos skirtos tik skaiciuoti highscore
void Highscore()
{
    system("cls");

    std::ifstream file("highscores.json");

    if (!file.is_open())
    {
        std::cout << "Highscore failas tuscias!\n";
        system("pause");
        return;
    }

    std::cout << "=================== HIGHSCORE ===================\n\n";

    std::cout << "Kaip skaiciuojami taskai:\n\n";
    std::cout << "Tasku formule:\n\n";

    std::cout << "Score = Gyvybes + Alkis + Energija + Patirtis*2 + Lygis*20\n\n";

    std::cout << "Paaiskinimas:\n";
    std::cout << "- Gyvybes pridedamos 1:1\n";
    std::cout << "- Alkis pridedamas 1:1\n";
    std::cout << "- Energija pridedama 1:1\n";
    std::cout << "- Patirtis (XP) verta dvigubai: *2\n";
    std::cout << "- Kiekvienas lygis vertas net 20 tasku\n\n";


    std::cout << "=================== TOP 100 GAMES ===================\n\n";

    int vieta = 1;

    HighscoreEntry h;

    while (file >> h.score
        >> h.Gyvybes
        >> h.Alkis
        >> h.Energija
        >> h.Patirtis
        >> h.Lygis)
    {
        file.ignore();
        std::getline(file, h.date);
        std::cout << vieta << ". "
            << h.date << "\n";
        std::cout << "Score: "
            << h.score << "\n";
        std::cout << "Gyvybes{ "
            << h.Gyvybes << " } ";
        std::cout << "Alkis{ "
            << h.Alkis << " } ";
        std::cout << "Energija{ "
            << h.Energija << " } ";
        std::cout << "Patirtis{ "
            << h.Patirtis << " } ";
        std::cout << "Lygis{ "
            << h.Lygis << " }\n\n";

        vieta++;
    }

    file.close();

    system("pause");
    return;
}
int SkaiciuotiTaskus()
{
    return GyvybesDabar
        + Alkis
        + Energija
        + Patirtis * 2
        + Lygis * 20;

}
void AddHighscore()
{
    std::vector<HighscoreEntry> scores;

    std::ifstream file("highscores.json");

    if (file.is_open())
    {
        HighscoreEntry h;

        while (true)
        {
            file >> h.score;
            file >> h.Gyvybes;
            file >> h.Alkis;
            file >> h.Energija;
            file >> h.Patirtis;
            file >> h.Lygis;

            if (file.fail())
            {
                break;
            }

            file.ignore();

            std::getline(file, h.date);

            scores.push_back(h);
        }

        file.close();
    }

    HighscoreEntry naujas;

    naujas.score = SkaiciuotiTaskus();

    naujas.Gyvybes = GyvybesDabar;

    naujas.Alkis = Alkis;

    naujas.Energija = Energija;

    naujas.Patirtis = Patirtis;

    naujas.Lygis = Lygis;

    naujas.date = GetDateTime();

    scores.push_back(naujas);

    std::sort(
        scores.begin(),
        scores.end(),

        [](HighscoreEntry pirmas, HighscoreEntry antras)
        {
            if (pirmas.score > antras.score)
            {
                return true;
            }

            return false;
        }
    );

    if (scores.size() > 100)
    {
        scores.resize(100);
    }

    std::ofstream out("highscores.json");

    if (out.is_open())
    {
        for (int i = 0; i < scores.size(); i++)
        {
            out << scores[i].score;
            out << " ";

            out << scores[i].Gyvybes;
            out << " ";

            out << scores[i].Alkis;
            out << " ";

            out << scores[i].Energija;
            out << " ";

            out << scores[i].Patirtis;
            out << " ";

            out << scores[i].Lygis;
            out << "\n";

            out << scores[i].date;
            out << "\n";
        }

        out.close();
    }
    EksportuotiHTML();
}

void EksportuotiHTML()
{
    std::ofstream css("highscores.css");

    if (css.is_open())
    {
        css << "@import url('https://fonts.googleapis.com/css2?family=Roboto&display=swap');";
        css << "body{background:black;color:white;font-family:'Roboto',sans-serif;}";
        css << "h1{text-align:center;}";
        css << "h2{text-align:center;}";
        css << "p{text-align:center;}";
        css << "table{width:100%;border-collapse:collapse;}";
        css << "th,td{border:1px solid white;padding:8px;text-align:center;}";
        css << "tbody tr:nth-child(1) .score{color:red;font-weight:bold;}";
        css << "tbody tr:nth-child(2) .score{color:hotpink;font-weight:bold;}";
        css << "tbody tr:nth-child(3) .score{color:violet;font-weight:bold;}";
        css.close();
    }

    std::ifstream file("highscores.json");

    if (!file.is_open())
    {
        return;
    }

    std::ofstream html("highscores.html");

    if (!html.is_open())
    {
        return;
    }

    html << "<!DOCTYPE html>";
    html << "<html>";
    html << "<head>";
    html << "<meta charset='UTF-8'>";
    html << "<title>Top 100 Rezultatu</title>";
    html << "<link rel='stylesheet' href='highscores.css'>";
    html << "</head>";
    html << "<body>";

    html << "<h1>TOP 100 REZULTATU</h1>";

    html << "<h2>Tasku formule</h2>";
    html << "<p><b>Score = Gyvybes + Alkis + Energija + Patirtis * 2 + Lygis * 20</b></p>";

    html << "<table>";

    html << "<thead>";
    html << "<tr>";
    html << "<th>Vieta</th>";
    html << "<th>Data</th>";
    html << "<th>Score</th>";
    html << "<th>Gyvybes</th>";
    html << "<th>Alkis</th>";
    html << "<th>Energija</th>";
    html << "<th>Patirtis</th>";
    html << "<th>Lygis</th>";
    html << "</tr>";
    html << "</thead>";

    html << "<tbody>";

    HighscoreEntry h;

    int vieta = 1;

    while (file >> h.score
        >> h.Gyvybes
        >> h.Alkis
        >> h.Energija
        >> h.Patirtis
        >> h.Lygis)
    {
        file.ignore();

        std::getline(file, h.date);

        html << "<tr>";

        html << "<td>" << vieta << "</td>";
        html << "<td>" << h.date << "</td>";
        html << "<td class='score'>" << h.score << "</td>";
        html << "<td>" << h.Gyvybes << "</td>";
        html << "<td>" << h.Alkis << "</td>";
        html << "<td>" << h.Energija << "</td>";
        html << "<td>" << h.Patirtis << "</td>";
        html << "<td>" << h.Lygis << "</td>";

        html << "</tr>";

        vieta++;
    }

    html << "</tbody>";
    html << "</table>";

    html << "</body>";
    html << "</html>";

    html.close();
    file.close();
}






















//Atmintine: RANDOM
//1. uniform_default
//2. random_device_uniform
//3. die_roll
//4. uniform_default_int
//5. mersenne_twister_engine_seed
//6. random_seed_mt19937_2



// Pakeisti nustatymai per visual studio
// ISO C++20 Standard (/std:c++20)
// Warning level: Level4
// Treat warnings as errors: no