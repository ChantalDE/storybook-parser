#ifndef STORY_H
#define STORY_H

#include <unordered_map>
#include <vector>
#include "StoryTokenizers.h"

using namespace std;

class Story 
{
    public:
        string file;
        string story;
        int pos = 0;
        int x;

        bool endStory = false;
        bool bValue = false;
        bool hitGoTo = false;
        bool hitLink = false;

        bool printBlock;
        bool matchVal; 

        unordered_map <string, bool> variablePair;
        vector <PassageToken*> passageVector;
        unordered_map <string, int> passageNum; // pairs index of passage w/ passageName
        unordered_map <string, string> links; // pairs passage Name w/ displayed Link
        unordered_map<int, string> numLink; //paires user typed num w/ passage name
        //vector<string> links;

        Story (string);
        void play();
        void print();
        void printPart();
};

#endif 
