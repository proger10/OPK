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
		free(ht);
		return NULL;
	}
	return ht;
}

static void free_entry(Entry *entry, void *null){
	free(entry);
}

void ht_destroy(HashTable *ht){
	for (size_t i = 0; i < ht->size; i++){
		slist_foreach(ht->table[i], free_entry, NULL);
		slist_free(ht->table[i]);
	}
	free(ht->table);
	free(ht);
}

int compare_key(Entry *a, void *key, CompareFunction compare){
	return compare(a->key, key);
}

static Entry *ht_get_low(HashTable *ht, void *key, unsigned hash){

	SList *list_entry = slist_find_custom_param(ht->table[hash], key, compare_key, ht->compare);
	if (list_entry == NULL){
		return NULL;
	}
	return list_entry->data;
}

Pointer ht_set(HashTable *ht, void *key, Pointer data){
	unsigned hash = ht->hash(key) % ht->size;

	Entry *entry = ht_get_low(ht, key, hash);
	if (entry != NULL){
		Pointer result = entry->data;
		entry->data = data;
		return result;
	}
	entry = malloc(sizeof(Entry));
	if (entry == NULL){
		return NULL;
	}
	entry->data = data;
	entry->key = key;
	ht->table[hash] = slist_prepend(ht->table[hash], entry);
	return NULL;
}

Pointer ht_get(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;

	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL)
		return NULL;
	return entry->data;
}

int ht_has(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;

	return ht_get_low(ht, key, hash) != NULL;
}

Pointer ht_delete(HashTable *ht, void *key){
	unsigned hash = ht->hash(key) % ht->size;

	Entry *entry = ht_get_low(ht, key, hash);
	if (entry == NULL){
		return NULL;
	}
	Pointer result = entry->data;
	ht->table[hash] = slist_remove(ht->table[hash], entry);
	free(entry);
	return result;
}

typedef struct _foreach{
	ForeachFunction f;
	Pointer userdata;
} Foreach;

static void foreach(Entry *entry, Foreach *foreach){
	foreach->f(entry->key, entry->data, foreach->userdata);
}

void ht_foreach(HashTable *ht, ForeachFunction f, Pointer userdata) {
	Foreach foreach_data;
	foreach_data.f = f;
	foreach_data.userdata = userdata;
	for (size_t i = 0; i < ht->size; i++){
		slist_foreach(ht->table[i], foreach, &foreach_data);
	}
}

static void foreach_add(Entry *entry, HashTable *new_ht){
	ht_set(new_ht, entry->key, entry->data);
}

void ht_resize(HashTable *ht, size_t new_size){
	HashTable *new_ht = ht_init(new_size, ht->hash, ht->compare);
	if (ht == NULL){
		return;
	}
	for (size_t i = 0; i < ht->size; i++){
		slist_foreach(ht->table[i], foreach_add, new_ht);
	}
	ht_destroy(ht);
	ht = malloc(sizeof(HashTable));
	if (ht == NULL)
		return;

	ht->size = new_ht->size;
	ht->compare = new_ht->compare;
	ht->hash = new_ht->hash;
	ht->table = new_ht->table;
	free(new_ht);
}
