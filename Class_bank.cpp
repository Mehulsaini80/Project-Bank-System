#include<iostream>
#include<cstdlib> 
#include<ctime> 
#include<fstream>
using namespace std;

class bank {
private:
    double acc_no;
    char name[20];
    int age;
    double balance;
    double password;

public:

    void list();
    void input();
    void generateAccountNumber();
    friend ostream& operator<<(ostream&, bank&);
    friend istream& operator>>(istream&, bank&);
    void save(bank obj);
    void showDataFromFile();
    void heading();
    void check_balance();
    void balance_heading();
    void delete_account(double accountNo);
    void modify_account(double accountNo, double password);
    void withdraw_cash(double accountNo, double password, double amount);
    void Deposit_cash(double accountNo, double password, double amount);
    bank() {
        generateAccountNumber();
    }

};

void bank::generateAccountNumber() {
    srand(time(0));
    acc_no = rand();
}



void bank::list() {
    cout<<"RBI BANK OPTIONS ARE: "<<endl;
    cout<<"PRESS 1 TO CREATE NEW ACCOUNT"<<endl;
    cout<<"PRESS 2 TO CHECK ACCOUNT BALANCE"<<endl;
    cout<<"PRESS 3 TO DEPOSIT CASH"<<endl;
    cout<<"PRESS 4 TO WITHDRAW CASH "<<endl;
    cout<<"PRESS 5 TO MODIFY ACCOUNT DETAILS "<<endl;
    cout<<"PRESS 6 TO DELETE ACCOUNT"<<endl;
    cout<<"PRESS 7 TO CHECK ACCOUNT DETAILS (FOR ADMIN USE ONLY)"<<endl;
    cout<<"PRESS 8 TO EXIT "<<endl;
    cout<<endl;
}



void bank::input() {
    cout<< "Enter name: ";
    cin.ignore();
    cin.getline(name, sizeof(name));
    cout << "Age: ";
    cin >> age;
    if(age>100 || age<18 ){
        cout<<"INVAILID AGE!"<<endl;
        cout<<endl;
        cout<<"PLEASE RE-ENTER AGE (18+): ";
        cin>>age;
    }
    cout<< "Enter amount: ";
    cin>>balance;
    cout<<endl;
    if(balance<1000){
        cout<<"PLEASE ENTER AMOUNT GREATER THEN 1000: ";
        cin>>balance;
    }
    cout<<"SET PASSWORD: ";
    cin>>password;
}



istream& operator>>(istream& is, bank& m) {
    m.input();
    return is;
}

ostream& operator<<(ostream& os, bank& m) {

    os<< m.acc_no <<"       "<< m.name <<"              "<< m.age <<"           "<< m.balance <<"           "<<m.password<<endl;
    return os;
}




void bank::save(bank obj) {
    ofstream outFile("bank_data.dat", ios::binary | ios::app);

    if (!outFile.is_open()) {
        cout<<"Error opening file for writing!" << endl;
        return;
    }

    outFile.write((char*)&obj, sizeof(obj));
    outFile.close();
}




