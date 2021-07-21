#ifndef STORYTOKENIZERS_CLASSES_H
#define STORYTOKENIZERS_CLASSES_H

#include <iostream>

using namespace std;

class PassageToken{
private:
    string name;
    string text;

public:
    PassageToken();
    PassageToken(string, string);
    string getName() const;
    string getText() const;
};



class StoryTokenizer{
private:
    int cursor = 0;
    int begin;
    int end;
    string st;
    string open = "<tw-passagedata";
    string close = "</tw-storydata>";
    bool next = true;

public:
    StoryTokenizer();
    StoryTokenizer(string st);
    bool hasNextPassage();
    PassageToken nextPassage();        
};



enum type_t{LINK, GOTO, SET, IF, ELSEIF, ELSE, BLOCK, TEXT};

class PartToken{
private:
    type_t type;
    string text;

public:
    PartToken();
    PartToken(type_t, string);
    string getText() const;
    type_t getType() const;
};




class PassageTokenizer{
private:
    int cursor = 0;
    string passage;
    string openCommand = "(";
    string closeCommand = ")";
    string openLink = "[[";
    string closeLink = "]]";
    string openBlock = "[";
    string closeBlock = "]";

    bool next = true;

public:
    PassageTokenizer();
    PassageTokenizer(string passage);
    bool hasNextPart();
    PartToken nextPart();
};

#endif