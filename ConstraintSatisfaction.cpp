#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

int l4;           // Number of unique letters
int values[10];   // Stores the digits assigned to letters
char letters[10]; // Stores unique letters

struct Solution
{
    int attempt;
    vector<pair<char, int>> letterMapping;
    int n1, n2, n3;
};

// Function to find the position of character 'x' in string 'str'
int pos(const char str[], char x)
{
    int l = strlen(str);
    for (int i = 0; i < l; i++)
    {
        if (str[i] == x)
        {
            return i;
        }
    }
    return -1; // Return -1 if character is not found
}

// Function to add unique letters from a string 'str' to 'letters[]'
void add(const char str[])
{
    int l = strlen(str);
    for (int i = 0; i < l; i++)
    {
        bool exists = false;
        for (int j = l4 - 1; j >= 0; j--)
        {
            if (letters[j] == str[i])
            { // If letter already exists
                exists = true;
                break;
            }
        }
        if (!exists)
        { // If letter is new
            letters[l4] = str[i];
            l4++;
        }
    }
}

// Function to generate the next permutation of digit values for letters
void findnext()
{
    int i = l4 - 1;
    values[i]++;
    while (values[i] == 10)
    {
        values[i] = 0;
        i--;
        if (i < 0)
            return; // Exit if we've exceeded all permutations
        values[i]++;
    }
}

// Function to calculate factorial of 'n'
int fact(int n)
{
    if (n == 0)
        return 1;
    else
        return n * fact(n - 1);
}

// Function to calculate number of permutations: P(n, r) = n! / (n - r)!
int perm(int n, int r)
{
    return fact(n) / fact(n - r);
}

int main()
{
    int i, j, k, n1, n2, n3, l1, l2, l3, p = 0, sol = 0, tries;
    char w1[20], w2[20], w3[20];

    cout << "Enter the words and their sum:" << endl;
    cin.getline(w1, 20); // Get the first word
    cout << "+";
    cin.getline(w2, 20); // Get the second word
    cout << "=";
    cin.getline(w3, 20); // Get the sum word

    l1 = strlen(w1);
    l2 = strlen(w2);
    l3 = strlen(w3);
    l4 = 0;

    // Add unique letters from all three words
    add(w1);
    add(w2);
    add(w3);

    if (l4 > 10)
    {
        cout << "Too many unique letters, can't solve." << endl;
        return 0;
    }

    cout << "\nUnique letters: ";
    for (int i = 0; i < l4; i++)
    {
        cout << letters[i] << " ";
    }
    cout << endl;

    // Initialize values array with 0
    for (i = 0; i < l4; i++)
    {
        values[i] = 0;
    }

    tries = perm(10, l4);       // Number of possible permutations
    vector<Solution> solutions; // To store solutions

    // Loop through all possible permutations
    for (i = 1; i <= tries; i++)
    {
        if ((int)(100.0 * i / tries) > p)
        {
            p = (int)(100.0 * i / tries);
            cout << "\rSearching " << tries << " possibilities... " << p << "% complete" << flush;
        }

        findnext();

        // Check for repeated digits in the current permutation
        for (j = 0; j < l4; j++)
        {
            for (k = j + 1; k < l4; k++)
            {
                if (values[j] == values[k])
                {
                    findnext(); // Skip this permutation
                    j = -1;     // Restart the check
                    break;
                }
            }
        }

        // Convert words to numbers using current digit mapping
        n1 = 0;
        n2 = 0;
        n3 = 0;
        for (j = 0; j < l1; j++)
            n1 = (n1 * 10) + values[pos(letters, w1[j])];
        for (j = 0; j < l2; j++)
            n2 = (n2 * 10) + values[pos(letters, w2[j])];
        for (j = 0; j < l3; j++)
            n3 = (n3 * 10) + values[pos(letters, w3[j])];

        // Check if the sum of the first two words equals the third
        if (n1 + n2 == n3)
        {
            Solution s;
            s.attempt = i;
            s.n1 = n1;
            s.n2 = n2;
            s.n3 = n3;
            for (j = 0; j < l4; j++)
            {
                s.letterMapping.push_back({letters[j], values[j]});
            }
            solutions.push_back(s); // Store solution in vector
        }

        // After all tries
        if (i == tries)
        {
            if (solutions.empty())
                cout << "\n\nNo solution found\n";
            else
            {
                cout << "\n\n"
                     << solutions.size() << " solutions found\n";
                // Display all solutions
                for (int idx = 0; idx < solutions.size(); ++idx)
                {
                    cout << "\nSolution " << idx + 1 << " (found in " << solutions[idx].attempt << " tries):\n";
                    for (auto &pair : solutions[idx].letterMapping)
                    {
                        cout << pair.first << " = " << pair.second << endl;
                    }
                    cout << endl;
                    cout << w1 << " = " << solutions[idx].n1 << endl;
                    cout << w2 << " = " << solutions[idx].n2 << endl;
                    cout << w3 << " = " << solutions[idx].n3 << endl;
                }
            }
        }
    }

    return 0;
}
