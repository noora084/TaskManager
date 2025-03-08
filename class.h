#ifndef TASKCLASS_H
#define TASKCLASS_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

class Task{
    private:
    std::string type ; 
    std::string name ;  
    int id ; 
    int deadline ; 
    std::string status ;

    public:
        int serialize(std::string Type , std::string Name , int Id , int Deadline , std::string Status);
        bool deserialize() ;
        void repalceUndone() ; 
        void deletetask() ;
        void edittask() ; //title or deadline
        void printtask(std::string type) ; 
        void adding(std::string type) ; 
        void readString(std::ifstream &File , std::string &str) ; 
        void logging(std::string failsuccess , std::string mission , int id) ; 
        void logging(std::string failsuccess , std::string mission) ; 
        int checkDeadLine(std::string deadline) ; 
        void timing() ; 
        int NewId(int id) ; 
};
#endif