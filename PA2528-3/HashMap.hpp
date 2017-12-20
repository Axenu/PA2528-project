#ifndef __HASH_MAP__
#define __HASH_MAP__

#include <functional>

#include "utility.hpp"

template<typename Key, typename Value>
class HashMap {
    private:
        struct Entry;

    public:
        class Iterator {
            public:
                Value& operator*() {
                    return mEntry->value;
                }

                void operator++() {
                    if(mTableIndex == mMap->TABLE_SIZE) {
                        return;
                    }
                    if(!mEntry || !mEntry->next) {
                        while(++mTableIndex < mMap->TABLE_SIZE) {
                            mEntry = mMap->mTable[mTableIndex];
                            if(mEntry) {
                                return;
                            }
                        }
                    }
                    else {
                        mEntry = mEntry->next;
                    }
                }

                bool operator!=(const Iterator& other) {
                    return  mEntry != other.mEntry;
                }

            private:
                friend HashMap;
                size_t mTableIndex = 0;
                Entry* mEntry = nullptr;
                HashMap<Key, Value>* mMap;
        };

    public:
        HashMap();

        // Return nullptr if not found.
        Value* find(const Key& key);

        void insert(const Key& key, const Value& value);

        void erase(const Key& key);

        Iterator begin();
        Iterator end();

    private:
        static constexpr size_t hash(const Key& key);

    private:
        struct Entry {
            Key key;
            Value value;
            Entry* next = nullptr;
        };

    private:
        friend Iterator;
        static constexpr size_t TABLE_SIZE = 1024;
        static_assert(isPow2(TABLE_SIZE), "Table size must be power of two.");
        static const std::hash<Key> HASHER;
        Entry* mTable[TABLE_SIZE];
};

#include "HashMap.inl"

#endif // __HASH_MAP__

