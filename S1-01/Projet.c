#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma warning (disable : 4996 6031)
#define MAX_ELEVES 100
#define NB_SEMESTRES 6
#define NB_UE_PAR_SEMESTRE 6

enum {
    TAILLE_MAX_ETU = 50 // la taille max du nom et prénom d'un etudiant
};

typedef struct {
    float note;
    int est_enregistree; // si c'est = 0 alors, il y aura ('*') sinon cela veut dire qu'elle est bien définie
} UE;

typedef struct {
    UE no_ue[NB_UE_PAR_SEMESTRE];
    char statut[20]; // "en cours", "demission", "ajourne", "admis", "defaillant", "diplome".
} Semestre;

typedef struct {
    char prenom[TAILLE_MAX_ETU];
    char nom[TAILLE_MAX_ETU];
    int id;
    int semestre_actuel; // Le numéro du semestre en cours (1 à 6)
    Semestre cursus_complet[NB_SEMESTRES];
} Etudiant;

Etudiant etudiant[MAX_ELEVES];

int inscription(char prenom[TAILLE_MAX_ETU], char nom[TAILLE_MAX_ETU], int compteur_etudiant) {
    if (compteur_etudiant < MAX_ELEVES) {

        // on verifie si l'étudiant est deja inscrit
        for (int i = 0; i < compteur_etudiant; i++) {
            if (strcmp(etudiant[i].prenom, prenom) == 0 && strcmp(etudiant[i].nom, nom) == 0) {
                printf("Nom incorrect\n");
                return 0; // on sort si un doublon est trouvé
            }
        }

        strcpy(etudiant[compteur_etudiant].prenom, prenom);
        strcpy(etudiant[compteur_etudiant].nom, nom);
        etudiant[compteur_etudiant].id = compteur_etudiant + 1;
        etudiant[compteur_etudiant].semestre_actuel = 1;
        strcpy(etudiant[compteur_etudiant].cursus_complet[0].statut, "en cours");
        for (int i = 0; i < NB_UE_PAR_SEMESTRE; i++) {
            etudiant[compteur_etudiant].cursus_complet[0].no_ue[i].est_enregistree = 0;
        }
        printf("Inscription enregistree (%d)\n", etudiant[compteur_etudiant].id);
        return 1;
    }
    printf("Nombre max d'étudiants atteint\n");
    return 0;
}

char* resultat_note(const float note) {
    if (note < 10.0) {
        return "AJ";
    }
    return "ADM";
}

void etudiants(int compteur_etudiants) {
    for (int i = 0; i < compteur_etudiants; i++) {
        // On calcule l'index du semestre actuel
        int no_semestre = etudiant[i].semestre_actuel - 1;

        printf("%i - %s %s - S%i - %s\n",
               etudiant[i].id,
               etudiant[i].prenom,
               etudiant[i].nom,
               etudiant[i].semestre_actuel,
               etudiant[i].cursus_complet[no_semestre].statut);
    }
}

void cursus(int compteur) {
    int id;
    scanf("%i", &id);
    if (id <= compteur && id > 0) {
        int id_e = id - 1;
        printf("%d %s %s\n", etudiant[id_e].id, etudiant[id_e].prenom, etudiant[id_e].nom);
        for (int i = 0; i < etudiant[id_e].semestre_actuel; i++) {
            printf("S%d - ", i + 1);
            for (int j = 0; j < NB_UE_PAR_SEMESTRE; j++) {
                UE ue_actuelle = etudiant[id_e].cursus_complet[i].no_ue[j];
                if (ue_actuelle.est_enregistree == 1) {
                    printf("%.1f (%s) - ", ue_actuelle.note, resultat_note(ue_actuelle.note));
                } else {
                    printf("* (*) - ");
                }
            }
            printf("%s\n", etudiant[id_e].cursus_complet[i].statut);
        }
    } else {
        printf("Identifiant incorrect\n");
    }
}

void note(int compteur) {
    int id, ue;
    float note;
    scanf("%i %i %f", &id, &ue, &note);
    if ((note < 0 || note > 20) || (id < 1 || id > compteur)) {
        printf("Erreur\n");
    }
    else {
        etudiant[id - 1].cursus_complet[etudiant[id - 1].semestre_actuel - 1].no_ue[ue - 1].note = note;
        etudiant[id - 1].cursus_complet[etudiant[id - 1].semestre_actuel - 1].no_ue[ue - 1].est_enregistree = 1;
        printf("Note enregistrée\n");
    }
}

int main() {
    char commande[20];
    int compteur_etudiant = 0;

    do {
        scanf("%s", commande);

        if (strcmp(commande, "INSCRIRE") == 0) {
            char prenom[TAILLE_MAX_ETU], nom[TAILLE_MAX_ETU];
            scanf("%50s %50s", prenom, nom);
            if (inscription(prenom, nom, compteur_etudiant) == 1) compteur_etudiant++;
        }
        else if (strcmp(commande, "ETUDIANTS") == 0) {
            etudiants(compteur_etudiant);
        }
        else if (strcmp(commande, "CURSUS") == 0) {
            cursus(compteur_etudiant);
        }
        else if (strcmp(commande, "NOTE") == 0){
            note(compteur_etudiant);
        }
    } while (strcmp(commande, "EXIT") != 0);
}