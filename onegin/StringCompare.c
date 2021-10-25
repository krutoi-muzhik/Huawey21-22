int StringCompare (char* string1, char* string2) {
	while ((*string1) && (*string2)) {
		while ((!isalpha (*string1)) && (!isdigit (*string1))) {
			if (!*string1)
				break;
			
			string1++;
		}
		while ((!isalpha (*string2)) && (!isdigit (*string2))) {if (!*string2) break; string2++;}
		if (*string1 == *string2) {string1++; string2++; continue;}
		return (*string1 - *string2);
	}
	if (*string1) return 1;
	if (*string2) return -1;
	return 0;
}
