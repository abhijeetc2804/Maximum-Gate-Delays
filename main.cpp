#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <sstream>
#include<map>
#include <queue>

using namespace std;

long double recur(map<string,long double> &dp,map<string,long double> &gate_delays,map<string,vector<string>> &graph,map<string,string> &gates,string str){
    if(graph.count(str)==0){
        return 0.0;
    }
    if(dp.count(str)!=0){
        return dp[str];
    }
    else if(gates[str]=="INV"){
        dp[str]=gate_delays["INV"]+recur(dp,gate_delays,graph,gates,(graph[str])[0]);
        return dp[str];
    }
    else{
        long double a=recur(dp,gate_delays,graph,gates,(graph[str])[0]);
        long double b=recur(dp,gate_delays,graph,gates,(graph[str])[1]);
        dp[str]=(gate_delays[gates[str]])+max(a,b);
        return dp[str];
    }
}

int main(int argc,char* argv[] ){
    fstream file;
    file.open("circuit.txt",ios::in);
    string s1;
    vector<string> v;
    while(getline(file, s1)){
        if(s1!="" && s1[0]!=' ' && s1[0]!='/'){
        v.push_back(s1);
        }
    }
    file.close();
    map<string, vector<string>> graph;
    map<string,string> gates;
    map<string,long double> dp;
    vector<string> in,out,int_sig;
    for(int i=0;i<v.size();i++){
        string word;
        vector<string> words;
        istringstream iss(v[i]);
        while (iss >> word) {
        words.push_back(word);
        }
        if(words[0]=="PRIMARY_INPUTS"){
            words.erase(words.begin());
            in=words;
        }
        else if(words[0]=="PRIMARY_OUTPUTS"){
            words.erase(words.begin());
            out=words;
        }
        else if(words[0]=="INTERNAL_SIGNALS"){
            words.erase(words.begin());
            int_sig=words;
        }
        else {
            int k=words[0].size();
            if(words[0][k-1]=='2'){
                gates[words[3]]=words[0];
                graph[words[3]]={words[1],words[2]};
            }
            else{
                gates[words[2]]=words[0];
                graph[words[2]]={words[1]};
            }
        }
    }
    file.open("gate_delays.txt",ios::in);
    map<string,long double> gate_delays;
    string s2;
    vector<string> v_gd;
    while(getline(file,s2)){
        if(s2!="" && s2[0]!=' ' && s2[0]!='/'){
        v_gd.push_back(s2);
        }
    }
    file.close();

    file.open("required_delays.txt",ios::in);
    string s5;
    vector<string> v5;
    while(getline(file, s5)){
        if(s5!="" && s5[0]!=' ' && s5[0]!='/'){
        v5.push_back(s5);
        }
    }
    
    for(int i=0;i<v_gd.size();i++){
        string word;
        vector<string> words;
        istringstream iss(v_gd[i]);
        while (iss >> word) {
        words.push_back(word);
        }
        gate_delays[words[0]]=stold(words[1]);
    }
    
    map<string,long double> req_delays;
    
    for(int i=0;i<v5.size();i++){
        string word;
        vector<string> words;
        istringstream iss(v5[i]);
        while (iss >> word) {
        words.push_back(word);
        }
        req_delays[words[0]]=stold(words[1]);
    }
    
    queue<string> Q;
    for(int i=0;i<out.size();i++){
        Q.push(out[i]);
    }
    
    while(!Q.empty()){
        string ss=Q.front();
        Q.pop();

        if(graph.count(ss)!=0){
            if(gates[ss]=="INV"){
                string s4=(graph[ss])[0];
                if(req_delays.count(s4)==0){
                    req_delays[s4]=req_delays[ss]-gate_delays["INV"];
                }
                else{
                    req_delays[s4]=min(req_delays[s4],req_delays[ss]-gate_delays[gates[ss]]);
                }
                Q.push(s4);
            }
            else{
                string s4=(graph[ss])[0];
                string s7=(graph[ss])[1];
                if(req_delays.count(s4)==0){
                    req_delays[s4]=req_delays[ss]-gate_delays[(gates[ss])];
                }
                else{
                    req_delays[s4]=min(req_delays[s4],req_delays[ss]-gate_delays[gates[ss]]);
                }
                Q.push(s4);
                
                if(req_delays.count(s7)==0){
                    req_delays[s7]=req_delays[ss]-gate_delays[(gates[ss])];
                }
                else{
                    req_delays[s7]=min(req_delays[s7],req_delays[ss]-gate_delays[gates[ss]]);
                }
                Q.push(s7);
            }
        }
    }
    if(*argv[1]=='B'){
    ofstream o1file;
    o1file.open("input_delays.txt");
    for(int i=0;i<in.size();i++){
        o1file <<in[i]<<" "<<req_delays[in[i]]<<"\n";
    }
    o1file.close();
    }
    else{
        ofstream ofile;
        ofile.open("output_delays.txt");
        for(int i=0;i<out.size();i++){
            ofile <<out[i]<<" "<<recur(dp,gate_delays,graph,gates,out[i])<<"\n";
        }
        ofile.close();
    }

}