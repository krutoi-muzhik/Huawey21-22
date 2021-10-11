#include <stdio.h>
#include <wchar.h>
#include <memory.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

//sigaction
//typeid

typedef int type;
#define OUTPUT_FORMAT "%d\n"

typedef struct Stack {
	type canary1;
	ssize_t capacity;
	ssize_t size;
	type* data;
	unsigned long long hash;
	type canary2;
} stack;

enum Errors {
	NULL_POINTER = 1,
	CANARY1_KILLED = 2,
	CANARY2_KILLED = 3,
	DATA_INVASION = 4,
	SIZE_ERROR = 5,
	CAPACITY_ERROR = 6,
	STACK_HASH_ERROR = 7,
	DATA_HASH_ERROR = 8
};

const type poison = 0xBEBABABA;
const type canary = 0xBEFEDABA;

unsigned long long GetCRC (char* data, size_t start, size_t end);
unsigned long long GetStackHash (stack* stk);
type GetDataHash (stack* stk);
size_t GetStackSize (stack* stk);
size_t GetStackCapacity (stack* stk);
void StackSizeUp (stack* stk);
void StackSizeDown (stack* stk);
void StackConstruct (stack* stk);
void StackDestruct (stack* stk);
void StackPush (stack* stk, type value);
type StackPop (stack* stk);
int StackError (stack* stk) {
	if (stk == NULL)
		return NULL_POINTER;
	
	if (stk->canary1 != canary)
		return CANARY1_KILLED;
	
	if (stk->canary2 != canary)
		return CANARY2_KILLED;

	stk->data--;
	if ((stk->data[0] != canary) || (stk->data[stk->capacity + 1] != canary))
		return DATA_INVASION;


}

// enum Errors {

// };

int main () {
	stack stk;
	StackConstruct (&stk);
	// printf ("hash = %lld\n", stk.hash);
	for (int i = 0; i < 20; i++) {
		StackPush (&stk, i);
		printf ("pushed %d\n", i);
	}

	// printf ("hash = %lld\n", stk.hash);
	printf (OUTPUT_FORMAT, StackPop (&stk));
	printf ("capacity = %ld\n", GetStackCapacity (&stk));
	StackDestruct (&stk);
	return 0;
}


unsigned long long GetStackHash (stack* stk) {
	unsigned long long hash = 0;
	hash = GetCRC ((char*) stk, sizeof (stk->canary1), sizeof (*stk) - sizeof (stk->canary2) - sizeof (stk->hash));
	//hash = GetCRC ((char*) stk, sizeof (stk->canary1), sizeof (stk) - sizeof (stk->canary2) - sizeof (stk->hash));
	return hash;
}

unsigned long long GetCRC (char* data, size_t start, size_t end) {
	const unsigned int Polynomial = 0xEDB88320;
	unsigned long long crc = 0;
	size_t length = end;
	while (length-- > start) {
		crc ^= *data++;

		for (size_t j = 0; j < 8; j++)
			crc = (crc >> 1) ^ (-(int)(crc & 1) & Polynomial);
	}

	return crc;
}

type GetDataHash (stack* stk) {
	type hash = 0;
	hash = (type) GetCRC ((char*)stk->data, 0, sizeof (stk->data[0]) * stk->size);
	return hash;
}

void StackConstruct (stack* stk) {
	stk->capacity = 1;	
	stk->data = (type*) calloc (stk->capacity + 2, sizeof (stk->data[0]));
	stk->size = 0;

	wmemset (stk->data, poison, stk->capacity + 3);
	stk->data[0] = canary;
	stk->data[stk->capacity + 1] = GetDataHash (stk);
	stk->data[stk->capacity + 2] = canary;
	stk->data += 1;

	stk->canary1 = canary;
	stk->canary2 = canary;
	stk->hash = GetStackHash (stk);
}

void StackDestruct (stack* stk) {
	stk->capacity = 0;
	stk->size = 0;
	stk->data--;
	free (stk->data);
}

void StackSizeUp (stack* stk) {
	stk->capacity *= 2;
	stk->data--;
	printf ("realloc : %ld\n", (stk->capacity + 3) * sizeof (stk->data[0]));
	stk->data = (type*) realloc (stk->data, (stk->capacity + 3) * sizeof (stk->data[0]));
	//type hash = stk->data[capacity + 1]
	wmemset (stk->data + stk->size + 1, poison, stk->capacity);
	stk->data[stk->capacity + 2] = canary;
	stk->data++;
}

void StackPush (stack* stk, type value) {
	if (stk->size == stk->capacity)
		StackSizeUp (stk);
	stk->data[stk->size] = value;
	stk->size++;
	stk->hash = GetStackHash (stk);
	stk->data[stk->capacity] = GetDataHash (stk);
}

void StackSizeDown (stack* stk) {
	stk->capacity /= 2;
	stk->data--;
	int newsize = (stk->capacity + 3) * sizeof (stk->data[0]);
	printf ("realloc : %d\n", newsize);
	stk->data = (type*) realloc (stk->data, (stk->capacity + 3) * sizeof (stk->data[0]));
	// stk->data = (type*) realloc (stk->data, newsize);
	stk->data[stk->capacity + 2] = canary;
	stk->data++;
}

type StackPop (stack* stk) {
	if (stk->size < stk->capacity / 3)
		StackSizeDown (stk);
	type value = 0;
	stk->size--;
	value = stk->data[stk->size];
	stk->data[stk->size] = poison;
	stk->hash = GetStackHash (stk);
	stk->data[stk->capacity] = GetDataHash (stk);
	return value;
}

size_t GetStackSize (stack* stk) {
	size_t size = 0;
	while (stk->data[size] != poison) {
		
		if (stk->data[size] == canary) {
			size --;
			break;
		}
		size++;
	}
	return size;	
}

size_t GetStackCapacity (stack* stk) {
	size_t capacity = 0;
	while (stk->data[capacity] != canary)
		capacity++;
	return capacity - 1;
}