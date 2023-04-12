/* ==================================================================================
 *
 *       Filename:  prod_cons_threads_using_signals_in_Queue.c
 *        Created:  10 March 2023 12:59:00  IST
 *         Author:  Pavan Kumar K A
 *   Organization:  NA
 *        Summary:  Program illustrating Producer and consumer threads 
 *                  with signals (to eachother threads) to implement Queue.
 *                  1. When data is written and stack index is updated, producer signals 
 *                     consumer to read data.
 *                  2. When data is read and stack is available, consumer signals
 *                     producer to write data.
 *
 *   Code version:  v0.1: create producer consumer threads.
 *   		    v0.2: Update it for queue and solve overflow underflow index issues. 
 *   		    v0.3: upgrade threads based on signals generated after data read/write.
 *   		    v0.4: Error handle: conditions when NOT to acquire lock and to release.
 *
 *  Enhancements:   1. Can use binary semaphore for lock mechanism.
 *		    2. Terminate threads based on conditions.
 *		    3. Implement based on Application Design / requirements.
 * ==================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Size of shared data or shared buffer for Queue*/
#define BUFFER_SIZE 3

int buffer[BUFFER_SIZE];	/* shared locations */
int index_offset = 0;		/* index to add next element */
int rem_offset = 0;		/* index to remove next element */
int of_uf_num  = 0;		/* check overflow/ underflow of elements in buffer */


/* initialization of thread variables*/
pthread_mutex_t m_lock = PTHREAD_MUTEX_INITIALIZER;   /* mutex lock for shared buffer */
pthread_cond_t  c_cond = PTHREAD_COND_INITIALIZER;    /* consumer condition var */
pthread_cond_t  p_cond = PTHREAD_COND_INITIALIZER;    /* producer condition var */

void *producer(void *param);
void *consumer(void *param);

int main (int argc, char *argv[])
{
	pthread_t t_id1, t_id2;		/* thread identifiers */
	int data;
	system("clear");

	/* create the producer thread */
	if (pthread_create(&t_id1,NULL,producer,NULL) != 0) {
		fprintf (stderr, "Failed to create producer thread\n");
		exit (1);
	}

	/* create the consumer thread */
	if (pthread_create(&t_id2,NULL,consumer,NULL) != 0) {
		fprintf (stderr, "Failed to create consumer thread\n");
		exit (1);
	}

	/* wait for threads to exit, those were created*/
	pthread_join(t_id1,NULL);
	pthread_join(t_id2,NULL);
	printf ("Exit of main does not reach here\n");
}

/* Producer thread */
void *producer(void *param)
{
	int data;
	for (data = 1; data <= 7; data++) {

		/* Acquire lock and Insert into shared buffer */
		pthread_mutex_lock (&m_lock);

		/* case for overflow */
		if (of_uf_num > BUFFER_SIZE){ 
			pthread_exit(param);
			exit(1);			
		}

		/* block thread if shared buffer is full */
		while (of_uf_num == BUFFER_SIZE)	
			pthread_cond_wait (&p_cond, &m_lock);

		/* if executing here, shared buffer is not full, so add element */
		buffer[index_offset] = data;
		index_offset = (index_offset+1) % BUFFER_SIZE;
		of_uf_num++;

		/*release lock */
		pthread_mutex_unlock (&m_lock);

		/* signal consumer */
		pthread_cond_signal (&c_cond);
		printf ("producer thread: inserted %d\n", data);  

		/* flush garbage if any */
		fflush (stdout);                       
	}
	printf ("Producer thread Exit Success\n");  
	fflush (stdout);
	pthread_exit(param);
}

/* Consumer thread */
void *consumer(void *param)   //In this design, consumer need not to terminate 
{
	int data;
	while (1) {

		/* Acquire lock and read data from shared buffer */
		pthread_mutex_lock (&m_lock);

		/* case for underflow */
		if (of_uf_num < 0){
			pthread_exit(param);
			exit(1);                
		}

		/* block thread if shared buffer is empty */
		while (of_uf_num == 0)		
			pthread_cond_wait (&c_cond, &m_lock);

		/* if executing here, buffer not empty so remove element */
		data = buffer[rem_offset];
		rem_offset = (rem_offset + 1) % BUFFER_SIZE;
		of_uf_num--;

		/*release lock */
		pthread_mutex_unlock (&m_lock);

		/* signal producer thread */
		pthread_cond_signal (&p_cond);
		printf ("Consumed value = %d\n", data);  

		/* flush grabage if any*/
		fflush(stdout);                    
	}
	printf ("Consumer thread Exit Success\n");  
	fflush (stdout);
	pthread_exit(param);
}
