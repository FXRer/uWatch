#include "stdio.h"
#include <inttypes.h>
#include <math.h>

//using namespace std;


int res;

int main(int argc, char *argv[])
{
int i,w;


for(i = 0 ; i < 60; i++)
{

	double r = 30*cos((double)(2 * 3.141 * i)/60);

	 printf("0x%02X,",(int8_t)r& 0xFF);


	if(i % 8 == 7 )
		printf("\r\n");

}
	
}
