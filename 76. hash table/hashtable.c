#include "hashtable.h"

HashTable *ht_init(size_t size, HashFunction hash, CompareFunction compare){
	HashTable *ht = malloc(sizeof(HashTable));
	if (ht == NULL)
		return NULL;
	ht->compare = compare;
	ht->hash = hash;
	ht->size = size;
	ht->table = calloc(size, sizeof(SList *));
	return ht;
}

static void freeentry(void *data, void *null){
	free(data);
}

void ht_destroy(HashTable *ht){
	slist_foreach(ht->table, freeentry, NULL);
	for (int i = 0; i < ht->size; i++){
		slist_free(ht->table[i]);
	}
	free(ht->table);
}

static Entry *ht_get_low(HashTable *ht, void *key, unsigned hash){
	if (hash >= ht->size){
		return NULL;
	}
	Compare compare;
	compare.compare = ht->compare;
	compare.data = key;
	SList *list_entry = slist_find_custom(ht->table[hash], key, &compare);
	if (list_entry == NULL){
		return NULL;
	}
	return (Entry *)list_entry->data;
}

Pointer ht_set(HashTable *ht, void *key, Pointer data){
	unsigned hash = ht->hash(key);
	if (hash >= ht->size){
		return NULL;
	}
	Entry *entry = malloc(sizeof(Entry));
	if (entry == NULL){
		return NULL;
	}
	entry->data = data;
	entry->key = key;
	slist_prepend(ht->table[hash], entry);
}

typedef struct _compare{
	CompareFunction compare;
	void *data;
} Compare;

static int compare(void *data, Compare *compare){
	return compare->compare(data, compare->data);
}

Pointer ht_get(HashTable *ht, void *key){
	unsigned hash = ht->hash(key);

	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL)
		return NULL;
	return entry->data;
}

int ht_has(HashTable *ht, void *key){
	unsigned hash = ht->hash(key);
	if (hash >= ht->size){
		return NULL;
	}
	Compare compare;
	compare.compare = ht->compare;
	compare.data = key;
	SList *list_entry = slist_find_custom(ht->table[hash], key, &compare);
	if (list_entry == NULL){
		return NULL;
	}
}

void ht_delete(HashTable *ht, void *key){

}

void ht_traverse(HashTable *ht, void(*f)(void *key, Pointer data)); {

}

void ht_resize(HashTable *ht, size_t new_size){

}