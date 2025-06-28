#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

void filterYellow(vector<string> &lineCandidates, const string &keyword, const string &pattern)
{
    for (int i = 0; i < 5; i++)
    {
        if (pattern[i] == '1')
        {
            for (int j = 0; j < lineCandidates.size(); j++)
            {
                if (lineCandidates[j][i] == pattern[i] || lineCandidates[j].find(keyword[i]) == string::npos)
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
                if (keyword.find(lineCandidates[i][j]) != string::npos)
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

void addCandidates(vector<string> (&candidates)[6], const vector<string> (&words)[5][26], const string &keyword, const string pattern[6])
{

    for (int i = 0; i < 6; i++)
    {
        prePopulateGreen(candidates[i], words, keyword, pattern[i]);

        if (candidates[i].empty())
            for (int i = 0; i < 5; i++)
                for (int j = 0; j < 26; j++)
                    candidates[i].insert(candidates[i].end(), words[i][j].begin(), words[i][j].end());

        filterYellow(candidates[i], keyword, pattern[i]);
        filterGray(candidates[i], keyword, pattern[i]);
    }
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
    cin >> keyword;
    if (keyword.length() != 5)
    {
        cerr << "Input must be 5 characters long." << endl;
        return 1;
    }
    // 0 = gray, 1 = yellow, 2 = green, X = either green or yellow
    string pattern[6] = {
        "21112",
        "21112",
        "10001",
        "01110",
        "10001",
        "22222"};
    vector<string> candidates[6];
    addCandidates(candidates, words, keyword, pattern);

    for (int i = 0; i < 6; i++)
    {
        cout << "Candidates for pattern " << pattern[i] << ":" << endl;
        int count = 0;
        for (const auto &candidate : candidates[i])
        {
            count++;
            if (count > 10)
                break;
            cout << candidate << endl;
        }
        cout << endl;
    }

    return 0;
}