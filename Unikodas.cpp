#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <string>
#include <bitset>
#include <vector>

using namespace std;

void gotoxy(int x, int y);
string decToBinary(int skc);
string utf8(string sab, string bin);
string binToHex(string bin);
string uniToUtf(string unicode);
void printUniUtf(int skaicius);
void readCP437();
void changeFileValues();
void specialSymbol(string utf);
int binToDec(string binary);

COORD coord;

vector<string> CP437;
vector<int> baitai;


void MENU(int i);
int klausimai();
int main()
{
    int skaicius;
    char p;
    int i = 0;
    while (i != 3) // Meniu infinity loop funkcija
    {
        i = klausimai();
        if (i == 1)
        {
        system("cls");
        cout << "Iveskite desimtaini skaiciu: ";
        cin >> skaicius;
        if(skaicius <= 1114111) // Nes char maximum value yra 1114111
        {
            printUniUtf(skaicius);
        }
        else
        {
            cout << "Maximali reiksme yra 1114111";
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
        cout << "Ar norite testi darba? : Y/N  ";
        cin >> p;
        if (p == 'y' || p == 'Y') {}
        else { return 0; }
        system("cls");
        }

        if (i == 2)
        {
            system("cls");
            readCP437();
            changeFileValues();
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
            cout << "Ar norite testi darba? : Y/N  ";
            cin >> p;
            if (p == 'y' || p == 'Y') {}
            else { return 0; }
            system("cls");
        }
        if (i == 3) { cout << "EXITING!"; Sleep(500); }

    }
    Sleep(10);
    return 0;
}
void MENU(int i)
{
    COORD cursorPosition;    cursorPosition.X = 0;    cursorPosition.Y = 0;    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11);
    cout << "   MENIU! Pasirinkite ka norite daryti!" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
    if (i == 1) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46); // SPALVA PARYSKINTA
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); // NEPARYSKINTA
    cout << "#1 Pirma dalis!" << endl;
    if (i == 2) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "#2 Antra dalis!" << endl;
    if (i == 3) SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 46);
    else SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
    cout << "#3 EXIT!" << endl;
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    cout << "============================================  " << endl;
}
int klausimai()
{
    int i = 0;
    bool log = true;
    MENU(i); // Spauzdina meniu teksta
    while (log)
    {
        if (GetAsyncKeyState(VK_RETURN)) log = false; // Paspaudus enter , nustoja fiksuoti klavisus ir grazina paskutine I reiksme.
        if (GetAsyncKeyState(VK_DOWN))
        {
            if (i < 3) i++; // Neleidzia pereit ribu
            cout << i;
            system("CLS");
            MENU(i);
        }
        if (GetAsyncKeyState(VK_UP))
        {
            if (i > 1) i--; // Neleidzia pereit ribu
            cout << i;
            system("CLS");
            MENU(i);
        }
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        Sleep(100);
        //system("CLS");
    }
    return i;
}


string decToBinary(int skc){
    int s = skc; // issisaugom skaiciu
    if(skc == 0){ // patikrina ar skaicius nera 0
        return "0";
    }
    // PVZ: (4) 1 0 0 (1/2=Nelyginis)=1 (2/2=Lyginis)=0 (4/2=Lyginis)=0
    string binary = ""; // skaicius bus irasytas i string
    while(s != 0){
        if(s % 2 == 0){ // jeigu lyginis
             binary = "0" + binary;
        }else{ // jeigu nelyginis
            binary = "1" + binary;
        }
        s = s / 2; // Padaugina is dvieju
    }

    return binary;
}

