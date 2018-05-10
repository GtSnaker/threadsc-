
#include "stdio.h"
#include "stdlib.h"
#include "GeneradorMatrices.h"
#include <string>
#include <ctime>


int main (int argc, char** argv) {
	srand (time(NULL));

	time_t tStart = clock();

	std::string nombreMatriz1 = "matrix/", nombreMatriz2 = "matrix/";
	std::string argument(argv[1]);
	std::string nThreadsString(argv[2]);
	int nThreads = std::stoi(nThreadsString);

	nombreMatriz1 += "matrizRandom" + argument + "x" + argument + ".txt";
	nombreMatriz2 += "matrizIdentidad" + argument + "x" + argument + ".txt";
	
	GestorMatrices gestor;
	
	MatrizVectores matriz = gestor.leerArchivoMatrizVectores(nombreMatriz1, false);
	time_t tMatrixReaded1 = clock();
	printf("Tiempo en leer la matriz 1: %f\n", double (tMatrixReaded1 - tStart) / CLOCKS_PER_SEC * 1000.0);

	MatrizVectores matrizIdentidad = gestor.leerArchivoMatrizVectores(nombreMatriz2, true);
	time_t tMatrixReaded2 = clock();
	printf("Tiempo en leer la matriz 2: %f\n", double (tMatrixReaded2 - tMatrixReaded1) / CLOCKS_PER_SEC * 1000.0);

	MatrizVectores resultado = gestor.multiplicarMatricesDivisionEstatica(matriz, matrizIdentidad, nThreads);
	// MatrizVectores resultado = gestor.multiplicarMatricesALoLoco(matriz, matrizIdentidad);
	//Matriz resultado = gestor.multiplicarMatrices(matriz, matrizIdentidad);
	

	time_t tMatrixMultiplied = clock();
	printf("Tiempo en multiplicar las matrices: %f\n", double (tMatrixMultiplied - tMatrixReaded2) / CLOCKS_PER_SEC * 1000.0);


	if(gestor.compararMatricesVectoresIguales(resultado, matriz)){
	 	printf("Son iguales\n");
	} else {
		printf("No son iguales\n");
	}

	return 0;
}