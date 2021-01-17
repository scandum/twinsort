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

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
	EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	tailsort 1.1.3.3
*/

#ifndef TAILSORT_H
#define TAILSORT_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <errno.h>

typedef int CMPFUNC (const void *a, const void *b);

///////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────┐//
// │   ██████┐ ██████┐    ██████┐ ██████┐████████┐  │//
// │   └────██┐└────██┐   ██┌──██┐└─██┌─┘└──██┌──┘  │//
// │    █████┌┘ █████┌┘   ██████┌┘  ██│     ██│     │//
// │    └───██┐██┌───┘    ██┌──██┐  ██│     ██│     │//
// │   ██████┌┘███████┐   ██████┌┘██████┐   ██│     │//
// │   └─────┘ └──────┘   └─────┘ └─────┘   └─┘     │//
// └────────────────────────────────────────────────┘//
///////////////////////////////////////////////////////

// Turn the array into sorted blocks of 2 elements.
// Detect and sort reverse order runs. So 6 5 4 3 2 1 becomes 1 2 3 4 5 6
// rather than 5 6 3 4 1 2

unsigned int twin_swap32(int *array, unsigned int nmemb, CMPFUNC *cmp)
{
	register int *pta, *pts, *ptt, *pte, tmp;

	pta = array;
	pte = pta + nmemb - 2;

	while (pta <= pte)
	{
		if (cmp(&pta[0], &pta[1]) <= 0)
		{
			pta += 2;

			continue;
		}
		pts = pta;

		pta += 2;

		while (1)
		{
			if (pta > pte)
			{
				if (pts == array)
				{
					if (nmemb % 2 == 0 || cmp(&pta[-1], &pta[0]) > 0)
					{
						ptt = pte + 1;

						while (pts < ptt)
						{
							tmp = *pts;
							*pts++ = *ptt;
							*ptt-- = tmp;
						}
						return 1; // entire array was in reverse order
					}
				}
				break;
			}

			if (cmp(&pta[0], &pta[1]) > 0)
			{
				if (cmp(&pta[-1], &pta[0]) > 0)
				{
					pta += 2;
					continue;
				}
				tmp = pta[0]; pta[0] = pta[1]; pta[1] = tmp;
			}
			break;
		}
		ptt = pta - 1;

		while (pts < ptt)
		{
			tmp = *pts;
			*pts++ = *ptt;
			*ptt-- = tmp;
		}
		pta += 2;
	}
	return 0;
}

// Bottom up merge sort. It copies the right block to swap, next merges
// starting at the tail ends of the two sorted blocks.
// Can be used stand alone. Uses at most nmemb / 2 swap memory.

void tail_merge32(int *array, int *swap, unsigned int nmemb, unsigned int block, CMPFUNC *cmp)
{
	unsigned int offset;
	register int *pta, *pts, *c, *c_max, *d, *d_max, *e;

	pts = swap;

	while (block < nmemb)
	{
		for (offset = 0 ; offset + block < nmemb ; offset += block * 2)
		{
			pta = array + offset;

			e = pta + block - 1;

			if (cmp(e, e + 1) <= 0)
			{
				continue;
			}

			if (offset + block * 2 <= nmemb)
			{
				c_max = pts + block;
				d_max = pta + block * 2;
			}
			else
			{
				c_max = pts + nmemb - (offset + block);
				d_max = array + nmemb;
			}

			d = d_max - 1;

			while (cmp(e, d) <= 0)
			{
				d_max--;
				d--;
				c_max--;
			}

			c = pts;
			d = pta + block;

			while (c < c_max)
			{
				*c++ = *d++;
			}
			c--;

			d = pta + block - 1;
			e = d_max - 1;

			if (cmp(pta, pta + block) <= 0)
			{
				*e-- = *d--;

				while (c >= pts)
				{
					while (cmp(d, c) > 0)
					{
						*e-- = *d--;
					}
					*e-- = *c--;
				}
			}
			else
			{
				*e-- = *d--;

				while (d >= pta)
				{
					while (cmp(d, c) <= 0)
					{
						*e-- = *c--;
					}
					*e-- = *d--;
				}
				while (c >= pts)
				{
					*e-- = *c--;
				}
			}
		}
		block *= 2;
	}
}

// This is a duplicate of the 32 bit version and the only difference is that
// each instance of 'int' has been changed to 'long long'. It's a bit
// unorthodox, but it does allow for string sorting on both 32 and 64 bit
// systems with (hopefully) optimal optimization.

