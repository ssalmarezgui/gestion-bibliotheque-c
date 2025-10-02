#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVRES 100 // Nombre maximum de livres dans la bibliothèque

// Structure pour représenter un livre avec ses informations de base et ses copies
typedef struct {
    char titre[50];
    char auteur[50];
    int isbn;
    int total_copies;       // Nombre total de copies du livre
    int copies_disponibles; // Nombre de copies disponibles pour emprunt
} Livre;

typedef struct {
    int idLivre;
    char nomClient[50];
    char dateEmprunt[11];
} Emprunt;

// Fonction pour ajouter un nouveau livre dans la bibliothèque
void ajouterLivre(Livre bibliotheque[], int *compteur) {
    printf("\t\t\t\tEntrez le titre du livre : ");
    scanf(" %[^\n]", bibliotheque[*compteur].titre); // Saisie du titre du livre
    printf("\t\t\t\tEntrez l'auteur du livre : ");
    scanf(" %[^\n]", bibliotheque[*compteur].auteur); // Saisie de l'auteur du livre
    printf("\t\t\t\tEntrez l'ISBN du livre : ");
    scanf("%d", &bibliotheque[*compteur].isbn); // Saisie de l'ISBN du livre
    printf("\t\t\t\tEntrez le nombre total de copies : ");
    scanf("%d", &bibliotheque[*compteur].total_copies); // Nombre total de copies

    bibliotheque[*compteur].copies_disponibles = bibliotheque[*compteur].total_copies; // Initialement, toutes les copies sont disponibles
    (*compteur)++; // Incrémenter le compteur de livres
    printf("\t\t\t\tLivre ajouté avec succès !\n");
}

// Fonction pour afficher tous les livres dans la bibliothèque
void afficherLivres(Livre bibliotheque[], int compteur) {
    printf("\n\t\t\t\tListe des livres disponibles :\n");
    for (int i = 0; i < compteur; i++) {
        printf("\t\t\t\tTitre : %s, Auteur : %s, ISBN : %d, Copies totales : %d, Copies disponibles : %d\n",
               bibliotheque[i].titre,
               bibliotheque[i].auteur,
               bibliotheque[i].isbn,
               bibliotheque[i].total_copies,
               bibliotheque[i].copies_disponibles);
    }
}

// Fonction pour emprunter un exemplaire d'un livre
void emprunterLivre(Livre bibliotheque[], int compteur, Emprunt emprunts[], int *compteurEmprunts) {
    int isbn;
    printf("\t\t\t\tEntrez l'ISBN du livre à emprunter : ");
    scanf("%d", &isbn);

    for (int i = 0; i < compteur; i++) {
        if (bibliotheque[i].isbn == isbn) {
            if (bibliotheque[i].copies_disponibles > 0) {
                bibliotheque[i].copies_disponibles--; // Réduire le nombre de copies disponibles
                printf("\t\t\t\tEntrez la date d'emprunt (JJ/MM/AAAA) : ");
                scanf("%s", emprunts[*compteurEmprunts].dateEmprunt);
                printf("\t\t\t\tEntrez le nom du client : ");
                scanf("%s", emprunts[*compteurEmprunts].nomClient);
                emprunts[*compteurEmprunts].idLivre = isbn;
                (*compteurEmprunts)++;
                printf("\t\t\t\tLivre emprunté avec succès par %s !\n", emprunts[*compteurEmprunts - 1].nomClient);
            } else {
                printf("\t\t\t\tAucune copie disponible pour ce livre.\n");
            }
            return;
        }
    }
    printf("\t\t\t\tLivre non trouvé.\n");
}
// Fonction pour retourner un exemplaire d'un livre
void retournerLivre(Livre bibliotheque[], int compteur, Emprunt emprunts[], int *compteurEmprunts) {
    int isbn;
    char nomClient[50];
    printf("\t\t\t\tEntrez l'ISBN du livre à retourner : ");
    scanf("%d", &isbn);
    printf("\t\t\t\tEntrez le nom du client qui retourne le livre : ");
    scanf("%s", nomClient);

    for (int i = 0; i < compteur; i++) {
        if (bibliotheque[i].isbn == isbn) {
            if (bibliotheque[i].copies_disponibles < bibliotheque[i].total_copies) {
                // Trouver l'emprunt correspondant au client et à l'ISBN
                for (int j = 0; j < *compteurEmprunts; j++) {
                    if (emprunts[j].idLivre == isbn && strcmp(emprunts[j].nomClient, nomClient) == 0) {
                        bibliotheque[i].copies_disponibles++; // Augmenter le nombre de copies disponibles

                        printf("\t\t\t\tLivre retourné avec succès par %s !\n", nomClient);

                        // Supprimer l'emprunt de la liste
                        for (int k = j; k < *compteurEmprunts - 1; k++) {
                            emprunts[k] = emprunts[k + 1];
                        }
                        (*compteurEmprunts)--;

                        return;
                    }
                }
                printf("\t\t\t\tAucun emprunt trouvé pour ce client avec cet ISBN.\n");
            } else {
                printf("\t\t\t\tToutes les copies de ce livre sont déjà disponibles.\n");
            }
            return;
        }
    }
    printf("\t\t\t\tLivre non trouvé.\n");
}

