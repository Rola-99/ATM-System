#pragma warning(disable : 4996)
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cctype>
using namespace std;

struct sClient {
    string AccountNum;
    string PinCode;
    string Name;
    string Phone;
    double AccBalance;
    bool MarkForDelete = false;
};

enum enATMMenue { QuickWithdraw = 1, NormalWithdraw = 2, Deposit = 3, CheckBalance = 4, LogOut = 5 };
const string sClientsFileName = "Clients.txt";
sClient CurrentClient;

void ShowATMMainMenu();
void LogInATMsystem();
void QuickWithdrawScreen();
void NormalWithdrawScreen();
void DepositScreen();

vector <string> SpiltString(string Text, string Delim) {
    vector <string> Tokens;

    short pos = 0;
    string sWord;

    while ((pos = Text.find(Delim)) != std::string::npos) {
        sWord = Text.substr(0, pos);
        if (sWord != "") {
            Tokens.push_back(sWord);
        }
        Text.erase(0, pos + Delim.length());
    }
    if (Text != "") {
        Tokens.push_back(Text);
    }
    return Tokens;
}

sClient ConvertClientLineToRecord(string LineRecord, string seperatr = "#//#") {

    sClient Data;
    vector <string> vClientsData;

    vClientsData = SpiltString(LineRecord, seperatr);

    Data.AccountNum = vClientsData[0];
    Data.PinCode = vClientsData[1];
    Data.Name = vClientsData[2];
    Data.Phone = vClientsData[3];
    Data.AccBalance = stod(vClientsData[4]);

    return Data;

}

string ConvertRecordToLine(sClient Person1, string Seperator = "#//#") {

    string stNewLineAllData = "";

    stNewLineAllData += Person1.AccountNum + Seperator;
    stNewLineAllData += Person1.PinCode + Seperator;
    stNewLineAllData += Person1.Name + Seperator;
    stNewLineAllData += Person1.Phone + Seperator;
    stNewLineAllData += to_string(Person1.AccBalance);
    return stNewLineAllData;
}

vector <sClient> LoadClientDataFromFile(string FileName) {

    vector < sClient> vClients;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line)) {

            Client = ConvertClientLineToRecord(Line);
            vClients.push_back(Client);

        }
        MyFile.close();
    }
    return vClients;
}

vector <sClient> SaveClientsDataToFile(string FileName, vector <sClient> vClients) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;
    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
    return vClients;
}

bool DepositBalanceToClientByAccountNum(string AccountNumber, double NewBalance, vector<sClient>& vClients)
{
    char Sure = 'y';

    cout << "Are you sure you want perform this transaction?";
    cin >> Sure;

    if (Sure == 'y' || Sure == 'Y')
    {
        for (sClient& C : vClients)
        {
            if (C.AccountNum == AccountNumber)
            {
                C.AccBalance += NewBalance;

                SaveClientsDataToFile(sClientsFileName, vClients);

                cout << "Done Successfully. Balance Updated. Your Account: "
                    << C.AccBalance << endl;

                return true;
            }
        }
    }

    return false;
}

bool FindClientByAccountNum(string AccountNumber, vector <sClient> vClients, sClient& Client1) {
    vClients = LoadClientDataFromFile(sClientsFileName);

    for (sClient c : vClients) {
        if (c.AccountNum == AccountNumber) {
            Client1 = c;
            return true;
        }
    }
    return false;
}

