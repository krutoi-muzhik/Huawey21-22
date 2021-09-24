int StringCompare (char* string1, char* string2) {
	size_t size1 = strlen (string1);
	size_t size2 = strlen (string2);

	printf ("size1 = %zu, size2 = %zu\n\n", size1, size2);

	size_t i1 = 0;
	size_t i2 = 0;
	while ((i1 < size1) && (i2 < size2)) {
		printf ("i1 = %zu  i2 = %zu\n", i1, i2);
		while ((!isalpha (string1[i1])) && (!isdigit (string1[i1]))) {printf ("%c = string1 (%zu)\n", string1[i1], i1); i1++;}
		while ((!isalpha (string2[i2])) && (!isdigit (string2[i2]))) {printf ("%c = string2 (%zu)\n", string2[i2], i2); i2++;}
		if (string1[i1] == string2[i2]) {printf ("%c = string1 (%zu), %c = string2 (%zu)\n", string1[i1], i1, string2[i2], i2); i1++; i2++; continue;}
		return (string1[i1] - string2[i2]);
	}
	printf ("\ni1 = %zu, i2 = %zu\n", i1, i2);
	if (size1 - i1 > 0) {printf ("\n%zu\n", size1 - 1 - i1); return 1;}
	if (size2 - i2 > 0) {printf ("\n%zu\n", size2 - 1 - i2); return -1;}
	return 0;
}
