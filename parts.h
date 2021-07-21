#ifndef PARTS_H
#define PARTS_H

#include "story.h"


class Set: public Story
{
    private:
        int start;
        int end;
        string var;
        bool val;
        string tval;
    public:
        Set(string, string);
        void perform(Story*);
        string getVar();
        bool getVal();
        void print();
};

class Text: public Story
{
    private:
        string text;
    public:
        Text(string, string);
        void perform();
        void print();
        void printP();
};

class Link : public Story
{
    private:
        int start;
        int end;
        string pName;
        string lName;
        //string passage;
    public:
        Link (string, string);
        void perform();
        //string getPassage();
        string getLname();
        string getPname();
        void print();
        void printP();
};

class GoTo : public Story
{
    private:
        int start;
        int end;
        string gName;
        //string passage;
    public:
        GoTo (string, string);
        //void perform();
        //string getPassage();
        string getGname();
        void print();
};

class IfElse: public Story
{
    private:
        int start;
        int end;
        string var;
        bool val;
        string tval;
        string type;
    public:
        IfElse(string, string);
        //void perform();
        string getVar();
        bool getVal();
        void print();
        bool match(Story*);
};

class OnlyElse: public Story
{
    private:
        string type;
    public:
        OnlyElse(string);
        void print();
        bool match(){return true;}
};

class Block: public Story
{
    private:
        string text;

    public:
        Block(string, string);
        void print();
        void play(Story*);
};

#endif 