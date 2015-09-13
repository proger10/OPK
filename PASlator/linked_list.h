#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef void * Pointer;

typedef struct _SList {
	Pointer data;
	struct _SList *next;
} SList;


/* Добавить в хвост. Возвращает новое начало списка. */
SList *slist_append(SList *list, Pointer data);

/* Добавить в голову. Возвращает новое начало списка. */
SList *slist_prepend(SList *list, Pointer data);

/* Вставить после указанного элемента */
void slist_insert(SList *sibling, Pointer data);

/* Удалить элемент из списка и освободить память.
Возвращает новое начало списка. */
SList *slist_remove(SList *list, Pointer data);

/* Удалить все элементы из списка со значением data и освободить память.
Возвращает новое начало списка. */
SList *slist_remove_all(SList *list, Pointer data);

/* Удалить элемент, следующий за sibling.
Возвращает данные из удаленного элемента */
Pointer slist_remove_next(SList *sibling);


/*Удалить если compare_func вернет 1*/
SList *slist_remove_if(SList *list, Pointer userdata, int(*compare_func)(Pointer listdata, Pointer userdata));

/* Уничтожить весь список */
void slist_free(SList *list);

/* Подсчитать длину списка */
unsigned slist_length(SList *list);

/* Скопировать список. Возвращает начало копии */
SList *slist_copy(SList *list);

/* Перевернуть список. Возвращает новое начало списка */
SList *slist_reverse(SList *list);

/* Конкатенация двух списков (присоединяет list2 к list1).
Возвращает указатель на начало объединенного списка */
SList *slist_concat(SList *list1, SList *list2);

/* Обход списка. Функции func на каждом шаге передаются два аргумента:
data, соответствующий данным текущего элемента списка, и user_data,
являющийся аргументом самой функции */
void slist_foreach(SList *list, void(*func)(Pointer data, Pointer user_data), Pointer user_data);

/* Последний элемент списка */
SList *slist_last(SList *list);

/* n-й элемент списка (или 0). Если n отрицательно, элементы
считаются с конца (-1 -- последний и т.д.) */
SList *slist_nth(SList *list, int n);

/* Поиск элемента со значением данных data (0 если не найден) */
SList *slist_find(SList *list, Pointer data);

/* Возвращает первый элемент, для которого compare_func вернет 0, сравнивая каждый элемент списка с data */
SList *slist_find_custom(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle));

/* Возвращает позицию элемента el в списке или -1,
если он не присутствует в списке */
int slist_position(SList *list, SList *el);
/* Возвращает первый элемент, для которого compare_func вернет 0, сравнивая каждый элемент списка с data. param передаётся функции сравнения */
SList *slist_find_custom_param(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle, Pointer param), Pointer param);

/*Добавить add к list и вернуть новое начало списка. */
SList *slist_contact(SList *list, SList *add);


/*Возвращает связный список из элементов, для которых compare_func вернет 0*/
SList *slist_find_all_custom(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle));

#endif