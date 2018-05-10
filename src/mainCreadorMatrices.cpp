#include "GeneradorMatrices.h"
#include "stdlib.h"

int main(int argc, char** argv) {
	
	srand (time(NULL));
	time_t tStart = clock();
	
	GestorMatrices gestor;

	std::string argument(argv[1]);
	std::string::size_type sz;

	int size = std::stoi (argument, &sz);

	gestor.crearArchivoMatriz(size, size, "matrix/matrizIdentidad" + argument + "x" + argument + ".txt", true);
	time_t tMatrix1Created = clock();
	printf("Tiempo en crear la matriz identidad %dx%d: %f\n", size, size, double (tMatrix1Created - tStart) / CLOCKS_PER_SEC * 1000.0);

	gestor.crearArchivoMatriz(size, size, "matrix/matrizRandom" + argument + "x" + argument + ".txt", false);
	time_t tMatrix2Created = clock();
	printf("Tiempo en crear la matriz random %dx%d: %f\n", size, size, double (tMatrix2Created - tMatrix1Created) / CLOCKS_PER_SEC * 1000.0);

	return 0;
}