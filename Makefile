CC = gcc 
OBJECTS = jsfmesgtype.c  utils.c
## CFLAGS=-g -m32 $(OPTFLAGS) -Wall -Wimplicit -Wimplicit-int -Wimplicit-function-declaration -W -Wstrict-prototypes -Wnested-externs  
CFLAGS=-g $(OPTFLAGS) -Wall -Wimplicit -Wimplicit-int -Wimplicit-function-declaration -W -Wstrict-prototypes -Wnested-externs  
LIBS = -lm


jsfmesgtype:$(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LIBS) -o jsfmesgtype


