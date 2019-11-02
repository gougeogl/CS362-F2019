#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

char inputChar()
{
	char output = ' ';
	unsigned ascii;
	unsigned select;

	/* FORMULA: rand() % ( MAX - MIN +1 ) + MIN */
	select = rand() % ( 10 - 1 + 1 ) + 1; /* [1..6] */	

	switch(select)
	{
		case 1:		ascii = rand() % ( 127 - 32 + 1) + 32; /* [32..127] ALL ascii */
				break;
	
		case 2:		ascii = 114; /* ascii 'r' */
				break;

		case 3:		ascii = rand() % ( 122 - 97 + 1) + 97; /* [97..122] lower-case */	
				break;

		case 4:		ascii = 101; /* ascii 'e' */
				break;	
	
		case 5:		ascii = rand() % ( 41 - 40 + 1 ) + 40; /* [40..41] ascii '(' and ')' */  
				break;

		case 6:		ascii = 115; /* ascii 's' */ 	
				break;

		case 7:		ascii = rand() % ( 127 - 114 + 1 ) + 114; /* ascii 'r' thru '}' */  
				break;

		case 8:		ascii = 116; /* ascii 't' */ 
				break;

		case 9:		ascii = rand() % ( 100 - 90 + 1 ) + 90; /* ascii 'Z' thru 'd' */  
				break;

		case 10:	ascii = 0; /* ascii NULL */
				break;
		

	}

	output = (char)ascii; 
	return output;
}

char inputChar2()
{
	char output = ' ';
	unsigned ascii;
	unsigned select;

	/* FORMULA: rand() % ( MAX - MIN +1 ) + MIN */
	select = rand() % ( 6 - 1 + 1 ) + 1; /* [1..6] */	

	switch(select)
	{
		case 1:	ascii = 114;
			break;

		case 2: ascii = 101;
			break;

		case 3: ascii = 115;
			break;

		case 4: ascii = 101;
			break;

		case 5: ascii = 116;
			break;

		case 6: ascii = 0;
			break; 

		case 7: ascii = 93;
			break;
	}

	output = (char)ascii; 
	return output;
}

char *inputString( )
{
	char* output;
	char str[6]; 

	memset(str, '\0', sizeof(str));

	str[0] = inputChar();
	str[1] = inputChar();
	str[2] = inputChar();
	str[3] = inputChar();
	str[4] = inputChar();
	str[5] = inputChar();

	output = str;
    	return output;
}

void testme()
{
  int tcCount = 0;
  char *s;
  char c;
  int state = 0;
  while (tcCount < 2000 )
  {
    tcCount++;
    c = inputChar2();
    s = inputString();
    printf("Iteration %d: c = %c, s = %s, state = %d\n", tcCount, c, s, state);

    if (c == '[' && state == 0) state = 1;
    if (c == '(' && state == 1) state = 2;
    if (c == '{' && state == 2) state = 3;
    if (c == ' '&& state == 3) state = 4;
    if (c == 'a' && state == 4) state = 5;
    if (c == 'x' && state == 5) state = 6;
    if (c == '}' && state == 6) state = 7;
    if (c == ')' && state == 7) state = 8;
    if (c == ']' && state == 8) state = 9;
    if (s[0] == 'r' && s[1] == 'e'
       && s[2] == 's' && s[3] == 'e'
       && s[4] == 't' && s[5] == '\0'
       && state == 9)
    {
      printf("error ");
      exit(200);
    }
  }
}


int main(int argc, char *argv[])
{
    srand(time(NULL));
    testme();
    return 0;
}
