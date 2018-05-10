
#include "GeneradorMatrices.h"
#include <stdlib.h>
// #include <thread>
#include <pthread.h>

GestorMatrices::GestorMatrices(){

}


int GestorMatrices::crearArchivoMatriz(int filas, int columnas, std::string nameFile, bool identidad){
	FILE* file;
	file = fopen(nameFile.c_str(), "w+");
	
	fseek(file, 0, SEEK_SET);
	fwrite(&filas, sizeof(int), 1, file);
	fwrite(&columnas, sizeof(int), 1, file);

	float randomFloat;
	for (int i = 0; i < filas; i++) {
		for(int j = 0; j < columnas; j++){
			if(identidad) {
				randomFloat = (i == j ? 1 : 0);
			} else {
				randomFloat = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);	
			}
			fwrite(&randomFloat, sizeof(float), 1, file);
		}
	}

	fflush(file);
	fclose(file);

	return 0;
}

int GestorMatrices::guardarMatrizArchivo(Matriz matriz, std::string nameFile) {
	FILE* file;
	file = fopen(nameFile.c_str(), "w+");
	
	fseek(file, 0, SEEK_SET);
	fwrite(&matriz.filas, sizeof(int), 1, file);
	fwrite(&matriz.columnas, sizeof(int), 1, file);

	int size = matriz.filas * matriz.columnas;

	for (int i = 0; i < size; i++) {
		fwrite(&matriz.matriz[i], sizeof(float), 1, file);
	}

	fflush(file);
	fclose(file);
	return 0;
}

Matriz GestorMatrices::leerArchivoMatriz(std::string nameFile){
	Matriz matriz;
	int filas, columnas;

	FILE* file;
	file = fopen(nameFile.c_str(), "r+");
	
	fseek(file, 0, SEEK_SET);
	fread(&filas, sizeof(int), 1, file);
	fread(&columnas, sizeof(int), 1, file);

	float* matrix = (float*)malloc(sizeof(float) * filas * columnas);

	float nextFloat;
	for(int i = 0; i < filas; i++) {
		for(int j = 0; j < columnas; j++){
			fread(&nextFloat, sizeof(float), 1, file);
			matrix[ (i*filas + j) ] = nextFloat;
		}
	}

	fflush(file);
	fclose(file);

	matriz.filas = filas;
	matriz.columnas = columnas;
	matriz.matriz = matrix;
	return matriz;
}

MatrizVectores GestorMatrices::leerArchivoMatrizVectores(std::string nameFile, bool traspuesta = false){
	MatrizVectores matriz;
	int filas, columnas;

	FILE* file;
	file = fopen(nameFile.c_str(), "r+");
	
	fseek(file, 0, SEEK_SET);
	fread(&filas, sizeof(int), 1, file);
	fread(&columnas, sizeof(int), 1, file);

	float** matrix = (float**)malloc(sizeof(float*) * filas);
	
	for(int i = 0; i < filas; i++) {
		matrix[i] = (float*)malloc(sizeof(float) * columnas);
	}

	float nextFloat;

	if(traspuesta) {
		for(int i = 0; i < filas; i++) {
			for(int j = 0; j < columnas; j++){
				fread(&nextFloat, sizeof(float), 1, file);
				matrix[j][i] = nextFloat;
			}
		}
	} else {
		for(int i = 0; i < filas; i++) {
			for(int j = 0; j < columnas; j++){
				fread(&nextFloat, sizeof(float), 1, file);
				matrix[i][j] = nextFloat;
			}
		}
	}

	fflush(file);
	fclose(file);

	matriz.filas = filas;
	matriz.columnas = columnas;
	matriz.matriz = matrix;
	return matriz;
}

Matriz GestorMatrices::multiplicarMatrices(Matriz a, Matriz b) {
	Matriz resultado;
	resultado.filas = a.filas;
	resultado.columnas = b.columnas;
	float* matrix = (float*)malloc(sizeof(float) * resultado.filas * resultado.columnas);
	float temporal;

	for(int i = 0; i < resultado.filas; i++) {
		for(int j = 0; j < resultado.columnas; j++) {
			temporal = 0 ;
	        for (int k = 0 ; k < a.filas ; k++ ) {                                 
	            temporal += a.matriz[i * a.filas + k] * b.matriz[k * b.filas + j];
	        }
			matrix[ i * resultado.filas + j ] = temporal;
		}
		printf("\r");
		fflush(stdout);
		printf("%.1f%c", (float)i / (float)a.filas * 100, '%');
	}
	printf("\n");
	resultado.matriz = matrix;
	return resultado;
}

struct argumentoMultiplicaVector
{
	float* comienzoA;
	float* comienzoB;
	int tamanoA;
	float* resultado;
};


void multiplicarVectorSinThread(float* a, float* b, int size, float *res) {
	float resultado = 0;
	for (int k = 0 ; k < size; k++ ) {                                 
		resultado += a[k] * b[k];
	}
	*res = resultado;
}



void* multiplicarVector(void* argumento) {
	argumentoMultiplicaVector* arg = (argumentoMultiplicaVector*)argumento;
	float res = 0;
	for (int k = 0 ; k < arg->tamanoA ; k++ ) {                                 
		res += arg->comienzoA[k] * arg->comienzoB[k];
	}
	*(arg->resultado) = res;
	pthread_exit(NULL);
	return NULL;
}


