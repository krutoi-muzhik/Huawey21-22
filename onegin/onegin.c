#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
// #include <io.h>


enum SIZE {
	DEFAULTSIZE = 10,
	EXTRASIZE = 10
};


struct Text {
	char* OriginalText;
	size_t NSymbols;
	size_t NLines;
	char** Pointers;
	size_t* Lens;
};


int StringCompare (char** ptr1, char** ptr2);
void QuickSort (char** ArrPtr, size_t ArrSize,  int (*Compare) (char** ptr1, char** ptr2));
int FileRead (const char* InputFile, struct Text* novel);
void TextClean (struct Text* novel);
void FileWrite (char* OutputFile, struct Text* novel);
int FromEndCompare (char** ptr1, char** ptr2);

int main () {
	
	struct Text novel;
	char* InputFile = "onegin.txt";
	char* OutputFile = "sorted.txt";
	if (FileRead (InputFile, &novel) == 0) return 0;
	TextClean (&novel);
	FileWrite (OutputFile, &novel);
	return 0;
}



void TextClean (struct Text* novel) {
	novel->Pointers = (char**) calloc (DEFAULTSIZE, sizeof (char*));
	novel->Lens = (size_t*) calloc (DEFAULTSIZE, sizeof (size_t));
	int CURRENTSIZE = DEFAULTSIZE;
	novel->Pointers[0] = novel->OriginalText;
	//printf ("NSymbols = %ld\n", novel->NSymbols);

	for (size_t i = 0; i < novel->NSymbols; i++) {
		//printf ("i = %ld, char[i] = %c (%d)\n", i, novel->OriginalText[i], novel->OriginalText[i]);
		if ((novel->OriginalText[i] == '\r') || (novel->OriginalText[i] == '\0')) {
			novel->OriginalText[i] = '\0';
			//printf ("\n\nстрока = %s\n\n", novel->Pointers[novel->NLines]);
			if (novel->NLines > CURRENTSIZE) {
				CURRENTSIZE += EXTRASIZE;
				novel->Pointers = (char**) realloc (novel->Pointers, CURRENTSIZE * sizeof (char*));
				novel->Lens = (size_t*) realloc (novel->Lens, CURRENTSIZE * sizeof (size_t));
			}
			//printf ("%ld\n", novel->NLines);
			//printf ("%ld\n", novel->Lens[novel->NLines]);
			novel->Lens[novel->NLines] = novel->OriginalText + i - novel->Pointers[novel->NLines];
			//printf ("%ld\n", novel->Lens[novel->NLines]);
			novel->NLines++;
		}
		if ((novel->OriginalText[i] == '\n') && (i + 1 < novel->NSymbols)) {
			if (novel->NLines >= CURRENTSIZE) {
				CURRENTSIZE += EXTRASIZE;
				novel->Pointers = (char**) realloc (novel->Pointers, CURRENTSIZE * sizeof (char*));
				novel->Lens = (size_t*) realloc (novel->Lens, CURRENTSIZE * sizeof (size_t));
			}
			novel->Pointers[novel->NLines] = novel->OriginalText + i + 1;
		}
	}
	printf ("NLines = %ld\n\n", novel->NLines);
	// QuickSort (novel->Pointers, novel->NLines, *StringCompare);
	// for (int i = 0; i < novel->NLines; i++) {
	//  	printf ("%s\n", novel->Pointers[i]);
	// }
	//free (novel->Pointers);
	//free (novel->Lens);
	return;
}



int FileRead (const char* InputFile, struct Text* novel) {
	struct stat StatBuff;
	int finp = open (InputFile, O_APPEND);
	if (finp == -1) {printf ("Error unable to open file %s\n", InputFile); return 0;}

	stat (InputFile, &StatBuff);
	printf ("size of %s is %ld\n", InputFile, StatBuff.st_size);

	char* CharBuffer = (char*) calloc (StatBuff.st_size + 1, sizeof (char));
	read (finp, CharBuffer, StatBuff.st_size + 1);
	close (finp);
	// for (int i = 0; i < StatBuff.st_size + 1; i++) {
	// 	printf ("%c (%d)\n", CharBuffer[i], CharBuffer[i]);
	// }

	//printf ("%s\n", CharBuffer);

	novel->NSymbols = StatBuff.st_size + 1;
	novel->OriginalText = CharBuffer;
	return 1;
}


void FileWrite (char* OutputFile, struct Text* novel) {
	
	
	//int foutp = open (OutputFile, O_WRONLY | O_APPEND);
	// for (int i = 0; i < novel->NLines; i++) {
	// 	for (int j = 0; j < novel->Lens[i]; j++) {
	// 		write (foutp, &novel->Pointers[i][j], 1);
	// 	}
	// 	write (foutp, novel->Pointers[i], novel->Lens[i]);
	// }
	//close (foutp);


	
	QuickSort (novel->Pointers, novel->NLines, *StringCompare);

	/*
	for (int i = 0; i < novel->NLines; i++) {
		printf ("%s\n", novel->Pointers[i]);
	}
	printf ("\n\n\n\n\n");
	*/

	//QuickSort (novel->Pointers, novel->NLines, *FromEndCompare);

	// for (int i = 0; i < novel->NLines; i++) {
	// 	printf ("%s\n", novel->Pointers[i]);
	// }

	int foutp = open (OutputFile, O_WRONLY);
	for (int i = 0; i < novel->NLines; i++) {
		//write (foutp, novel->Pointers[i]);
		
		write (foutp, novel->Pointers[i], strlen (novel->Pointers[i]));
		write (foutp, "\n", 1);
	}
	close (foutp);


	free (novel->Pointers);
	free (novel->Lens);
	return;
}



