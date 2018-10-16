#include "split.h"

using namespace std;
int split(const string &txt, vector<string> &strs)
{
	char ch = ' ';
    int pos = txt.find(ch);
    int initialPos = 0;
    strs.clear();

    while(pos != string::npos) 
    {
        strs.push_back(txt.substr(initialPos, pos - initialPos));
        initialPos = pos + 1;

        pos = txt.find(ch, initialPos);
    }

    strs.push_back(txt.substr(initialPos, min(pos, (int)txt.size()) - initialPos + 1));

    return strs.size();
}