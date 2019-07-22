/**
 * @desc just a shitty project
 * 
 * A word frequency thing
 * 
 * @author alex vo
 */


/* Includes */
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <chrono>

typedef std::map<std::string, int> stringMap;
typedef std::pair<std::string,int> pair;

#define MAX_NUM_PUNC 15                                                             /* Number of punctuations to remove */
#define MAX_BANNED_WORDS 4                                                          /* Number of words to remove */

#define INPUTFILE_NAME "chatHist.txt"
#define OUTPUTFILE_NAME "output.xls"

const char punctuation[MAX_NUM_PUNC] = {'.','?','!','*','(',')','-',
                                        '+','=','"','\'',',', ':','@'};             /* All the punctuation you want removed */
const std::string bannedWords[MAX_BANNED_WORDS] = {"pm", "am", "aileen", "nerd"};   /* All the words you want removed */    

/*      INPUT VALIDATION      */

/**
 * @brief Validates input
 * 
 * Validates input by cleaning the input up
 * Changes all uppercase to lowercase
 * Removes all punctuation
 * Checks if the end word is banned
 * 
 * @param temp - The word it is checking
 *               It is modified during the function
 * 
 * @return true if the word is not banned
 *         false if the word is banned
 */
bool validateInput(std::string& temp);
/**
 * @brief Checks if the string is a date
 * 
 * @param temp - The word it is checking
 * 
 * @return true if it is a date
 *         false if it is not a date
 */
bool isDate(const std::string temp);

/*      READING AND WRITING THE ARRAYS      */

/**
 * @brief Main processing for word frequency
 * 
 * Cycles through the inputfile and checks validates each word before
 * adding it to the map
 * 
 * @param input - The input file object
 * @param table - The table that holds everything
 * 
 * @return void
 */
void wordCounter(std::istream& input, stringMap& table);


int main()
{
    auto start = std::chrono::high_resolution_clock::now(); /* Timer for runtime */
    
    std::ifstream input(INPUTFILE_NAME); 
    std::ofstream output(OUTPUTFILE_NAME);
    std::vector<pair> sortedTable;                          /* A vector of type pair */
    stringMap table;

    wordCounter(input, table);                           

    std::copy(table.begin(), table.end(), std::back_inserter<std::vector<pair>>(sortedTable)); //Copy map to vector

    std::cout << "Sorting...";
    std::sort(sortedTable.begin(), sortedTable.end(), [](const pair& left, const pair& right) //Sort vector by frequency
    {
        if(left.second != right.second)
            return left.second > right.second;
        return left.first > right.first;
    });
    std::cout << "Done\n";

    std::cout << "Writing to file...";
    for(auto const &pair: sortedTable) //output to file
        output << pair.first << "\t" << pair.second << "\n";
    std::cout << "Done\n";


    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "\n\nTotal Runtime: " << duration.count() << " milliseconds\n\n";

    return 0;
}

bool isDate(const std::string temp)
{
    if(isdigit(temp[0]) && isdigit(temp[1])) //shitty just checks the first two 
        return true;
    else
        return false;
}

bool validateInput(std::string& temp)
{ 
    bool valid = true;
    for(int i = 0; i <= temp.length(); i++)
    {
        if(temp[i] >= 65 && temp[i] <= 92)
            temp[i] = temp[i] + 32;             //Change to lowercase

        for(int j = 0; j < MAX_NUM_PUNC; j++)
            if(temp[i] == punctuation[j])
                temp.erase(i,1);                //Erase punctuation
    }
    for(int i = 0; i < MAX_BANNED_WORDS; i++)   //Checks if the word is banned
        if(temp == bannedWords[i])
            valid = false;

    return valid;
}

void wordCounter(std::istream& input, stringMap& table)
{
    std::string temp;
    int wordCounter = 0;
    int validWords = 0;

    std::cout << "Writing to map";

    while (input >> temp)
    {
        validateInput(temp);
        if(!isDate(temp) && validateInput(temp))
        {
            ++table[temp];
            validWords++;
        }
        wordCounter++;

        if(wordCounter % 20000 == 0)
            std::cout << ".";
    }
    std::cout << "\nTotal words parsed: " << wordCounter << "\n";
    std::cout << "Total valid words: " << validWords << "\n";
    std::cout << "Total unique words: " << table.size() << "\n\n\n\n\n\n";
}
