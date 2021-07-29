#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <regex>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <cctype>
#include <locale>
#include <bits/stdc++.h>
using namespace std;


// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline string trim(std::string &s) {
    ltrim(s);
    rtrim(s);
    return s;
}

int main(){

    int count=0;
    string line,Bilabel,DocDate,ticketNum,txt_fileName,fileName;

    cout<<"type bilable name: "<<endl;
    getline(cin,Bilabel);
    Bilabel=trim(Bilabel);
    transform(Bilabel.begin(), Bilabel.end(), Bilabel.begin(), ::toupper);

    cout<<"type verification date: "<<endl;
    getline(cin,DocDate);
    DocDate=trim(DocDate);
    transform(DocDate.begin(), DocDate.end(), DocDate.begin(), ::toupper);

    cout<<"type Ticket: "<<endl;
    getline(cin,ticketNum);
    ticketNum=trim(ticketNum);
    transform(ticketNum.begin(), ticketNum.end(), ticketNum.begin(), ::toupper);

    cout<<"type below txt file path:(Ex: ../xyz/yz/.txt)"<<endl;
    getline(cin,txt_fileName);
    txt_fileName=trim(txt_fileName);
    cout<<"type below original file path:(Ex: ../xyz/yz/.js or .vb or .sql)"<<endl;
    getline(cin,fileName);
    fileName=trim(fileName);
    // cout<<endl<<endl<<"'"<<Bilabel<<"'"<<endl;
    // cout<<"'"<<DocDate<<"'"<<endl;
    // cout<<"'"<<ticketNum<<"'"<<endl;

    vector<int> StartLineNumbers={},endLineNumber={},jointIndex={};
    stack<string> check_function;  
    ifstream file(txt_fileName);
    if(file.is_open()){
        while (!file.eof())
        {
            getline(file,line);
            count++;
            // if(count==559){
                // if(line.find(cmp) != string::npos)
                
                if(line.find("Line:")!= string::npos)
                {
                    if(line[0]=='L' && line[1]=='i' && line[2]=='n' && line[3]=='e' && line[4]==':'){
                        
                        // stringstream st;
                        // st<<line;
                        // string temp;
                        int start=0,end=0,foe=line.length();
                        bool endFlag=false;
                        for(int i=5;i<=foe;i++){
                            // st>>temp;
                            // if(stringstream(temp) >> num){
                            //     cout<<num<<endl;
                            // }
                            if(line[i]=='-'){
                                endFlag=true;
                            }
                            if((int)line[i] >= 48 && (int)line[i]<=57){
                                if(endFlag){
                                // for the End
                                    int temp=(int)line[i]-48;
                                    end=end*10;
                                    end=end+temp; 
                                }
                                else{
                                // for the Start
                                    int temp=(int)line[i]-48;
                                    start=start*10;
                                    start=start+temp;                                    
                                }
                            }
                        }
                        if(start !=0){
                            // cout<<"start"<<start<<endl;
                            StartLineNumbers.push_back(start);
                        }   
                        if(end !=0){
                            // cout<<"end"<<end<<endl;
                            jointIndex.push_back(start);
                            // cout<<"joint index"<<start<<endl;
                            endLineNumber.push_back(end);
                        } 
                        
                    }
                }
                // else{
                //     cout<<"Not found"<<endl;
                // }
            //}
        }
    }
    else{
        cout<<count;
    }
    line=line.empty();
    // cout<<"start Line number: "<<endl;
    // for(int i=0;i<StartLineNumbers.size();i++){
    //     cout<<StartLineNumbers[i]<<endl;
    // }
    // cout<<"end Line number: "<<endl;
    // for(int i=0;i<endLineNumber.size();i++){
    //     cout<<endLineNumber[i]<<endl;
    // }

    int I_file=0,Line_num=0,I_of_joint=0,I_End_file=0;
    bool is_fun=false,flag_end=true,is_start_searching_br=false,is_revision=false;

    ifstream file2(fileName);
    
    if(file2.is_open()){
        string NewBilabel=Bilabel+" ",NewDocDate=DocDate+" ",NewticketNum=ticketNum+",";
        while(!file2.eof()){

            getline(file2,line);
            Line_num++;

            if(is_fun){
                if(line.find(NewBilabel) != string::npos &&
                    line.find(NewDocDate) != string::npos &&
                    line.find(NewticketNum) != string::npos){
                        is_revision=true;
                    }
                if(line.find("***********/") != string::npos && flag_end){
                    if(!is_revision){
                        cout<<"Faild! not found Revision history before line: "<<Line_num<<endl;
                    }
                    flag_end=false;
                    is_start_searching_br=true;
                }
                if(is_start_searching_br){
                    int len=line.length();
                    for(int i=0;i<=len;i++){
                        if(line[i]=='{'){
                            check_function.push("{");
                        }
                        else if(line[i]=='}'){
                            if(!check_function.empty()){
                                check_function.pop();
                            }
                            else{
                                
                                cout<<"line number curly brackets missing :"<<Line_num<<endl;
                            }
                        }
                    }
                }
            }

            if(StartLineNumbers[I_file]==Line_num){

                if(StartLineNumbers[I_file]==jointIndex[I_of_joint]){
                    
                    if(line.find("Start")!= string::npos &&
                         line.find(NewBilabel)!= string::npos &&
                         line.find(NewDocDate)!= string::npos &&
                         line.find(NewticketNum)!= string::npos){
                        
                        cout<<"Successfully found Line Number: "<<StartLineNumbers[I_file]<<endl;

                    }
                    else if(line.find("<summary>") != string::npos || line.find("/********")!= string::npos){
                        if(line.find("/********")!= string::npos){
                            is_fun=true;
                        }
                        is_start_searching_br=false;
                        flag_end=true;
                        is_revision=false;
                        cout<<"Successfully function Added from the Line Number: "<<StartLineNumbers[I_file]<<endl;
                    }
                    else{
                        
                        cout<<"Faild! Not found on Line number: "<<StartLineNumbers[I_file]<<endl;

                    }
                    I_of_joint=I_of_joint+1;
                }
                else{
                    if(line.find("Start")!= string::npos){
                        if(line.find("Start")!= string::npos &&
                         line.find("End")!= string::npos &&
                         line.find(NewBilabel)!= string::npos &&
                         line.find(NewDocDate)!= string::npos &&
                         line.find(NewticketNum)!= string::npos){
                        
                        cout<<"Successfully found Line Number: "<<StartLineNumbers[I_file]<<endl;

                        }
                        else{
                            
                            cout<<"Falid! Not found on Line number: "<<StartLineNumbers[I_file]<<endl;

                        }
                    }
                    else{
                        if(line.find(NewBilabel)!= string::npos &&
                         line.find(NewDocDate)!= string::npos &&
                         line.find(NewticketNum)!= string::npos){
                             
                            cout<<"Successfully found Line Number: "<<StartLineNumbers[I_file]<<endl;
                        }
                        else{
                            
                            cout<<"Faild! Not found on Line number: "<<StartLineNumbers[I_file]<<endl;

                        }
                    }
                } 
                I_file=I_file+1;
            }
            else if(endLineNumber[I_End_file]==Line_num){
                if(line.find("End")!= string::npos &&
                     line.find(NewBilabel)!= string::npos &&
                     line.find(NewDocDate)!= string::npos &&
                     line.find(ticketNum)!= string::npos){
                     
                    cout<<"Successfully found Line Number: "<<endLineNumber[I_End_file]<<endl;
                }
                else if((line.find("End") != string::npos &&
                         (line.find("Function") != string::npos || line.find("Sub") != string::npos)) ||
                         (line.find("}") != string::npos && is_fun)){

                    if(is_fun){
                        is_fun=false;
                        if(check_function.empty()){
                            
                            cout<<"Successfully function close at line number :"<<endLineNumber[I_End_file]<<endl;
                        }
                        else{
                            
                            cout<<"Faild! function faild to close at line number :"<<endLineNumber[I_End_file]<<endl;

                        }
                    }

                }
                else{
                    
                    cout<<"Faild! Not found on Line number: "<<endLineNumber[I_End_file]<<endl;
                }
                I_End_file=I_End_file+1;
            }

            

        }

    }
    else{
        cout<<"this file is not found"<<endl;
    }
    getch();
    return 0;

}