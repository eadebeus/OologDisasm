#ifndef __HASH_H
#define __HASH_H

#include "Basic.h"

const int NBUCKETS = 101;

typedef void (*HashMapFcn)(void *);

class HashTable {
protected:
	struct HashEntry {
		dword key;
		void *value;
		HashEntry *next, *prev;
		HashEntry(dword newkey) {
			key  = newkey;
			next = (HashEntry *) 0;
			prev = (HashEntry *) 0;
		}
		~HashEntry(void) {
			if(next) next->prev = prev;
			if(prev) prev->next = next;
		}
	};

	HashEntry *table_[NBUCKETS];

    int Hash(dword key) const;
public:
    HashTable();
	virtual ~HashTable();

    int Add(dword key, void *data);
    void *Lookup(dword key) const;
    int Replace(dword key, void *data);
    int Remove(dword key);
	void Map(HashMapFcn fcn);
	dword HashKey(const char *) const;
};


class StringTable : public HashTable {
private:
    struct Cursor {
		int bucket;
		HashEntry *p;
		Cursor(void) {
			bucket = -1;
			p = (HashEntry *) 0;
		}
	} cursor_;
public:
	StringTable(void) {};
	~StringTable(void) {};
	
	int Add(char *);
	void Restart(void);
	void *Next(void);
};

#endif
