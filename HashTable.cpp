#include "stdafx.h"
#include "HashTable.h"


int HashTable::Hash(dword key) const
{
	return (int) (key % (word) NBUCKETS);
}

HashTable::HashTable(void)
{
	for(int i = 0; i < NBUCKETS; i++)
		table_[i] = (HashEntry *) 0;
}

static void DeleteData(void *data)
{
	delete data;
}

HashTable::~HashTable(void)
{
	Map((HashMapFcn) DeleteData);
	for(int i = 0; i < NBUCKETS; i++) {
		HashEntry *p = table_[i];

		while(p) {
			table_[i] = p->next;
			delete p;
			p = table_[i];
		}
	}
}

int HashTable::Add(dword key, void *data)
{
	if(!Lookup(key)) {
		int bucket = Hash(key);
		HashEntry *p = new HashEntry(key);
		p->next = table_[bucket];
		if(table_[bucket])
			table_[bucket]->prev = p;
		table_[bucket] = p;
		p->value = data;
		return 1;
	}
	else
		return 0;
}

int HashTable::Remove(dword key)
{
	int bucket = Hash(key);
	HashEntry *p = table_[bucket];
	while(p) {
		if(p->key == key)
			break;
		p = p->next;
	}
	if(!p)
		return 0;
	if(table_[bucket] == p)
		table_[bucket] = p->next;
	delete p;
	return 1;
}

void *HashTable::Lookup(dword key) const
{
	int bucket = Hash(key);
	HashEntry *p = table_[bucket];
	while(p) {
		if(p->key == key)
			break;
		p = p->next;
	}
	if(!p)
		return (void *) 0;
	return p->value;

}

int HashTable::Replace(dword key, void *data)
{
	if(Lookup(key))
		Remove(key);
	return Add(key, data);
}

void HashTable::Map(HashMapFcn fcn)
{
	for(int i = 0; i < NBUCKETS; i++) {
		HashEntry *p = table_[i];
		while(p) {
			fcn(p->value);
			p = p->next;
		}
	}
}

dword HashTable::HashKey(const char *s) const
{
	int len = strlen(s);
	word hash = 0;
	for(int i = 0; i < len; i++) {
		hash ^= s[i];
		hash = _rotl(hash, 1);
	}
	return (dword) hash;
}

/*
 * StringTable
 */

int StringTable::Add(char *s)
{
	int len;
	char *data;

	len = strlen(s);
	if(!(data = new char[len + 1]))
		return 0;
	memcpy(data, s, len);
	data[len] = '\0';
    return HashTable::Add(HashKey(data), data);
}

void StringTable::Restart(void)
{
	cursor_.bucket = -1;
	cursor_.p = (HashEntry *) 0;
}

void *StringTable::Next(void)
{
	void *data;

	while(1) {
		/*
		 * If at end of current bucket: if current bucket is
		 * the last bucket, return NULL. Else, bump down to
		 * next bucket.
		 */
		if(cursor_.p == (HashEntry *) 0) {
			if(cursor_.bucket >= (NBUCKETS - 1))
				return (void *) 0;
			else {
				cursor_.p = table_[++(cursor_.bucket)];
				/*
				 * If found in-use bucket, return bucket current element data,
				 * and bump cursor to point to next bucket element.
				 */
				if(cursor_.p != (HashEntry *) 0) {
					data = cursor_.p->value;
					cursor_.p = cursor_.p->next;
					return data;
				}
			}
		}
		/*
		 * Else (if not at end of current bucket), return bucket
		 * current element data, and bump cursor to next bucket element.
		 */
		else {
			data = cursor_.p->value;
			cursor_.p = cursor_.p->next;
			return data;
		}
	}
}
