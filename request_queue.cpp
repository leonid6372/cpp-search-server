#include "request_queue.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server)
        : search_server_(search_server)
    {
    }

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        return AddFindRequest(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
        return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
    }

int RequestQueue::GetNoResultRequests() const {
        int amount = 0;
        for(const auto& request : requests_){
            if(request.query_result_.empty()){
                ++amount;
            }
        }
        return amount;
    }