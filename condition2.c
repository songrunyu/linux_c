#include <pthread.h>  
#include <stdio.h>  
#include <stdlib.h>  

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   //初始化互斥锁  
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;      //初始化条件变量  

void *thread1(void *arg);  
void *thread2(void *arg);  

int i=1;  

int main(void)  
{  
	pthread_t t_a;  
	pthread_t t_b;  

	pthread_create(&t_a, NULL, thread1, NULL); //创建进程t_a  
	pthread_create(&t_b, NULL, thread2, NULL); //创建进程t_b
	
	pthread_join(t_a, NULL);         //等待进程t_a结束 
	pthread_join(t_b, NULL);         //等待进程t_b结束 
	
	pthread_mutex_destroy(&mutex);  
	pthread_cond_destroy(&cond);  
	
	exit(0);  
}  

void *thread1(void *arg)  
{  
	for(i=1; i<=6; i++)  
	{  
			pthread_mutex_lock(&mutex);       //设置锁
			printf("thread1: lock %d\n", __LINE__);  
			if(i%3 == 0)
			{  
				printf("thread1:signal   %d\n", __LINE__);  
				pthread_cond_signal(&cond);                 //激活cond所指条件变量
				sleep(1);  
			}
			printf("thread1: %d\n",i);
			printf("thread1: unlock %d\n\n", __LINE__);  
			pthread_mutex_unlock(&mutex);   //解锁
			sleep(1); 						
	}  
}  

void *thread2(void *arg)  
{  
	while(i <= 6)  
	{  
			pthread_mutex_lock(&mutex);    //设置锁
			printf("thread2: lock %d\n", __LINE__);  
			if(i%3 != 0)
			{  
				printf("thread2: wait   %d\n", __LINE__);  
				pthread_cond_wait(&cond,&mutex);      //解锁mutex，并等待cond改变 
			} 
			printf("thread2:%d\n",i);
		 	printf("thread2: unlock %d\n\n", __LINE__);  
			pthread_mutex_unlock(&mutex);        //解锁
			sleep(1);  
			
	} 
}  
