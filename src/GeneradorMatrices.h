
#include <string>

struct Matriz {
	float* matriz;
	int filas, columnas;
};

struct MatrizVectores {
	float** matriz;
	int filas, columnas;
};

class GestorMatrices {
public:
	GestorMatrices();

	int crearArchivoMatriz(int filas, int columnas, std::string nameFile, bool identidad);
	int guardarMatrizArchivo(Matriz matriz, std::string nameFile);
	
	Matriz leerArchivoMatriz(std::string nameFile);
	MatrizVectores leerArchivoMatrizVectores(std::string nameFile, bool traspuesta);
	
	bool fileExists(std::string name);

	Matriz multiplicarMatrices(Matriz a, Matriz b);
	MatrizVectores multiplicarMatricesALoLoco(MatrizVectores a, MatrizVectores b);
	MatrizVectores multiplicarMatricesDivisionEstatica(MatrizVectores a, MatrizVectores b, int nProcesos);

	
	bool compararMatricesIguales(Matriz a, Matriz b);
	bool compararMatricesVectoresIguales(MatrizVectores a, MatrizVectores b);
	void pintarMatriz(Matriz matriz);
	void pintarMatrizVectores(MatrizVectores matriz);
};


