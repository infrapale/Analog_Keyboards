#include "akbd.h"


akbd::akbd(int pin_nbr)
{
  _pin_nbr = pin_nbr;
  pinMode(_pin_nbr, INPUT);  
  nbr_keys = NBR_KEYS;
}

void akbd::begin(void)
{
  kbdState=0;
}

void akbd::set_nbr_keys(uint8_t n){
	nbr_keys = n;
}




void akbd::set_aval(byte key_indx, uint16_t key_value){
    key_val[key_indx] = key_value;
}    

//int kbdPin = 15; 
int akbd::rd_analog(void){
  return( analogRead(_pin_nbr));
}

void akbd::scan(void) {
  int kbdVal = 0; 
  int d;
  int i=0;
  char k=0;
  kbdVal = analogRead(_pin_nbr);
  if (kbdVal > KEY_TOL ){
    while (i< nbr_keys){
      d = kbdVal-key_val[i];
      d = abs(d);
      if (d <= KEY_TOL) {
        k = i+'0';
        i =99;
      }
      else i++;
    }
    switch(k){    // fix asterisk and hash ASCII codes
      case '0'+10: k='*'; break;
      case '0'+11: k='#'; break;
      case '0'+12: k='@'; break;
      case '0'+13: k='='; break;
      case '0'+14: k='>'; break;
      case '0'+15: k='<'; break;
    }
    
    switch( kbdState ){
      case 0:  // no key pressed
        if (k) {
           lastKey = k;
           kbdDebCntr=KBD_DEB;
           kbdState= 1;
        }
        break;
      case 1:  // key pressed but debounce still active
         if (k == lastKey) {
           if (kbdDebCntr) kbdDebCntr--;
           else kbdState= 2;
         } else kbdState = 0;
         break;
      case 2:  // key pressed and accepted, not read
          if (k != lastKey) kbdState = 0;
          break;
      case 3:  // key read
           if (k != lastKey) kbdState = 0;
           break;
      }          
      if (kbdState == 0){
         lastKey = k;
         kbdDebCntr=KBD_DEB;      
      }
   } else kbdState = 0;   // no key pressed
}

char akbd::read(void){
  if (kbdState == 2){ 
     kbdState = 3;
     return(lastKey);
  } else return(0);
}
