#include <algorithm>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

const char char_space = ' ', char_minus = '-';

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == char_space) {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        } else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

struct Query {
    set<string> plus_words;
    set<string> minus_words;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        
        const double inv_word_count = 1.0 / words.size();
        for (const string& word : words)    
            word_to_documents_freqs_[word][document_id] += inv_word_count;
        
        ++document_count_;
    }

    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query query = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(query);

        sort(matched_documents.begin(), matched_documents.end(),
             [](const Document& lhs, const Document& rhs) {
                 return lhs.relevance > rhs.relevance;
             });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    map<string, map<int, double>> word_to_documents_freqs_;
    set<string> stop_words_;
    int document_count_ = 0;
    
    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query query;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if(word[0] != char_minus)
                query.plus_words.insert(word);
            else
                query.minus_words.insert(word.substr(1));
        }
        return query;
    }
    
    double CalculateIDF(const int& document_count, const map<string, map<int, double>>& word_to_documents_freqs, const string& word) const{
        return log(static_cast<double>(document_count) / word_to_documents_freqs.at(word).size());
    }
    
    vector<Document> FindAllDocuments(const Query& query) const {
        vector<Document> matched_documents;
        map<int, double> document_to_relevance;
        double idf;
   
        for(const auto& word : query.plus_words)
            if(word_to_documents_freqs_.count(word)){
                idf = CalculateIDF(document_count_, word_to_documents_freqs_, word);
                for(const auto& [id, tf] : word_to_documents_freqs_.at(word))
                    document_to_relevance[id] += tf * idf;
            }
        
        for(const auto& word : query.minus_words)
            if(word_to_documents_freqs_.count(word))
                for(const auto& [id, tf] : word_to_documents_freqs_.at(word))
                    document_to_relevance.erase(id);
        
        for(const auto& [id, relevance] : document_to_relevance)
            matched_documents.push_back({id, relevance});
        
        return matched_documents;
    }
};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", "
             << "relevance = "s << relevance << " }"s << endl;
    }
}