// LogOut
void LogOut1() {
    LogInATMsystem();
}
// // ///////////////////////////////////////////////////////////////////////////////////////
// Go Back to ATM Main Menue
void GoBackToATMmainMenue() {
    cout << "Press any Key to back The Main Menue...";
    system("pause>0");
    ShowATMMainMenu();
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Quick Withdraw
string ReadChooseQuick() {
    string choose;
    cout << "Choose what to withdraw from [1 to 8] ? ";
    cin >> choose;
    return choose;
}
int GetQuickWithDrawAmount(string ChoiceNum) {
    int Balance = 0;
    if (ChoiceNum == "1") {
        Balance += 20;
    }
    else if (ChoiceNum == "2") {
        Balance += 50;
    }
    else if (ChoiceNum == "3") {
        Balance += 100;
    }
    else if (ChoiceNum == "4") {
        Balance += 200;
    }
    else if (ChoiceNum == "5") {
        Balance += 400;
    }
    else if (ChoiceNum == "6") {
        Balance += 600;
    }
    else if (ChoiceNum == "7") {
        Balance += 800;
    }
    else if (ChoiceNum == "8") {
        Balance += 1000;
    }
    else
    {
        ShowATMMainMenu();
    }
    return Balance;
}

void QuickWithdrawScreen() {
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
   
    cout << "---------------------------------------------\n";
    cout << "\t\t Quick WithDraw \n";
    cout << "---------------------------------------------\n";
    cout << "[1] 20" << "      " << "[2] 50" << endl;
    cout << "[3] 100" << "     " << "[4] 200" << endl;
    cout << "[5] 400" << "     " << "[6] 600" << endl;
    cout << "[7] 800" << "     " << "[8] 1000" << endl;
    cout << "[9] Exit" << endl;

   
   
    cout << "Your balance is " << CurrentClient.AccBalance << endl;

    int WithDrawBalance = GetQuickWithDrawAmount(ReadChooseQuick());
    if (WithDrawBalance > CurrentClient.AccBalance) {
        cout << "The Amount exceeds Your balance, make another choice.\n";
        system("pause>0");
        NormalWithdrawScreen();
        return;
    }
    DepositBalanceToClientByAccountNum(CurrentClient.AccountNum, WithDrawBalance * -1, vClients);
    CurrentClient.AccBalance -= WithDrawBalance;
}

/////////////////////////////////////////////////////////////////////////////////////////
// Normal Withdraw
int ReadNormalAmount() {
    double NAmount;
    cout << "\nEnter an amount multiple of 5's ?";
    cin >> NAmount;
    return NAmount;
}
int CheckAmountNum(int Number) {
    
    if (Number % 5 == 0) {
        return Number;
    }
    else
    {
        return ReadNormalAmount();
    }
}

void NormalWithdrawScreen() {
    cout << "---------------------------------------------\n";
    cout << "\t\tWithDraw Screen\n";
    cout << "---------------------------------------------\n";
    cout << "Your balance is " << CurrentClient.AccBalance << endl;
    int WithDrawBalance = CheckAmountNum(ReadNormalAmount());

    if (WithDrawBalance > CurrentClient.AccBalance) {
        cout << "The Amount exceeds Your balance, make another choice.\n";
        system("pause>0");
        NormalWithdrawScreen();
        return;
    }
    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
  

    DepositBalanceToClientByAccountNum(CurrentClient.AccountNum, WithDrawBalance * -1, vClients);
    CurrentClient.AccBalance -= WithDrawBalance;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Deposit 
double ReadDepositAmount() {
    double DAmount;
    cout << "\nEnter a positive Deposit Amount?";
    cin >> DAmount;
    while (DAmount <= 0)
    {
        cout << "\nEnter a positive Deposit Amount?";
        cin >> DAmount;
    }
    return DAmount;
}
void DepositScreen() {
    cout << "---------------------------------------------\n";
    cout << "\t\tDeposit Screen\n";
    cout << "---------------------------------------------\n";

    vector <sClient> vClients = LoadClientDataFromFile(sClientsFileName);
    double NewBalance3 = ReadDepositAmount();
    
    DepositBalanceToClientByAccountNum(CurrentClient.AccountNum, NewBalance3, vClients);
    CurrentClient.AccBalance += NewBalance3;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Check Balance
void CheckBalance1() {
    cout << "---------------------------------------------\n";
    cout << "\t\t Check Balance Screen\n";
    cout << "---------------------------------------------\n";
    
    cout << "Your balance is " << CurrentClient.AccBalance << "\n\n";
    
}
////////////////////////////////////////////////////////////////////////////////////////////

int ReadATMMainMenueOption() {
    short Choose;
    cout << "Choose What do you want to do? [1 to 5]?";
    cin >> Choose;
    return Choose;
}
void PerformATMMainMenueOption(enATMMenue ATMmenue) {
    switch (ATMmenue) {
    case enATMMenue::QuickWithdraw:
        system("cls");
        QuickWithdrawScreen();
        GoBackToATMmainMenue();

    case enATMMenue::NormalWithdraw:
        system("cls");
        NormalWithdrawScreen();
        GoBackToATMmainMenue();

    case enATMMenue::Deposit:
        system("cls");
        DepositScreen();
        GoBackToATMmainMenue();

    case enATMMenue::CheckBalance:
        system("cls");
        CheckBalance1();
        GoBackToATMmainMenue();
    case enATMMenue::LogOut:
        system("cls");
        LogOut1();
    }
}
void ShowATMMainMenu() {

    system("cls");
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\tATM Main Menue Screen\n";
    cout << "\t\t\t\t--------------------------------------------------\n";
    cout << "\t\t\t\t\t\t[1] Quick Withdraw.\n";
    cout << "\t\t\t\t\t\t[2] Normal WithDraw.\n";
    cout << "\t\t\t\t\t\t[3] Deposit.\n";
    cout << "\t\t\t\t\t\t[4] Check Balance.\n";
    cout << "\t\t\t\t\t\t[5] Logout.\n";
    
    cout << "\t\t\t\t--------------------------------------------------\n";

    PerformATMMainMenueOption((enATMMenue)ReadATMMainMenueOption());
}
/////////////////////////////////////////////////////////////////////////////////////////////
// LogInATMsystem
string ReadAccountNumber() {
    string AccountNum;
    cout << "Enter Account Number: ";
    getline(cin >> ws, AccountNum);
    return AccountNum;
}
string ReadPinCod() {
    string PinCode;
    cout << "Enter Pin Code: ";
    getline(cin >> ws, PinCode);
    return PinCode;
}

bool FindAccountNumAndPinCode(string AccountNum, string PinCode, sClient& Client1) {
    vector <sClient> vClient = LoadClientDataFromFile(sClientsFileName);

    for (sClient C : vClient) {
        if (C.AccountNum == AccountNum && C.PinCode == PinCode) {
            Client1 = C;
            return true;
        }
    }
    return false;
}
bool LoadClientInfo(string AccountNum, string PinCode) {
    if (FindAccountNumAndPinCode(AccountNum, PinCode, CurrentClient)) {
        return true;
    }
    return false;
}
void LogInATMsystem() {
    bool loginATMFail = false;
    
    string ReadPinC;

    do {
        system("cls");
        cout << "\n------------------------------------------\n";
        cout << "\t\t\tLogin Screen\n";
        cout << "------------------------------------------\n";

        CurrentClient.AccountNum = ReadAccountNumber();
        CurrentClient.PinCode = ReadPinCod();
        

        if (!loginATMFail) {
            cout << "Invalidate Account Number/ PinCode.";
        }
        loginATMFail = !LoadClientInfo(CurrentClient.AccountNum, CurrentClient.PinCode);


    } while (loginATMFail);
    ShowATMMainMenu();
}


int main() {

    srand((unsigned)time(NULL));

    LogInATMsystem();

    system("pause>0");
    return 0;
}