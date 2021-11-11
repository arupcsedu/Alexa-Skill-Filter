// SkillFilter.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Author: Arup Kumar Sarker
// Creation date: 5th November, 2021
// Description: Implementation of finding vulnerable commands for Alexa.


#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <filesystem>
#include <fstream>
#include <sys/types.h>
#include "dirent.h"
#include <stdio.h>


//namespace fs = std::filesystem;

using namespace std;
vector<string> filesNames;

// Function name : trim
// Param: arg1- Input String; arg2,3,4 - pattarns 
// Description: Utterance Trimmer
// Author: Arup Kumar Sarker
// Creation date: 6th November, 2021

std::string trim(const std::string& str,
    const std::string& whitespace = " \t", 
    const std::string& forCommas = ",",
    const std::string& forSemiColon = "\"")

{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

     auto strEnd = str.find_last_not_of(forSemiColon);

    if (str[str.length() - 1] == ',' || str[str.length() - 1] == '"')
        strEnd = strEnd - 1;
    //const auto strNEnd = str.substr(strBegin, strRange).find_last_not_of(forCommas); // For commas
    if (str[str.length() - 2] == ',' || str[str.length() - 2] == '"')
        strEnd = strEnd - 2;
    auto strRange = strEnd - strBegin + 1;

    //const auto newStrRange = strNEnd - strBegin + 1;
    return str.substr(strBegin+1, strRange);
}

std::string reduce(const std::string& str,
    const std::string& fill = " ",
    const std::string& whitespace = " \t")
{
    // trim first
    auto result = trim(str, whitespace);

    // replace sub ranges
    auto beginSpace = result.find_first_of(whitespace);
    while (beginSpace != std::string::npos)
    {
        const auto endSpace = result.find_first_not_of(whitespace, beginSpace);
        const auto range = endSpace - beginSpace;

        result.replace(beginSpace, range, fill);

        const auto newStart = beginSpace + fill.length();
        beginSpace = result.find_first_of(whitespace, newStart);
    }

    return result;
}

string skillTok(string str) {

    string token;
    regex reg("\"alexa +");
    auto callback = [&](std::string const& m) {
        if (std::regex_match(m, reg)) {
            /* matching */
            token = trim(str);
           // cout << token << endl;
        }
        else {
            /* non-matching */
            //cout << "Not Found" << endl;
        }
    };

    std::sregex_token_iterator
        begin(str.begin(), str.end(), reg, { -1, 0 }),
        end;
    std::for_each(begin, end, callback);

    return token;
}

vector<string> skillTokenString(string str) {
    regex reg("\"alexa +");

    vector<string> res;
    sregex_token_iterator iter(str.begin(), str.end(), reg, -1);
    sregex_token_iterator start;
    sregex_token_iterator end;
    if (iter != start) {
        vector<string> vec(iter, end);
        res = vec;
    }
    return res;
}

char* skillStrTok(string str) {

    char* skillStr = (char*)str.c_str();

    rsize_t strmax = sizeof str;
    const char* delim = "alexa";
    char* next_token = NULL;
  

    char* token = strtok_s(skillStr, delim, &next_token);
    if (next_token != NULL)
        printf("%s", next_token);
    return token;
}

vector<string> list_dir(const char* path) {
    vector<string> AllFilesName;
    struct dirent* entry;
    DIR* dir = opendir(path);
    string dirPath = string(path);

    if (dir == NULL) {
        cout << path << endl;
        cout << "Unable to open directory" << endl;
        return AllFilesName;
    }

    //readdir return a pointer to the entry of a folder (could be any type not only .txt)

    while ((entry = readdir(dir)) != NULL) {

        AllFilesName.push_back(dirPath+"\\"+entry->d_name);
        filesNames.push_back(entry->d_name);

        //push the name of every file
    }
    closedir(dir);
    return AllFilesName;
}

