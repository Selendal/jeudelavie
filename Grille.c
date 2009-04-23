#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>

#include "Grille.h"

#define TAILLE_BLOC 8
#define EXIT_INTERRUPTION 2

void pause(){
	int continuer=1;
	SDL_Event event;

	while(continuer){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT:
				continuer=0; break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_c: continuer=0; break;
					case SDLK_d: continuer=0; break;
					default: break;
				}
			break;
		}
	}
}

void saisirGraphique(grille *t, SDL_Surface *ecran, int m){
	SDL_Surface *vide=NULL, *bleu=NULL, *rouge=NULL; /*les images des cellules et du grille*/
	SDL_Rect position;
	int realx,realy; /*pour determiner le x,y du tableau par rapport au clique de la sourris*/
	int i,j;
	int continuer=1;
	char typePlace; /*type de cellule qui va etre mise ou autre option(a regarder plus loin)*/
	SDL_Event event;

	/*Initialisation de la fenetre*/
	ecran=SDL_SetVideoMode(m*TAILLE_BLOC,m*TAILLE_BLOC,32,SDL_HWSURFACE);
	if (ecran== NULL){
		fprintf(stderr,"Erreur de la chagrement de mode: %s",SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("Remplissage du tableau",NULL);
	SDL_FillRect(ecran,NULL,SDL_MapRGB(ecran->format,0,0,0));

	/*chargement des images*/
	rouge=SDL_LoadBMP("rouge.bmp");
	bleu=SDL_LoadBMP("bleu.bmp");
	vide=SDL_LoadBMP("vide.bmp");

	/*dessin de la grille(grace aux plusieurs "vide.bmp")*/
	for(i=0;i<m;i++){
		for(j=0;j<m;j++){
			position.x=j*TAILLE_BLOC;
			position.y=i*TAILLE_BLOC;
			SDL_BlitSurface(vide,NULL,ecran,&position);
		}
	}
	/*mise a jour de la fenetre*/
	SDL_Flip(ecran);
	printf("Rq: RESPECTEZ LES MINUSCULES !\n");
	printf("Veuillez taper: r: pour placer les cellules rouges\n                b: pour placer les cellules bleus\n");
	printf("                s: pour supprimer la cellule\n                e: pour finir le saisi\n");
	printf("Et puis, pour placer les cellules, veuillez cliquer sur la grille qui est apparu dans une fenetre..\n");
	while(continuer==1){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT : printf("Le saisi était intrrompu par l'utilisateur\n"); exit(EXIT_INTERRUPTION); break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_e: continuer=0; break;
					case SDLK_s: typePlace = 'V'; break;
					case SDLK_r: typePlace = 'R'; break;
					case SDLK_b: typePlace = 'B'; break;
					default: break;
				}
			break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button){
					case SDL_BUTTON_LEFT:
						if(SDL_MOUSEMOTION){
							/*calcul des coordonnées de la cellule dans le tableau*/
							realx=event.motion.x/8;
							realy=event.motion.y/8;
							/*calcul des coordonnées de la cellule dans le fenetre*/
							position.x=realx*8;
							position.y=realy*8;
							if(typePlace == 'R'){
								(*t)[realy][realx].type=1;
								SDL_BlitSurface(rouge,NULL,ecran,&position);
							}
							if(typePlace == 'B'){
								(*t)[realy][realx].type=-1;
								SDL_BlitSurface(bleu,NULL,ecran,&position);
							}
							if(typePlace == 'V'){
								(*t)[realy][realx].type=0;
								SDL_BlitSurface(vide,NULL,ecran,&position);
							}
						}
					break;
				}
			break;
		}
		SDL_Flip(ecran);
	}
}

