#include "request_queue.h"

using namespace std;

RequestQueue::RequestQueue(const SearchServer& search_server)
        : search_server_(search_server)
    {
    }

int RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
        return AddFindRequest(
            raw_query, [status](int document_id, DocumentStatus document_status, int rating) {
                return document_status == status;
            });
    }

int RequestQueue::AddFindRequest(const std::string& raw_query) {
        return AddFindRequest(raw_query, DocumentStatus::ACTUAL);
    }

int RequestQueue::GetNoResultRequests() const {
        int amount = 0;
        for(const auto& request_result : requests_){
            if(!request_result){
                ++amount;
            }
        }
        return amount;
    }