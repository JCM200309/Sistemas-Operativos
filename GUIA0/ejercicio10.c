#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char* toUpperCase(char* texto){
	for (int i = 0; texto[i];i++){ //Poniendo texto[i] itera hasta llegar al /0 que es -1 por lo tanto da false 
		texto[i] = toupper(texto[i]);
	}
	return texto;
}

char* reverse(char* texto){
	int j = strlen(texto) - 1;
	int i = 0;
	char *reverso = malloc((strlen(texto)+1) * sizeof(char));
	while(j >= 0){
		reverso[i] = texto[j];
		i++;
		j--;
	}
	reverso[i]= '\0';
	return reverso;
}

//argc es la cantidad de argumentos
//argv son los argumentos en si
int main(int argc, char* argv[]){
	int n = atoi(argv[1]);
	char* texto = argv[2];
	
	char *palabra = strtok(texto, " ");
	char res[200] = ""; 
	while (palabra != NULL){
		if(strlen(palabra) < n){
			char* reversa = reverse(palabra);
			char* mayusculas = toUpperCase(reversa);
			strcat(res, " ");
			strcat(res, mayusculas);
			free(reversa);
		}
		else{
			strcat(res, " ");
			strcat(res,palabra);
		}
		palabra = strtok(NULL, " ");
	}
	printf("%s\n", res);
	return 0;

}
