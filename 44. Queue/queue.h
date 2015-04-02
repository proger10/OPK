#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdbool.h>

typedef void *data;
typedef data Pointer;
typedef struct _Queue {
	size_t initial_size;
	int inc;
	data *q;
	size_t qsize;
	size_t head;
	size_t count;
} Queue;


/* Создать пустую очередь */
void queue_create(Queue *pqueue);

/* Уничтожить очередь, освободив выделенную память */
void queue_destroy(Queue *pqueue);

/* Поместить значение value в конец очереди */
bool queue_enqueue(Queue *pqueue, data value);

/* Возвращает количество элементов в очереди (0, если очередь пуста) */
size_t queue_size(Queue *pqueue);

/* Исключить и вернуть значение первого элемента очереди.
Если очередь пуста, возвращает 0 */
data queue_dequeue(Queue *pqueue);

/*
* Возвращает значение первого элемента, не удаляя его из очереди.
* Если очередь пуста, возвращает 0
*/
data queue_peek(Queue *pqueue);

/*
* Настраивает параметры очереди.
* initial_size: начальный размер очереди, при первом выделении памяти
*               (по умолч.: 50)
* increment: на сколько элементов расширять очередь при последующих
*            выделениях памяти
*/
void queue_tune(Queue *pqueue, size_t initial_size, int increment);

#endif
