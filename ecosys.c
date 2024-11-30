#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

float p_ch_dir = 0.01;
float p_reproduce_proie = 0.4;
float p_reproduce_predateur = 0.5;
int temps_repousse_herbe = -15;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);
  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  Animal *new=creer_animal(x,y,energie);
  *liste_animal=ajouter_en_tete_animal(*liste_animal,new);

}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
   

    Animal *prev = NULL;
    Animal *curr = *liste;

    // Parcours de la liste pour trouver l'animal à supprimer
    while (curr != NULL) {
        if (curr == animal) { // On a trouvé l'animal
            if (prev == NULL) {
                // Si l'animal à supprimer est le premier de la liste
                *liste = curr->suivant;
            } else {
                // Sinon, on le retire en ajustant le pointeur du précédent
                prev->suivant = curr->suivant;
            }
            // Libération de la mémoire de l'animal
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->suivant;
    }
}


/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
    Animal *tmp;
    while (liste != NULL) {
        tmp = liste->suivant;
        free(liste);
        liste = tmp;
    }
    return NULL;
}


/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
    unsigned int i, j;
    char ecosys[SIZE_X][SIZE_Y];
    Animal *pa = NULL;

    // On initialise le tableau avec des espaces
    for (i = 0; i < SIZE_X; ++i) {
        for (j = 0; j < SIZE_Y; ++j) {
            ecosys[i][j] = ' ';
        }
    }

    // On ajoute les proies
    pa = liste_proie;
    while (pa) {
        if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) {
            ecosys[pa->x][pa->y] = '*';
        }
        pa = pa->suivant;
    }

    // On ajoute les prédateurs
    pa = liste_predateur;
    while (pa) {
        if (pa->x >= 0 && pa->x < SIZE_X && pa->y >= 0 && pa->y < SIZE_Y) {
            if (ecosys[pa->x][pa->y] == '*') { // Proies aussi présentes
                ecosys[pa->x][pa->y] = '@';
            } else {
                ecosys[pa->x][pa->y] = 'O';
            }
        }
        pa = pa->suivant;
    }

    // On affiche le tableau
    printf("+");
    for (j = 0; j < SIZE_Y; ++j) {
        printf("-");
    }
    printf("+\n");
    for (i = 0; i < SIZE_X; ++i) {
        printf("|");
        for (j = 0; j < SIZE_Y; ++j) {
            putchar(ecosys[i][j]);
        }
        printf("|\n");
    }
    printf("+");
    for (j = 0; j < SIZE_Y; ++j) {
        printf("-");
    }
    printf("+\n");
    int nbproie = compte_animal_it(liste_proie);
    int nbpred = compte_animal_it(liste_predateur);

    printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);
}



void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
    FILE *f = fopen(nom_fichier, "w");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour écriture");
        return;
    }

    // Écriture des proies
    fprintf(f, "<proies>\n");
    while (liste_proie != NULL) {
        fprintf(f, "x=%d y=%d dir=[%d %d] e=%f\n", 
                liste_proie->x, liste_proie->y, 
                liste_proie->dir[0], liste_proie->dir[1], 
                liste_proie->energie);
        liste_proie = liste_proie->suivant;
    }
    fprintf(f, "</proies>\n");

    // Écriture des prédateurs
    fprintf(f, "<predateurs>\n");
    while (liste_predateur != NULL) {
        fprintf(f, "x=%d y=%d dir=[%d %d] e=%f\n", 
                liste_predateur->x, liste_predateur->y, 
                liste_predateur->dir[0], liste_predateur->dir[1], 
                liste_predateur->energie);
        liste_predateur = liste_predateur->suivant;
    }
    fprintf(f, "</predateurs>\n");

    fclose(f);
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {
    FILE *f = fopen(nom_fichier, "r");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier pour lecture");
        return;
    }

    char ligne[100];
    int x, y, dir_x, dir_y;
    float energie;
    Animal *nouvel_animal;

    // Lecture des proies
    while (fgets(ligne, sizeof(ligne), f)) {
        if (strcmp(ligne, "<proies>\n") == 0) {
            while (fgets(ligne, sizeof(ligne), f) && strcmp(ligne, "</proies>\n") != 0) {
                sscanf(ligne, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir_x, &dir_y, &energie);
                nouvel_animal = creer_animal(x, y, energie);
                nouvel_animal->dir[0] = dir_x;
                nouvel_animal->dir[1] = dir_y;
                *liste_proie = ajouter_en_tete_animal(*liste_proie, nouvel_animal);
            }
        }
        // Lecture des prédateurs
        if (strcmp(ligne, "<predateurs>\n") == 0) {
            while (fgets(ligne, sizeof(ligne), f) && strcmp(ligne, "</predateurs>\n") != 0) {
                sscanf(ligne, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir_x, &dir_y, &energie);
                nouvel_animal = creer_animal(x, y, energie);
                nouvel_animal->dir[0] = dir_x;
                nouvel_animal->dir[1] = dir_y;
                *liste_predateur = ajouter_en_tete_animal(*liste_predateur, nouvel_animal);
            }
        }
    }

    fclose(f);
}


