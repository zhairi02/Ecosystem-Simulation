#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"

int main(void) {
    srand(time(NULL));

    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;

    // Création de 20 proies et 20 prédateurs aléatoires
    for (int i = 0; i < 20; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 10.0, &liste_proie);
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, 10.0, &liste_predateur);
    }

    // Sauvegarde de l'écosystème
    ecrire_ecosys("ecosysteme.txt", liste_predateur, liste_proie);

    // Chargement de l'écosystème dans de nouvelles listes
    Animal *nouvelle_liste_proie = NULL;
    Animal *nouvelle_liste_predateur = NULL;
    lire_ecosys("ecosysteme.txt", &nouvelle_liste_predateur, &nouvelle_liste_proie);

    // Affichage des écosystèmes pour comparaison visuelle
    printf("État initial de l'écosystème (proies et prédateurs) :\n");
    afficher_ecosys(liste_proie, liste_predateur);

    printf("\nÉtat de l'écosystème après lecture du fichier (proies et prédateurs) :\n");
    afficher_ecosys(nouvelle_liste_proie, nouvelle_liste_predateur);

    // Libération de la mémoire
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateur);
    liberer_liste_animaux(nouvelle_liste_proie);
    liberer_liste_animaux(nouvelle_liste_predateur);

    return 0;
}
