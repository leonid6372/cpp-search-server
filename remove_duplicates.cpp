#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server){
    set<int> duplicate_id;
    map<set<string>, int> words_to_id;
    for(const auto document_id : search_server){
        set<string> words;
        for(const auto [word, freq] : search_server.GetWordFrequencies(document_id)){
            words.insert(word);
        }
        if(words_to_id.count(words)){
            duplicate_id.insert(document_id);
        } else {
            words_to_id.insert({words, document_id});
        }
    }
    for(const auto document_id : duplicate_id){
        cout << "Found duplicate document id " << document_id << endl;
        search_server.RemoveDocument(document_id);
    }
}