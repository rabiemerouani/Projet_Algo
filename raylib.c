#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "raylib.h"

// Structure du bouton
typedef struct Button {
    Rectangle rect;
    Color color;
    bool pressed;
} Button;

// Fonction pour dessiner un bouton avec les contrôles nécessaires
void dessinerBouton(float x, float y, float width, float height, const char text[20], Button* button);

// Structure pour un nœud de la liste chaînée
struct Node {
    int data;
    struct Node* next;
};

// Fonction pour ajouter un nœud à la fin de la liste chaînée
struct Node* addNode(struct Node* head, int data, int position);

// Fonction pour trier la liste chaînée par tri à bulles
struct Node* trierListe(struct Node* head);

// Fonction pour afficher la liste chaînée
void afficherListeChainee(struct Node* head);

// Fonction pour créer une liste chaînée à partir d'une chaîne de valeurs séparées par des virgules
struct Node* createListFromString(const char* values);

int main() {
    const int screenWidth = 1700;
    const int screenHeight = 900;

    // Initialisation de la fenêtre avec fond noir
    SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
    InitWindow(screenWidth, screenHeight, "Tri de liste chaînée avec Raylib");

    // Changer la couleur de fond en utilisant ClearBackground
    SetTargetFPS(60);

    // Champ de texte pour saisir les valeurs séparées par des virgules
    char inputValues[256] = "\0";
    char inputValue[256] = "\0";
    char inputPosition[256] = "\0";

    // Liste chaînée initiale vide
    struct Node* list = NULL;

    Button boutonTrier, boutonCreer;

    while (!WindowShouldClose()) {
        // Choisir la couleur noire pour le fond
        ClearBackground(BLACK);

        // Affichage de la liste chaînée triée
        afficherListeChainee(list);

        dessinerBouton(50, 40, 200, 40, "Creer", &boutonCreer);
        DrawRectangle(350, 40, 400, 40, WHITE);
        DrawText(inputValues, 360, 50, 20, BLACK);

        // Bouton "Creer"
        if ((CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 40, 400, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) ||
            (CheckCollisionPointRec(GetMousePosition(), boutonCreer.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            // Créer une liste chaînée à partir des valeurs saisies par l'utilisateur
            list = createListFromString(inputValues);

            // Effacer le champ de texte après la création
            inputValues[0] = '\0';
        }

        dessinerBouton(850, 40, 400, 40, "Trier", &boutonTrier);
        if (CheckCollisionPointRec(GetMousePosition(), boutonTrier.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Trier la liste chaînée
            list = trierListe(list);
        }

        // Gestion de la suppression d'un élément
        struct Node* temp = list;
        int xPos = 50; // Position initiale à gauche
        while (temp != NULL) {
            Rectangle deleteButton = {xPos - 40, 250, 80, 30};
            if (CheckCollisionPointRec(GetMousePosition(), deleteButton) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                // Supprimer l'élément de la liste
                struct Node* current = list;
                struct Node* previous = NULL;
                while (current != NULL && current->data != temp->data) {
                    previous = current;
                    current = current->next;
                }

                if (previous == NULL) {
                    // Si l'élément à supprimer est en tête de liste
                    list = current->next;
                } else {
                    // Sinon, mettre à jour les liens dans la liste
                    previous->next = current->next;
                }

                free(current);
                break;
            }

            temp = temp->next;
            xPos += 120; // Ajuster l'espacement
        }

        // Mise à jour des valeurs saisies dans le champ de texte
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 40, 400, 40})) {
            int key = GetKeyPressed();
            if ((key >= KEY_ZERO && key <= KEY_NINE) || (key == KEY_BACKSPACE) || (key == KEY_COMMA)) {
                int len = strlen(inputValues);
                if (key == KEY_BACKSPACE && len > 0) {
                    inputValues[len - 1] = '\0';
                } else if (key == KEY_COMMA) {
                    // Permettre la saisie de virgules
                    if (len < 255) {
                        inputValues[len] = ',';
                        inputValues[len + 1] = '\0';
                    }
                } else if (len < 255) {
                    inputValues[len] = (char)key;
                    inputValues[len + 1] = '\0';
                }
            }
        }

        dessinerBouton(850, 160, 400, 40, "Inserer a la position", &boutonTrier);
        DrawRectangle(350, 160, 200, 40, WHITE);
        DrawText(inputValue, 360, 170, 20, BLACK);

        DrawRectangle(600, 160, 200, 40, WHITE);
        DrawText(inputPosition, 610, 170, 20, BLACK);

        // Champ texte pour la valeur à insérer
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){350, 160, 200, 40})) {
            int key3 = GetKeyPressed();
            if ((key3 >= KEY_ZERO && key3 <= KEY_NINE) || (key3 == KEY_BACKSPACE) || (key3 == KEY_COMMA)) {
                int len3 = strlen(inputValue);
                if (key3 == KEY_BACKSPACE && len3 > 0) {
                    inputValue[len3 - 1] = '\0';
                } else if (len3 < 255) {
                    inputValue[len3] = (char)key3;
                    inputValue[len3 + 1] = '\0';
                }
            }
        }

        // Champ texte pour la position d'insertion
        if (CheckCollisionPointRec(GetMousePosition(), (Rectangle){600, 160, 200, 40})) {
            int key4 = GetKeyPressed();
            if ((key4 >= KEY_ZERO && key4 <= KEY_NINE) || (key4 == KEY_BACKSPACE)) {
                int len4 = strlen(inputPosition);
                if (key4 == KEY_BACKSPACE && len4 > 0) {
                    inputPosition[len4 - 1] = '\0';
                } else if (len4 < 255) {
                    inputPosition[len4] = (char)key4;
                    inputPosition[len4 + 1] = '\0';
                }
            }
        }

        // Bouton "Inserer a la position"
        if (CheckCollisionPointRec(GetMousePosition(), boutonTrier.rect) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            int valueToInsert = atoi(inputValue);
            int positionToInsert = atoi(inputPosition);
            list = addNode(list, valueToInsert, positionToInsert);
        }

        BeginDrawing();
        EndDrawing();
    }

    // Libération de la mémoire utilisée par la liste chaînée
    while (list != NULL) {
        struct Node* temp = list;
        list = list->next;
        free(temp);
    }

    CloseWindow();

    return 0;
}

