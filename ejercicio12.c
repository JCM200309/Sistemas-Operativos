#include <stdio.h>

struct Persona {
	char nombre[100];
	int edad;
	long long cuit;
};

char* agregarInformacionUsuario(){
	struct Persona p1;
	printf("Ingrese nombre de persona: \n");
	scanf("%s", &p1.nombre);
	printf("Ingrese edad: \n");
	scanf("%d", &p1.edad);
	printf("Ingrese cuit: \n");
	scanf("%lld", &p1.cuit);
	if (p1.edad >= 18 && p1.edad <= 99){
		return ("Su informacion fue cargada con exito!");
	}else{
		printf("La informacion cargada es invalida por favor, intente de nuevo");
		return (agregarInformacionUsuario());
	}
}



int main(){
	char comando[100];
	scanf("%99s",comando);
	if (strcmp(comando,"agregarInformacionUsuario") == 0){
		char* rta = agregarInformacionUsuario();
		printf("%s", rta);
	}
	return 0;
}
