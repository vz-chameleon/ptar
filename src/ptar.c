/*
 * ptar.c
 *
 *  Created on: Oct 23, 2016
 *      Authors: Timothy Garwood and Valentina Zelaya
 */

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void initialisation(){

}


/*Autres...*/



long octal_to_decimal(char * octalString){

  long oct = atoi(octalString);
  long dec=0;
  int i=0;

  while(oct!=0){
    dec+= (oct%10)* pow(8,i);
    ++i;
    oct=oct/10;
  }
  
  return dec;
}

  