// Fonction pour dessiner un bouton avec les contrôles nécessaires
void dessinerBouton(float x, float y, float width, float height, const char text[20], Button* button) {
    *button = (Button){(Rectangle){x, y, width, height}, WHITE, false};
    float textX = button->rect.x + button->rect.width / 2 - MeasureText(text, 20) / 2;
    float textY = button->rect.y + button->rect.height / 2 - 20 / 2;
    bool mouseOverButton = CheckCollisionPointRec(GetMousePosition(), button->rect);

    // Check if button is clicked
    if (mouseOverButton) {
        button->color = BLUE;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            button->pressed = true;
        }
    } else
        button->color = WHITE;

    // Dessin du rectangle
    DrawRectangleRec(button->rect, button->color);
    // Dessin du texte
    DrawText(text, textX, textY, 20, BLACK);
}

// Fonction pour ajouter un nœud à la fin de la liste chaînée
struct Node* addNode(struct Node* head, int data, int position) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = data;
    newNode->next = NULL;

    if (position == 1 || head == NULL) {
        // Si la position est 1 ou la liste est vide, le nouveau nœud devient la tête
        newNode->next = head;
        return newNode;
    }

    // Trouver le nœud précédent à la position d'insertion
    struct Node* temp = head;
    for (int i = 1; i < position - 1 && temp != NULL; i++) {
        temp = temp->next;
    }

    if (temp == NULL) {
        // Si la position est plus grande que la taille actuelle de la liste, ajouter à la fin
        temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
    }

    // Ajouter le nouveau nœud à la position spécifiée
    newNode->next = temp->next;
    temp->next = newNode;

    return head;
}

// Fonction pour trier la liste chaînée par tri à bulles
struct Node* trierListe(struct Node* head) {
    int swapped;
    struct Node* ptr1;
    struct Node* lptr = NULL;

    // Vérifier si la liste est vide
    if (head == NULL)
        return head;

    do {
        swapped = 0;
        ptr1 = head;

        while (ptr1->next != lptr) {
            if (ptr1->data > ptr1->next->data) {
                // Échanger les données des nœuds
                int temp = ptr1->data;
                ptr1->data = ptr1->next->data;
                ptr1->next->data = temp;
                swapped = 1;
            }
            ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);

    return head;
}

// Fonction pour afficher la liste chaînée
void afficherListeChainee(struct Node* head) {
    struct Node* temp = head;

    int xPos = 50; // Position initiale à gauche

    while (temp != NULL) {
        // Dessin d'un cercle plus grand pour chaque élément
        DrawCircle(xPos, 300, 30, WHITE);

        // Dessin du nombre à l'intérieur du cercle avec une taille de police plus grande
        DrawText(TextFormat("%d", temp->data), xPos - MeasureText(TextFormat("%d", temp->data), 30) / 2, 285, 30, BLACK);

        // Dessin d'une ligne pour connecter les cercles (sauf pour le premier élément)
        if (temp->next != NULL) {
            DrawLine(xPos + 30, 300, xPos + 100, 300, WHITE);
        }

        // Rectangle blanc pour chaque élément avec le texte "Supprimer"
        Rectangle deleteButton = {xPos - 40, 250, 80, 30};
        DrawRectangleRec(deleteButton, WHITE);
        DrawText("Supprimer", xPos - 35, 255, 20, BLACK);

        temp = temp->next;
        xPos += 120; // Ajuster l'espacement
    }
}

// Fonction pour créer une liste chaînée à partir d'une chaîne de valeurs séparées par des virgules
struct Node* createListFromString(const char* values) {
    struct Node* list = NULL;
    char* token = strtok((char*)values, ",");

    int position = 1; // Position d'insertion pour maintenir l'ordre


    while (token != NULL) {
        int value = atoi(token);
        list = addNode(list, value, position);
        token = strtok(NULL, ",");
        position++; // Incrémenter la position pour chaque ajout
    }

    return list;
}