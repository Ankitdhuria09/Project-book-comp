#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include <unordered_set>
#include <iomanip>
using namespace std;

static vector<string> bookNames = {
    "Cats by Moncrif.txt",
    "Foxes Book of Martyrs Part 1.txt",
    "Foxes Book of Martyrs Part 2.txt",
    "Foxes Book of Martyrs Part 3.txt",
    "Foxes Book of Martyrs Part 4.txt",
    "Foxes Book of Martyrs Part 5.txt",
    "Foxes Book of Martyrs Part 6.txt",
    "Gerards Herbal Vol. 1.txt",
    "Gerards Herbal Vol. 2.txt",
    "Gerard's Herbal Vol. 3.txt",
    "Gerards Herbal Vol.4.txt",
    "Gil Blas.txt",
    "Gossip in a Library.txt",
    "Hudibras.txt",
    "King of the Beggars.txt",
    "Knocknagow.txt",
    "Les Chats par Moncrif.txt",
    "Love and Madness - Herbert Croft.txt",
    "Lives and Anecdotes of Misers.txt",
    "Memoirs of Laetitia Pilkington V 1.txt",
    "Memoirs of Laetitia Pilkington V 2.txt",
    "Memoirs of Laetitia Pilkington V 3.txt",
    "Memoirs of Mrs Margaret Leeson - Peg Plunkett.txt",
    "Monro his Expedition.txt",
    "Mrs Beetons Book of Household Management.txt",
    "Out of the Hurly-Burly.txt",
    "Percys Reliques.txt",
    "Pompey the Little.txt",
    "Radical Pamphlets from the English Civil War.txt",
    "Scepsis Scientifica.txt",
    "The Anatomy of Melancholy Part 1.txt",
    "The Anatomy of Melancholy Part 2.txt",
    "The Anatomy of Melancholy Part 3.txt",
    "The Complete Cony-catching.txt",
    "The Consolation of Philosophy.txt",
    "The Covent Garden Calendar.txt",
    "The Devil on Two Sticks.txt",
    "The Diary of a Lover of Literature.txt",
    "The History Of Ireland - Geoffrey Keating.txt",
    "The History of the Human Heart.txt",
    "The Ingoldsby Legends.txt",
    "The Life of Beau Nash.txt",
    "The Life of john Buncle by Thomas Amory.txt",
    "The Life of King Richard III.txt",
    "The Life of Pico della Mirandola.txt",
    "The Martyrdom of Man.txt",
    "The Masterpiece of Aristotle.txt",
    "The Memoirs of Count Boruwlaski.txt",
    "The Metamorphosis of Ajax.txt",
    "The Newgate Calendar - Supplement 3.txt",
    "The Newgate Calendar Supplement 2.txt",
    "The Newgate Calendar Supplement.txt",
    "The Newgate Calendar V 1.txt",
    "The Newgate Calendar V 2.txt",
    "The Newgate Calendar V 3.txt",
    "The Newgate Calendar V 4.txt",
    "The Newgate Calendar V 5.txt",
    "The Newgate Calendar V 6.txt",
    "The Poems of Ossian.txt",
    "The Poetical Works of John Skelton.txt",
    "The Protestant Reformation.txt",
    "The Real Story of John Carteret Pilkington.txt",
    "The Rowley Poems.txt",
    "The Silver Fox.txt"
};

class BookAnalyzer {
private:
    map<string, vector<string>> bookWords;
    map<string, map<string, int>> wordCountMap;
    map<string, map<string, double>> normalizedScores;
    set<string> excludedWords;

    void initializeExcludedWords() {
        excludedWords = {"A","AN","THE","OF","IN"};
    }

public:
    BookAnalyzer() {
        initializeExcludedWords();
    }

    void loadBookWords() {
        cout << "Loading books..." << endl;

        for (const string& bookName : bookNames) {
            ifstream file("books/" + bookName);
            if (file) {
                vector<string> words;
                string word;

                while (file >> word) {
                    transform(word.begin(), word.end(), word.begin(), ::toupper);
                    word.erase(remove_if(word.begin(), word.end(), ::ispunct), word.end());

                    if (excludedWords.find(word) == excludedWords.end() && !word.empty()) {
                        words.push_back(word);
                    }
                }
                bookWords[bookName] = words;
            } else {
                cerr << "Error opening file: " << bookName << endl;
            }
        }
        cout << "Books loaded successfully!" << endl << endl;
    }

    void countWords() {
        cout << "Counting words..." << endl;

        for (const auto& [bookName, words] : bookWords) {
            map<string, int> counts;
            for (const string& word : words) {
                counts[word]++;
            }
            counts["total"] = words.size();
            wordCountMap[bookName] = counts;
        }
        cout << "Word counts calculated!" << endl << endl;
    }

