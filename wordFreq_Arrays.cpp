/**
 * @desc just a shitty project
 *
 * A word frequency thing
 * This time with arrays instead of a map
 *
 * @author alex vo
 */

#include <iostream>
#include <fstream>

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
 * adding it to the arrays
 * 
 * @param input - The input file object
 * @param word  - The array that holds all unique words
 * @param count - The array that holds the number of occurances for each unique word
 * 
 * @return The total number of unique words found
 */
int wordCounter(std::istream& input, std::string word[], int count[]);
/**
 * @brief Gets the total number of words from a file
 * 
 * Cycles through an inputfile and counts the number of words
 * 
 * @param input - The input file object
 * 
 * @return The total number of words inside an file
 */
int getTotalWords(std::istream& input);
/**
 * @brief Adds a word to the array
 * 
 * Searches previous entries in the array (binary) to see if the word has been added already
 * If not found, adds the word and increases the counter
 * If found, increases the frequency count for that word
 * 
 * @param input - The input file object
 * @param word  - The array that holds all unique words
 * @param count - The array that holds the number of occurances for each unique word
 * 
 * @return Returns the current position of the array
 *         aka the number of unique words added
 */
int addToArrays(std::string temp, std::string word[], int count[]);

/*      ARRAY SORTING       */

/* Dont bother trying to understand it lol just look up quicksort */
int partition (int count[], std::string word[], int low, int high);
void quickSort(int count[], std::string word[], int low, int high);


int main()
{
    std::ifstream input(INPUTFILE_NAME);
    std::ofstream output(OUTPUTFILE_NAME);
    std::string * word = nullptr;                           /* Initialization of word array */
    int * count = nullptr;                                  /* Initialization of counter array */
    unsigned int MAX_WORDS = 0;
    unsigned int uniqueWords;

    MAX_WORDS = getTotalWords(input);

    word = new std::string[MAX_WORDS];                      /* Look up dynamic allocation for this */
    count = new int[MAX_WORDS];

    uniqueWords = wordCounter(input, word, count);

    std::cout << "\nSorting inputs...";
    quickSort(count, word, 0, uniqueWords);
    std::cout << "Done\n";

    std::cout << "Writing to file...";
    for(int i = 0; i < uniqueWords; i++)
        output << word[i] << '\t' << count[i] << '\n';
    std::cout << "Done\n";

    delete [] word;                                         /* Make sure to delete these to free up memory */
    delete [] count;                                        /* Technically not required but it's nice to have */

    return 0;
}

bool isDate(const std::string temp)
{
    if(isdigit(temp[0]) && isdigit(temp[1])) //shitty just checks the first two spots
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

int wordCounter(std::istream& input, std::string word[], int count[])
{
    std::string temp;
    int wordCounter = 0;
    int validWords = 0;
    int uniqueWords = 0;

    std::cout << "Writing to arrays";

    while (input >> temp)
    {
        validateInput(temp);
        if(!isDate(temp) && validateInput(temp))
        {
            uniqueWords = addToArrays(temp, word, count);
            validWords++;
        }
        wordCounter++;

        if(wordCounter % 20000 == 0)
            std::cout << ".";
    }
    std::cout << "\nTotal words parsed: " << wordCounter << "\n";
    std::cout << "Total valid words: " << validWords << "\n";
    std::cout << "Total unique words: " << uniqueWords << "\n\n\n\n\n\n";

    return uniqueWords;
}

int getTotalWords(std::istream& input)
{
    int count = 0;
    std::string temp;
    while(input >> temp)
        count++;
    input.clear();
    input.seekg(0, std::ios::beg);	//Resets spot in inputfile
    return count;
}

int addToArrays(std::string temp, std::string word[], int count[])
{
    static int index = 0;
    bool found = false;
    unsigned int foundAt = 0;

    for(int i = 0; i < index; i++)
        if (word[i] == temp)
        {
            found = true;
            foundAt = i;
            break;
        }

    if(found)
    {
        if(count[foundAt])
            count[foundAt]++;
    }
    else
    {
        word[index] = temp;
        count[index] = 1;
        index++;
    }

    return index;
}

int partition (int count[], std::string word[], int low, int high)  
{  
    int pivot = count[high]; 
    int i = (low - 1); 
  
    for (int j = low; j <= high - 1; j++)  
    {  
        if (count[j] >= pivot)  
        {  
            i++; 
            std::swap(count[i], count[j]);
            std::swap(word[i], word[j]);
        }  
    }  
    std::swap(count[i + 1], count[high]);
    std::swap(word[i + 1], word[high]);
    return (i + 1);  
}  
  
void quickSort(int count[], std::string word[], int low, int high)  
{  
    if (low < high)  
    {  
        int pi = partition(count, word, low, high);  
  
        quickSort(count, word, low, pi - 1);  
        quickSort(count, word, pi + 1, high);  
    } 
}  
