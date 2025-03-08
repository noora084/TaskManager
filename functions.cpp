#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include "class.h"
using namespace std ;
using namespace chrono ; 

bool isNumber(string str){
    for (char c : str){
        if (c < '0' || c > '9')
            return false;
    }
    return true;
}

int Task::checkDeadLine(string deadline){
    string tempdeadline = deadline;
    if (isNumber(tempdeadline))
        return stoi(tempdeadline) ; 

    return -1; //deadline is not a number
}

int Task:: NewId(int id){
    int final_id ; 
    return final_id = rand()%10 + id ; 
}
void Task::timing(){
    fstream file("logging.txt", ios::app);
    auto now = system_clock::now();
    time_t currentTime = system_clock::to_time_t(now);
    file << ctime(&currentTime);
}
void Task::logging(string failsuccess , string mission , int id){
    fstream file("logging.txt", ios::app);
    timing() ; 
    file << mission << " id:" << id << " " <<failsuccess << endl << endl;   
}
void Task:: logging(string failsuccess , string mission){
    fstream file("logging.txt", ios::app);
    timing() ; 
    file << mission << " " << failsuccess << endl << endl ;  

}
void Task::readString(ifstream &File , string &stringg){
    size_t len;
    File.read(reinterpret_cast<char*>(&len), sizeof(len));
    char *buffer = new char[len + 1];
    File.read(buffer, len);
    buffer[len] = '\0';
    stringg = string(buffer);
    delete[] buffer;
}
int Task::serialize(string Type , string Name , int Id , int Deadline , string Status){ //write
    fstream outfile("example.dat" , ios::binary | ios:: app);
    if (!outfile){
        cerr << "Error opening file for writing!" << endl;
        return -1 ; 
        logging("opening file" , "fail") ; 
    }
    
    size_t nameLen = Name.length();
    size_t StatusLen = Status.length();
    size_t typeLen = Type.length();

    outfile.write(reinterpret_cast<char*>(&Id),sizeof(Id));

    outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
    outfile.write(Type.c_str(), typeLen);


    outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
    outfile.write(Name.c_str(), nameLen);

    outfile.write(reinterpret_cast<char*>(&Deadline),sizeof(Deadline));


    outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
    outfile.write(Status.c_str(), StatusLen);

    return 1 ; 

    outfile.close();
}
bool Task::deserialize(){ //read
    ifstream inFile("example.dat" , ios::binary);

    string Type , Name , Status ; 
    int Deadline , Id  ; 
    
    if(!inFile){
        cerr << "Error opening file for reading!" << endl;
        logging("opening file - " , "fail") ; 
        return false ; 
    }
    while(inFile.read(reinterpret_cast<char*>(&Id), sizeof(Id))){

            readString(inFile, Type);
            readString(inFile, Name);
            inFile.read(reinterpret_cast<char*>(&Deadline), sizeof(Deadline));
            readString(inFile, Status);
 
        cout << Id << " " << Type << " " << Name << " " << Deadline << " " << Status << endl;
    }

    inFile.close();
    return true ; 
}
void Task::adding(string type){
    string Name , Deadline ; 
    srand(time(0)) ; 
    int Id = rand() ; 
    int bb = NewId(Id) ; 
    cin >> Name >> Deadline ; 
    int deadlineinteger = checkDeadLine(Deadline) ; 
    if(deadlineinteger == -1){
        logging("add(invalid deadline)" , "fail") ; 
        cerr << "invalid deadline. try again " ; 
        exit(0) ; 
    }
    string Status = "undone";
    if(serialize(type , Name , bb , deadlineinteger , Status) == 1){
        cout << "added successfully!" << endl ; 
        logging("add" , "success" , bb) ;     
    } 
}
void Task::printtask(string type){
    int id;
    cin >> id;
    ifstream File("example.dat", ifstream::binary | ifstream::in);
    if (!File){
        cout << "Error opening file!" << endl;
        logging("opening file" , "fail");
        return;
    }

    int ID, DEADLINE;
    string TYPE, NAME, STATUS;
    bool found = false;

    if (id == -1){//نشون دادن همه تسک ها
        //undone
        while (File.read(reinterpret_cast<char*>(&ID), sizeof(ID))){
            readString(File, TYPE);
            readString(File, NAME);
            File.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
            readString(File, STATUS);

            if (STATUS == "undone")
                cout << ID << " " << TYPE << " " << NAME << " " << DEADLINE << " " << STATUS << endl;
        }
        File.clear(); //به اول فایل برگردیم
        File.seekg(0, ios::beg);

        //done
        while (File.read(reinterpret_cast<char*>(&ID), sizeof(ID))){
            readString(File, TYPE);
            readString(File, NAME);
            File.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
            readString(File, STATUS);

            if(STATUS == "done")
                cout << ID << " " << TYPE << " " << NAME << " " << DEADLINE << " " << STATUS << endl;
            
        }
        logging("print all task" , "success");

    }
    else{
        while(File.read(reinterpret_cast<char*>(&ID), sizeof(ID))){
            readString(File, TYPE);
            readString(File, NAME);
            File.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
            readString(File, STATUS);

            if (id == ID) {
                cout << ID << " " << TYPE << " " << NAME << " " << DEADLINE << " " << STATUS << endl;
                logging("print" , "success" , id);
                found = true;
                break;
            }
        }
        if (!found){
            cout << "id not valid" << endl;
            logging("print" , "fail");
        }
    }
    File.close();
}

