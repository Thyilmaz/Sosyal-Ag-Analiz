#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED 0
#define BLACK 1
#define MAX_LINE 256
#define MAX_FRIENDS 100

// Kullanici yapisi
typedef struct User {
    int id;
    int friendCount;
    int friends[MAX_FRIENDS];
} User;

// Red-Black agac dugum yapisi
typedef struct Node {
    User *user;
    int color;
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

// Global kok dugum
Node *root = NULL;

// NIL yaprak dugum (sentinel)
Node *NIL;

// Ileri tanimlamalar
void communityDfs(int userId, int *visited);
void influenceDfs(int currentId, int *visited, int *influenceCount);

// Yeni kullanici olusturma
User* createUser(int id) {
    User *newUser = (User*)malloc(sizeof(User));
    if (newUser == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }

    newUser->id = id;
    newUser->friendCount = 0;

    return newUser;
}

// Yeni dugum olusturma
Node* createNode(int userId) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Bellek ayirma hatasi!\n");
        exit(1);
    }

    newNode->user = createUser(userId);
    newNode->color = RED; // Yeni dugumler kirmizi olarak baslar
    newNode->left = NIL;
    newNode->right = NIL;
    newNode->parent = NIL;

    return newNode;
}

// Sol rotasyon islemi
void leftRotate(Node *x) {
    Node *y = x->right;
    x->right = y->left;

    if (y->left != NIL)
        y->left->parent = x;

    y->parent = x->parent;

    if (x->parent == NIL)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}

// Sag rotasyon islemi
void rightRotate(Node *y) {
    Node *x = y->left;
    y->left = x->right;

    if (x->right != NIL)
        x->right->parent = y;

    x->parent = y->parent;

    if (y->parent == NIL)
        root = x;
    else if (y == y->parent->left)
        y->parent->left = x;
    else
        y->parent->right = x;

    x->right = y;
    y->parent = x;
}

// Ekleme sonrasi agacin dengesini duzeltme
void fixInsert(Node *k) {
    Node *u;

    while (k->parent->color == RED) {
        if (k->parent == k->parent->parent->right) {
            u = k->parent->parent->left;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->left) {
                    k = k->parent;
                    rightRotate(k);
                }

                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                leftRotate(k->parent->parent);
            }
        } else {
            u = k->parent->parent->right;

            if (u->color == RED) {
                u->color = BLACK;
                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                k = k->parent->parent;
            } else {
                if (k == k->parent->right) {
                    k = k->parent;
                    leftRotate(k);
                }

                k->parent->color = BLACK;
                k->parent->parent->color = RED;
                rightRotate(k->parent->parent);
            }
        }

        if (k == root)
            break;
    }

    root->color = BLACK;
}

// Agaca kullanici ekleme
void insertUser(int userId) {
    Node *newNode = createNode(userId);
    Node *y = NIL;
    Node *x = root;

    while (x != NIL) {
        y = x;
        if (newNode->user->id < x->user->id)
            x = x->left;
        else if (newNode->user->id > x->user->id)
            x = x->right;
        else {
            // Zaten var, bellegi serbest birak ve cik
            free(newNode->user);
            free(newNode);
            return;
        }
    }

    newNode->parent = y;

    if (y == NIL)
        root = newNode;
    else if (newNode->user->id < y->user->id)
        y->left = newNode;
    else
        y->right = newNode;

    // Yeni dugum yaprak ise ve kok degilse, duzeltme gerekir
    if (newNode->parent == NIL) {
        newNode->color = BLACK;
        return;
    }

    if (newNode->parent->parent == NIL)
        return;

    fixInsert(newNode);
}

// Kullaniciyi ID'ye gore arama
Node* searchUser(int userId) {
    Node *current = root;

    while (current != NIL) {
        if (userId == current->user->id)
            return current;
        else if (userId < current->user->id)
            current = current->left;
        else
            current = current->right;
    }

    return NULL; // Kullanici bulunamadi
}

// Arkadaslik iliskisi ekleme
void addFriendship(int userId1, int userId2) {
    Node *user1Node = searchUser(userId1);
    Node *user2Node = searchUser(userId2);

    if (user1Node == NULL || user2Node == NULL) {
        printf("Arkadaslik eklenemiyor: Bir veya her iki kullanici da bulunamadi.\n");
        return;
    }

    // Kullanici 1'e arkadas ekleme
    User *user1 = user1Node->user;
    int friendExists1 = 0;

    for (int i = 0; i < user1->friendCount; i++) {
        if (user1->friends[i] == userId2) {
            friendExists1 = 1;
            break;
        }
    }

    if (!friendExists1 && user1->friendCount < MAX_FRIENDS) {
        user1->friends[user1->friendCount++] = userId2;
    }

    // Kullanici 2'ye arkadas ekleme
    User *user2 = user2Node->user;
    int friendExists2 = 0;

    for (int i = 0; i < user2->friendCount; i++) {
        if (user2->friends[i] == userId1) {
            friendExists2 = 1;
            break;
        }
    }

    if (!friendExists2 && user2->friendCount < MAX_FRIENDS) {
        user2->friends[user2->friendCount++] = userId1;
    }
}