void affichageGraphique(grille t, SDL_Surface *ecran2, int m,char *rep){
	SDL_Surface *bleu = NULL, *rouge = NULL, *vide = NULL;
	SDL_Rect position;
	SDL_Event event;
	int continuer=1;
	int i,j;

	/*initialisation de la fenetre*/
	ecran2=SDL_SetVideoMode(m*TAILLE_BLOC,m*TAILLE_BLOC,32,SDL_HWSURFACE);
	if (ecran2== NULL){
		fprintf(stderr,"Erreur de la chagrement de mode: %s",SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption("Resultat du simulation",NULL);
	SDL_FillRect(ecran2,NULL,SDL_MapRGB(ecran2->format,0,0,0));

	/*chargement des images*/
	rouge=SDL_LoadBMP("rouge.bmp");
	bleu=SDL_LoadBMP("bleu.bmp");
	vide=SDL_LoadBMP("vide.bmp");

	position.x=0;
	position.y=0;

	/*chargement de l'image de chaque BLOC en fonction des données du tableau*/
	for(i=0;i<m;i++){
		for (j=0;j<m;j++){
			position.x=j*TAILLE_BLOC;
			position.y=i*TAILLE_BLOC;
			if(t[i][j].type == 1){
				SDL_BlitSurface(rouge,NULL,ecran2,&position);
			}
			if(t[i][j].type == -1){
				SDL_BlitSurface(bleu,NULL,ecran2,&position);
			}
			if(t[i][j].type == 0){
				SDL_BlitSurface(vide,NULL,ecran2,&position);
			}
		}
	}
	SDL_Flip(ecran2);

	while(continuer == 1){
		SDL_WaitEvent(&event);
		switch(event.type){
			case SDL_QUIT :
				printf("L'affichage etait interrompu par l'utilisateur\n");
				SDL_FreeSurface(rouge);
				SDL_FreeSurface(bleu);
				exit(EXIT_INTERRUPTION);
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym){
					case SDLK_c: continuer=0; break;
					case SDLK_e: continuer=0; *rep='e'; break;
					default: break;
				}
			break;
		}
	}

	/*liberation de la memoire*/
	SDL_FreeSurface(rouge);
	SDL_FreeSurface(bleu);
	SDL_FreeSurface(vide);
}

void allocationGrille(grille *t, int taille){
	int i;
	if (((*t) = (grille) calloc (taille, sizeof(Case*))) == NULL) {
		fprintf (stderr, "Erreur a l'allocation d'un tableau de %d entiers.\n", taille) ;
		exit (EXIT_FAILURE) ;
	}

	for (i = 0 ; i < taille ; ++i){
		if (((*t)[i] = (Case*) calloc (taille, sizeof (Case))) == NULL) {
		free(*t);
		fprintf (stderr, "Erreur a l'allocation d'un tableau de %d entiers.\n", taille) ;
		exit (EXIT_FAILURE) ;
		}
	}
}

void initialisationGrille(grille *t,int taille){
	int i,j;
	for (i=0; i<taille ; i++){
		for(j=0; j<taille; j++){
			(*t)[i][j].type=0;
			(*t)[i][j].voisin=0;
			(*t)[i][j].voisinB=0;
			(*t)[i][j].voisinR=0;
		}
	}
	printf("Ok\n");
}

void saisirGrille(grille *t, int taille){
	int x,y;
	char reponse;

  do{
	printf("Coordonnées de quelle type de case voulez vous saisir ? (R/B) ou bien tapez N pour sortir: ");
		while(getchar() != '\n');
	scanf("%c",&reponse);
	if((reponse == 'R') || (reponse == 'B')){
		printf("Les coordonnées doivent être entre 0 et %d\n",taille-1);
		printf("x: ");
		scanf("%d",&x);
		printf("y: ");
		scanf("%d",&y);
		saisirCase(&(*t)[x][y],reponse);
	}
  }while(reponse != 'N');
}

void affichage(grille t,int taille){
	int i,j;
	printf("Affichage(du tableau du type de chaque case):\n");
	for (i=0; i<taille; ++i){
		for(j=0; j<taille; ++j){
			afficherTypeCase(t[i][j]);
		}
		printf("\n");
	}
}

void affichageVoisin(grille t,int taille){
	int i,j;
	printf("Affichage(du tableau avec t[i][j].voisin):\n");
	for (i=0; i<taille; ++i){
		for(j=0; j<taille; ++j){
			afficherVoisinCase(t[i][j]);
		}
		printf("\n");
	}
}

/*Une fonction qui parcours les cases autour de la case donnee(x,y) et qui compte des cellules-voisins*/
void voisinAutour (grille *tab, int x, int y,int taille) {
	int k , l ;
	/*printf("x: %d y: %d\n",x,y);*/
	taille--;
    /*Operateur du Terner pour ne pas depasser les bords du tableau...*/
    /*cad si x=0 y=0 pour qu'il ne touche pas les cases (-1,-1) ou (0,-1) .. pour qu'il ne depasse pas le bord :)*/
    /*k=(x > 0 ? x-1 : x) : on peut le lire comme:*/
    /*si x>0 alors k=x-1 sinon k=x*/
    /*ca simplifie vachement l'ecriture ^^*/
	for (k = (x > 0 ? x - 1 : x) ; k <= (x < taille ? x + 1 : x) ; k++){
		for (l = (y > 0 ? y - 1 : y) ; l <= (y < taille ? y + 1 : y) ; l++){
		/*printf("k: %d l: %d\n",k,l);*/
	    /*si la cellule n'est pas morte ET pour que ca ne sois pas elle-meme(car elle n'est pas voisine pour elle-meme..) !!*/
			if (((*tab)[k][l].type != 0) && ((k != x) || (l != y))){
	    /*un autre if pour calculer les voisines Rouges*/
				if((*tab)[k][l].type == 1){
					(*tab)[x][y].voisinR++;
					/*printf("GogogoRouge\n");*/
				}
				(*tab)[x][y].voisin++ ;
			/*printf("NB++ ete effectue...\n");*/
			}
		}
	}
    /*calcul des voisines Bleu*/
	(*tab)[x][y].voisinB=(*tab)[x][y].voisin-(*tab)[x][y].voisinR;
}

void voisinAutourMirror (grille *tab, int x, int y,int taille) {
	int k , l ;
	/*printf("x: %d y: %d\n",x,y);*/
	taille--;
    (*tab)[x][y].voisinR=0;
	(*tab)[x][y].voisin=0;
	k=((x-1)<0 ? taille : x-1);
    /*while(getchar() != '\n');*/
	do{
		l=((y-1)<0 ? taille : y-1);
		do{
			if (((*tab)[k][l].type != 0) && ((k != x) || (l != y))){
	    /*un autre if pour calculer les voisines Rouges*/
				if((*tab)[k][l].type == 1){
					(*tab)[x][y].voisinR++;
					/*printf("GogogoRouge\n");*/
				}
				(*tab)[x][y].voisin++ ;
				/*printf("NB++ ete effectue...\n");*/
			}
			/*printf("x: %d y: %d\n",x,y);
			printf("k: %d l: %d\n",k,l);*/
			l++;
		if ((l == taille+1) && (y != taille-1)) l=0;
		}while((l<=(y+1>taille ? 0 : y+1)) || ((l == taille) && (y==taille)));
		k++;
		if ((k == taille+1) && (x != taille-1)) k=0;
	}while((k<=(x+1>taille ? 0 : x+1)) || ((k == taille) && (x == taille)));
    /*calcul des voisines Bleu*/
    /*getchar();*/
	(*tab)[x][y].voisinB=(*tab)[x][y].voisin-(*tab)[x][y].voisinR;
}

void calculVoisin(grille *t,int taille){
	int i,j;

	for (i=0; i<taille; ++i){
		for(j=0; j<taille; ++j){
			/*printf("i> %d j> %d\n",i,j);*/
			voisinAutourMirror(t,i,j,taille);
		}
	}
	/*printf("Fait\n");*/
}

void simulation(grille *t,int taille, int etapes,SDL_Surface *ecran, int affiche){
	int i,j,numEtape=0;
	char reponse;
    if(affiche == 1){
        if (numEtape == 0){
            while(getchar() != '\n');
            printf("\n------------------------------------------------\n");
            printf("Veuillez-vous suivre chaque etape du simulation ou bien voir juste l'etat final de la grille ?\n");
            printf("Repondez 'C' : chaque etape\n         'F' : QUE l'état finale(sans regarder les etapes intermediaires..)\n");
            printf("Reponse: ");
            scanf("%c",&reponse);
            printf("\n------------------------------------------------\n");
            printf("Merci pour votre reponse =) maintenant veuillez REouvrir la fenetre en bas..\n");
            printf("C'est l'etape initial du plateau\n");
            printf("Veuillez taper 'c' pour commencer la simulation..\n");
            affichageGraphique(*t,ecran,taille,&reponse);
        }
    }
	while (getchar() != '\n');
	while(etapes != 0){
		calculVoisin(t,taille);
		/*affichageVoisin(*t,taille);
		getchar();*/
		for(i=0;i<taille;i++){
			for(j=0;j<taille;j++){
				/*si la cellule n'est pas morte on regard des conditions quand est-ce qu'elle va desceder..*/
				if((*t)[i][j].type != 0){
					if (((*t)[i][j].voisin != 2) && ((*t)[i][j].voisin != 3)){
						(*t)[i][j].type = 0;
					}
				} else {
					if ((*t)[i][j].voisin == 3){
						if ((*t)[i][j].voisinR > (*t)[i][j].voisinB){
							(*t)[i][j].type=1;
						}else{
							(*t)[i][j].type=-1;
						}
					}
				}
			}
		}
		numEtape++;
		if(affiche == 1){
		    if(reponse == 'C'){
                printf("\n-------------Etape : %d/%d------------\n",numEtape,etapes+numEtape-1);
                printf("Veuillez taper 'c' pour continuer..\n");
                printf("..ou bien tapez 'e' pour passer a l'etat finale de la simulation\n");
                affichageGraphique((*t),ecran,taille,&reponse);
            }
		}
		/*affichage(*t,taille);
		printf("Taper n'importe quel button pour continuer..\n");*/
		/*getchar();*/
		etapes--;
	}
}

void liberationMemoire(grille *t,int taille){
	int i;
	for (i=0; i<taille; i++){
	free((*t)[i]);
	}
	free(*t);
	printf("Ok\n");
}

