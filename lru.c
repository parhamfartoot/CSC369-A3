#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

extern struct frame *coremap;

struct frame *head;
struct frame *tail;

/* Page to evict is chosen using the accurate LRU algorithm.
 * Returns the page frame number (which is also the index in the coremap)
 * for the page that is to be evicted.
 */

int lru_evict()
{
	struct frame *evict = tail;
	return evict->pte->frame >> PAGE_SHIFT;

}

/* This function is called on each access to a page to update any information
 * needed by the lru algorithm.
 * Input: The page table entry for the page that is being accessed.
 */
void lru_ref(pgtbl_entry_t *p) {

	int frame = p->frame >> PAGE_SHIFT;
	struct frame *mrf = &coremap[frame]; //Most Recently used frame

	//Refrenced frame already is head
	if(mrf == head)
	{
		return;
	}
	mrf->prev->next = mrf->next;
	if(mrf == tail)
	{
		tail = tail->prev;
	}
	else
	{
		mrf->next->prev = mrf->prev;
	}
	mrf->prev = NULL;
	mrf->next =head;
	mrf->next->prev = mrf;
	head = mrf;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	
	//First Frame has NULL prev
	(&coremap[0])->prev = NULL;
	for (int i = 1; i < (memsize - 1); i++)
	{
		(&coremap[i])->prev = &coremap[i - 1];
		(&coremap[i])->next = &coremap[i + 1];
	}
	//Last Frame has NULL next
	(&coremap[-1])->next = NULL;
	head = (&coremap[0]);
	tail = (&coremap[memsize - 1]);
}