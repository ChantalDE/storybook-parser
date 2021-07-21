#include "story.h"
#include "parts.h"

#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

Story:: Story(string file)
{
    this->file = file;
    string line;

    ifstream in(file);

    getline(in, line);
    while (in && line != "</html>")
    {
        story += line + '\n';
        getline(in, line);
    }

    StoryTokenizer st(story);
    int i = 0;
    while (st.hasNextPassage())
    {
        PassageToken* ptok = new PassageToken(st.nextPassage());
        passageNum[ptok->getName()] = i; //accessed Number w/ passage
        passageVector.push_back(ptok);
        i++;
    }
}

void Story:: print()
{
    StoryTokenizer st(story);
    while (st.hasNextPassage())
    {
        PassageToken ptok = st.nextPassage();
        PassageTokenizer pt(ptok.getText());

        cout << "Passage " << ptok.getName() << ":  " << endl;
        while (pt.hasNextPart())
        {
            PartToken stok = pt.nextPart();
            if (stok.getType() == LINK)
            {
                Link lk(stok.getText(), story);
                lk.print();
            }
            else if (stok.getType() == GOTO)
            {
                GoTo gt(stok.getText(), story);
                gt.print();
            }
            else if (stok.getType() == SET)
            {
                Set st(stok.getText(), story);
                st.print();
            }
            else if ((stok.getType() == IF) || (stok.getType() == ELSEIF))
            {
                IfElse ifel(stok.getText(), story);
                ifel.print();
            }
            else if ((stok.getType() == ELSE))
            {
                OnlyElse el(story);
                el.print();
            }
            else if (stok.getType() == BLOCK)
            {
                Block bl(stok.getText(), story);
                bl.print();
            }
            else if (stok.getType() == TEXT)
            {
                Text tl(stok.getText(), story);
                tl.print();
            }
            else
            {
                cout << "  Unknown token:  " << stok.getText();
            }
        }
    }
}

void Story::play(){

    StoryTokenizer st(story);

    while (endStory == false){ //while in reach passage

        PassageToken ptok(*passageVector[pos]); //create Passage object through passageVector start @passage 0

        PassageTokenizer pt(ptok.getText());

        hitLink = false;
        hitGoTo = false;
        //inside the passage
        cout << "\n\n" << ptok.getName() << ":  " << endl;
        while (pt.hasNextPart())
        {
            PartToken stok = pt.nextPart();

            if (stok.getType() == LINK)
            {
                Link lk(stok.getText(), story);
                lk.printP();
                links[lk.getPname()] = lk.getLname(); /*--new object needs to pushback in a vector--*/
                hitLink = true;
            }

            else if (stok.getType() == GOTO)
            {
                GoTo gt(stok.getText(), story);
                hitGoTo = true;
                x = passageNum.at(gt.getGname());
                break;           
            }

            else if (stok.getType() == SET)
            {
                Set st(stok.getText(), story);
                st.perform(this); //sets variables to true or false
            }

            //uses bValue as flag value to skip or not skip next value
            else if (stok.getType() == IF || stok.getType()==ELSEIF)
            {
                if (stok.getType()==IF)
                    printBlock = false;
                IfElse ifel(stok.getText(), story);
                matchVal = ifel.match(this); //create matchVal int in header                  
            }    

            else if ((stok.getType() == ELSE))
            {
                OnlyElse el(story);
                matchVal = el.match();
            }

            else if (stok.getType() == BLOCK)
            {
                Block bl(stok.getText(), story);
                if( matchVal== 1 && printBlock==0){
                    //bl.print();
                    bl.play(this);
                    printBlock = true;                    
                }
            }

            else if (stok.getType() == TEXT)
            {
                Text tl(stok.getText(), story);
                tl.printP();
            }

            else
            {
                cout << "  Unknown token:  " << stok.getText();
            }           
        }


             
        
        //break's out of loop, terminates story when paragraph does not have links or goTo's
        if(hitGoTo == false && hitLink == false){
            endStory = true;
            break;
        }

        //if hit GoTo, it goes to this paragraph by itself
        if(hitGoTo == true){
            pos = x;
        }

        //prints out links paired map to display for user
        else{
            int i = 0;
            //for(auto it = links.begin(); it!= links.end(); it++){
                for(auto it : links){
                cout << "\n" << i + 1 << ": " << it.second;;
                numLink[i + 1] = it.first; //pairing passageName w/ number
                i++;
             }

            //user typing in link number, and accessing position of passage
            int userChoice;
            cout << "\nEnter the corresponding number: ";
            cin >> userChoice;
            while (userChoice > links.size() || userChoice <= 0)
            {
                cout << "Invalid entry. choose someting in range: ";
                cin >> userChoice;
                if(userChoice <= links.size() && userChoice > 0)
                    break;
            }
            
            //cout << numLink.at(userChoice); //should print out the passageName
        
            pos = passageNum.at(numLink.at(userChoice));

            //resets links and numLink unordered map
            links.clear(); 
            numLink.clear();
        }
   }


}
 
