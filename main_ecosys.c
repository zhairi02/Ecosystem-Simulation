#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>  // pour usleep
#include "ecosys.h"

#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000
#define MAX_ITER 200


int main(void) {
    srand(time(NULL));

    // Test de la fonction de déplacement avec toricité
    Animal *test_animal = creer_animal(0, 0, 10.0);
    test_animal->dir[0] = 1;
    test_animal->dir[1] = 1;
    printf("Position initiale: x = %d, y = %d\n", test_animal->x, test_animal->y);
    for (int i = 0; i < 5; i++) {
        bouger_animaux(test_animal);
        printf("Après déplacement %d : x = %d, y = %d\n", i + 1, test_animal->x, test_animal->y);
    }
    free(test_animal);

    // Test de la fonction de reproduction avec taux de 1
    Animal *liste_proie = NULL;
    for (int i = 0; i < 5; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 10.0, &liste_proie);
    }
    printf("\nÉtat initial des proies :\n");
    afficher_ecosys(liste_proie, NULL);
    reproduce(&liste_proie, p_reproduce_proie);
    printf("\nÉtat des proies après reproduction :\n");
    afficher_ecosys(liste_proie, NULL);
    liberer_liste_animaux(liste_proie);

    // Simulation de l'écosystème avec 20 proies et prédateurs
    int monde[SIZE_X][SIZE_Y] = {0};
    liste_proie = NULL;
    Animal *liste_predateur = NULL;

    for (int i = 0; i < NB_PROIES; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 20.0, &liste_proie);
    }
    for (int i = 0; i < NB_PREDATEURS; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 20.0, &liste_predateur);
    }

    int iteration = 0;
    while ((compte_animal_it(liste_proie) > 0 || compte_animal_it(liste_predateur) > 0) && iteration < MAX_ITER) {
        printf("\nÉtat de l'écosystème à l'itération %d:\n", iteration + 1);

        rafraichir_proies(&liste_proie, monde);
        rafraichir_predateurs(&liste_predateur, &liste_proie);
        rafraichir_monde(monde);

        afficher_ecosys(liste_proie, liste_predateur);
        
        iteration++;
    }

    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateur);

    return 0;
}
