# C++模拟多线程生产者与消费者模型

## 1. 互斥量与条件变量模拟实现

```cpp
#include <stdio.h>                                                                                     
#include <unistd.h>
#include <pthread.h>
#include <iostream>
#include <queue>
using namespace std;
// 生产着与消费者模型
//  1.线程安全的队列( queue  )
//    互斥：pthread_mutex_t
//    同步：pthread_cond_t
//  2.两种线程：生产者线程 + 消费者线程
//    生产者线程->入口函数
//    消费者线程->入口函数



#define THREAD_COUNT 4

template<class T>
class BlockQueue
{
    public:
    BlockQueue()
    {
        capacity = 1;  //初始化默认队列大小
        lock = PTHREAD_MUTEX_INITIALIZER; //初始化互斥锁
        //初始化 条件变量
        producer_cond = PTHREAD_COND_INITIALIZER;
        consumer_cond = PTHREAD_COND_INITIALIZER;
    }
    ~BlockQueue()
    {
        pthread_mutex_destroy(&lock);
        pthread_cond_destroy(&consumer_cond);
        pthread_cond_destroy(&producer_cond);
    }

    //生产者线程：插入数据
    void safe_push(T data)
    {
        pthread_mutex_lock(&lock);
        while(safe_queue.size() >= capacity)
        {
            pthread_cond_wait(&producer_cond,&lock);
        }
        safe_queue.push(data);
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&consumer_cond);
    }

    //消费者线程：取出数据
    T safe_pop()
    {
        pthread_mutex_lock(&lock);
        while(safe_queue.empty())
        {
            pthread_cond_wait(&consumer_cond,&lock);
        }
        T data = safe_queue.front();
        safe_queue.pop();
        pthread_mutex_unlock(&lock);
        pthread_cond_signal(&producer_cond);
        return data;
    }
    private:
    queue<T> safe_queue;
    size_t capacity;  //设置队列中最大存储量

    pthread_mutex_t lock; //保证队列互斥的互斥锁

    //保证生产者线程与消费者线程同步的条件变量
    pthread_cond_t consumer_cond;
    pthread_cond_t producer_cond;
};

// 消费者线程：从 线程安全的队列 中一直 取出数据
void* consumer_task(void* arg)
{
    BlockQueue<int> *safe_queue = (BlockQueue<int>*)arg;

    while(1)
    {
        int data = safe_queue->safe_pop();
        printf("consumer[%p]: %d\n",pthread_self(),data);
    }
    return NULL;
}
// 生产者线程：从 线程安全的队列 中一直 插入数据
void* producer_task(void* arg)
{
    BlockQueue<int> *safe_queue = (BlockQueue<int>*)arg;
    int data = 0;
    while(1)
    {
        safe_queue->safe_push(data);
        printf("producer[%p] : %d\n",pthread_self(),data);
        data++;
    }
    return NULL;
}

int main()
{
    BlockQueue<int> *safe_queue = new BlockQueue<int>();

    pthread_t consumer_thread[THREAD_COUNT],producer_thread[THREAD_COUNT];
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        int ret_consumer = pthread_create(&consumer_thread[i],NULL,consumer_task,(void*)safe_queue);
        int ret_prodecer = pthread_create(&producer_thread[i],NULL,producer_task,(void*)safe_queue);
        if(ret_consumer < 0 || ret_prodecer)
        {
            perror("pthread_create");
            return -1;
        }
    }

    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(consumer_thread[i],NULL);
        pthread_join(producer_thread[i],NULL);
    }

    return 0;
}

```



## 2. 信号量模拟实现

```cpp
 #include <stdio.h> 
 #include <iostream>
 #include <pthread.h>
 #include <semaphore.h>
 #include <vector>
 #include <unistd.h>
 using namespace std;

 /*
   线程安全的队列：封装vector
   1.读写下标的计算 pos = (pos + 1) % 数组大小
   2.如何实现线程安全？
     互斥: 一个信号量，sem_t lock; sem_init(&lock,0,1);
     同步: 两个信号量
       生产者信号量：sem_t producer_sem; sem_init(&producer_sem, 0, 数组大小)
       消费者信号量：sem_t consumer_sem; sem_init(&consumer_sem, 0, 0)
 */
 
 #define CAPACITY 4
 #define THREAD_COUNT 4
 
 template<class T>
class BlockQueue{
     public:
     BlockQueue():safe_queue(CAPACITY)
     {
         capacity = CAPACITY;
         sem_init(&lock, 0, 1);
         sem_init(&producer_sem, 0, capacity);
         sem_init(&consumer_sem, 0, 0);
         pos_read = pos_write = 0;
     }
     ~BlockQueue()
     {
         sem_destroy(&lock);
         sem_destroy(&consumer_sem);
         sem_destroy(&producer_sem);
     }

     void push(T data)
     {
         sem_wait(&producer_sem);
         sem_wait(&lock);

         safe_queue[pos_write] = data;
         pos_write = (pos_write + 1) % capacity;

         sem_post(&lock);
         sem_post(&consumer_sem);
     }
     T pop()
     {
         sem_wait(&consumer_sem);
         sem_wait(&lock);

         T data = safe_queue[pos_read];
         pos_read = (pos_read + 1) % capacity;

         sem_post(&lock);
         sem_post(&producer_sem);

         return data;
     }

     private:
     vector<T> safe_queue;
     size_t capacity;
     sem_t lock, producer_sem, consumer_sem;

     size_t pos_write;
     size_t pos_read;
 };

void* consumer_task(void* arg)
{
    BlockQueue<int>* p = (BlockQueue<int>*)arg;

    while(1)
    {
        int data = p->pop();
        printf("consumer:[%p],I get :%d\n",pthread_self(),data);
        sleep(1);
    }
    return NULL;
}
void* producer_task(void* arg)
{
    BlockQueue<int>* p = (BlockQueue<int>*)arg;

    int data = 0;
    while(1)
    {
        p->push(data);
        printf("producer:[%p],I put :%d\n",pthread_self(),data);
        data++;
        sleep(1);
    }
    return NULL;
}
 
int main()
{
    BlockQueue<int> *p_safe_queue = new BlockQueue<int>();
    pthread_t consumer_thread[THREAD_COUNT], producer_thread[THREAD_COUNT];
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        int ret_c = pthread_create(&consumer_thread[i],NULL,consumer_task,(void*)p_safe_queue);
        int ret_p = pthread_create(&producer_thread[i],NULL,producer_task,(void*)p_safe_queue);
        if(ret_c < 0 || ret_p < 0)
        {
            perror("pthread_create");
            return -1;
        }
    }
    for(int i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(consumer_thread[i],NULL);
        pthread_join(producer_thread[i],NULL);
    }

    return 0;
}

```