void bank::showDataFromFile() {
    ifstream inFile("bank_data.dat", ios::binary);
    bank obj;

    if (!inFile.is_open()) {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    cout<< "---------------------------------------------------------------------"<<endl;
    cout<< "A/C No." <<"      "<< "Name "<<"              "<< "Age "<<"       "<< "Balance "<<endl;
    cout<< "---------------------------------------------------------------------"<<endl;

    while (inFile.read((char*)&obj, sizeof(obj))) {

        cout<<obj.acc_no <<"       "<< obj.name <<"          "<< obj.age <<"         "<< obj.balance  <<endl;
    }

    inFile.close();
}



void bank::balance_heading(){
    cout<<"--------------------------"<<endl;
    cout<<"Name"<<"           "<<"Balance"<<endl;
    cout<<"--------------------------"<<endl;
}



void bank::heading(){
    cout<< "-------------------------------------------------------------------------------------------------------------"<<endl;
    cout<< "A/C No." <<"         "<< "Name "<<"              "<< "Age "<<"            "<< "Balance "<<"        "<<"Password"<<endl;
    cout<< "-------------------------------------------------------------------------------------------------------------"<<endl;
}



void bank::check_balance() {
    double account_no;
    double password;
    cout<<"TO CHECK ACCOUNT BALANCE "<<endl;
    cout<<"Enter your Account Number: ";
    cin>>account_no;

    cout<<"Enter your password: ";
    cin>>password;

    ifstream inFile("bank_data.dat", ios::binary);
    bank obj;
    bool accountFound = false;

    if (!inFile.is_open()) {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    while (inFile.read((char*)&obj, sizeof(obj))) {
        if (obj.acc_no == account_no && obj.password==password) {

            balance_heading();
            cout<< obj.name<<"     "<< obj.balance<<endl;
            cout<<endl;
            accountFound = true;
            break;
        }
    }

    inFile.close();

    if (!accountFound) {
        cout << "Account not found! Please check your account number and password." << endl;
    }
}




void bank::delete_account(double accountNo) {
    ifstream inFile("bank_data.dat", ios::binary);
    ofstream outFile("temp.dat", ios::binary);

    bank obj;
    bool accountFound = false;

    if (!inFile.is_open()) {
        cout << "Error opening file for reading!" << endl;
        return;
    }

    while (inFile.read((char*)&obj, sizeof(obj))) {
        if (obj.acc_no == accountNo) {
            accountFound = true;
            cout << "Account with A/C No. " << accountNo << " has been deleted." << endl;
        } else {
            outFile.write((char*)&obj, sizeof(obj));
        }
    }

    inFile.close();
    outFile.close();

    if (!accountFound) {
        cout << "Account not found! Please check the account number." << endl;
    }

    remove("bank_data.dat");
    rename("temp.dat", "bank_data.dat");
}



void bank::modify_account(double accountNo,double password) {
    fstream file("bank_data.dat", ios::binary | ios::in | ios::out);

    if (!file.is_open()) {
        cout << "Error opening file for reading and writing!" << endl;
        return;
    }

    bool accountFound = false;

    while (file.read((char*)this, sizeof(*this))) {
        if (this->acc_no == accountNo && this->password == password) {
            accountFound = true;

            cout << "Modify Account Details for A/C No. " << accountNo << endl;
            cout << "Enter new name: ";
            cin.ignore();
            cin.getline(this->name, sizeof(this->name));
            cout << "Enter new age: ";
            cin >> this->age;

            if (this->age > 100 || this->age < 18) {
                cout << "INVALID AGE!" << endl;
                cout << "PLEASE RE-ENTER AGE: ";
                cin >> this->age;
            }
            cout << "Enter new balance: ";
            cin >> this->balance;
            cout << "Enter new password: ";
            cin >> this->password;

            // Move the file pointer back to the beginning of the record
            file.seekp(static_cast<std::streamoff>(file.tellg()) - static_cast<std::streamoff>(sizeof(*this)));


            // Write the modified record back to the file
            file.write((char*)this, sizeof(*this));

            cout << "Account details for A/C No. " << accountNo << " have been modified." << endl;
            break;
        }
    }

    file.close();

    if (!accountFound) {
        cout << "Account not found! Please check the account number." << endl;
    }
}





void bank::withdraw_cash(double accountNo, double password, double amount) {

    ifstream inFile("bank_data.dat", ios::binary);
    bank obj;
    bool accountFound = false;

    while (inFile.read((char*)&obj, sizeof(obj))) {
        if (obj.acc_no == accountNo && obj.password == password) {
            accountFound = true;
            break;
        }
    }

    inFile.close();

    if (!accountFound) {
        cout<<"Account not found or incorrect password! Unable to withdraw." << endl;
    } else {

        if (amount > obj.balance) {
            cout << "Insufficient balance! Unable to withdraw." << endl;
        } else {
            obj.balance -= amount;
            cout << "Withdrawal successful. Remaining balance: " << obj.balance << endl;

            // Update the file with the modified balance
            fstream file("bank_data.dat", ios::binary | ios::in | ios::out);

            if (!file.is_open()) {
                cout << "Error opening file for reading and writing!" << endl;
                return;
            }

            while (file.read((char*)this, sizeof(*this))) {
                if (this->acc_no == accountNo) {
                    // Move the file pointer back to the beginning of the record
                    file.seekp(file.tellg() - static_cast<std::streamoff>(sizeof(*this)));

                    // Write the modified record back to the file
                    file.write((char*)&obj, sizeof(obj));

                    break;
                }
            }

            file.close();
        }
    }
}


void bank::Deposit_cash(double accountNo, double password, double amount)
{
    ifstream inFile("bank_data.dat", ios::binary);
    bank obj;
    bool accountFound = false;

    while (inFile.read((char*)&obj, sizeof(obj))) {
        if (obj.acc_no == accountNo && obj.password == password) {
            accountFound = true;
            break;
        }
    }

    inFile.close();

    if (!accountFound) {
        cout << "Account not found or incorrect password! Unable to Deposit." << endl;
    } 
    else {
            obj.balance += amount;
            cout << "Deposit successful. Current balance: " << obj.balance << endl;

            // Update the file with the modified balance
            fstream file("bank_data.dat", ios::binary | ios::in | ios::out);

            if (!file.is_open()) {
                cout << "Error opening file for reading and writing!" << endl;
                return;
            }

            while (file.read((char*)this, sizeof(*this))) {
                if (this->acc_no == accountNo) {
                    // Move the file pointer back to the beginning of the record
                    file.seekp(file.tellg() - static_cast<std::streamoff>(sizeof(*this)));

                    // Write the modified record back to the file
                    file.write((char*)&obj, sizeof(obj));

                    break;
                }
            }

            file.close();
    }
}



int main() {
    bank l;
    char n;

    do {
        l.list();
        int i;
        cout<<"Choose Your Option: ";
        cin>>i;

        switch (i) {
            case 1: {
                bank P1;
                cin>>P1;
                cout<<"YOUR REGISTRATION IS COMPLETED!"<<endl;
                cout<<endl;

                P1.heading();
                cout << P1;
                P1.save(P1);
                break;
            }
            case 2:
                l.check_balance();

                break;
            case 3:
                double AccNo;
                double Pass;
                double amt;

                cout<<"TO DEPOSIT CASH"<<endl;
                cout<<"Enter your Account Number: ";
                cin>>AccNo;

                cout<<"Enter your password: ";
                cin>>Pass;

                cout<<"Enter Amount to Deposit: ";
                cin>>amt;
                if(amt<0){
                    cout<<"Negative Amount Cannot Deposit! "<<endl;
                    cout<<endl;
                    cout<<"Please Enter Possitive Amount : ";
                    cin>>amt;
                }
                if(amt<0){
                    cout<<"INVAILID AMOUNT CHECK YOUR AMOUNT AND TRY AGAIN!"<<endl;
                    break;
                }

                l.Deposit_cash(AccNo, Pass, amt);
                cout<<endl;
                break;
            case 4:
                double AccountNo;
                double Password;
                double amount;

                cout<<"TO WITHDRAW CASH"<<endl;
                cout<<"Enter your Account Number: ";
                cin>>AccountNo;

                cout<<"Enter your password: ";
                cin>>Password;

                cout<<"Enter Amount to withdraw: ";
                cin>>amount;
                if(amount<0){
                    cout<<"Negative Amount Cannot Withdraw! "<<endl;
                    cout<<endl;
                    cout<<"Please Enter Possitive Amount : ";
                    cin>>amount;
                }
                if(amt<0){
                    cout<<"INVAILID AMOUNT CHECK YOUR AMOUNT AND TRY AGAIN!"<<endl;
                    break;
                }

                l.withdraw_cash(AccountNo, Password, amount);
                cout<<endl;
                break;
            case 5:
                double modify_acc;
                double password;
                cout<<"Enter the account number to modify: ";
                cin>>modify_acc;
                cout<<"ENTER YOUR PASSWORD: ";
                cin>>password;
                l.modify_account(modify_acc,password);
                break;
            case 6:
                double a;
                cout<<"ENTER ADMIN-PASSWORD: ";
                cin>>a;
                if (a == 456) {
                    double accountNo;
                    cout<<"Enter the account number to delete: ";
                    cin>>accountNo;
                    l.delete_account(accountNo);
                } else {
                    cout<<"WRONG PASSWORD!"<< endl;
                    cout<<"Acces Denied";
                }
                break;
            case 7:
                double admin;
                cout<<"ENTER ADMIN-PASSWORD: ";
                cin>>admin;
                if(admin==456){
                    cout<<"ACCOUNT DETAILS (FROM FILE):"<<endl;
                    l.showDataFromFile();
                }
                else{
                    int ad;
                    cout<<"WRONG PASSWORD!"<<endl;
                    cout<<"PLEASE RE-ENTER THE PASSWORD: ";
                    cin>>ad;
                    cout<<"ACCOUNT DETAILS (FROM FILE):"<<endl;
                    l.showDataFromFile();
                }
                break;
            case 8:
                cout<<"Thank you for being with us!:)"<<endl;
                return 0;
            default:
                cout<<"Invalid option! Please choose a valid option."<<endl;
        }

        cout<<"WANT TO PERFORM MORE OPERATIONS (y/n): ";
        cin>>n;
        cout<<endl;


    } while (n=='y'||n=='Y');

    cout<<"Thank you for being with us!:)" <<endl;

    return 0;
}

