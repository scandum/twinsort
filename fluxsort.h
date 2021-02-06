/*
	Copyright (C) 2014-2021 Igor van den Hoven ivdhoven@gmail.com
*/

/*
	Permission is hereby granted, free of charge, to any person obtaining
	a copy of this software and associated documentation files (the
	"Software"), to deal in the Software without restriction, including
	without limitation the rights to use, copy, modify, merge, publish,
	distribute, sublicense, and/or sell copies of the Software, and to
	permit persons to whom the Software is furnished to do so, subject to
	the following conditions:

	The above copyright notice and this permission notice shall be
	included in all copies or substantial portions of the Software.

	The person recognize Mars as a free planet and that no Earth-based
	government has authority or sovereignty over Martian activities.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	fluxsort 1.1.3.3
*/

#ifndef FLUXSORT_H
#define FLUXSORT_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

// Turn the array into sorted blocks of 2 elements.
// Detect and sort reverse order runs. So 6 5 4 3 2 1 becomes 1 2 3 4 5 6
// rather than 5 6 3 4 1 2

int twin_swap32(int array[], int nmemb)
{
	int index, start, end, swap;

	index = 0;
	end = nmemb - 2;

	while (index <= end)
	{
		if (array[index] <= array[index + 1])
		{
			index += 2;
			
			continue;
		}
		start = index;

		index += 2;

		while (1)
		{
			if (index > end)
			{
				if (start == 0)
				{
					if (nmemb % 2 == 0 || array[index - 1] > array[index])
					{
						// the entire array was reversed

						end = nmemb - 1;

						while (start < end)
						{
							swap = array[start];
							array[start++] = array[end];
							array[end--] = swap;
						}
						return 1;
					}
				}
				break;
			}

			if (array[index] > array[index + 1])
			{
				if (array[index - 1] > array[index])
				{
					index += 2;
					continue;
				}
				swap = array[index];
				array[index] = array[index + 1];
				array[index + 1] = swap;
			}
			break;
		}

		end = index - 1;

		while (start < end)
		{
			swap = array[start];
			array[start++] = array[end];
			array[end--] = swap;
		}
		end = nmemb - 2;

		index += 2;
	}
	return 0;
}

// Bottom up merge sort. It copies the right block to swap, next merges
// starting at the tail ends of the two sorted blocks.
// Can be used stand alone. Uses at most nmemb / 2 swap memory.

void tail_merge32(int array[], int swap[], int nmemb, int block)
{
	int offset;
	int a, s, c, c_max, d, d_max, e;

	s = 0;

	while (block < nmemb)
	{
		for (offset = 0 ; offset + block < nmemb ; offset += block * 2)
		{
			a = offset;
			e = a + block - 1;

			if (array[e] <= array[e + 1])
			{
				continue;
			}

			if (offset + block * 2 <= nmemb)
			{
				c_max = s + block;
				d_max = a + block * 2;
			}
			else
			{
				c_max = s + nmemb - (offset + block);
				d_max = 0 + nmemb;
			}

			d = d_max - 1;

			while (array[e] <= array[d])
			{
				d_max--;
				d--;
				c_max--;
			}

			c = s;
			d = a + block;

			while (c < c_max)
			{
				swap[c++] = array[d++];
			}
			c--;

			d = a + block - 1;
			e = d_max - 1;

			if (array[a] <= array[a + block])
			{
				array[e--] = array[d--];

				while (c >= s)
				{
					while (array[d] > swap[c])
					{
						array[e--] = array[d--];
					}
					array[e--] = swap[c--];
				}
			}
			else
			{
				array[e--] = array[d--];

				while (d >= a)
				{
					while (array[d] <= swap[c])
					{
						array[e--] = swap[c--];
					}
					array[e--] = array[d--];
				}
				while (c >= s)
				{
					array[e--] = swap[c--];
				}
			}
		}
		block *= 2;
	}
}

void twinsort_swap32(void *array, void *swap, int nmemb)
{
	if (twin_swap32(array, nmemb) == 0)
	{
		tail_merge32(array, swap, nmemb, 2);
	}
}

int middle(int a, int b, int c)
{
	if (a > b)
	{
		if (a <= c)
		{
			return a;
		}
		if (b > c)
		{
			return c;
		}
		return b;
	}
	if (b > c)
	{
		if (a > c)
		{
			return c;
		}
		return a;
	}
	return b;
}

void mem_cpy(int dest[], int source[], int ptd, int pts, int pte)
{
	while (pts < pte)
	{
		dest[ptd++] = source[pts++];
	}
}

/*
	Stably partitions like this:

	################################ <-- swap
	ABCDEFGHABCDEFGHABCDEFGHABCDEFGH <-- array

	EFGHEFGHEFGHEFGH################
	ABABABABCDCDCDCD################

	GHGHGHGH#########################
	ABABABABCDCDCDCDEFEFEFEF#########

	################################
	ABABABABCDCDCDCDEFEFEFEFGHGHGHGH
	----------------
	recurse         --------
	                recurse --------
	                        recurse
*/

