#include <string>
#include <iostream>
#include "story.h"
using namespace std;

const string FILENAME = "long_input.txt";

int main()
{
  Story* story = new Story(FILENAME);
  story->play();

  delete story;
  return 0;
}
