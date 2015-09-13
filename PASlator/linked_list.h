#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef void * Pointer;

typedef struct _SList {
	Pointer data;
	struct _SList *next;
} SList;


/* �������� � �����. ���������� ����� ������ ������. */
SList *slist_append(SList *list, Pointer data);

/* �������� � ������. ���������� ����� ������ ������. */
SList *slist_prepend(SList *list, Pointer data);

/* �������� ����� ���������� �������� */
void slist_insert(SList *sibling, Pointer data);

/* ������� ������� �� ������ � ���������� ������.
���������� ����� ������ ������. */
SList *slist_remove(SList *list, Pointer data);

/* ������� ��� �������� �� ������ �� ��������� data � ���������� ������.
���������� ����� ������ ������. */
SList *slist_remove_all(SList *list, Pointer data);

/* ������� �������, ��������� �� sibling.
���������� ������ �� ���������� �������� */
Pointer slist_remove_next(SList *sibling);


/*������� ���� compare_func ������ 1*/
SList *slist_remove_if(SList *list, Pointer userdata, int(*compare_func)(Pointer listdata, Pointer userdata));

/* ���������� ���� ������ */
void slist_free(SList *list);

/* ���������� ����� ������ */
unsigned slist_length(SList *list);

/* ����������� ������. ���������� ������ ����� */
SList *slist_copy(SList *list);

/* ����������� ������. ���������� ����� ������ ������ */
SList *slist_reverse(SList *list);

/* ������������ ���� ������� (������������ list2 � list1).
���������� ��������� �� ������ ������������� ������ */
SList *slist_concat(SList *list1, SList *list2);

/* ����� ������. ������� func �� ������ ���� ���������� ��� ���������:
data, ��������������� ������ �������� �������� ������, � user_data,
���������� ���������� ����� ������� */
void slist_foreach(SList *list, void(*func)(Pointer data, Pointer user_data), Pointer user_data);

/* ��������� ������� ������ */
SList *slist_last(SList *list);

/* n-� ������� ������ (��� 0). ���� n ������������, ��������
��������� � ����� (-1 -- ��������� � �.�.) */
SList *slist_nth(SList *list, int n);

/* ����� �������� �� ��������� ������ data (0 ���� �� ������) */
SList *slist_find(SList *list, Pointer data);

/* ���������� ������ �������, ��� �������� compare_func ������ 0, ��������� ������ ������� ������ � data */
SList *slist_find_custom(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle));

/* ���������� ������� �������� el � ������ ��� -1,
���� �� �� ������������ � ������ */
int slist_position(SList *list, SList *el);
/* ���������� ������ �������, ��� �������� compare_func ������ 0, ��������� ������ ������� ������ � data. param ��������� ������� ��������� */
SList *slist_find_custom_param(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle, Pointer param), Pointer param);

/*�������� add � list � ������� ����� ������ ������. */
SList *slist_contact(SList *list, SList *add);


/*���������� ������� ������ �� ���������, ��� ������� compare_func ������ 0*/
SList *slist_find_all_custom(SList *list, Pointer needle, int(*compare_func)(Pointer listdata, Pointer needle));

#endif