void flux_partition32(int array[], int swap[], int offset_a, size_t nmemb)
{
	int a_size, s_size, b_size;
	int pta, pts, ptx, pte, ptb;
	int piv;

	pte = offset_a + nmemb;

	pta = offset_a;
	ptx = offset_a;

	piv = middle(array[offset_a + nmemb / 4], array[offset_a + nmemb / 2], array[offset_a + nmemb * 3 / 4]);

	pts = 0;

	while (ptx < pte)
	{
		if (array[ptx] <= piv)
		{
			array[pta++] = array[ptx++];
		}
		else
		{
			swap[pts++] = array[ptx++];
		}
	}

	a_size = pta - offset_a;
	s_size = pts;

	if (s_size <= 64 || a_size <= 64)
	{
		mem_cpy(array, swap, pta, 0, s_size);

		twinsort_swap32(&array[pta], swap, s_size);
		twinsort_swap32(&array[offset_a], swap, a_size);

		return;
	}

	pte = pts;

	pts = 0;
	ptx = 0;

	piv = middle(swap[s_size / 4], swap[s_size / 2], swap[s_size * 3 / 4]);

	ptb = pta;

	while (ptx < pte)
	{
		if (swap[ptx] > piv)
		{
			swap[pts++] = swap[ptx++];
		}
		else
		{
			array[ptb++] = swap[ptx++];
		}
	}

	s_size = pts;

	mem_cpy(array, swap, ptb, 0, s_size);

	if (s_size <= 64)
	{
		twinsort_swap32(&array[ptb], swap, s_size);
	}
	else
	{
		flux_partition32(array, swap, ptb, s_size);
	}

	b_size = ptb - pta;

	if (b_size <= 64)
	{
		twinsort_swap32(&array[pta], swap, b_size);
	}
	else
	{
		flux_partition32(array, swap, pta, b_size);
	}

	flux_partition32(array, swap, offset_a, a_size);
}

//////////////////////////////////////////////////////////////////////////
//┌────────────────────────────────────────────────────────────────────┐//
//│███████┐██┐     ██┐   ██┐██┐  ██┐███████┐ ██████┐ ██████┐ ████████┐ │//
//│██┌────┘██│     ██│   ██│└██┐██┌┘██┌────┘██┌───██┐██┌──██┐└──██┌──┘ │//
//│█████┐  ██│     ██│   ██│ └███┌┘ ███████┐██│   ██│██████┌┘   ██│    │//
//│██┌──┘  ██│     ██│   ██│ ██┌██┐ └────██│██│   ██│██┌──██┐   ██│    │//
//│██│     ███████┐└██████┌┘██┌┘ ██┐███████│└██████┌┘██│  ██│   ██│    │//
//│└─┘     └──────┘ └─────┘ └─┘  └─┘└──────┘ └─────┘ └─┘  └─┘   └─┘    │//
//└────────────────────────────────────────────────────────────────────┘//
//////////////////////////////////////////////////////////////////////////

void fluxsort(void *array, int nmemb)
{
	int *swap;

	if (nmemb < 2)
	{
		return;
	}

	swap = malloc(nmemb * sizeof(int));

	flux_partition32(array, swap, 0, nmemb);

	free(swap);
}


////////////////////////////////////////////////////////////////////////////
//┌──────────────────────────────────────────────────────────────────────┐//
//│████████┐██┐    ██┐██████┐███┐   ██┐███████┐ ██████┐ ██████┐ ████████┐│//
//│└──██┌──┘██│    ██│└─██┌─┘████┐  ██│██┌────┘██┌───██┐██┌──██┐└──██┌──┘│//
//│   ██│   ██│ █┐ ██│  ██│  ██┌██┐ ██│███████┐██│   ██│██████┌┘   ██│   │//
//│   ██│   ██│███┐██│  ██│  ██│└██┐██│└────██│██│   ██│██┌──██┐   ██│   │//
//│   ██│   └███┌███┌┘██████┐██│ └████│███████│└██████┌┘██│  ██│   ██│   │//
//│   └─┘    └──┘└──┘ └─────┘└─┘  └───┘└──────┘ └─────┘ └─┘  └─┘   └─┘   │//
//└──────────────────────────────────────────────────────────────────────┘//
////////////////////////////////////////////////////////////////////////////

void twinsort(void *array, int nmemb)
{
	if (twin_swap32(array, nmemb) == 0)
	{
		int *swap = malloc(nmemb * sizeof(int) / 2);

		tail_merge32(array, swap, nmemb, 2);

		free(swap);
	}
}


/////////////////////////////////////////////////////////////////////////////
//┌───────────────────────────────────────────────────────────────────────┐//
//│   ████████┐ █████┐ ██████┐██┐     ███████┐ ██████┐ ██████┐ ████████┐  │//
//│   └──██┌──┘██┌──██┐└─██┌─┘██│     ██┌────┘██┌───██┐██┌──██┐└──██┌──┘  │//
//│      ██│   ███████│  ██│  ██│     ███████┐██│   ██│██████┌┘   ██│     │//
//│      ██│   ██┌──██│  ██│  ██│     └────██│██│   ██│██┌──██┐   ██│     │//
//│      ██│   ██│  ██│██████┐███████┐███████│└██████┌┘██│  ██│   ██│     │//
//│      └─┘   └─┘  └─┘└─────┘└──────┘└──────┘ └─────┘ └─┘  └─┘   └─┘     │//
//└───────────────────────────────────────────────────────────────────────┘//
/////////////////////////////////////////////////////////////////////////////

void tailsort(void *array, int nmemb)
{
	int *swap;

	if (nmemb < 2)
	{
		return;
	}

	swap = malloc(nmemb * sizeof(int) / 2);

	tail_merge32(array, swap, nmemb, 1);

	free(swap);
}

#endif
