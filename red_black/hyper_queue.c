#include "hyper_queue.h"
#include <stdio.h>
#include <malloc.h>
#include <memory.h>

int hyper_queue_init(hyper_queue_t* phq, int struct_size, int queue_size)
{
	int ret = 0;
	if (phq == NULL)
	{
		ret = -1;
		goto ERR;
	}
	phq->pool = calloc(queue_size + 1, struct_size);
	if (phq->pool == NULL)
	{
		ret = -1;
		goto ERR;
	}
	phq->queue_size = queue_size + 1;
	phq->struct_size = struct_size;
	phq->rp = 0;
	phq->wp = 0;
	return ret;
ERR:
	return ret;
}
int hyper_queue_get(hyper_queue_t* phq, void* dst, int size)
{
	int ret = 0;
	void* src = NULL;
	if (phq == NULL)
	{
		ret = -1;
		goto ERR;
	}
	if (size != phq->struct_size)
	{
		ret = -1;
		goto ERR;
	}
	if (phq->rp == phq->wp)   //队列为空
	{
		ret = -1;
		goto ERR;
	}
	src = (char*)(phq->pool) + phq->struct_size * phq->rp;
	memcpy(dst, src, size);
	phq->rp = (phq->rp + 1) % (phq->queue_size);
	return ret;
ERR:
	return ret;
}
int hyper_queue_put(hyper_queue_t* phq, void* src, int size)
{
	int ret = 0;
	void* dst = NULL;
	if (phq == NULL)
	{
		ret = -1;
		goto ERR;
	}
	if (size != phq->struct_size)
	{
		ret = -1;
		goto ERR;
	}
	if ((phq->wp + 1) % phq->queue_size == phq->rp)   //队列已满
	{
		ret = -1;
		goto ERR;
	}
	dst = (char*)(phq->pool) + (phq->struct_size * phq->wp);
	memcpy(dst, src, size);
	phq->wp = (phq->wp + 1) % phq->queue_size;
	return ret;
ERR:
	return ret;

}
int hyper_queue_empty(hyper_queue_t* phq)
{
	int ret = 0;
	if (phq == NULL)
	{
		ret = -1;
		goto ERR;
	}
	if (phq->rp == phq->wp)
	{
		ret = TRUE;
	}
	else
	{
		ret = FALSE;
	}
	return ret;
ERR:
	return ret;
}