// Inorder traversal ile agaci yazdirma
void inorderTraversal(Node *root) {
    if (root != NIL) {
        inorderTraversal(root->left);

        printf("Kullanici ID: %d, Renk: %s, Arkadaslar: ",
               root->user->id,
               root->color == RED ? "KIRMIZI" : "SIYAH");

        for (int i = 0; i < root->user->friendCount; i++) {
            printf("%d ", root->user->friends[i]);
        }
        printf("\n");

        inorderTraversal(root->right);
    }
}

// DFS ile belirli mesafede arkadaslari bulma
void findFriendsAtDistance(int userId, int distance) {
    Node *userNode = searchUser(userId);

    if (userNode == NULL) {
        printf("Kullanici bulunamadi.\n");
        return;
    }

    // Maksimum kullanici ID araligi icin bellek ayir (ID'ler 101-105 gibi kucuk)
    int maxId = 200; // 105'in biraz uzerinde bir deger
    int *visited = (int*)calloc(maxId, sizeof(int));
    int *distances = (int*)malloc(maxId * sizeof(int));

    for (int i = 0; i < maxId; i++) {
        distances[i] = -1;
    }

    // BFS kullanarak arkadaslari mesafelerine gore bulma
    int queue[maxId];
    int front = 0, rear = 0;

    queue[rear++] = userId;
    visited[userId] = 1;
    distances[userId] = 0;

    while (front < rear) {
        int currentId = queue[front++];
        Node *currentNode = searchUser(currentId);

        if (currentNode == NULL) continue;

        for (int i = 0; i < currentNode->user->friendCount; i++) {
            int friendId = currentNode->user->friends[i];

            if (!visited[friendId]) {
                visited[friendId] = 1;
                distances[friendId] = distances[currentId] + 1;
                queue[rear++] = friendId;
            }
        }
    }

    printf("%d ID'li kullanicidan %d mesafede bulunan arkadaslar:\n", userId, distance);
    for (int i = 100; i <= 150; i++) {
        if (distances[i] == distance) {
            printf("- Kullanici %d\n", i);
        }
    }

    free(visited);
    free(distances);
}

// Ortak arkadaslari bulma
void findCommonFriends(int userId1, int userId2) {
    Node *user1Node = searchUser(userId1);
    Node *user2Node = searchUser(userId2);

    if (user1Node == NULL || user2Node == NULL) {
        printf("Bir veya her iki kullanici da bulunamadi.\n");
        return;
    }

    User *user1 = user1Node->user;
    User *user2 = user2Node->user;

    printf("%d ve %d ID'li kullanicilarin ortak arkadaslari:\n", userId1, userId2);
    int commonCount = 0;

    for (int i = 0; i < user1->friendCount; i++) {
        for (int j = 0; j < user2->friendCount; j++) {
            if (user1->friends[i] == user2->friends[j]) {
                printf("- Kullanici %d\n", user1->friends[i]);
                commonCount++;
                break;
            }
        }
    }

    if (commonCount == 0) {
        printf("Ortak arkadas bulunamadi.\n");
    }
}

// Düzeltilmiş topluluk tespiti kodu için yardımcı DFS fonksiyonu
void communityDfs(int userId, int *visited) {
    if (userId < 0 || userId >= 200 || !visited) return;

    visited[userId] = 1;
    printf("- Kullanici %d\n", userId);

    Node *userNode = searchUser(userId);
    if (userNode == NULL) return;

    for (int i = 0; i < userNode->user->friendCount; i++) {
        int friendId = userNode->user->friends[i];
        if (friendId >= 0 && friendId < 200 && !visited[friendId]) {
            communityDfs(friendId, visited);
        }
    }
}