string utf8(string sab, string bin){

        string sablonas = sab;
        string binary = bin;
        int j = binary.size()-1;
        for(int i = sablonas.size()-1; i >= 0; i--){
            if(sablonas[i] == 'x' && j >= 0){ // Jeigu sablone gale yra X ir J>=0
               sablonas[i] = binary[j]; // Irasoma i sablona binary skaicius.
               j--;
            }
        }
        for(int i = 0; i < sablonas.size(); i++){
            if(sablonas[i] == 'x'){ // Tuomet visus X pakeiciame y 0
                sablonas[i] = '0';
            }
        }

        return binToHex(sablonas); // Grazinamas sablonas pilnai sutvarkytas
}
void changeFileValues(){

    ifstream fin("386intel.txt");
    ofstream fout("output.txt");

    if(fin.fail()){
        cout << "Nepavyko atidaryti 386intel.txt failo..." << endl;
    }else{
        string eilute;
        string naujaEilute;
        int desimtaineVerte;
        cout << "Failas perrasomas!" << endl;

        while( !fin.eof() ){ // Tol kol yra ka skaityti
            naujaEilute = "";
            getline(fin, eilute); // Nuskaitoma eilute
            for(int i = 0; i < eilute.size(); i++){ // Einama per visus eilutes elementus
                bitset<8> dvejetaineVerte(eilute[i]); // Paverciama i dvejetaini skaiciu
                desimtaineVerte = binToDec(dvejetaineVerte.to_string()); // Bin to desimtaine verte convertorius
                if(desimtaineVerte >= 128 || desimtaineVerte <= 31){ // ASCII lentele iki 127 arba Nuo 31 prasideda nauji simboliai
                    string unicode = CP437.at(desimtaineVerte); // Unicode = desimtaineverte CP437 faile
                    string utf = uniToUtf(unicode); // UTF= Unicode to UTF
                    specialSymbol(utf); // Kreipiamasi i special simboliu funkcija
                    for(int j = 0; j < baitai.size(); j++){
                         naujaEilute += (char)baitai.at(j); // Nauja eilute surasoma po viena baita.
                    }
                }else{
                    naujaEilute += eilute[i]; // Jeigu nebuvo specialiu simboliu islieka ta pati eilute
                }
            }
            fout << naujaEilute << endl;
        }

        fin.close();
        fout.close();
        cout << "Failas sekmingai perrasytas!" << endl;

    }

}
void specialSymbol(string utf){ // E29590
    char byte[2];
    int j = 0;
    int baitas;
    baitai.clear();

    for(int i = 0; i < utf.size(); i++){
        byte[j] = utf[i];
        j++;
        if(j == 2){
            j = 0;
            baitas = 0;
            if(byte[0] >= 48 && byte[0] <= 57){
                // skaicius
                baitas += (byte[0] - '0')*16;
            }else if(byte[0] >= 65 && byte[0] <= 70){
                // raide
                baitas += (byte[0] - 'A' + 10)*16;
            }

            if(byte[1] >= 48 && byte[1] <= 57){
                // skaicius
                baitas += byte[1] - '0';
            }else if(byte[1] >= 65 && byte[1] <= 70){
                // raide
                baitas += (byte[1] - 'A' + 10);
            }
                baitai.push_back(baitas);
        }
    }

}

void printUniUtf(int skaicius){ // PIRMA DALIS

    string binary = decToBinary(skaicius);
    string uni = binToHex(binary);
    cout << "UNICODE: U+";
    switch(uni.size()){ // Pridedami 0 jeigu truksta iki 4skaitmenu
    case 1:
         cout << "000" << uni << endl;
        break;
    case 2:
        cout << "00" << uni << endl;
        break;
    case 3:
        cout << "0" << uni << endl;
        break;
    default:
        cout << uni << endl;
        break;
    }

    string utf = "";
    if( ((uni.size() == 2) && (uni.at(1) <= 'F' && uni.at(0) <= '7')) || uni.size() == 1){
        // 1 Bytas
        string sablonas = "0xxxxxxx"; // 8
        cout << "Naudojamas pirmas sablonas";
        cout << uni << endl;
        utf = utf8(sablonas, binary);

    }else if( ( (uni.size() == 3) && uni.at(0) <= '7' && uni.at(1) <= 'F' && uni.at(2) <= 'F' ) || ( (uni.size() == 2) && (uni.at(0) >= '8') && (uni.at(1) >= '0') ) ){
        // 2 Bytai
        string sablonas = "110xxxxx10xxxxxx"; // 16
        cout << "Naudojamas 2 sablonas";
        utf = utf8(sablonas, binary);


    }else if( ( (uni.size() == 3) && uni.at(0) >= '8'  ) || (uni.size() == 4)  ){
        // 3 Bytai
        string sablonas = "1110xxxx10xxxxxx10xxxxxx"; // 24
        cout << "Naudojamas 3 sablonas";
        utf = utf8(sablonas, binary);

    }else{
        // 4 Bytai
        string sablonas = "11110xxx10xxxxxx10xxxxxx10xxxxxx"; // 32
        cout << "Naudojamas 4 sablonas";
        utf = utf8(sablonas, binary);
    }

    cout << "UTF-8: ";
    for(int i = 0; i < utf.size(); i++){
        cout << utf[i];
        if(i % 2 == 1){
            cout << " ";
        }
    }

    cout << "\nCHAR: " << (char)skaicius << endl;
}

