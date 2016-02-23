void insSort(double arr[], int size) {
	
	int j, temp; 
	for (int i = 0; i < size; i++) {
		j = i;
		while ((j > 0) && (arr[j] < arr[j - 1])) {
			temp = arr[j];
			arr[j] = arr[j - 1];
			arr[j - 1] = temp;
			j--;
		}
	}
}