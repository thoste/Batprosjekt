double middle(double arr[], int size) {

	double* temp = new double[size];

	for (int i = 0; i < size; i++) {
		temp[i] = arr[i];
	}

	insSort(temp, size);
	return temp[size / 2];

	delete[] temp;
}