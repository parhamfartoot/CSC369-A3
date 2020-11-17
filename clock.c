#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"
extern int memsize;

extern int debug;

extern struct frame *coremap;

int clock;
/* Page to evict is chosen using the clock algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int clock_evict()
{
	while (1)
	{
		//find a frame that has not been refrenced and return its page number
		if (coremap[clock].pte->frame & PG_REF)
		{
			coremap[clock].pte->frame &= ~PG_REF;
			clock++;

			if (clock >= memsize)
			{
				clock = 0;
			}
		}
		else
		{
			return clock;
		}
	}
}

/* This function is called on each access to a page to update any information
 * needed by the clock algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void clock_ref(pgtbl_entry_t *p)
{

	p->frame |= PG_REF;
	return;
}

/* Initialize any data structures needed for this replacement
 * algorithm. 
 */
void clock_init()
{

	clock = 0;
}
