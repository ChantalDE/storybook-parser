#include <iostream>
#include <string>

#include "StoryTokenizers.h"

using namespace std;

/*-----------------------------------------
PassageToken --- create Passage Class
-------------------------------------------*/
PassageToken::PassageToken(){
    name = "";
    text = "";
}

PassageToken::PassageToken(string name, string text){
    this->name = name;
    this->text = text;
}

string PassageToken::getName() const{
    return name;
}

string PassageToken::getText() const{
    return text;
}

/*-----------------------------------------
StoryTokenizer --- extract passage Class
-------------------------------------------*/
StoryTokenizer::StoryTokenizer(){
    st = "";
}

StoryTokenizer::StoryTokenizer(string st){
    this->st = st;
}

PassageToken StoryTokenizer::nextPassage(){

    cursor = st.find("pid", cursor); //get to position to find passage 
    cursor = st.find("name", cursor); //get to position before name
    begin = st.find("\"", cursor) + 1; //go to beginning of name
    cursor = begin; //update cursor
    end = st.find("\"", cursor); //go to end of name
    cursor = end; // update cursor
    string nameA = st.substr(begin, end - begin); // substring name

    begin = st.find(">", cursor) + 1; // go to begining of text
    cursor = begin; // update cursor
    end = st.find("<", cursor); // go to end of text
    string textA = st.substr(begin, end - begin); // substring entire text

    PassageToken token(nameA, textA);

return token;
}

bool StoryTokenizer::hasNextPassage(){
    
    int check = st.find(open, cursor);

    if(check < 0){
      next = false;
    }

    return next;
}


/*-----------------------------------------
PartToken --- Create Token Class
-------------------------------------------*/
PartToken::PartToken(){
    text = "";
    type = {};
}

PartToken::PartToken(type_t type, string text){
    this->type = type;
    this->text = text;
}

type_t PartToken::getType() const{
    return type;
}

string PartToken::getText() const{
    return text;
}



/*-----------------------------------------
PassageTokenizer --- Extract Token Class
-------------------------------------------*/

PassageTokenizer::PassageTokenizer(){
    passage = " ";
}

PassageTokenizer::PassageTokenizer(string passage){
    this->passage = passage;
}

PartToken PassageTokenizer::nextPart(){
    string text;
    type_t tokenType;

    //detect Links
   if (passage.substr(cursor,2) == openLink){        
        tokenType = LINK;
        text = passage.substr(passage.find(openLink, cursor), passage.find(closeLink, cursor) - passage.find(openLink, cursor) + 2);

        cursor = passage.find(closeLink, cursor) + 2; // update cursor
    }

    //detect Blocks
    else if (passage.substr(cursor, 1) == openBlock){   
        tokenType = BLOCK;
                
        int bCount = 0;
        int i;

        i = passage.find('[', cursor);
        bCount++;
                
        while(bCount != 0){
            i++;
            if(passage[i] == '['){
                bCount++;
            }
            else if(passage[i] == ']'){
                bCount--;
            }
        }               
        i++;
        text = passage.substr(cursor, i-cursor);
        cursor = i;
    }
        

    //detect commands
    else if (passage.substr(cursor,1) == openCommand){
        int p1;
        int p2;
        string command;

        p1 = passage.find(openCommand, cursor) + 1;
        p2 = passage.find(":", cursor) -1;
        command = passage.substr(p1, p2 - p1 + 1);

        if(command == "go-to"){
            tokenType = GOTO;
        }

        else if(command == "set"){
            tokenType = SET;
        }

        else if(command == "if"){
            tokenType = IF;
        }

        else if(command == "else-if"){
            tokenType = ELSEIF;
        }

        else if(command == "else"){
            tokenType = ELSE;
        }

       text = passage.substr(passage.find(openCommand, cursor), passage.find(closeCommand, cursor) - passage.find(openCommand, cursor) + 1);
       cursor = passage.find(closeCommand, cursor) + 1; // update cursor
    }

    //detect text       
    else{
        tokenType = TEXT;

        if(passage.find(openBlock, cursor) > passage.find(openCommand, cursor)){
            text = passage.substr(cursor, passage.find(openCommand, cursor) - cursor);
            cursor = passage.find(openCommand, cursor); // update cursor
        }
        else if (passage.find(openBlock, cursor) < passage.find(openCommand, cursor)){
            text = passage.substr(cursor, passage.find(openBlock, cursor) - cursor);
            cursor = passage.find(openLink, cursor); // update cursor
        }
        else{
            text = passage.substr(cursor, passage.length() - cursor);
            cursor = passage.length();
        }
    }

    PartToken pt(tokenType, text);

return pt;
}

bool PassageTokenizer::hasNextPart(){   
    if(cursor == passage.length())
        next = false;

    return next;
}





