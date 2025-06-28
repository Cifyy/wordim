#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <set>

using namespace std;

void eliminateMultipleCharacters(vector<string> &lineCandidates, const string &pattern)
{
    for (int j = 0; j < lineCandidates.size(); j++)
    {
        set<char> uniqueChars;
        for (int i = 0; i < 5; i++)
        {
            if (pattern[i] == '2' || pattern[i] == '1' || pattern[i] == 'X')
            {
                if (uniqueChars.find(lineCandidates[j][i]) != uniqueChars.end())
                {
                    lineCandidates.erase(lineCandidates.begin() + j);
                    j--;
                    break;
                }
                uniqueChars.insert(lineCandidates[j][i]);
            }
        }
    }
}

void filterYellow(vector<string> &lineCandidates, const string &keyword, const string &pattern)
{
    for (int i = 0; i < 5; i++)
    {
        if (pattern[i] == '1')
        {
            for (int j = 0; j < lineCandidates.size(); j++)
            {
                if (keyword.find(lineCandidates[j][i]) == string::npos || lineCandidates[j][i] == keyword[i])
                {
                    lineCandidates.erase(lineCandidates.begin() + j);
                    j--;
                }
            }
        }
        else if (pattern[i] == 'X')
        {
            for (int j = 0; j < lineCandidates.size(); j++)
            {
                if (keyword.find(lineCandidates[j][i]) == string::npos)
                {
                    lineCandidates.erase(lineCandidates.begin() + j);
                    j--;
                }
            }
        }
    }
}

void filterGray(vector<string> &lineCandidates, const string &keyword, const string &pattern)
{
    for (int i = 0; i < 5; i++)
    {
        if (pattern[i] == '0')
        {
            for (int j = 0; j < lineCandidates.size(); j++)
            {
                if (keyword.find(lineCandidates[j][i]) != string::npos)
                {
                    lineCandidates.erase(lineCandidates.begin() + j);
                    j--;
                }
            }
        }
    }
}

void prePopulateGreen(vector<string> &lineCandidates, const vector<string> (&words)[5][26], const string &keyword, const string &pattern)
{
    int i;
    for (i = 0; i < 5; i++)
        if (pattern[i] == '2')
        {
            lineCandidates.insert(lineCandidates.end(), words[i][keyword[i] - 'a'].begin(), words[i][keyword[i] - 'a'].end());
            break;
        }
    i++;

    for (i; i < 5; i++)
    {
        if (pattern[i] == '2')
        {
            for (int j = 0; j < lineCandidates.size(); j++)
            {
                if (lineCandidates[j][i] != keyword[i])
                {
                    lineCandidates.erase(lineCandidates.begin() + j);
                    j--;
                }
            }
        }
    }
}

void addCandidates(vector<string> (&candidates)[6], const vector<string> (&words)[5][26], const string &keyword, const string pattern[6], const bool only_unique = false)
{
    cout << "0/6" << endl;
    for (int i = 0; i < 6; i++)
    {
        prePopulateGreen(candidates[i], words, keyword, pattern[i]);

        if (candidates[i].empty())
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 26; j++)
                    candidates[i].insert(candidates[i].end(), words[i][j].begin(), words[i][j].end());

        filterYellow(candidates[i], keyword, pattern[i]);
        filterGray(candidates[i], keyword, pattern[i]);

        if (only_unique)
            eliminateMultipleCharacters(candidates[i], pattern[i]);

        system("cls");
        cout << i + 1 << "/6" << endl;
    }
    system("cls");
}

int main()
{
    vector<string> words[5][26];

    ifstream infile("valid-wordle-words.txt");
    string line;
    while (getline(infile, line))
    {
        for (int i = 0; i < 5; i++)
        {
            words[i][line[i] - 'a'].push_back(line);
        }
    }
    infile.close();

    string keyword;
    cout << "Enter the keyword: ";
    cin >> keyword;
    if (keyword.length() != 5)
    {
        cerr << "Input must be 5 characters long." << endl;
        return 1;
    }
    system("cls");
    // 0 = gray, 1 = yellow, 2 = green, X = either green or yellow
    string pattern[6] = {
        "21112",
        "21112",
        "10001",
        "01110",
        "10001",
        "22222"};
    vector<string> candidates[6];
    addCandidates(candidates, words, keyword, pattern, only_unique);

    for (int i = 0; i < 6; i++)
    {
        // cout << candidates[i].size() << " candidates found for pattern " << pattern[i] << ":" << endl;
        // int count = 0;
        // for (const auto &candidate : candidates[i])
        // {
        // count++;
        // if (count > 10)
        // break;
        // cout << candidate << endl;
        // }
        // cout << endl;
        if (candidates[i].empty())
        {
            cout << "No candidates found for pattern " << pattern[i] << "." << endl;
        }
        else
        {
            cout << i + 1 << ". " << candidates[i][0] << endl;
        }
    }

    return 0;
}