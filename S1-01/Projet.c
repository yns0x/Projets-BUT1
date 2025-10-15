#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#pragma warning (disable : 4996 6031)

enum {
    TAILLE_MAX_ETU = 50,
    NB_UE_PAR_SEMESTRE = 6,
    MAX_ELEVES = 100,
    NB_SEMESTRES = 6
};

typedef struct {
    float note;
    int est_enregistree;
} UE;

typedef struct {
    UE no_ue[NB_UE_PAR_SEMESTRE];
    char statut[20];
} Semestre;

typedef struct {
    char prenom[TAILLE_MAX_ETU];
    char nom[TAILLE_MAX_ETU];
    int id;
    int semestre_actuel;
    int est_defaillant;
    int demission;
    Semestre cursus_complet[NB_SEMESTRES];
} Etudiant;

int inscription(Etudiant etudiant[], char prenom[TAILLE_MAX_ETU], char nom[TAILLE_MAX_ETU], int compteur_etudiant) {
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
        etudiant[compteur_etudiant].est_defaillant = 0;
        etudiant[compteur_etudiant].demission = 0;
        return 1;
    }
    printf("Nombre max d'étudiants atteint\n");
    return 0;
}

char* resultat_note(float note) {
    return note < 10 ? "AJ" : "ADM";
}


void etudiants(Etudiant etudiant[], int compteur_etudiants) {
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

void cursus(Etudiant etudiant[], int compteur) {
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
                }
                else {
                    printf("* (*) - ");
                }
            }
            printf("%s\n", etudiant[id_e].cursus_complet[i].statut);
        }
    }
    else {
        printf("Identifiant incorrect\n");
    }
}

// enregistre la note pour l'élève avec l'identifiant sélectionné, l'UE voulue et la note qu'il a obtenue
void note(Etudiant etudiant[], int compteur) {
    int id, ue;
    float note;
    scanf("%i %i %f", &id, &ue, &note);
    if (etudiant[id - 1].est_defaillant == 1 || etudiant[id - 1].demission == 1) {
        printf("Etudiant hors formation\n");
        return;
    }
    else if ((note < 0 || note > 20) || (id < 1 || id > compteur)) {
        printf("Erreur\n");
    }
    else {
        etudiant[id - 1].cursus_complet[etudiant[id - 1].semestre_actuel - 1].no_ue[ue - 1].note = note;
        etudiant[id - 1].cursus_complet[etudiant[id - 1].semestre_actuel - 1].no_ue[ue - 1].est_enregistree = 1;
        printf("Note enregistree\n");
    }
}

void jury(Etudiant etudiant[], int parite, int compteur_etudiant) {
    for (int etu = 0; etu < compteur_etudiant; ++etu) {
        int semestre_actuel = etudiant[etu].semestre_actuel;
        if (semestre_actuel % 2 == parite % 2) {
            int nb_notes = 0;
            for (int i = 0; i < NB_UE_PAR_SEMESTRE; ++i) {
                if (etudiant[etu].cursus_complet[semestre_actuel - 1].no_ue[i].est_enregistree == 1)
                    nb_notes++;
            }
            if (nb_notes == NB_UE_PAR_SEMESTRE) {
                strcpy(etudiant[etu].cursus_complet[semestre_actuel - 1].statut, "valide");
                if (semestre_actuel < NB_SEMESTRES) {
                    etudiant[etu].semestre_actuel++;
                    strcpy(etudiant[etu].cursus_complet[etudiant[etu].semestre_actuel - 1].statut, "en cours");
                    for (int i = 0; i < NB_UE_PAR_SEMESTRE; i++) {
                        etudiant[etu].cursus_complet[etudiant[etu].semestre_actuel - 1].no_ue[i].est_enregistree = 0;
                    } 
                }
            }
        }
    }
}

void defaillance(Etudiant etudiant[], int compteur) {
    int id;
    scanf("%i", &id);
    if (id >= 0 && id <= compteur) {
        int semestre = etudiant[id - 1].semestre_actuel;
        if (strcmp(etudiant[id - 1].cursus_complet[semestre - 1].statut, "en cours") == 0){
            etudiant[id - 1].est_defaillant = 1;
            printf("Defaillance enregistree\n");
            strcpy(etudiant[id - 1].cursus_complet[semestre - 1].statut, "defaillance");
        }
        else {
            printf("Etudiant hors formation\n");
        }
        return;
    }
    printf("Identifiant incorrect\n");
}

void demission(Etudiant etudiant[], int compteur) {
    int id;
    scanf("%i", &id);
    if (id >= 0 && id <= compteur) {
        int semestre = etudiant[id - 1].semestre_actuel;
        if (strcmp(etudiant[id - 1].cursus_complet[semestre - 1].statut, "en cours") == 0) {
            etudiant[id - 1].demission = 1;
            strcpy(etudiant[id - 1].cursus_complet[semestre - 1].statut, "demission");
            printf("Demission enregistree\n");
        }
        else {
            printf("Etudiant hors formation\n");
        }
        return;
    }
    printf("Identifiant incorrect\n");
}



int main() {

    Etudiant etudiant[MAX_ELEVES];

    char commande[20];
    int compteur_etudiant = 0;

    do {
        scanf("%s", commande);

        if (strcmp(commande, "INSCRIRE") == 0) {
            char prenom[TAILLE_MAX_ETU], nom[TAILLE_MAX_ETU];
            scanf("%50s %50s", prenom, nom);
            if (inscription(etudiant, prenom, nom, compteur_etudiant) == 1) compteur_etudiant++;
        }
        else if (strcmp(commande, "ETUDIANTS") == 0) {
            etudiants(etudiant, compteur_etudiant);
        }
        else if (strcmp(commande, "CURSUS") == 0) {
            cursus(etudiant, compteur_etudiant);
        }
        else if (strcmp(commande, "NOTE") == 0) {
            note(etudiant, compteur_etudiant);
        }
        else if (strcmp(commande, "JURY") == 0) {
            int parite;
            scanf("%i", &parite);
            jury(etudiant, parite, compteur_etudiant);
        }
        else if (strcmp(commande, "DEFAILLANCE") == 0) {
            defaillance(etudiant, compteur_etudiant);
        }
        else if (strcmp(commande, "DEMISSION") == 0) {
            demission(etudiant, compteur_etudiant);
        }
    } while (strcmp(commande, "EXIT") != 0);
}
