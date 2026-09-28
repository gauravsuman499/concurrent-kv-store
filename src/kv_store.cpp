#include "kv_store/kv_store.h"

void KVStore::set(const std::string& key, const std::string& value)
{
    std::lock_guard<std::mutex> lock(mutex_);

    data_[key] = value;
}

std::optional<std::string> KVStore::get(const std::string& key) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = data_.find(key);

    if (it == data_.end())
    {
        return std::nullopt;
    }

    return it->second;
}

bool KVStore::exists(const std::string& key) const
{
    std::lock_guard<std::mutex> lock(mutex_);

    return data_.find(key) != data_.end();
}

bool KVStore::del(const std::string& key)
{
    std::lock_guard<std::mutex> lock(mutex_);

    return data_.erase(key) > 0;
}