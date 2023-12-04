#include <stdio.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

sem_t space, item, mutex;
int _fill = 0, use = 0, jobCount = 0;

int queueSize;
int *queue;
struct timespec timeout;

void addJob(int val) {
	queue[_fill] = val;
	_fill = (_fill + 1) % queueSize;
	jobCount++;
}

int doJob() {
	int res = queue[use];
	use = (use + 1) % queueSize;
	jobCount--;
	return res;
}

void* producer(void* arg) {
	int id = ((int*) arg)[0];
	int jobs = ((int*) arg)[1];
	int numProduced = 0;
	srand(time(NULL));

	for (int i = 0; i < jobs; i++){
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec += 10;

		int job = rand() % 10 + 1;
		
		if (sem_timedwait(&space, &timeout) == 0) {
			sem_wait(&mutex);

			addJob(job);

			sem_post(&mutex);
			sem_post(&item);
			
			fprintf(stderr, "Producer %i produced job %i\n", id, job);
			numProduced++;
		} else {
			fprintf(stderr, "Producer %i gave up\n", id);
			break;
		}
	}
	fprintf(stderr, "---------- Producer %i produced %i jobs ----------\n", id, numProduced);
	return NULL;
}

void* consumer(void* arg) {
	int id = *(int*) arg;
	int res;
	int numConsumed = 0;
	while (true) {
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec += 10;

		if (sem_timedwait(&item, &timeout) == 0 && jobCount > 0) {
			sem_wait(&mutex);

			res = doJob();

			sem_post(&mutex);
			sem_post(&space);

			fprintf(stderr, "Consumer %i consumed job %i\n", id, res);
			numConsumed++;

			sleep(res);
		} else {
			fprintf(stderr, "Consumer %i gave up\n", id);
			break;
		}
	}
	fprintf(stderr, "---------- Consumer %i consumed %i jobs ----------\n", id, numConsumed);
	return NULL;
}


int main(int argc, char* argv[]) {
	if (argc != 5) {
		cerr << "usage: <queue_size> <jobs_per_producer> \
				<num_producers> <num_consumers>" << endl;
		exit(1);
	}

	queueSize = atoi(argv[1]);
	int jobsPerProducer = atoi(argv[2]);
	int numProducers = atoi(argv[3]);
	int numConsumers = atoi(argv[4]);

	if (queueSize <= 0 || jobsPerProducer <= 0 || numProducers <= 0 || numConsumers <= 0) {
		cerr << "Invalid inputs." << endl;
		exit(1);
	}

	queue = (int*) calloc(queueSize, sizeof(int));
	assert(queue != NULL);

	sem_init(&space, 0, queueSize);
	sem_init(&item, 0, 0);
	sem_init(&mutex, 0, 1);

	pthread_t pThreads[numProducers], cThreads[numConsumers];

	int args[numProducers][2];
	for (int i = 0; i < numProducers; i++) {
		args[i][0] = i + 1;
		args[i][1] = jobsPerProducer;
		pthread_create(&pThreads[i], NULL, producer, args[i]);
	}

	for (int i = 0; i < numConsumers; i++) {
		pthread_create(&cThreads[i], NULL, consumer, &i);
	}

	for (int i = 0; i < numProducers; i++) {
		pthread_join(pThreads[i], NULL);
	}

	for (int i = 0; i < numConsumers; i++) {
		pthread_join(cThreads[i], NULL);
	}
	
	sem_destroy(&space);
	sem_destroy(&item);
	sem_destroy(&mutex);

	if (queue != NULL) {
		free(queue);
		queue = NULL;
	}

	return 0;
}

