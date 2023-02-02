#pragma once

#include <vector>
#include <deque>
#include "search_server.h"
#include "document.h"

class RequestQueue {
public:
    explicit RequestQueue(const SearchServer& search_server);
    // сделаем "обёртки" для всех методов поиска, чтобы сохранять результаты для нашей статистики
    template <typename DocumentPredicate>
    int AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        const int result_size_ = (search_server_.FindTopDocuments(raw_query, document_predicate)).size();
        if(requests_.size() < min_in_day_){
            requests_.push_back(result_size_);
        } else {
            requests_.pop_front();
            requests_.push_back(result_size_);
        }
        return result_size_;
    }
    int AddFindRequest(const std::string& raw_query, DocumentStatus status);
    int AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    int result_size_;
    std::deque<int> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
}; 