string uniToUtf(string unicode){

    string uni = unicode;
    string bin = "";
    string utf;

    for(int i = 0; i < unicode.size(); i++){
        switch(unicode[i]){ // Paprastas pavertimas i UTF

        case '0':
            bin += "0000";
            break;
        case '1':
            bin += "0001";
            break;
        case '2':
            bin += "0010";
            break;
        case '3':
            bin += "0011";
            break;
        case '4':
            bin += "0100";
            break;
        case '5':
            bin += "0101";
            break;
        case '6':
            bin += "0110";
            break;
        case '7':
            bin += "0111";
            break;
        case '8':
            bin += "1000";
            break;
        case '9':
            bin += "1001";
            break;
        case 'A':
            bin += "1010";
            break;
        case 'B':
            bin += "1011";
            break;
        case 'C':
            bin += "1100";
            break;
        case 'D':
            bin += "1101";
            break;
        case 'E':
            bin += "1110";
            break;
        case 'F':
            bin += "1111";
            break;

        }
    }

    if( ((uni.size() == 2) && (uni.at(1) <= 'F' && uni.at(0) <= '7')) || uni.size() == 1){
        // 1 Bytas
        string sablonas = "0xxxxxxx";
        utf = utf8(sablonas, bin);

    }else if( ( (uni.size() == 3) && uni.at(0) <= '7' && uni.at(1) <= 'F' && uni.at(2) <= 'F' ) || ( (uni.size() == 2) && (uni.at(0) >= '8') && (uni.at(1) >= '0') ) ){
        // 2 Bytas
        string sablonas = "110xxxxx10xxxxxx";
        utf = utf8(sablonas, bin);


    }else if( ( (uni.size() == 3) && uni.at(0) >= '8'  ) || (uni.size() == 4)  ){
        // 3 Bytas
        string sablonas = "1110xxxx10xxxxxx10xxxxxx";
        utf = utf8(sablonas, bin);

    }else{
        // 4 Bytas
        string sablonas = "11110xxx10xxxxxx10xxxxxx10xxxxxx";
        utf = utf8(sablonas, bin);
    }

    return utf;

}

int binToDec(string binary){ // Pavertimas i paprasta skaiciu

    string bin = binary;
    int desimtaine = 0;
    int laipsnis = 1;

    for (int i = bin.size()-1; i >= 0; i--) {
        if (bin[i] == '1') // PVZ: 1010 -> 0-nieko 1-(+2) 0-nieko 1-(+8) = 10
            desimtaine += laipsnis;
        laipsnis = laipsnis * 2;
    }

    return desimtaine;
}

string binToHex(string bin){
    // Pvz: paduoda 100 , kas yra = 4
    string binary = bin;
    char ats[10];
    int z = binary.size()-1; // Ilgis - 1 (PVZ: 3-1)
    int index = 0;
    int temp = 0;
    while(z>=0){
        for(int y = 0; y < 4&&z>=0;y++)
        { // Tol kol Y yra maziau uz 4irZ ilgi
            temp += (binary[z--]-48)*pow(2,y);
        }
        if(temp > 9)
        {
            // Raide +55
            ats[index++] = (char)(temp+55);
        }else{
            // Skaicius +48
            ats[index++] = (char)(temp+48);
        }
        temp = 0;
    }
    ats[index] = '\0';
    strrev(ats); // Apversti stringa.

    string hex(ats); // Hex=ats
    return hex;
}

void readCP437(){
    ifstream cp437Read("CP437.txt");
     if(cp437Read.fail()){
        cout << "Nepavyko atidaryti CP437.txt failo..." << endl;
    }
    else{
    string unicode;
    while(!cp437Read.eof()){
        getline(cp437Read, unicode);
        CP437.push_back(unicode);
    }
    cp437Read.close();
    }

}


