#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<semaphore.h>

class DataPool{
    private:
	std::vector<std::string> pool;
	int cap;
	sem_t data_sem;
	sem_t blank_sem;  //设置信号量，使之进行pv操作，原子化
	int product_step;
	int consume_step;
    public:
	DataPool(int cap_ = 1024)
		:cap(cap_)
		,pool(cap_)
	{
	    sem_init(&data_sem,0,0);
	    sem_init(&blank_sem,0,cap);
	    product_step = 0;
	    consume_step = 0;
	}
	
	void PutMessage(std::string &msg)
	{
	    sem_wait(&blank_sem);  //有空格，可以进行放入p操作
	    pool[product_step] = msg;
	    product_step++;
	    product_step %= cap;
	    sem_post(&data_sem);  //空格被消费，有了数据
	}

	void GetMessage(std::string &msg)
	{
	    sem_wait(&data_sem);  //存在数据，可以开始取出
	    msg = pool[consume_step];
	    consume_step++;
	    consume_step %= cap;
	    sem_post(&blank_sem);  //有了空格，可以进行放入
	}

	~DataPool()
	{
	    sem_destroy(&data_sem);
	    sem_destroy(&blank_sem);
	}
};