/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    while (la) {
        // Demander dans l'exo :
        if ((float)rand() / RAND_MAX < p_ch_dir) {
            la->dir[0] = rand() % 3 - 1; 
            la->dir[1] = rand() % 3 - 1;
        }

        la->x = (la->x + la->dir[0] + SIZE_X) % SIZE_X;
        la->y = (la->y + la->dir[1] + SIZE_Y) % SIZE_Y;

        la = la->suivant;
    }
}


/* Part 2. Exercice 4, question 3 */

/*Problème :

Ajouter de nouveaux animaux en tête de liste tout en la parcourant peut entraîner des problèmes, comme des boucles infinies, car les nouveaux animaux peuvent être parcourus dans la même itération.
Correction :

Collectez les nouveaux animaux dans une liste séparée et fusionnez-les après le parcours.*/
void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal *pa = *liste_animal;
    Animal *nouveaux_animaux = NULL;

    while (pa) {
        if ((float)rand() / RAND_MAX < p_reproduce) {
            // Crée un nouvel animal (progéniture)
            Animal *offspring = creer_animal(pa->x, pa->y, pa->energie / 2.0);
            offspring->dir[0] = rand() % 3 - 1;
            offspring->dir[1] = rand() % 3 - 1;

            // Réduit l'énergie du parent
            pa->energie /= 2.0;

            // Ajoute la progéniture à la liste des nouveaux animaux
            offspring->suivant = nouveaux_animaux;
            nouveaux_animaux = offspring;
        }
        pa = pa->suivant;
    }

    // Fusionne les nouveaux animaux avec la liste originale
    if (nouveaux_animaux) {
        Animal *temp = nouveaux_animaux;
        while (temp->suivant) {
            temp = temp->suivant;
        }
        temp->suivant = *liste_animal;
        *liste_animal = nouveaux_animaux;
    }
}


/* Part 2. Exercice 6, question 1 */

/*Problèmes :

Appeler reproduce à l'intérieur de la boucle sur les animaux peut conduire à modifier la liste pendant son parcours, ce qui n'est pas sûr.

Ne pas mettre à jour correctement le pointeur prev lors de la suppression d'un animal peut entraîner le saut d'éléments ou un crash.

Corrections :

Déplacez l'appel à reproduce en dehors de la boucle de parcours.

Mettez à jour correctement le pointeur prev.*/


void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    Animal *curr = *liste_proie;
    Animal *prev = NULL;

    while (curr != NULL) {
        bouger_animaux(curr);
        curr->energie -= 1;

        // Les proies mangent de l'herbe (à implémenter si nécessaire)

        if (curr->energie <= 0) {
            // Supprime l'animal
            if (prev == NULL) {
                *liste_proie = curr->suivant;
                free(curr);
                curr = *liste_proie;
            } else {
                prev->suivant = curr->suivant;
                free(curr);
                curr = prev->suivant;
            }
        } else {
            prev = curr;
            curr = curr->suivant;
        }
    }

    // Reproduction après avoir mis à jour toutes les proies
    reproduce(liste_proie, p_reproduce_proie);
}



/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    while(l){
      if (l->x == x && l-> y == y) return l;
      l = l->suivant;
    }

  return NULL;
}

/* Part 2. Exercice 7, question 2 */

/*Problèmes :

Comme pour rafraichir_proies, modifier la liste pendant son parcours peut causer des problèmes.

L'énergie du prédateur n'est pas correctement augmentée lorsqu'il consomme une proie.

La suppression des prédateurs avec une énergie nulle ou négative doit gérer correctement prev.

Corrections :

Ajustez la boucle pour gérer les modifications de la liste en toute sécurité.

Assurez-vous que l'énergie est correctement mise à jour lorsqu'un prédateur mange une proie.

Déplacez l'appel à reproduce en dehors de la boucle.*/
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
    Animal *curr = *liste_predateur;
    Animal *prev = NULL;

    while (curr != NULL) {
        bouger_animaux(curr);
        curr->energie -= 1;

        // Vérifie s'il y a une proie à la position du prédateur
        Animal *proie = animal_en_XY(*liste_proie, curr->x, curr->y);
        if (proie) {
            curr->energie += proie->energie;
            enlever_animal(liste_proie, proie);
        }

        if (curr->energie <= 0) {
            // Supprime le prédateur
            if (prev == NULL) {
                *liste_predateur = curr->suivant;
                free(curr);
                curr = *liste_predateur;
            } else {
                prev->suivant = curr->suivant;
                free(curr);
                curr = prev->suivant;
            }
        } else {
            prev = curr;
            curr = curr->suivant;
        }
    }

    // Reproduction après avoir mis à jour tous les prédateurs
    reproduce(liste_predateur, p_reproduce_predateur);
}




/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]) {
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
                monde[i][j] += 1;
              
        }
    }
}


