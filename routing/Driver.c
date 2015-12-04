#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NO_CONNECTION -1
#define INITIAL_SIZE 8

struct linked_list {
    int id;
    struct linked_list *next;
    
    int numNeighbors, maxNeighbors;
    int *connectedTo;
    int *weights;
};

int init_nodeList(struct linked_list *nodeList) {
    nodeList->next = NULL;
    nodeList->numNeighbors = 0;
    nodeList->maxNeighbors = INITIAL_SIZE;
    nodeList->connectedTo = (int *)malloc((nodeList->maxNeighbors) * sizeof(int));
    nodeList->weights = (int *)calloc((nodeList->maxNeighbors), sizeof(int));
    for (int i = nodeList->numNeighbors; i < nodeList->maxNeighbors; i++)
        (nodeList->connectedTo)[i] = NO_CONNECTION;

    return nodeList->maxNeighbors;
}

void destroy_nodeList(struct linked_list *nodeList) {
    if (nodeList == NULL) return;

    free(nodeList->connectedTo);
    free(nodeList->weights);

    if (nodeList->next != NULL)
        destroy_nodeList(nodeList->next);

    free(nodeList);
}

int resize_nodeList(struct linked_list *nodeList) {
    if (nodeList->numNeighbors == nodeList->maxNeighbors) {
        (nodeList->maxNeighbors) *= 2;
        
    }
    else if (nodeList->numNeighbors <= (nodeList->maxNeighbors) / 4 && (nodeList->maxNeighbors) / 2 >= INITIAL_SIZE) {
        (nodeList->maxNeighbors) /= 2;
    }
    int *temp = (int *)malloc((nodeList->maxNeighbors) * sizeof(int));
    memcpy(temp, nodeList->connectedTo, (nodeList->numNeighbors)*sizeof(int));
    free(nodeList->connectedTo);
    nodeList->connectedTo = temp;
    temp = (int *)calloc((nodeList->maxNeighbors), sizeof(int));
    memcpy(temp, nodeList->weights, (nodeList->numNeighbors)*sizeof(int));
    free(nodeList->weights);
    nodeList->weights = temp;

    for (int i = nodeList->numNeighbors; i < nodeList->maxNeighbors; i++)
        (nodeList->connectedTo)[i] = NO_CONNECTION;

    return nodeList->maxNeighbors;
}

void newNode_nodeList(struct linked_list **last, int id) {
    struct linked_list *node = (struct linked_list *) malloc(sizeof(struct linked_list));
    init_nodeList(node);
    node->id = id;
    if ((*last) != NULL) {
        (*last)->next = node;
    }
    else {
        *last = node;
    }
}

void insert_minHeap(struct linked_list *node, int dest, int weight) {
    // MIN LIST STUFF
    int index = node->numNeighbors;
    int temp;
    node->connectedTo[index] = dest;
    node->weights[index] = weight;
    while (index > 0) {
        if (node->weights[index - 1] > node->weights[index]) {
            // swap
            temp = node->weights[index - 1];
            node->weights[index - 1] = node->weights[index];
            node->weights[index] = temp;

            temp = node->connectedTo[index - 1];
            node->connectedTo[index - 1] = node->connectedTo[index];
            node->connectedTo[index] = temp;

            index--;
        }
        else {
            break;
        }
    }
    (node->numNeighbors)++;

}

void insert_nodeList(struct linked_list **head, int a, int b, int weight, int *num_nodes) {
    //printf("insert(%i,%i,%i)\n", a, b, weight);
    if (a == b || weight < 0) return;

    if (*head == NULL) {
        // new list
        newNode_nodeList(head, ((a < b) ? a : b));
        (*num_nodes)++;
    }
    struct linked_list *current, *prev;
    current = *head;
    prev = *head;
    while (current != NULL && current->id != a) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        newNode_nodeList(&prev, a);
        current = prev->next;
        (*num_nodes)++;
    }
    insert_minHeap(current, b, weight);
    resize_nodeList(current);
    ////////////////////
    current = *head;
    prev = *head;
    while (current != NULL && current->id != b) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        newNode_nodeList(&prev, b);
        current = prev->next;
        (*num_nodes)++;
    }
    insert_minHeap(current, a, weight);
    resize_nodeList(current);
}


struct linked_list * getNode_nodeList(struct linked_list *head, int id) {
    struct linked_list *current;
    current = head;
    while (current != NULL && current->id != id) {
        current = current->next;
    }
    return current;
}

int in_S(int *S, int length, int id) {
    for (int i = 0; i < length; i++) {
        if (S[i] == id) return 1;
    }
}