    void calculateNormalizedScores() {
        cout << "Calculating normalized word scores..." << endl;

        for (const auto& [bookName, counts] : wordCountMap) {
            map<string, double> scores;
            int totalWords = counts.at("total");

            for (const auto& [word, count] : counts) {
                if (word != "total") {
                    scores[word] = static_cast<double>(count) / totalWords;
                }
            }
            normalizedScores[bookName] = scores;
        }
        cout << "Normalized scores calculated!" << endl << endl;
    }

    void findTopWords() {
        cout << "Identifying the top words..." << endl;

        ofstream outFile("outputFiles/common_words.txt");
        outFile << "Most Common Words in Each Book\n";
        outFile << "=================================\n\n";

        for (const auto& [bookName, scores] : normalizedScores) {
            vector<pair<string, double>> sortedWords(scores.begin(), scores.end());

            sort(sortedWords.begin(), sortedWords.end(),
                 [](const pair<string, double>& a, const pair<string, double>& b) {
                     return a.second > b.second;
                 });

            outFile << "Book: " << bookName << "\n" << string(80, '-') << "\n";
            outFile << "Top " << min(100, static_cast<int>(sortedWords.size())) << " most frequent words:\n\n";

            for (size_t i = 0; i < min(static_cast<size_t>(100), sortedWords.size()); ++i) {
                outFile << (i + 1) << ". " << left << setw(30) << sortedWords[i].first 
                        << " (Frequency: " << fixed << setprecision(4) << sortedWords[i].second << ")\n";
            }
            outFile << "\n\n";
        }
        outFile.close();
        cout << "Top words written to outputFiles/common_words.txt!" << endl << endl;
    }

    void compareBooks() {
        cout << "Comparing books..." << endl;

        ofstream outFile("outputFiles/similarity_matrix.txt");
        if (!outFile) {
            cerr << "Error opening outputFiles/similarity_matrix.txt for writing" << endl;
            return;
        }

        outFile << "Book Similarity Matrix\n" << string(100, '=') << endl << endl;

        vector<string> bookList;
        for (const auto& pair : normalizedScores) {
            bookList.push_back(pair.first);
        }

        for (size_t i = 0; i < bookList.size(); ++i) {
            for (size_t j = i + 1; j < bookList.size(); ++j) {
                const string& book1 = bookList[i];
                const string& book2 = bookList[j];

                unordered_set<string> wordsInBook1(normalizedScores[book1].begin(), normalizedScores[book1].end());
                int matchCount = 0;

                for (const auto& word : normalizedScores[book2]) {
                    if (wordsInBook1.count(word.first)) {
                        matchCount++;
                    }
                }

                double similarity = static_cast<double>(matchCount) / min(100.0, static_cast<double>(wordsInBook1.size()));
                outFile << "Book Pair: " << book1 << " and " << book2 
                        << " | Similarity: " << fixed << setprecision(4) << similarity << endl;
                outFile << string(80, '-') << endl;
            }
        }
        outFile.close();
        cout << "Book comparisons complete! Results written to outputFiles/similarity_matrix.txt" << endl << endl;
    }

    void findSimilarBooks() {
        cout << "Finding the top 10 similar books..." << endl;

        vector<pair<string, double>> similarities;
        ifstream inFile("outputFiles/similarity_matrix.txt");
        string line;

        while (getline(inFile, line)) {
            if (line.find("Similarity:") != string::npos) {
                size_t pos = line.find("and");
                string bookPair = line.substr(0, pos);
                double similarity = stod(line.substr(line.find_last_of(' ') + 1));
                similarities.emplace_back(bookPair, similarity);
            }
        }
        inFile.close();

        sort(similarities.begin(), similarities.end(),
             [](const pair<string, double>& a, const pair<string, double>& b) {
                 return a.second > b.second;
             });

        ofstream outFile("outputFiles/top_similar_books.txt");
        outFile << "Top 10 Similar Books\n";
        outFile << "=====================\n\n";

        for (size_t i = 0; i < min(static_cast<size_t>(10), similarities.size()); ++i) {
            outFile << (i + 1) << ". " << similarities[i].first << " | Similarity: " << fixed << setprecision(4) << similarities[i].second << endl;
        }
        outFile.close();
        cout << "Top similar books written to outputFiles/top_similar_books.txt" << endl << endl;
    }
};

int main() {
    BookAnalyzer analyzer;
    analyzer.loadBookWords();
    analyzer.countWords();
    analyzer.calculateNormalizedScores();
    analyzer.findTopWords();
    analyzer.compareBooks();
    analyzer.findSimilarBooks();
    return 0;
}
