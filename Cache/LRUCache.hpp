//
//  LRUCache.hpp
//  vision
//
//  Created by Aswin Raj Kharel on 9/2/20.
//

#ifndef LRUCache_hpp
#define LRUCache_hpp

#include <stdio.h>
#include <list>
#include <map>
#include <string>

template <typename T>
class LRUCache{
private:
    std::map<std::string, std::pair<std::list<std::string>::iterator, T>> _table;
    std::list<std::string> _list;
    int _capacity;
    
public:
    LRUCache(int capacity):_capacity(capacity){}
    
    void insert(std::string const& key, T value){
        if (_table.find(key) != _table.end()){
            _table[key].second = value;
            // Move to front
            _list.erase(_table[key].first);
            _list.push_front(key);
            _table[key].first = _list.begin();
        }
        
        // If not found
        // Check capacity
        if (_list.size() == _capacity){
            // Remove last one
            _table.erase(_list.back());
            _list.pop_back();
        }
        
        _list.push_front(key);
        _table.insert({key, {_list.begin(), value}});
    }
    
    std::pair<bool, T> lookup(std::string const& key){
        if (_table.find(key) == _table.end()){
            return {false, std::shared_ptr<std::string>()};
        }
        
        // Found
        _list.erase(_table[key].first);
        _list.push_front(key);
        _table[key].first = _list.begin();
        return {true, _table[key].second};
    }
    
    void erase(std::string const& key){
        if (_table.find(key) == _table.end()){
            return;
        }
        
        // Found
        _list.erase(_table[key].first);
        _table.erase(key);
    }
};

#endif /* DirCache_hpp */
