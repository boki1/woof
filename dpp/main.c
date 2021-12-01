#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define N 5
#define LEFT(p) ((p) + N - 1) % N
#define RIGHT(p) ((p) + 1) % N

#define do_nothing() (sleep (rand () % 100))
#define eat() do_nothing ()
#define think() do_nothing ()

enum { EATING, HUNGRY, THINKING };

int state[N];
int phil[N] = { 0, 1, 2, 3, 4 };

pthread_mutex_t update_state;
sem_t forks_sems[N];

void
test (int phnum)
{
    if (state[phnum] == HUNGRY && state[LEFT (phnum)] != EATING
	&& state[RIGHT (phnum)] != EATING) {

	state[phnum] = EATING;
	printf ("Philosopher %d takes fork %d and %d\n", phnum + 1,
		LEFT (phnum) + 1, phnum + 1);

	printf ("Philosopher %d is eating\n", phnum + 1);

	// sem_post(&S[phnum]) has no effect
	// during takefork
	// used to wake up hungry philosophers
	// during putfork
	sem_post (&forks_sems[phnum]);
    }
}

void
take_fork (int phnum)
{

    pthread_mutex_lock (&update_state);

    state[phnum] = HUNGRY;

    printf ("Philosopher %d is hungry\n", phnum + 1);

    // eat if neighbours are not eating
    test (phnum);

    pthread_mutex_unlock (&update_state);

    // if unable to eat wait to be signalled
    sem_wait (&forks_sems[phnum]);

    sleep (1);
}

// put down chopsticks
void
put_fork (int phnum)
{

    pthread_mutex_lock (&update_state);

    // state that thinking
    state[phnum] = THINKING;

    printf ("Philosopher %d putting fork %d and %d down\n", phnum + 1,
	    LEFT (phnum) + 1, phnum + 1);
    printf ("Philosopher %d is thinking\n", phnum + 1);

    test (LEFT (phnum));
    test (RIGHT (phnum));

    pthread_mutex_unlock (&update_state);
}

void *
philospher (void *num)
{

    while (1) {
	int i = *(int *)num;
	think ();
	take_fork (i);
	eat ();
	put_fork (i);
    }
}

int
main ()
{
    int i;
    pthread_t tids[N];
    srand (time (0));

    pthread_mutex_init (&update_state, 0);

    for (i = 0; i < N; i++)
	sem_init (&forks_sems[i], 0, 0);

    for (i = 0; i < N; i++) {
	pthread_create (&tids[i], NULL, philospher, &phil[i]);
	printf ("Philosopher %d is thinking\n", i + 1);
    }

    for (i = 0; i < N; i++)
	pthread_join (tids[i], NULL);

    return EXIT_SUCCESS;
}

