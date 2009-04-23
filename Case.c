#include <stdio.h>
#include <stdlib.h>
#include "Case.h"

void saisirCase(Case *cel, char types){
	Case* aux;
/*  printf("Just pour voir l'adress de pointeur -> %p\n",(void*) cel);
  aux = (Case*) malloc (sizeof(Case));
  /*scanf("%d",&(aux->type));*/
	if (types == 'R')
		cel->type=1;
	if (types == 'B')
		cel->type=-1;
/*  printf("aux = %d\n",aux->type);
  cel->type=aux->type;
  free(aux);
  printf("Allocation ok\n");*/
   
}

void afficherTypeCase(Case cel){
	if(cel.type<0)
		printf("%d",cel.type);
	else
		printf("%d",cel.type);
}

void afficherVoisinCase(Case cel){
	if(cel.type<0)
		printf("%d ",cel.voisin);
	else
		printf(" %d ",cel.voisin);
}
