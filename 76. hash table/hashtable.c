#include "hashtable.h"

HashTable *ht_init(size_t size, HashFunction hash, CompareFunction compare){
	HashTable *ht = malloc(sizeof(HashTable));
	if (ht == NULL)
		return NULL;
	ht->compare = compare;
	ht->hash = hash;
	ht->size = size;
	ht->table = calloc(size, sizeof(SList *));
	if (ht->table == NULL){
		return NULL;
	}
	return ht;
}

static void freeentry(void *data, void *null){
	free(data);
}

void ht_destroy(HashTable *ht){
	for (size_t i = 0; i < ht->size; i++){
		slist_foreach(ht->table[i], freeentry, NULL);
		slist_free(ht->table[i]);
	}
	free(ht->table);
	free(ht);
}


typedef struct _compare{
	CompareFunction compare;
	void *data;
} Compare;

static int compare(void *data, Compare *compare){
	return compare->compare(data, compare->data);
}

static Entry *ht_get_low(HashTable *ht, void *key, unsigned hash){
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
	unsigned hash = ht->hash(key) % ht->size;
	Entry *entry = ht_get_low(ht, key, hash);
	if (entry != NULL){
		Pointer result = entry->data;
		entry->data = data;
		return result;
	}
	slist_prepend(ht->table[hash], entry);
	return NULL;
}
Pointer ht_get(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;;
	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL)
		return NULL;
	return entry->data;
}

int ht_has(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;;
	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL){
		return 0;
	}
	return 1;
}

Pointer ht_delete(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;;
	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL){
		return;
	}
	Pointer result = entry->data;
	slist_remove(ht->table[hash], entry);
	free(entry);
	return result;
}

static void foreach(Entry *entry, void(*f)(void *key, Pointer data)){
	f(entry->key, entry->data);
}

void ht_traverse(HashTable *ht, void(*f)(void *key, Pointer data)); {
	for (int i = 0; i < ht->size; i++){
		slist_foreach(ht->table, foreach,f);
	}
	free(ht->table);
}

void ht_resize(HashTable *ht, size_t new_size){
	SList **table = calloc(new_size, sizeof(SList *));
	if (table == NULL)
		return;
	for (int i = 0; i < ht->size; i++){
		//slist_foreach(ht->table, foreach, f);
	}
	free(ht->table);
	return ht;
}
