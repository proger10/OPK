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

/* ���������������� �������.
*
* size - ������ �������� �������;
* hf   - ���-�������;
* dtor - ����������. ���� ������� ����� ������������ data ��������� ���������
*        (ht_destroy, ht_delete, � ����� ��� ���������� ��������� � ht_set).
*
* ���� hf=0, ������� ����������� ������� (��������).
* ���� dtor=0, ���������� ����������� (�� ����� ����������).
*/
HashTable *ht_init(size_t size, HashFunction hash, CompareFunction compare);

/* ���������� ������� */
void ht_destroy(HashTable *ht);

/* �������� � ������� ������������ key -> data. ���� key ��� �����������,
* �����. ���� data ����� ������� (dtor) � ������������ */
Pointer ht_set(HashTable *ht, char *key, Pointer data);

/* �������� �������� �� �����. ���� ����� ��� � �������, ������� 0. */
Pointer ht_get(HashTable *ht, char *key);

/* �������� ������������� ����� key � �������. 1 - ����, 0 - ���. */
int ht_has(HashTable *ht, char *key);

/* ������� ������� � ������ key �� ������� (���� �� ����) */
void ht_delete(HashTable *ht, char *key);

/* ����� ������� � ���������� ���� ���������. ������� f ����� ������� ���
* ���� ��� (key, data) �� ������� */
void ht_traverse(HashTable *ht, void(*f)(char *key, Pointer data));

/* �������� ������ �������� �������.
*
* ��������� ��������: �������� ����� ������ table � ��������� � ���� ���
* ������������ ��������� List �� ������ table, ����� ������ ���������� �
* �������� �����.
*
* ��� �����������, ��� ��������� ����� ������� � ������ � ��� �����������
* �������.
*/
void ht_resize(HashTable *ht, size_t new_size);

#endif;