void dijkstra(struct linked_list *head, int source, int num_nodes, int *S, int *D, int *P) {
    struct linked_list *current, *prev;
    current = head;
    while (current != NULL) {
        /*
        for (int i = 0; i < current->numNeighbors; i++) {
            printf("%i->%i = %i\n", current->id, current->connectedTo[i], current->weights[i]);
        }
        */
        current = current->next;
    }

    int num_S = 1; // for the source node
    S[0] = source; // S={u}

    current = getNode_nodeList(head, source);
    if (current == NULL || current->id != source) {
        printf("No source node found.\n");
        return ;
    }

    for (int i = 0; i < current->numNeighbors; i++) {
        D[current->connectedTo[i] - 1] = current->weights[i];
        P[current->connectedTo[i] - 1] = source;
    }
    /*
    printf("set initial adjacent weights\n");
    printf("\nD: ");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d  ", D[i]);
    }
    printf("\n");
    */

    while (num_S < num_nodes) {
        int minIndex = -1;
        for (int i = 0; i < num_nodes; i++) {
            if (in_S(S, num_nodes, i + 1) == 1) continue;

            if (D[i] > 0 && minIndex == -1)
                minIndex = i;
            else if (D[i] > 0 && D[i] < D[minIndex])
                minIndex = i;
        }
        S[num_S++] = minIndex + 1;
        current = getNode_nodeList(head, minIndex + 1);
        //update
        for (int i = 0; i < current->numNeighbors; i++) {

            if (in_S(S, num_nodes, current->connectedTo[i]) == 1) continue;
            // not in S
            //printf("old=%i, new=%i\n", D[current->connectedTo[i] - 1], D[(current->id) - 1] + current->weights[i]);
            if (D[current->connectedTo[i] - 1] == 0) {
                D[current->connectedTo[i] - 1] = D[(current->id) - 1] + current->weights[i];
                P[current->connectedTo[i] - 1] = current->id;
            }
            else {
                if (D[current->connectedTo[i] - 1] > D[(current->id) - 1] + current->weights[i]) {
                    D[current->connectedTo[i] - 1] = D[(current->id) - 1] + current->weights[i];
                    P[current->connectedTo[i] - 1] = current->id;
                }
            }
            //printf("updating D[%i]=%i, P[%i]=%i\n", current->connectedTo[i] - 1, D[current->connectedTo[i] - 1], current->connectedTo[i]-1, P[current->connectedTo[i]-1]);
        }

        //D[minIndex] = 0;
    }
}

int isAdjacent_nodeList(struct linked_list *head, int node, int target) {
    if (node == target) return 1;
    struct linked_list *current = getNode_nodeList(head, node);
    for (int i = 0; i < current->numNeighbors; i++) {
        if (current->connectedTo[i] == target) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    struct linked_list *head;
    head = NULL;
    int num_nodes = 0;

    char *filename;

    if (argc > 1)
        filename = argv[1];
    else
        filename = "input.txt";

    FILE* file = fopen(filename, "r");
    char line[256];

    int source, dest, a, b, weight;
    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
        presence would allow to handle lines longer that sizeof(line) */
        //printf("%s", line);
        char* token = strtok(line, "$");
        while (token) {
            //printf("token: %s\n", token);
            if (strcmp(token, "Source") == 0) {
                token = strtok(NULL, "$");
                source = atoi(token);
            }
            else if (strcmp(token, "Destination") == 0) {
                token = strtok(NULL, "$");
                dest = atoi(token);
            }
            else {
                //printf("token: %s\n", token);
                a = atoi(token);
                token = strtok(NULL, "$");
                b = atoi(token);
                token = strtok(NULL, "$");
                weight = atoi(token);
                insert_nodeList(&head, a, b, weight, &num_nodes);
            }
            token = strtok(NULL, "$");
        }
    }
    //printf("Source: %i\tDest:%i\n", source, dest);
    FILE *output = fopen("output.txt", "w");

    int *S = calloc(num_nodes, sizeof(int));
    int *D = calloc(num_nodes, sizeof(int));
    int *P = calloc(num_nodes, sizeof(int));

    int *path = calloc(num_nodes, sizeof(int));
    int backtrack = num_nodes - 1;
    int previous_hop = dest;

    P[source-1] = source;
    dijkstra(head, source, num_nodes, S, D, P);
    fprintf(output, "-------------------------------------------------\n");
    fprintf(output, "%i -> %i\n", source, dest);

    while (backtrack > 0) {
        previous_hop = P[previous_hop-1];
        path[backtrack] = previous_hop;
        if (path[backtrack] == source) break;
        backtrack--;
    }

    for (int i = backtrack; i < num_nodes; i++) {
        fprintf(output, "%i -> ", path[i]);
    }
    fprintf(output, "%i\n", dest);
    fprintf(output, "Total Distance: %i\n", D[dest-1]);
    fprintf(output, "-------------------------------------------------\n");

    free(path);
    free(S);
    free(D);
    free(P);
    for (int i = 0; i < num_nodes; i++) {
        int *S = calloc(num_nodes, sizeof(int));
        int *D = calloc(num_nodes, sizeof(int));
        int *P = calloc(num_nodes, sizeof(int));
        P[i] = i + 1;
        dijkstra(head, i+1, num_nodes, S, D, P);

        fprintf(output, "Routing Table %i\n", i+1);
        fprintf(output, "%20s%20s%20s\n", "Destination", "Next Hop", "Cost");
        for (int j = 0; j < num_nodes; j++) {
            //printf("S=%i, Dest=%i\n", i+1, j + 1);
            if (i == j) continue;
            int next = P[j];
            //printf("Predecessor of %i is %i\n", j + 1, next);
            int cost = D[j];
                // N=S
            if (next == i + 1 || next == j+1) {
                next = j + 1;
            }
            else {
                //printf("Traceback %i->%i.%s adjacent\n", j + 1, next, ((isAdjacent_nodeList(head, next, i+1)==0)?"not":""));
                while (P[next-1] != i+1) {
                    next = P[next - 1];
                    //printf("\tenroute ->%i\n", next);
                }
            }
            fprintf(output, "%20i%20i%20i\n", j+1, next, cost);
        }
        free(S);
        free(D);
        free(P);
    }


    destroy_nodeList(head);
    fclose(output);
    fclose(file);
    return 0;
}
