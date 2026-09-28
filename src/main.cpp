#include <iostream>

#include "kv_store/kv_store.h"

int main()
{
    KVStore store;

    store.set("name", "Gaurav");

    const auto result = store.get("name");

    if (result.has_value())
    {
        std::cout << "name: " << result.value() << '\n';
    }

    std::cout << "name exists: "
              << std::boolalpha
              << store.exists("name")
              << '\n';

    store.del("name");

    std::cout << "name exists after deletion: "
              << std::boolalpha
              << store.exists("name")
              << '\n';

    return 0;
}