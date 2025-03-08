#include <iostream>
#include <fstream>
#include "class.h"
using namespace std ; 

int main(){
    Task tasks ; 
    cout << "welcome to task manager. what do you want to do? " ; 
    while(true){
        string type , mission ; 
        cin >> type >> mission ; 
        if(mission == "add"){
            tasks.adding(type) ; 
            cout << "what else do you want to do? " ; 
        }
        else if(mission == "print"){
            tasks.printtask(type) ;
            cout << "what else do you want to do? " ; 
        }
        else if(mission == "done"){
            tasks.repalceUndone() ;  
            cout << "what else do you want to do? " ; 
        }
        else if(mission == "edit"){
            tasks.edittask() ; 
            cout << "what else do you want to do? " ; 
        }
        else if(mission == "del"){
            tasks.deletetask() ; 
            cout << "what else do you want to do? " ; 
        }
        else if(mission == "0" || type == "0"){
            cout << "bye bye." ; 
            exit(0) ; 

        }
        else{
            cout << "mission is not valid. try again" << endl ; 
            tasks.logging("fail" , "not valid mission") ; 
            break ; 
        }
    }


}