// Düzeltilmiş topluluk tespiti fonksiyonu
void detectCommunities() {
    // Bellek ayır ve sıfırla
    int *visited = (int*)calloc(200, sizeof(int));
    if (!visited) {
        printf("Bellek ayirma hatasi!\n");
        return;
    }

    int communityCount = 0;

    // Inorder traversal ile tüm kullanıcıları kontrol et
    void visitInorder(Node *node) {
        if (node == NIL) return;

        visitInorder(node->left);

        int userId = node->user->id;
        if (userId >= 0 && userId < 200 && !visited[userId]) {
            communityCount++;
            printf("Topluluk %d:\n", communityCount);
            communityDfs(userId, visited);
            printf("\n");
        }

        visitInorder(node->right);
    }

    // Ağacı dolaş
    visitInorder(root);

    if (communityCount == 0) {
        printf("Hic topluluk bulunamadi.\n");
    } else {
        printf("Toplam %d topluluk tespit edildi.\n", communityCount);
    }

    // Belleği temizle
    free(visited);
}

// DFS ile etki alani hesaplama (düzeltilmiş versiyon)
void influenceDfs(int currentId, int *visited, int *influenceCount) {
    if (currentId < 0 || currentId >= 200 || !visited || !influenceCount) return;

    visited[currentId] = 1;
    (*influenceCount)++;

    Node *currentNode = searchUser(currentId);
    if (currentNode == NULL) return;

    for (int i = 0; i < currentNode->user->friendCount; i++) {
        int friendId = currentNode->user->friends[i];
        if (friendId >= 0 && friendId < 200 && !visited[friendId]) {
            influenceDfs(friendId, visited, influenceCount);
        }
    }
}

// Kullanicinin etki alanini hesaplama (düzeltilmiş versiyon)
int calculateInfluence(int userId) {
    Node *userNode = searchUser(userId);

    if (userNode == NULL) {
        printf("Kullanici bulunamadi.\n");
        return 0;
    }

    int *visited = (int*)calloc(200, sizeof(int));
    if (!visited) {
        printf("Bellek ayirma hatasi!\n");
        return 0;
    }

    int influenceCount = 0;

    // DFS ile etki alanini hesapla
    influenceDfs(userId, visited, &influenceCount);

    // Kullanicinin kendisini cikar
    int result = influenceCount - 1;
    free(visited);

    return result;
}

// Dosyadan veri okuma
void readDataFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file)) {
        // Satir sonundaki newline karakterini kaldir
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n')
            line[len-1] = '\0';

        if (strncmp(line, "USER ", 5) == 0) {
            int userId;
            if (sscanf(line + 5, "%d", &userId) == 1) {
                insertUser(userId);
            }
        }
        else if (strncmp(line, "FRIEND ", 7) == 0) {
            int userId1, userId2;
            if (sscanf(line + 7, "%d %d", &userId1, &userId2) == 2) {
                addFriendship(userId1, userId2);
            }
        }
    }

    fclose(file);
}

// Ana fonksiyon
int main() {
    // NIL sentinel dugumu baslatma
    NIL = (Node*)malloc(sizeof(Node));
    NIL->color = BLACK;
    NIL->left = NULL;
    NIL->right = NULL;
    NIL->parent = NULL;
    NIL->user = NULL;

    root = NIL;

    // Dosyadan veri okuma
    readDataFromFile("veriseti.txt");

    printf("Veri dosyasi basariyla yuklendi.\n");

    int choice, userId1, userId2, distance;

    do {
        printf("\n--- Sosyal Ag Analizi Menusu ---\n");
        printf("1. Agac yapisini goruntule\n");
        printf("2. Belirli mesafedeki arkadaslari bul\n");
        printf("3. Ortak arkadaslari bul\n");
        printf("4. Topluluklari tespit et\n");
        printf("5. Etki alanini hesapla\n");
        printf("0. Cikis\n");
        printf("Seciminiz: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nKirmizi-Siyah Agac Yapisi:\n");
                inorderTraversal(root);
                break;

            case 2:
                printf("\nKullanici ID: ");
                scanf("%d", &userId1);
                printf("Mesafe: ");
                scanf("%d", &distance);
                findFriendsAtDistance(userId1, distance);
                break;

            case 3:
                printf("\nIlk kullanici ID: ");
                scanf("%d", &userId1);
                printf("Ikinci kullanici ID: ");
                scanf("%d", &userId2);
                findCommonFriends(userId1, userId2);
                break;

            case 4:
                printf("\nTopluluk Tespiti:\n");
                detectCommunities();
                break;

            case 5:
                printf("\nKullanici ID: ");
                scanf("%d", &userId1);
                int influence = calculateInfluence(userId1);
                printf("%d ID'li kullanicinin etki alani: %d kullanici\n", userId1, influence);
                break;

            case 0:
                printf("\nProgramdan cikiliyor...\n");
                break;

            default:
                printf("\nGecersiz secim! Tekrar deneyin.\n");
        }
    } while (choice != 0);

    // Bellegi temizleme
    free(NIL);

    return 0;
}