int isSubstring(string s1, string s2)
{
    int M = s1.length();
    int N = s2.length();

    /* A loop to slide pat[] one by one */
    for (int i = 0; i <= N - M; i++) {
        int j;

        /* For current index i, check for
 pattern match */
        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;

        if (j == M)
            return i;
    }

    return -1;
}

void readFileData(vector<string> allFilesName) {

    vector <string> tokens;
    vector <string> fileName;
    string skippedSkilled = "\"alexa whats my flash briefing\"";
    int skippedCount = 0;
    if (allFilesName.size() != 0) {
        for (int i = 0; i < allFilesName.size(); i++) {

            std::ifstream infile(allFilesName[i]);

            std::string line;
            // Vector of string to save tokens
            
            while (getline(infile, line))
            {
                string tokstr = skillTok(line);
                if (tokstr.length() != 0) {
                    if (tokstr.compare(skippedSkilled) != 0) {
                        tokens.push_back(tokstr);
                        fileName.push_back(filesNames[i]);
                    }
                    else
                        skippedCount++;
                }
                    

            }

        }
        /*for (auto a : tokens)
        {
            cout << a << endl;
        }*/
        
        
       // cout << skippedSkilled << " is affecting " << skippedCount << " times" << endl ;
        
    }
    else {
        cout << "No files in the directory" << endl;
        return;
    }


    int len = tokens.size(); 
   // vector<vector<string>> vulStrList;
   // vector<string> keySkill;
    cout << "Number of utterances which are considered to be processed : " << len << endl;
    cout << "(#) Here # is the skill number from all combined files"  << endl;
    int count = 0;
    for (int i = 0; i < len; i++) {
        int flag = 0;
        vector<string> vulStr;
        for (int j = i+1; j < len - 1; j++) {
           // if (j != i && tokens[j].find(tokens[i]) != std::string::npos && tokens[j].compare(tokens[i])) {
                //std::cout << "First token: " << tokens[j] << '\n' << "Second Token: " << tokens[i] << '\n'; // Console Print
            if(j != i && isSubstring(tokens[j], tokens[i]) != -1 ) {//&& tokens[j].compare(tokens[i]) != 0) {
                if (flag == 0) {
                    cout << "Vulnerable Voice Command is found : " << fileName[i] << endl;
                    cout << "(" << i+1 << ")   " << tokens[i] << endl; // Console Print
                    //keySkill.push_back(tokens[i]);
                    flag = 1;
                    cout << "Unexpected trigger is happened on below skills: " << endl; // Console Print

                }
                count++;
               // vulStr.push_back(tokens[j]);
                cout <<"ID: "<< fileName[j] << "(" << j + 1 << ")   " << tokens[j] << endl; // Console Print
            }
        }
        flag = 0;
       // vulStrList.push_back(vulStr);
    }
    cout << "Number of affected skills : " << count << endl;
    //For further optimization

    /*for (int i = 0; i < keySkill.size(); i++) {
        //cout << " Vulnerable Voice Command : " << keySkill[i] << endl; // Console Print
        //printf("Vulnerable Voice Command : %s\n", keySkill[i].c_str());
        //cout << "Unexpected trigger is happened on below skills: " << endl; // Console Print
       // outFile << "Unexpected trigger is happened on below skills: " << endl;
        for (int j = 0; j < vulStrList[i].size(); j++) {
            //cout << vulStrList[i][j] << endl; // Console Print
            //outFile << vulStrList[i][j] << endl;
        }
    }
    */
    fclose(stdout);
    return;

}

int main()
{
    freopen("output.txt", "wt", stdout);
    //char dirPath[] = "C:\***\***\***\Fall2021\CS 6333\Assignment2_main\skill_profiles\skill_profiles";
    //char dirPath[] = "C:\\Users\\new_user\\Downloads\\skill_profiles\\skill_profiles";
    char dirPath[] = "C:\\Users\\new_user\\Downloads\\Test_Files";
    vector<string> allFilesName = list_dir(dirPath);
    cout << "Total processed file Number : " << allFilesName.size() << endl;
    
    readFileData(allFilesName);
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
