#include <cassert>

template<typename Key, typename Value>
const std::hash<Key> HashMap<Key, Value>::HASHER;

template<typename Key, typename Value>
HashMap<Key, Value>::HashMap() {
    memset(mTable, 0, sizeof(mTable));
}

template<typename Key, typename Value>
Value* HashMap<Key, Value>::find(const Key& key) {
    for(Entry* entry = mTable[hash(key)]; entry != nullptr; entry = entry->next) {
        if(entry->key == key) {
            return &entry->value;
        }
    }
    return nullptr;
}


template<typename Key, typename Value>
void HashMap<Key, Value>::insert(const Key& key, const Value& value) {
    Entry** entry = &mTable[hash(key)];
    while(*entry != nullptr) {
        if((*entry)->key == key) {
            (*entry)->value = value;
            return;
        }
        entry = &((*entry)->next);
    }
    assert(*entry == nullptr);

    *entry = new Entry();
    (*entry)->key = key;
    (*entry)->value = value;
}

template<typename Key, typename Value>
void HashMap<Key, Value>::erase(const Key& key) {
    for(Entry** entry = &mTable[hash(key)]; *entry != nullptr; entry = &((*entry)->next)) {
        if((*entry)->key == key) {
            delete *entry;
            *entry = (*entry)->next;
            return;
        }
    }
}

template<typename Key, typename Value>
typename HashMap<Key, Value>::Iterator HashMap<Key, Value>::begin() {
    Iterator it;
    it.mMap = this;
    it.mTableIndex = 0;
    it.mEntry = mTable[it.mTableIndex];
	if (!it.mEntry) {
		++it;
	}
    return it;
}

template<typename Key, typename Value>
typename HashMap<Key, Value>::Iterator HashMap<Key, Value>::end() {
    Iterator it;
    it.mMap = this;
    it.mTableIndex = TABLE_SIZE;
    it.mEntry = nullptr;
    return it;
}

template<typename Key, typename Value>
constexpr size_t HashMap<Key, Value>::hash(const Key& key) {
    return HASHER(key) & (TABLE_SIZE - 1);
}
