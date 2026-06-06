#include "index.hpp"

bool Index::index(const std::string& pkg, const std::vector<std::string>& deps) {
    std::unique_lock<std::shared_mutex> lock(mu_);
    for (const auto& dep : deps)
        if (packages_.find(dep) == packages_.end())
            return false;
    auto existing = packages_.find(pkg);
    if (existing != packages_.end())
        for (const auto& dep : existing->second)
            if (--dependents_[dep] <= 0) dependents_.erase(dep);
    packages_[pkg] = deps;
    for (const auto& dep : deps) ++dependents_[dep];
    return true;
}

bool Index::remove(const std::string& pkg) {
    std::unique_lock<std::shared_mutex> lock(mu_);
    auto it = packages_.find(pkg);
    if (it == packages_.end()) return true;          // not present is success
    if (dependents_[pkg] > 0) return false;          // something still depends on it
    for (const auto& dep : it->second)
        if (--dependents_[dep] <= 0) dependents_.erase(dep);
    packages_.erase(it);
    dependents_.erase(pkg);
    return true;
}

bool Index::query(const std::string& pkg) const {
    std::shared_lock<std::shared_mutex> lock(mu_); // Shared lock for query
    return packages_.find(pkg) != packages_.end();
}