int StringCompare (char** ptr1, char** ptr2) {



	char* string2 = *ptr2;
	char* string1 = *ptr1;

	//printf ("запущен компаратор s1 = %s, s2 = %s\n", string1, string2);

	while ((*string1) && (*string2)) {
		while ((!isalpha (*string1)) && (!isdigit (*string1))) {
			if (!*string1) break;
			string1++;
		}
		while ((!isalpha (*string2)) && (!isdigit (*string2))) {
			if (!*string2) break; 
			string2++;
		}
		if (*string1 == *string2) {
			string1++; 
			string2++; 
			continue;
		}
		return (*string1 - *string2);
	}
	if (*string1) return 1;
	if (*string2) return -1;
	return 0;
}


void QuickSort (char** ArrPtr, size_t ArrSize, int (*Compare) (char** ptr1, char** ptr2)) {
	if (ArrSize == 1) return;
	size_t i = 0;
	size_t right = ArrSize - 1;
	size_t left = 0;
	//char** tmp;
	char** mid = ArrPtr + ArrSize / 2;
	//printf ("%ld  mid = %s\n\n", ArrSize / 2, *mid);
	//for (int i = 0; i < 20; i++) printf ("ArrPtr[%d] = %s\n", i, ArrPtr[i]);
	//printf ("mid = %s\n", *mid);
	//printf ("mid - left = %d\n", mid - ArrPtr);
	do {
		while (Compare (ArrPtr + left, mid) < 0) {left++;}// printf ("left = %ld\n", left);}
		while (Compare (ArrPtr + right, mid) > 0) {right--;}// printf ("%ld\n", right);}
		if (left < right) {
			//printf ("tmp\n");
			char* tmp = *(ArrPtr + left);
			//printf ("tmp = %s\n", tmp);
			*(ArrPtr + left) = *(ArrPtr + right);
			*(ArrPtr + right) = tmp;
			left++;
			right--;
		}
	} while (left < right);

	if (right > 0) {QuickSort (ArrPtr, right, (*Compare));}
	if (left < ArrSize - 1) {QuickSort (ArrPtr + left, ArrSize - left, (*Compare));}
}


int FromEndCompare (char** ptr1, char** ptr2) {
	char* string1 = *ptr1;
	char* string2 = *ptr2;

	//printf ("\n\n started comparison \ns1 = %s\ns2 = %s\n\n", string1, string2);

	// if (!*string1) return -1;
	// if (!*string2) return 1;


	while (*string1) string1++;
	while (*string2) string2++;

	while ((string1 != *ptr1) && (string2 != *ptr2)) {
		while ((!isalpha (*string1)) && (!isdigit (*string1))) {
			if (string1 == *ptr1) break;
			string1--;
		}
		while ((!isalpha (*string2)) && (!isdigit (*string2))) {
			if (string2 == *ptr2) break; 
			string2--;
		}
		if (*string1 == *string2) {
			string1--; 
			string2--; 
			continue;
		}
		return (*string1 - *string2);
	}
	if (string1 != *ptr1) return 1;
	if (string2 != *ptr2) return -1;
	return 0;


}


// int StringCompare (void* ptr1, void* ptr2) {

// 	char* string1 = *((char**) ptr1);
// 	char* string2 = *((char**) ptr2);

// 	while ((*string1) && (*string2)) {
// 		while ((!isalpha (*string1)) && (!isdigit (*string1))) {
// 			if (!*string1) break;
// 			string1++;
// 		}
// 		while ((!isalpha (*string2)) && (!isdigit (*string2))) {
// 			if (!*string2) break; 
// 			string2++;
// 		}
// 		if (*string1 == *string2) {
// 			string1++; 
// 			string2++; 
// 			continue;
// 		}
// 		return (*string1 - *string2);
// 	}
// 	if (*string1) return 1;
// 	if (*string2) return -1;
// 	return 0;
// }


// void QuickSort (void* ArrPtr, size_t ArrSize, size_t ElemSize, int (*Compare) (void* string1, void* string2)) {
// 	size_t i = 0;
// 	size_t right = ArrSize - 1;
// 	size_t left = 0;
// 	void* change;
// 	void* mid = ArrPtr + ArrSize * ElemSize / 2;
// 	do {
// 		while (Compare (ArrPtr + left * ElemSize, mid) > 0) left++;
// 		while (Compare (ArrPtr + right * ElemSize, mid) < 0) right--;
// 		if (left <= right) {
// 			*change = *(ArrPtr + left * ElemSize);
// 			*(ArrPtr + left * ElemSize) = *(ArrPtr + right * ElemSize);
// 			*(ArrPtr + right * ElemSize) = *change;
// 			left++;
// 			right--;
// 		}
// 	} while (left <= right);

// 	if (right > 0) QuickSort (ArrPtr, ArrSize / 2, ElemSize, (*Compare) (char* string1, char* string2));
// 	if (left < ArrSize - 1) QuickSort (ArrPtr + left * ElemSize, ArrSize - ArrSize / 2, ElemSize, (*Compare) (char* string1, char* string2));
// }
