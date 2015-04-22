#include "queue.h"
#include <queue>        // подключаем заголовочный файл очереди

using namespace std;

void queue_create(Queue *pqueue){
	(pqueue->q) = new queue<Pointer>;

}
void queue_destroy(Queue *pqueue){
	delete (pqueue->q);
}
bool queue_enqueue(Queue *pqueue, data value){
	(*(pqueue->q)).push(value);
	return true;
}
data queue_dequeue(Queue *pqueue){
	if ((*(pqueue->q)).size() == 0)
		return NULL;
	Pointer res = (*(pqueue->q)).front();
	(*(pqueue->q)).pop();
	return res;
}