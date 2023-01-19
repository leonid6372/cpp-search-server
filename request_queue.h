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
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentPredicate document_predicate) {
        const QueryResult result(search_server_.FindTopDocuments(raw_query, document_predicate));
        if(requests_.size() < min_in_day_){
            requests_.push_back(result);
        } else {
            requests_.pop_front();
            requests_.push_back(result);
        }
        return result.query_result_;
    }
    std::vector<Document> AddFindRequest(const std::string& raw_query, DocumentStatus status);
    std::vector<Document> AddFindRequest(const std::string& raw_query);
    int GetNoResultRequests() const;
private:
    struct QueryResult {
        QueryResult(const std::vector<Document>& temp){
            query_result_ = temp;
        }
        std::vector<Document> query_result_;
    };
    std::deque<QueryResult> requests_;
    const static int min_in_day_ = 1440;
    const SearchServer& search_server_;
}; 