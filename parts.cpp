#include "parts.h"
#include <iostream>
#include <unordered_map>

/*--------------------------------
            Set
---------------------------------*/
Set:: Set(string tokentext, string fl)
:Story(fl)
{
    start = tokentext.find("$");
    end = tokentext.find("to")-1;
    var = tokentext.substr(start, end-start);

    start = end+4;
    end = tokentext.find(")");
    tval = tokentext.substr(start, end-start);
    if (tval == "true")
        val = true;
    else if (tval == "false")
        val = false; 
}

string Set::getVar()
{
    return var;
}

bool Set::getVal()
{
    return val;
}

void Set::print()
{
    cout << "Set: variable=" << var << ", value=" << tval <<endl;
}

void Set::perform(Story* ins)
{
    if(ins->variablePair.find(var) == ins->variablePair.end())   
        ins->variablePair.emplace(var,val);
    else
        ins->variablePair[var] = val;
}



/*--------------------------------
            Text
---------------------------------*/
Text:: Text(string tokentext, string fl)
:Story(fl)
{
    text = tokentext;
}

void Text::print()
{
    cout<< "Text: \"" << text << "\"" << endl;
}

void Text::printP(){
    cout  << text;
}

void Text::perform()
{
    cout << text;
}



/*--------------------------------
            Link
---------------------------------*/
Link:: Link(string tokentext, string fl)
:Story(fl)
{
    start = tokentext.find("[") + 2;
    if (tokentext.find("-&gt;")==-1)
    {
        end = tokentext.find("]");
        lName = tokentext.substr(start,end-start);
        pName = lName;
    }
    else
    {
        end = tokentext.find("-&gt;");
        lName = tokentext.substr(start,end-start);
        start = end + 5;
        end = tokentext.find("]");
        pName = tokentext.substr(start,end-start);
    }
}

string Link::getLname()
{
    return lName;
}

string Link::getPname()
{
    return pName;
}

void Link::print()
{
    cout << "Link: display=" << lName << ", target=" << pName << endl;
}

void Link::printP()
{
    cout << lName;
}


/*--------------------------------
            GoTo
---------------------------------*/
GoTo:: GoTo(string tokentext, string fl)
:Story(fl)
{
    start = tokentext.find("&quot;") + 6;
    end = tokentext.find("&quot;", start);
    gName = tokentext.substr(start,end-start);
    
}

string GoTo::getGname()
{
    return gName;
}

void GoTo::print()
{
    cout << "Goto: target=" << gName << endl;
}

/*--------------------------------
            IfElse
---------------------------------*/
IfElse:: IfElse(string tokentext, string fl)
:Story(fl)
{
    if (tokentext.find("if:"))
        type = "If";
    else if  (tokentext.find("else-if:"))
        type = "Else-if";
    
    start = tokentext.find("$");
    end = tokentext.find("is") - 1;
    var = tokentext.substr(start, end-start);
    
    start = end+4;
    end = tokentext.find(")");
    tval = tokentext.substr(start, end-start);
    if (tval == "true")
       val = true;
    else if (tval == "false")
        val = false;
}

string IfElse::getVar()
{
    return var;
}

bool IfElse::getVal()
{
    return val;
}

void IfElse::print()
{
    cout << type << ": variable=" << var << ", value=" << tval << endl;
}

bool IfElse::match(Story* check){  
    if(check->variablePair[var] == val)
        return true;
    else
        return false;    
}

/*--------------------------------
            Else
---------------------------------*/
OnlyElse:: OnlyElse(string fl)
:Story(fl)
{
    type = "Else";
}

void OnlyElse::print()
{
    cout << type << endl;
}

/*--------------------------------
            Block
---------------------------------*/
Block:: Block(string tokentext, string fl)
:Story(fl)
{
    text = tokentext.substr(1, tokentext.size()-2);
}

void Block::print()
{
    cout << "START BLOCK" << endl;
    PassageTokenizer bt(text);
    while (bt.hasNextPart())
    {
        PartToken bp = bt.nextPart();
        if (bp.getType() == LINK)
        {
            Link lk(bp.getText(), story);
            lk.print();
        }
        else if (bp.getType() == GOTO)
        {
            GoTo gt(bp.getText(), story);
            gt.print();
        }
        else if (bp.getType() == SET)
        {
            Set st(bp.getText(), story);
            st.print();
        }
        else if ((bp.getType() == IF) || (bp.getType() == ELSEIF))
        {
            IfElse ifel(bp.getText(), story);
            ifel.print();
        }
        else if ((bp.getType() == ELSE))
        {
            OnlyElse el(story);
            el.print();
        }
        else if (bp.getType() == BLOCK)
        {
            Block bl(bp.getText(), story);
            bl.print();
        }
        else if (bp.getType() == TEXT)
        {
            Text tl(bp.getText(), story);
            tl.print();
        }
    }
    cout << "END BLOCK" << endl;
}


void Block::play(Story* ins){
PassageTokenizer bt(text);


        //inside the passage
        while (bt.hasNextPart())
        {
            PartToken bp = bt.nextPart();

            if (bp.getType() == LINK)
            {
                Link lk(bp.getText(), story);
                lk.printP();
                ins->links[lk.getPname()] = lk.getLname(); /*--new object needs to pushback in a vector--*/
                ins->hitLink = true;
            }

            else if (bp.getType() == GOTO)
            {
                GoTo gt(bp.getText(), story);
                ins->hitGoTo = true;
                ins->x = ins->passageNum.at(gt.getGname());

                break;             
            }

            else if (bp.getType() == SET)
            {
                Set st(bp.getText(), story);
                st.perform(ins); //sets variables to true or false
            }

            //uses bValue as flag value to skip or not skip next value
            else if (bp.getType() == IF || bp.getType()==ELSEIF)
            {
                if (bp.getType()==IF)
                    ins->printBlock = 0; //add in the header
                IfElse ifel(bp.getText(), story);
                ins->matchVal = ifel.match(ins); //create matchVal int in header      
            }    

            else if ((bp.getType() == ELSE))
            {
                OnlyElse el(story);
                ins->matchVal = el.match();
            }

            else if (bp.getType() == BLOCK)
            {
                Block bl(bp.getText(), story);
                if( matchVal== 1 && printBlock==0){
                    ins->printBlock = 1;
                    bl.play(ins);
                }
            }

            else if (bp.getType() == TEXT)
            {
                Text tl(bp.getText(), story);
                tl.printP();
            }

            else
            {
                cout << "  Unknown token:  " << bp.getText();
            }            
        }
}