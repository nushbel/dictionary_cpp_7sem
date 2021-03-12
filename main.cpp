#include <iostream>
#include <string>
#include <set>
#include <fstream>
#include <locale>
#include <vector>

/**
 * creates a vector of words with Levenshtein distance to our fixed word equal to 1
 * @param similarWords
 * @param word
 */
void createSimilarWords(std::set<std::string>& similarWords, std::string word)
{
    std::string temp;
    std::string temp2;
    std::string temp3;
    for (char character = 'a'; character <= 'z'; ++character)
    {
        for (int i = 0; i < word.length(); ++i)
        {
            temp = word;
            temp2 = word;
            temp3 = word;
            similarWords.insert(temp.insert(i, 1, character));
            similarWords.insert(temp2.replace(i, 1, 1, character));
            similarWords.insert(temp3.erase(i, 1));
        }
    }
}
/**
 * reads text from input file and works with it, writes new file with corrected words
 * @param text
 * @param out
 * @param dict
 */
void readAndWriteText(std::istream& text, std::ostream& outputf, std::set<std::string>& dict) {
    std::string word1;
    char punct;
    std::set<char> punctChars{'.', ',', '!', '?', ';', ':'};
    while (text >> word1)
    {
        bool uppercaseCheck = false;
        if (word1[0] >= 'A' and word1[0] <= 'Z')
        {
             uppercaseCheck = true;
        }
        //to low case
        std::locale loc;
        std::string word = "";
        for (auto elem : word1)
        {
            word = word + std::tolower(elem, loc);
        }

        //check if there is some punctuation
        if (punctChars.find(word[word.size() - 1]) != punctChars.end())
        {
            punct = word[word.size() - 1];
            word.erase(word.size() - 1);
        } else {
            punct = 0;
        }

        if (dict.find(word) == dict.end()) {

            std::set<std::string> similarWords;
            createSimilarWords(similarWords, word);
            std::vector<std::string> similarWordsInDic;
            /*
             for (auto i = similarWords.begin(); i != similarWords.end(); ++i)
            {
                std::cout << *i << "\n";
            }
             */
            for (auto i = similarWords.begin(); i != similarWords.end(); ++i)
            {
                if(dict.find(*i) != dict.end())
                {
                    similarWordsInDic.push_back(*i);
                }
            }

            int num_answ = 0;
            if (!similarWordsInDic.empty())
            {
                std::cout << "Probably by \"" << word << "\" you meant" << std::endl;
                for (int i = 0; i < similarWordsInDic.size(); ++i)
                {
                    std::cout << i + 1 << ". " << similarWordsInDic[i] << std::endl;
                }
                std::cout << "(Type a word number or 0)" << std::endl;
                std::cin.clear();
                std::cin >> num_answ;
                if (num_answ != 0) {
                    word = similarWordsInDic[num_answ - 1];
                }
            }
            if (num_answ == 0)
            {
                std::cout << "The word \"" << word << "\" doesn't exist in dictionary" << std::endl
                          << "Do you want to add it?" << std::endl;
                std::cout << "(Type 'y' or 'n')" << std::endl;
                char answ;
                std::cin.clear();
                std::cin >> answ;
                if (answ == 'y') {
                    dict.insert(word);
                }
            }
        }
        if (uppercaseCheck == true)
        {
            char a = toupper(word[0]);
            word.replace(0, 1, 1, a);
        }
        outputf << word + punct + " ";
    }
    std::ofstream updated_dict("dictionary.txt");
    for (auto i = dict.begin(); i != dict.end(); ++i)
    {
        updated_dict << *i << "\n";
    }
}

int main() {
    std::ifstream dict("dictionary.txt");
    if (dict.is_open()){
        // std::cout << "Файл открыт" << std::endl;
    }
    std::string word;
    std::set<std::string> my_dictionary;
    while (dict >> word)
    {
        //std::cout << word << std::endl;
        my_dictionary.insert(word);
    }

    std::ifstream text("text.txt");
    std::ofstream outputf("result.txt");

    readAndWriteText(text, outputf, my_dictionary);

    return 0;
}