// Fonction pour sauvegarder les données dans un fichier
void sauvegarderDonnees(Livre bibliotheque[], int compteur, Emprunt emprunts[], int compteurEmprunts) {
    FILE *fLivres = fopen("livres.txt", "w");
    for (int i = 0; i < compteur; i++) {
        fprintf(fLivres, "\t\t\t\t%s;%s;%d;%d;%d\n", bibliotheque[i].titre, bibliotheque[i].auteur, bibliotheque[i].isbn, bibliotheque[i].total_copies, bibliotheque[i].copies_disponibles);
    }
    fclose(fLivres);

    FILE *fEmprunts = fopen("emprunts.txt", "w");
    for (int i = 0; i < compteurEmprunts; i++) {
        fprintf(fEmprunts, "\t\t\t\t%d;%s;%s\n", emprunts[i].idLivre, emprunts[i].nomClient, emprunts[i].dateEmprunt);
    }
    fclose(fEmprunts);
}

// Fonction pour charger les données depuis un fichier
void chargerDonnees(Livre bibliotheque[], int *compteur, Emprunt emprunts[], int *compteurEmprunts) {
    FILE *fLivres = fopen("livres.txt", "r");
    if (fLivres != NULL) {
        while (fscanf(fLivres, "\t\t\t\t%[^;];%[^;];%d;%d;%d\n", bibliotheque[*compteur].titre, bibliotheque[*compteur].auteur, &bibliotheque[*compteur].isbn, &bibliotheque[*compteur].total_copies, &bibliotheque[*compteur].copies_disponibles) != EOF) {
            (*compteur)++;
        }
        fclose(fLivres);
    }

    FILE *fEmprunts = fopen("emprunts.txt", "r");
    if (fEmprunts != NULL) {
        while (fscanf(fEmprunts, "\t\t\t\t%d;%[^;];%[^\n]\n", &emprunts[*compteurEmprunts].idLivre, emprunts[*compteurEmprunts].nomClient, emprunts[*compteurEmprunts].dateEmprunt) != EOF) {
            (*compteurEmprunts)++;
        }
        fclose(fEmprunts);
    }
}

// Fonction pour afficher les clients ayant emprunté des livres
void afficherEmprunts(Emprunt emprunts[], int compteurEmprunts) {
    printf("\n\t\t\t\tListe des emprunts :\n");
    for (int i = 0; i < compteurEmprunts; i++) {
        printf("\t\t\t\tID Livre : %d, Nom du client : %s, Date d'emprunt : %s\n",
               emprunts[i].idLivre,
               emprunts[i].nomClient,
               emprunts[i].dateEmprunt);
    }
}

int main() {
    Livre bibliotheque[MAX_LIVRES]; // Tableau pour stocker les livres
    Emprunt emprunts[MAX_LIVRES]; // Tableau pour stocker les emprunts
    int compteur = 0; // Compteur de livres dans la bibliothèque
    int compteurEmprunts = 0; // Compteur des emprunts
    int choix; // Variable pour stocker le choix de l'utilisateur dans le menu

    // Charger les données existantes
    chargerDonnees(bibliotheque, &compteur, emprunts, &compteurEmprunts);

    // Boucle principale pour afficher le menu et effectuer des actions
    do {
        printf("\n\t\t\t\t----------------------------------------------------------\n");
        printf("\n\t\t\t\t----------- Système de Gestion de Bibliothèque -----------\n");
        printf("\n\t\t\t\t----------------------------------------------------------\n");
        printf("\t\t\t\t\t\t1. Ajouter un livre\n");
        printf("\t\t\t\t\t\t2. Afficher tous les livres\n");
        printf("\t\t\t\t\t\t3. Emprunter un livre\n");
        printf("\t\t\t\t\t\t4. Retourner un livre\n");
        printf("\t\t\t\t\t\t5. Afficher les emprunts\n");
        printf("\t\t\t\t\t\t6. Sauvegarder les données\n");
        printf("\t\t\t\t\t\t0. Quitter\n");
        printf("\n\t\t\t\t----------------------------------------------------------\n");
        printf("\n\t\t\t\t----------------------------------------------------------\n");
        printf("\t\t\t\t\t\tChoisissez une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                 ajouterLivre(bibliotheque, &compteur);
                 break;
            case 2:
                 afficherLivres(bibliotheque, compteur);
                 break;
            case 3:
                     emprunterLivre(bibliotheque, compteur, emprunts, &compteurEmprunts);
                     break;
            case 4:
                     retournerLivre(bibliotheque, compteur, emprunts, &compteurEmprunts);
                     break;
            case 5:
                     afficherEmprunts(emprunts, compteurEmprunts);
                     break;
            case 6:
                     sauvegarderDonnees(bibliotheque, compteur, emprunts, compteurEmprunts);
                     printf("\t\t\t\t\t\tDonnées sauvegardées avec succès !\n");
                     break;
            case 0:
                 printf("\t\t\t\t\t\tAu revoir !\n");
                 break;
            default:
                 printf("\t\t\t\tOption invalide. Veuillez réessayer.\n");
        }
    } while (choix != 0);
    return 0;
}
