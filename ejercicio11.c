#include <stdio.h>

int main(int argc, char *argv[]){
	
	//Basicamente lo que tendriamos que hacer es agarrar los primeros 10 digitos, hacer toda la cuenta y despues ver si esta bien el codigo verificador
	long long num1;
	sscanf (argv[1],"%lld",&num1);
	printf("%lld\n", num1);
	return 0;
}
