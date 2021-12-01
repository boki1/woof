#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

struct alarm
{
    struct alarm *link;
    int seconds;
    time_t time; /* seconds from EPOCH */
    char message[64];
};

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_cond = PTHREAD_COND_INITIALIZER;
struct alarm *alarm_list = NULL;
time_t current_alarm = 0;

/*
 * Insert alarm entry on list, in order.
 */
void
alarm_insert (struct alarm *alarm)
{
    int status;
    struct alarm **last, *next;

    /*
     * LOCKING PROTOCOL:
     *
     * This routine requires that the caller have locked the
     * alarm_mutex!
     */
    last = &alarm_list;
    next = *last;
    while (next != NULL)
	{
	    if (next->time >= alarm->time)
		{
		    alarm->link = next;
		    *last = alarm;
		    break;
		}
	    last = &next->link;
	    next = next->link;
	}
    /*
     * If we reached the end of the list, insert the new alarm
     * there.  ("next" is NULL, and "last" points to the link
     * field of the last item, or to the list header.)
     */
    if (next == NULL)
	{
	    *last = alarm;
	    alarm->link = NULL;
	}
#ifdef DEBUG
    printf ("[list: ");
    for (next = alarm_list; next != NULL; next = next->link)
	printf ("%d(%d)[\"%s\"] ", next->time, next->time - time (NULL),
		next->message);
    printf ("]\n");
#endif
    /*
     * Wake the alarm thread if it is not busy (that is, if
     * current_alarm is 0, signifying that it's waiting for
     * work), or if the new alarm comes before the one on
     * which the alarm thread is waiting.
     */
    if (current_alarm == 0 || alarm->time < current_alarm)
	{
	    current_alarm = alarm->time;
	    status = pthread_cond_signal (&alarm_cond);
	    if (status != 0)
		perror ("pthread_cond_signal");
	}
}

/*
 * The alarm thread's start routine.
 */
void *
alarm_thread (void *arg)
{
    struct alarm *alarm;
    struct timespec cond_time;
    time_t now;
    int status, expired;

    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits. Lock the mutex
     * at the start -- it will be unlocked during condition
     * waits, so the main thread can insert alarms.
     */
    status = pthread_mutex_lock (&alarm_mutex);
    if (status != 0)
	perror ("pthread_mutex_lock");

    while (1)
	{
	    /*
	     * If the alarm list is empty, wait until an alarm is
	     * added. Setting current_alarm to 0 informs the insert
	     * routine that the thread is not busy.
	     */
	    current_alarm = 0;
	    while (alarm_list == NULL)
		{
		    status = pthread_cond_wait (&alarm_cond, &alarm_mutex);
		    if (status != 0)
			perror ("pthread_cond_wait");
		}
	    alarm = alarm_list;
	    alarm_list = alarm->link;
	    now = time (NULL);
	    expired = 0;
	    if (alarm->time > now)
		{
#ifdef DEBUG
		    printf ("[waiting: %d(%d)\"%s\"]\n", alarm->time,
			    alarm->time - time (NULL), alarm->message);
#endif
		    cond_time.tv_sec = alarm->time;
		    cond_time.tv_nsec = 0;
		    current_alarm = alarm->time;
		    while (current_alarm == alarm->time)
			{
			    status = pthread_cond_timedwait (
				&alarm_cond, &alarm_mutex, &cond_time);
			    if (status == ETIMEDOUT)
				{
				    expired = 1;
				    break;
				}
			    if (status != 0)
				perror ("pthread_cond_timedwait");
			}

		    if (!expired)
			alarm_insert (alarm);
		}
	    else
		expired = 1;
	    if (expired)
		{
		    printf ("(%d) %s\n", alarm->seconds, alarm->message);
		    free (alarm);
		}
	}
}

int
main (int argc, char *argv[])
{
    int status;
    char line[128];
    struct alarm *alarm;
    pthread_t thread;

    status = pthread_create (&thread, NULL, alarm_thread, NULL);
    if (status != 0)
	perror ("pthread_create");

    while (1)
	{
	    printf ("Alarm> ");
	    if (fgets (line, sizeof (line), stdin) == NULL)
		exit (0);
	    if (strlen (line) <= 1)
		continue;
	    alarm = malloc (sizeof (struct alarm));
	    if (alarm == NULL)
		perror ("malloc");

	    /*
	     * Parse input line into seconds (%d) and a message
	     * (%64[^\n]), consisting of up to 64 characters
	     * separated from the seconds by whitespace.
	     */
	    if (sscanf (line, "%d %64[^\n]", &alarm->seconds, alarm->message)
		< 2)
		{
		    fprintf (stderr, "Bad command\n");
		    free (alarm);
		}
	    else
		{
		    status = pthread_mutex_lock (&alarm_mutex);
		    if (status != 0)
			perror ("pthread_mutex_lock");

		    alarm->time = time (NULL) + alarm->seconds;
		    /*
		     * Insert the new alarm into the list of alarms,
		     * sorted by expiration time.
		     */
		    alarm_insert (alarm);
		    status = pthread_mutex_unlock (&alarm_mutex);
		    if (status != 0)
			perror ("pthread_mutex_unlock");
		}
	}
}