void Task::repalceUndone(){

    ifstream infile("example.dat", ios::binary | ios::in);
    fstream outfile("temp.dat", ios::binary | ios::out);
    if (!infile || !outfile) {
        cout << "Error opening file!" << endl;
        logging("opening file" , "fail") ; 
        return;
    }
    int id ; 
    cin >> id ; 

    int ID, DEADLINE;
    string TYPE, NAME, STATUS;
    string NEWSTATUS = "done" ;
    bool a = false; 

    while (true){
        infile.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        if (infile.eof()) 
            break;

        readString(infile, TYPE);
        readString(infile, NAME);
        infile.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
        readString(infile, STATUS);

        size_t nameLen = NAME.length();
        size_t newstatusLen = NEWSTATUS.length(); 
        size_t StatusLen = STATUS.length() ; 
        size_t typeLen = TYPE.length();

        if (id == ID){
            // نوشتن با وضعیت جدید
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
            outfile.write(reinterpret_cast<char*>(&newstatusLen),sizeof(newstatusLen));
            outfile.write(NEWSTATUS.c_str(), newstatusLen);
            a = true ;         
        }else{
            // نوشتن بدون تغییر
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
        }
    }
    if(!a){
        logging("replace undone to done" , "fail" , id) ; 
        cout << "id not valid" << endl ; 
    }
    infile.close();
    outfile.close();

    remove("example.dat");
    rename("temp.dat", "example.dat");

    if(a){
    cout << "task updated successfully!" << endl;
        logging("replace undone to done" , "success" , id) ; 
    }
}
void Task::edittask(){
    int id;
    string section;
    cin >> id >> section;

    if (section == "title"){
        cout << "enter a new title: ";
        string title;
        cin >> title;

    ifstream infile("example.dat", ios::binary | ios::in);
    fstream outfile("temp.dat", ios::binary | ios::out);
    if (!infile || !outfile) {
        cout << "Error opening file!" << endl;
        logging("opening file" ,"fail") ; 
        return;
    } 
    int ID, DEADLINE;
    string TYPE, NAME, STATUS;
    bool a = false ; 

    while (true){
        infile.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        if (infile.eof()) 
            break;

        readString(infile, TYPE);
        readString(infile, NAME);
        infile.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
        readString(infile, STATUS);

        size_t nameLen = NAME.length();
        size_t titlelen = title.length(); //new name
        size_t StatusLen = STATUS.length() ; 
        size_t typeLen = TYPE.length();

        if (id == ID){
            // نوشتن با وضعیت جدید
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&titlelen),sizeof(titlelen));
            outfile.write(title.c_str(), titlelen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
            a = true ; 
        
        }else{
            // نوشتن  بدون تغییر
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
        }
    }
    if(!a){
        logging("edit title" , "fail") ; 
        cout << "id not valid" << endl ; 
    }
    if(a){
        cout << "task updated successfully!" << endl;
            logging("edit title" , "success" , id) ; 
        }
    infile.close();
    outfile.close();

    remove("example.dat");
    rename("temp.dat", "example.dat");
    }

    if(section == "deadline"){
        cout << "enter a new deadline: "; 
        int newdeadline;
        cin >> newdeadline;

        bool a = false ; 

        if(newdeadline < 0){
            a = false ; 
            cout << "deadline can not be negative" << endl ; 
            logging("invalid deadline" , "fail" , id) ; 
        }else{

    ifstream infile("example.dat", ios::binary | ios::in);
    fstream outfile("temp.dat", ios::binary | ios::out);
    if (!infile || !outfile) {
        cout << "Error opening file!" << endl;
        logging("opening file" , "fail") ; 

        return;
    } 
    int ID, DEADLINE;
    string TYPE, NAME, STATUS;

    while (true){
        infile.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        if (infile.eof()) 
            break;

        readString(infile, TYPE);
        readString(infile, NAME);
        infile.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
        readString(infile, STATUS);

        size_t nameLen = NAME.length();
        size_t StatusLen = STATUS.length() ; 
        size_t typeLen = TYPE.length();

        if (id == ID){
            // نوشتن با حالت جدید
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&newdeadline),sizeof(newdeadline));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
            a = true ; 
        
        }else{
            // نوشتن بدون تغییر
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
        }
    }
    if(!a){
        logging("edit deadline" ,"fail") ; 
        cout << "id not valid" << endl ; 
    }
    if(a){
        cout << "task updated successfully!" << endl;
            logging("edit deadline" , "success" , id) ; 
        }
    infile.close();
    outfile.close();

    remove("example.dat");
    rename("temp.dat", "example.dat");
    }
    }
}
void Task::deletetask(){
    int id ; 
    cin >> id ; 
    ifstream infile("example.dat", ios::binary | ios::in);
    fstream outfile("temp.dat", ios::binary | ios::out);
    if (!infile || !outfile) {
        cout << "Error opening file!" << endl;
        logging("opening file" , "fail") ; 
        return;
    } 
    int ID, DEADLINE;
    string TYPE, NAME, STATUS;
    bool a = false ; 

    while (true){
        infile.read(reinterpret_cast<char*>(&ID), sizeof(ID));
        if (infile.eof()) 
            break;

        readString(infile, TYPE);
        readString(infile, NAME);
        infile.read(reinterpret_cast<char*>(&DEADLINE), sizeof(DEADLINE));
        readString(infile, STATUS);

        size_t nameLen = NAME.length();
        size_t StatusLen = STATUS.length() ; 
        size_t typeLen = TYPE.length();

        if (id == ID){
            a = true ; 
            continue;
        }
        else{
            //بدون تغییر بنویسش
            outfile.write(reinterpret_cast<char*>(&ID),sizeof(ID));
        
            outfile.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
            outfile.write(TYPE.c_str(), typeLen);
        
        
            outfile.write(reinterpret_cast<char*>(&nameLen),sizeof(nameLen));
            outfile.write(NAME.c_str(), nameLen);
        
            outfile.write(reinterpret_cast<char*>(&DEADLINE),sizeof(DEADLINE));
        
        
            outfile.write(reinterpret_cast<char*>(&StatusLen),sizeof(StatusLen));
            outfile.write(STATUS.c_str(), StatusLen);
        }
    }
    if(!a){
        logging("delete" , "fail") ; 
        cout << "id not valid!" << endl ; 
    }
    if(a){
        cout << "deleted successfully" << endl;
            logging("delete" , "success" , id) ; 
        }

    infile.close();
    outfile.close();

    remove("example.dat");
    rename("temp.dat", "example.dat");
}
