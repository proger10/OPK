#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdlib.h>
#include <stdbool.h>
#include "linked_list.h"

typedef void *Pointer;
typedef unsigned(*HashFunction)(void *key);
typedef bool(*CompareFunction)(void *key);

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

/* Инициализировать таблицу.
*
* size - размер базового массива;
* hf   - хеш-функция;
* dtor - деструктор. Этой функции будут передаваться data удаляемых элементов
*        (ht_destroy, ht_delete, а также при перезаписи элементов в ht_set).
*
* Если hf=0, берется стандартная функция (Дженкинс).
* Если dtor=0, деструктор отсутствует (не будет вызываться).
*/
HashTable *ht_init(size_t size, HashFunction hash, CompareFunction compare);

/* Уничтожить таблицу */
void ht_destroy(HashTable *ht);

/* Записать в таблицу соответствие key -> data. Если key уже существовал,
* соотв. поле data будет удалено (dtor) и перезаписано */
Pointer ht_set(HashTable *ht, char *key, Pointer data);

/* Получить значение по ключу. Если ключа нет в таблице, вернуть 0. */
Pointer ht_get(HashTable *ht, char *key);

/* Проверка существования ключа key в таблице. 1 - есть, 0 - нет. */
int ht_has(HashTable *ht, char *key);

/* Удалить элемент с ключом key из таблицы (если он есть) */
void ht_delete(HashTable *ht, char *key);

/* Обход таблицы с посещением всех элементов. Функция f будет вызвана для
* всех пар (key, data) из таблицы */
void ht_traverse(HashTable *ht, void(*f)(char *key, Pointer data));

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