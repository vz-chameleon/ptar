/*
 * ptar.c
 *
 *  Created on: Oct 23, 2016
 *      Authors: Timothy Garwood and Valentina Zelaya
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ptar.h"


int octalToDecimal(char * octalString){

  int oct = atoi(octalString);
  int dec=0;
  int i=0;

  while(oct!=0){
    dec+= (oct%10)* pow(8,i);
    ++i;
    oct=oct/10;
  }
  
  return dec;
}

  
