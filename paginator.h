#pragma once

#include <vector>

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end){
        begin_ = begin;
        end_ = end;
    }
    
    Iterator begin() const{
        return begin_;
    }
    
    Iterator end() const{
        return end_;
    }
    
private:
    Iterator begin_;
    Iterator end_;
};

template <typename Iterator>
class Paginator {
public:
    Paginator(Iterator pages_begin, Iterator pages_end, size_t page_size){
        for(auto it = pages_begin; it != pages_end; ){
            if(it+page_size < pages_end){
                pages_.push_back({it, it+page_size});
                it+=page_size;
            } else {
                pages_.push_back({it, pages_end});
                break;
            }
        }
    }
    
    auto begin() const{
        return pages_.begin();
    }
    
    auto end() const{
        return pages_.end();
    }
    
private:
    std::vector<IteratorRange<Iterator>> pages_;
}; 

template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}