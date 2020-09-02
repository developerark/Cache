//
//  DIRCache.hpp
//  vision
//
//  Created by Aswin Raj Kharel on 9/2/20.
//

#ifndef DIRCache_hpp
#define DIRCache_hpp

#include <stdio.h>
#include "LRUCache.hpp"
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

namespace fs = std::__fs::filesystem;

class DIRCache{
private:
    std::string _root;
    LRUCache<std::shared_ptr<std::string>> _lruCache;
    
    std::string _read(std::string const& filepath){
        std::ifstream file(filepath);
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
public:
    DIRCache(std::string const& root, int capacity=20):
        _root(root),
        _lruCache(LRUCache<std::shared_ptr<std::string>>(capacity))
    {
        fs::file_status fsStatus = std::__fs::filesystem::status(root);
        if (!fs::is_directory(fsStatus)){
            throw std::runtime_error("Root is not a directory");
        }
    }
    
    std::pair<bool, std::shared_ptr<std::string>> lookup(std::string path){
        std::pair<bool, std::shared_ptr<std::string>> status = _lruCache.lookup(path);
        if (status.first){
            // Found in cache
            std::cout << "cache hit" << std::endl;
            return {true, status.second};
        }
        
        // Not found
        std::cout << "cache miss" << std::endl;
        std::string absPath = fs::path(_root) / path;
        
        fs::file_status fsStatus = std::__fs::filesystem::status(absPath);
        if (!fs::is_regular_file(fsStatus)){
            // Not found in disk
            return {false, std::make_shared<std::string>()};
        }
        
        std::shared_ptr<std::string> content = std::make_shared<std::string>(_read(absPath));
        _lruCache.insert(path, content);
        
        return {true, content};
    }
};
#endif /* DIRCache_hpp */
