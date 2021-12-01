#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define err_abort(code, text)                                                 \
    do                                                                        \
	{                                                                     \
	    fprintf (stderr, "%s at \"%s\":%d: %s\n", text, __FILE__,         \
		     __LINE__, strerror (code));                              \
	    abort ();                                                         \
	}                                                                     \
    while (0);

struct stage
{
    pthread_mutex_t lock;
    pthread_cond_t available;
    pthread_cond_t ready;
    int data_is_ready;
    int data;
    pthread_t thread;
    struct stage *next;
};

struct pipeline
{
    pthread_mutex_t lock;
    struct stage *head;
    struct stage *tail;
    int num_stages;
    int num_active;
};

void
pipe_send (struct stage *stage, int data)
{
    int status;

    status = pthread_mutex_lock (&stage->lock);
    if (status)
	err_abort (status, "pthread_mutex_lock (pipe_send)");

    // Wait for data to be consumed.
    while (stage->data_is_ready)
	{
	    status = pthread_cond_wait (&stage->ready, &stage->lock);
	    if (status)
		{
		    status = pthread_mutex_unlock (&stage->lock);
		    if (status)
			err_abort (status, "pthread_mutex_unlock (pipe_send)");
		    return;
		}
	}

    // Send new data
    stage->data = data;
    stage->data_is_ready = 1;
    status = pthread_cond_signal (&stage->available);

    status = pthread_mutex_unlock (&stage->lock);
    if (status)
	err_abort (status, "pthread_mutex_unlock 2 (pipe_send)");
}

void *
pipe_stage (void *arg)
{
    struct stage *this = arg;
    int status;

    status = pthread_mutex_lock (&this->lock);
    if (status)
	err_abort (status, "pthread_mutex_lock (pipe_stage)");

    for (;;)
	{
	    while (!this->data_is_ready)
		{
		    // Wait implicitly unlocks the mutex so the function does
		    // not need to do it explicitly.
		    status = pthread_cond_wait (&this->available, &this->lock);
		    if (status)
			err_abort (status, "pthread_cond_wait (pipe_stage)");
		}

	    pipe_send (this->next, this->data + 1);
	    this->data_is_ready = 0;

	    status = pthread_cond_signal (&this->ready);
	    if (status)
		err_abort (status, "pthread_cond_signal (pipe_stage)");
	}

    return NULL;
}

int
pipe_create (struct pipeline *pipe, int p_num_stages)
{
    struct stage **link = &pipe->head;
    struct stage *new_stage;
    int status;

    status = pthread_mutex_init (&pipe->lock, NULL);
    if (status)
	err_abort (status, "pthread_mutex_init (pipe)");

    pipe->num_stages = p_num_stages;
    pipe->num_active = 0;

    for (int index = 0; index <= pipe->num_stages; ++index)
	{
	    struct stage *new_stage = malloc (sizeof (struct stage));
	    if (!new_stage)
		err_abort (0, "malloc (pipe)");

	    status = pthread_mutex_init (&new_stage->lock, NULL);
	    if (status)
		err_abort (status, "pthread_mutex_init 2 (pipe)");

	    status = pthread_cond_init (&new_stage->available, NULL);
	    if (status)
		err_abort (status, "pthread_cond_init (pipe)");

	    status = pthread_cond_init (&new_stage->ready, NULL);
	    if (status)
		err_abort (status, "pthread_cond_init 2 (pipe)");

	    new_stage->data_is_ready = 0;
	    *link = new_stage;
	    link = &new_stage->next;
	}

    // Terminates the list
    *link = NULL;
    pipe->tail = new_stage;

    struct stage *curr_stage;
    // This for loop excludes the last stage, since the last is only a
    // container for the result and does not need a thread
    for (curr_stage = pipe->head; curr_stage->next;
	 curr_stage = curr_stage->next)
	{
	    status = pthread_create (&curr_stage->thread, NULL, pipe_stage,
				     (void *)curr_stage);
	    if (status)
		err_abort (status, "pthread_create (pipe)");
	}

    return 0;
}

void
pipe_start (struct pipeline *pipe, int value)
{
    int status;

    status = pthread_mutex_lock (&pipe->lock);
    if (status)
	err_abort (status, "pthread_mutex_lock (pipe_start)");

    pipe->num_active++;

    status = pthread_mutex_unlock (&pipe->lock);
    if (status)
	err_abort (status, "pthread_mutex_unlock (pipe_start)");

    // Push into the first stage
    pipe_send (pipe->head, value);
}

int
pipe_result (struct pipeline *pipe, int *result)
{
    struct stage *tail = pipe->tail;
    int value;
    int is_empty = 0;
    int status;

    status = pthread_mutex_lock (&pipe->lock);
    if (status)
	err_abort (status, "pthread_mutex_lock (pipe_result)");

    if (pipe->num_active <= 1)
	is_empty = 1;
    else
	pipe->num_active--;

    status = pthread_mutex_unlock (&pipe->lock);
    if (status)
	err_abort (status, "pthread_mutex_unlock (pipe_result)");

    if (is_empty)
	return 1;

    status = pthread_mutex_lock (&tail->lock);
    if (status)
	err_abort (status, "pthread_mutex_lock 2 (pipe_result)");

    while (!tail->data_is_ready)
	{
	    status = pthread_cond_wait (&tail->available, &tail->lock);
	    if (status)
		err_abort (status, "pthread_cond_wait (pipe_result)");
	}

    *result = tail->data;
    tail->data_is_ready = 0;

    status = pthread_cond_signal (&tail->ready);
    if (status)
	err_abort (status, "pthread_cond_signal (pipe_result)");

    status = pthread_mutex_unlock (&tail->lock);
    if (status)
	err_abort (status, "pthread_mutex_unlock 2 (pipe_result)");

    return 0;
}

int
main (int argc, char *argv[])
{

    char line[128];
    int len;

    int value;
    int result;

    struct pipeline pipe;

    for (;;)
	{
	    puts ("> ");
	    if (fgets (line, sizeof (line), stdin) == NULL)
		exit (0);

	    len = strlen (line);
	    if (len <= 1)
		continue;

	    if (strcmp (line, "="))
		{
		    if (!pipe_result (&pipe, &result))
			printf ("result=%d\n", result);
		    else
			puts ("empty pipeline");
		}
	    else
		{
		    if (sscanf (line, "%d", &value) < 1)
			fprintf (stderr, "enter an integer\n");
		    else
			pipe_start (&pipe, value);
		}
	}

    return EXIT_SUCCESS;
}
