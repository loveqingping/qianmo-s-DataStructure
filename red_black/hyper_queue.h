#ifndef HYPER_QUEUE_H
#define HYPER_QUEUE_H
#define  TRUE 1
#define  FALSE 0
typedef struct __hyper_queue
{
	volatile int wp;
	volatile int rp;
	int struct_size;  //ÔªËØ´óÐ¡
	int queue_size;
	void* pool;
}hyper_queue_t;
int hyper_queue_init(hyper_queue_t* phq, int struct_size, int queue_size);
int hyper_queue_get(hyper_queue_t* phq, void* dst, int size);
int hyper_queue_put(hyper_queue_t* phq, void* src, int size);
int hyper_queue_empty(hyper_queue_t* phq);
#endif
