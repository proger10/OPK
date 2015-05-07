#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

typedef void *Pointer;
typedef unsigned(*HashFunction)(void *key);
typedef int(*CompareFunction)(void *key1, void *key2);
typedef void(*ForeachFunction)(void *key, Pointer data, Pointer userdata);

typedef struct _Entry{
	void *key;
	Pointer data;
} Entry;

typedef struct HashTable {
	size_t size;
	SList **table;
	HashFunction hash;
	CompareFunction compare;
} HashTable;

/* Инициализировать таблицу */
HashTable *ht_init(size_t size, HashFunction hash, CompareFunction compare);

/* Уничтожить таблицу */
void ht_destroy(HashTable *ht);

/* Записать в таблицу соответствие key -> data. Если key уже существовал,
* соотв. поле data будет удалено (dtor) и перезаписано */
Pointer ht_set(HashTable *ht, void *key, Pointer data);

/* Получить значение по ключу. Если ключа нет в таблице, вернуть 0. */
Pointer ht_get(HashTable *ht, void *key);

/* Проверка существования ключа key в таблице. 1 - есть, 0 - нет. */
int ht_has(HashTable *ht, void *key);

/* Удалить элемент с ключом key из таблицы (если он есть) */
Pointer ht_delete(HashTable *ht, void *key);

/* Обход таблицы с посещением всех элементов. Функция f будет вызвана для
* всех пар (key, data) из таблицы */
void ht_foreach(HashTable *ht, ForeachFunction, Pointer userdata);

int ht_size(HashTable *ht);

/* Изменить размер базового массива.
*
* Примерный алгоритм: выделить новый массив table и перенести в него все
* существующие структуры List из старой table, затем старую освободить и
* заменить новой.
*
* Это эффективнее, чем создавать новую таблицу и делать в нее полноценные
* вставки.
*/
void ht_resize(HashTable *ht, size_t new_size);

#endif;
