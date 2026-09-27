#include <cassert>
#include <iostream>

#include "kv_store/kv_store.h"

void test_set_and_get()
{
    KVStore store;

    store.set("name", "Gaurav");

    auto result = store.get("name");

    assert(result.has_value());
    assert(result.value() == "Gaurav");
}

void test_missing_key()
{
    KVStore store;

    auto result = store.get("missing");

    assert(!result.has_value());
}

void test_exists()
{
    KVStore store;

    assert(!store.exists("name"));

    store.set("name", "Gaurav");

    assert(store.exists("name"));
}

void test_overwrite()
{
    KVStore store;

    store.set("name", "Gaurav");
    store.set("name", "Rahul");

    auto result = store.get("name");

    assert(result.has_value());
    assert(result.value() == "Rahul");
}

void test_empty_value()
{
    KVStore store;

    store.set("name", "");

    auto result = store.get("name");

    assert(result.has_value());
    assert(result.value().empty());
}

void test_empty_key()
{
    KVStore store;

    store.set("", "empty-key-value");

    assert(store.exists(""));

    auto result = store.get("");

    assert(result.has_value());
    assert(result.value() == "empty-key-value");
}

void test_delete()
{
    KVStore store;

    store.set("name", "Gaurav");

    assert(store.del("name"));
    assert(!store.exists("name"));
    assert(!store.get("name").has_value());

    assert(!store.del("name"));
}

int main()
{
    test_set_and_get();
    test_missing_key();
    test_exists();
    test_overwrite();
    test_empty_value();
    test_empty_key();
    test_delete();

    std::cout << "All tests passed.\n";

    return 0;
}