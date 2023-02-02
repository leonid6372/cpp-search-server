#include "remove_duplicates.h"

using namespace std;

void RemoveDuplicates(SearchServer& search_server){
    set<int> duplicate_id;
    vector<pair<set<string>, int>> temp;
    for(auto document_id : search_server){
        set<string> words;
        for(const auto [word, freq] : search_server.GetWordFrequencies(document_id)){
            words.insert(word);
        }
        temp.push_back({words, document_id});
    }
    sort(temp.begin(), temp.end());
    for(auto it = next(temp.begin(), 1); it != temp.end(); ++it){
        if((*prev(it, 1)).first == (*it).first){
            duplicate_id.insert((*it).second);
        }
    }
    for(auto document_id : duplicate_id){
        cout << "Found duplicate document id " << document_id << endl;
        search_server.RemoveDocument(document_id);
    }
}