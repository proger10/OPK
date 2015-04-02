#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef void *data;

typedef struct _Queue {
	size_t initial_size;
	int inc;
	data *q;
	int qsize;
	size_t head;
	size_t tail;
} Queue;


/* ������� ������ ������� */
void queue_create(Queue *pqueue);

/* ���������� �������, ��������� ���������� ������ */
void queue_destroy(Queue *pqueue);

/* ��������� �������� value � ����� ������� */
bool queue_enqueue(Queue *pqueue, data value);

/* ���������� ���������� ��������� � ������� (0, ���� ������� �����) */
size_t queue_size(Queue *pqueue);

/* ��������� � ������� �������� ������� �������� �������.
���� ������� �����, ���������� 0 */
data queue_dequeue(Queue *pqueue);

/*
* ���������� �������� ������� ��������, �� ������ ��� �� �������.
* ���� ������� �����, ���������� 0
*/
data queue_peek(Queue *pqueue);

/*
* ����������� ��������� �������.
* initial_size: ��������� ������ �������, ��� ������ ��������� ������
*               (�� �����.: 50)
* increment: �� ������� ��������� ��������� ������� ��� �����������
*            ���������� ������
*/
void queue_tune(Queue *pqueue, size_t initial_size, int increment);

#endif