MatrizVectores GestorMatrices::multiplicarMatricesALoLoco(MatrizVectores a, MatrizVectores b) {
	MatrizVectores resultado;
	resultado.filas = a.filas;
	resultado.columnas = b.columnas;
	
	float** matrix = (float**)malloc(sizeof(float*) * resultado.filas);
	for(int i = 0; i < a.filas; i++) {
		matrix[i] = (float*)malloc(sizeof(float) * b.columnas);
	}
	

	int nThreads = 1000;
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * nThreads);
	int threadsUsados = 0;
	
	argumentoMultiplicaVector* arg = (argumentoMultiplicaVector*)malloc(sizeof(argumentoMultiplicaVector) * nThreads);;

	for(int i = 0; i < resultado.filas; i++) {
		for(int j = 0; j < resultado.columnas; j++) {
			// std::thread t = std::thread(multiplicarVector, a.matriz[i], b.matriz[j], a.filas, &matrix[i][j]);
			// t.join();

			arg[threadsUsados].comienzoA = a.matriz[i];
			arg[threadsUsados].comienzoB = b.matriz[j];
			arg[threadsUsados].tamanoA = a.filas;
			arg[threadsUsados].resultado = &matrix[i][j];

			pthread_create(&threads[threadsUsados], NULL, multiplicarVector, &(arg[threadsUsados]));
			threadsUsados++;
			if(threadsUsados == nThreads){
				for(int k = 0; k < threadsUsados; k++){
					pthread_join(threads[k], NULL);		
				}
				threadsUsados = 0;
			}
		}
		// printf("\r");
		// fflush(stdout);
		// printf("%.1f%c", (float)i / (float)a.filas * 100, '%');
	}
	printf("\n");
	for(int k = 0; k < threadsUsados; k++){
		pthread_join(threads[k], NULL);		
	}

	resultado.matriz = matrix;
	return resultado;
}

struct argumentoMultiplicaMatriz
{
	MatrizVectores matrizVectoresA;
	MatrizVectores matrizVectoresB;
	int filaInicioA;
	int filaFinA;
	MatrizVectores resultado;
	int threadId;
};

//suponemos que b esta traspuesta
void* multiplicaMatriz(void* argumentoMultiplicaMatriz_) {
	argumentoMultiplicaMatriz* argumento = (argumentoMultiplicaMatriz*)argumentoMultiplicaMatriz_;
	printf("thread: %d, filaInicio: %d, filaFin %d, filas %d\n", argumento->threadId, argumento->filaInicioA, argumento->filaFinA, argumento->matrizVectoresB.filas);
	for(int i = argumento->filaInicioA; i < argumento->filaFinA; i++) {
		for(int j = 0; j < argumento->matrizVectoresB.filas; j++) {
			// multiplicarVectorSinThread(argumento->matrizVectoresA.matriz[i], argumento->matrizVectoresB.matriz[j], argumento->matrizVectoresA.filas, &argumento->resultado.matriz[i][j]);
			float resultado = 0;
			for (int k = 0 ; k < argumento->matrizVectoresA.filas; k++ ) {                                 
				resultado += argumento->matrizVectoresA.matriz[i][k] * argumento->matrizVectoresB.matriz[j][k];
			}
			argumento->resultado.matriz[i][j] = resultado;
		}
	}
	pthread_exit(NULL);
	return NULL;
}

MatrizVectores GestorMatrices::multiplicarMatricesDivisionEstatica(MatrizVectores a, MatrizVectores b, int nProcesos) {

	MatrizVectores resultado;
	resultado.filas = a.filas;
	resultado.columnas = b.columnas;
	resultado.matriz = (float**)malloc(sizeof(float*) * resultado.filas);
	for(int i = 0; i < a.filas; i++) {
		resultado.matriz[i] = (float*)malloc(sizeof(float) * b.columnas);
	}

	// std::thread threads[nProcesos];
	pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t)* nProcesos);
	argumentoMultiplicaMatriz* arg = (argumentoMultiplicaMatriz*)malloc(sizeof(argumentoMultiplicaMatriz)* nProcesos);

	int sizePart = a.filas / nProcesos;
	for(int i = 0; i < nProcesos; i++) {
		int init = i * sizePart;
		int end = i * sizePart + sizePart;
		if(i == nProcesos-1) end = a.filas;
		
		arg[i].matrizVectoresA = a;
		arg[i].matrizVectoresB = b;
		arg[i].filaInicioA = init;
		arg[i].filaFinA = end;
		arg[i].resultado = resultado;
		arg[i].threadId = i;

		pthread_create(&threads[i], NULL, multiplicaMatriz, &(arg[i]));
	}
	
	for(int i = 0; i < nProcesos; i++) {
		pthread_join(threads[i], NULL);
		printf("lala\n");
	}

	return resultado;
}


bool GestorMatrices::compararMatricesIguales(Matriz a, Matriz b) {
	int size = a.filas * a.columnas;
	for(int i = 0; i < size; i++) {
		if(a.matriz[i] != b.matriz[i]) {
			return false;
		}
	}
	return true;
}

bool GestorMatrices::compararMatricesVectoresIguales(MatrizVectores a, MatrizVectores b) {
	for(int i = 0; i < a.filas; i++) {
		for(int j = 0; j < a.columnas; j++) {
			if(a.matriz[i][j] != b.matriz[i][j]) {
				return false;
			}
		}
	}
	return true;
}



void GestorMatrices::pintarMatriz(Matriz matriz) {
		for(int i = 0; i < matriz.filas; i++) {
		for(int j = 0; j < matriz.columnas; j++){
			printf("%f\t", matriz.matriz[i * matriz.filas + j]);
		}
		printf("\n");
	}
}

void GestorMatrices::pintarMatrizVectores(MatrizVectores matriz) {
		for(int i = 0; i < matriz.filas; i++) {
		for(int j = 0; j < matriz.columnas; j++){
			printf("%f\t", matriz.matriz[i][j]);
		}
		printf("\n");
	}
}

bool GestorMatrices::fileExists(std::string name){
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }  
}