///////////////////////////////////////////////////////
// ┌────────────────────────────────────────────────┐//
// │    █████┐ ██┐  ██┐   ██████┐ ██████┐████████┐  │//
// │   ██┌───┘ ██│  ██│   ██┌──██┐└─██┌─┘└──██┌──┘  │//
// │   ██████┐ ███████│   ██████┌┘  ██│     ██│     │//
// │   ██┌──██┐└────██│   ██┌──██┐  ██│     ██│     │//
// │   └█████┌┘     ██│   ██████┌┘██████┐   ██│     │//
// │    └────┘      └─┘   └─────┘ └─────┘   └─┘     │//
// └────────────────────────────────────────────────┘//
///////////////////////////////////////////////////////

unsigned int twin_swap64(long long *array, unsigned int nmemb, CMPFUNC *cmp)
{
	register long long *pta, *pts, *ptt, *pte, tmp;

	pta = array;
	pte = pta + nmemb - 2;

	while (pta <= pte)
	{
		if (cmp(&pta[0], &pta[1]) <= 0)
		{
			pta += 2;

			continue;
		}
		pts = pta;

		pta += 2;

		while (1)
		{
			if (pta > pte)
			{
				if (pts == array)
				{
					if (nmemb % 2 == 0 || cmp(&pta[-1], &pta[0]) > 0)
					{
						ptt = pte + 1;

						while (pts < ptt)
						{
							tmp = *pts;
							*pts++ = *ptt;
							*ptt-- = tmp;
						}
						return 1; // entire array was in reverse order
					}
				}
				break;
			}

			if (cmp(&pta[0], &pta[1]) > 0)
			{
				if (cmp(&pta[-1], &pta[0]) > 0)
				{
					pta += 2;
					continue;
				}
				tmp = pta[0]; pta[0] = pta[1]; pta[1] = tmp;
			}
			break;
		}

		ptt = pta - 1;
		pta += 2;

		while (pts < ptt)
		{
			tmp = *pts;
			*pts++ = *ptt;
			*ptt-- = tmp;
		}
	}
	return 0;
}

void tail_merge64(long long *array, long long *swap, size_t nmemb, size_t block, CMPFUNC *cmp)
{
	size_t offset;
	register long long *pta, *pts, *c, *c_max, *d, *d_max, *e;

	pts = swap;

	while (block < nmemb)
	{
		for (offset = 0 ; offset + block < nmemb ; offset += block * 2)
		{
			pta = array + offset;

			e = pta + block - 1;

			if (cmp(e, e + 1) <= 0)
			{
				continue;
			}

			if (offset + block * 2 <= nmemb)
			{
				c_max = pts + block;
				d_max = pta + block * 2;
			}
			else
			{
				c_max = pts + nmemb - (offset + block);
				d_max = array + nmemb;
			}

			d = d_max - 1;

			while (cmp(e, d) <= 0)
			{
				d_max--;
				d--;
				c_max--;
			}

			c = pts;
			d = pta + block;

			while (c < c_max)
			{
				*c++ = *d++;
			}
			c--;

			d = pta + block - 1;
			e = d_max - 1;

			if (cmp(pta, pta + block) <= 0)
			{
				*e-- = *d--;

				while (c >= pts)
				{
					while (cmp(d, c) > 0)
					{
						*e-- = *d--;
					}
					*e-- = *c--;
				}
			}
			else
			{
				*e-- = *d--;

				while (d >= pta)
				{
					while (cmp(d, c) <= 0)
					{
						*e-- = *c--;
					}
					*e-- = *d--;
				}
				while (c >= pts)
				{
					*e-- = *c--;
				}
			}
		}
		block *= 2;
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

void tailsort(void *array, size_t nmemb, size_t size, CMPFUNC *cmp)
{
	if (nmemb < 2)
	{
		return;
	}

	if (size == sizeof(int))
	{
		if (twin_swap32(array, nmemb, cmp) == 0)
		{
			int *swap = malloc(nmemb * size / 2);

			if (swap == NULL)
			{
				fprintf(stderr, "tailsort(%p,%zu,%zu,%p): malloc() failed: %s\n", array, nmemb, size, cmp, strerror(errno));

				return;
			}

			tail_merge32(array, swap, nmemb, 2, cmp);

			free(swap);
		}
	}
	else if (size == sizeof(long long))
	{
		if (twin_swap64(array, nmemb, cmp) == 0)
		{
			long long *swap = malloc(nmemb * size / 2);

			if (swap == NULL)
			{
				fprintf(stderr, "tailsort(%p,%zu,%zu,%p): malloc() failed: %s\n", array, nmemb, size, cmp, strerror(errno));

				return;
			}
			tail_merge64(array, swap, nmemb, 2, cmp);

			free(swap);
		}
	}
	else
	{
		assert(size == sizeof(int) || size == sizeof(long long));
	}
}

#endif
