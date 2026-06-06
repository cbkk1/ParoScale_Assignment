#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <shared_mutex>

class Index {
public:
    bool index(const std::string& pkg, const std::vector<std::string>& deps);
    bool remove(const std::string& pkg);
    bool query(const std::string& pkg) const;

private:
    mutable std::shared_mutex mu_;
    std::unordered_map<std::string, std::vector<std::string>> packages_;
    std::unordered_map<std::string, int> dependents_; // indexed packages depending on key
};

