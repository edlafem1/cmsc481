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
    printf("Source: %i\tDest:%i\n", source, dest);

    struct linked_list *current, *prev;
    current = head;
    while (current != NULL) {
        for (int i = 0; i < current->numNeighbors; i++) {
            printf("%i->%i = %i\n", current->id, current->connectedTo[i], current->weights[i]);
        }
        current = current->next;
    }

    /*
    Dijsktra's Algorithm
    Initialization:
    S = {u}
    for all nodes v
        if v adjacent to u {
            D(v) = c(u,v)
        else 
            D(v) = ?
    
    Loop
        find w not in S with the smallest D(w)
        add w to S
        update D(v) for all v adjacent to w and not in S:
            D(v) = min{D(v), D(w) + c(w,v)}
    until all nodes in S 
    */
    int *S = calloc(num_nodes, sizeof(int));
    int *D = calloc(num_nodes, sizeof(int));
    int num_S = 1; // for the source node
    S[0] = source; // S={u}

    printf("dj start\n");
    current = getNode_nodeList(head, source);
    printf("got source node\n");
    if (current == NULL || current->id != source) {
        printf("No source node found.\n");
        free(S);
        if (head != NULL) {
            destroy_nodeList(head);
        }
        fclose(file);
        return 0;
    }
    printf("we good 1\n");
    /*
    for all nodes v
        if v adjacent to u {
            D(v) = c(u,v)
        else 
            D(v) = ?

    This is taken care of in our connectedTo and weights arrays
    */
    for (int i = 0; i < current->numNeighbors; i++) {
        D[current->connectedTo[i]-1] = current->weights[i];
    }
    printf("set initial adjacent weights\n");
    printf("\nD: ");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d  ", D[i]);
    }
    printf("\n");

    /*
     Loop
        find w not in S with the smallest D(w)
        add w to S
        update D(v) for all v adjacent to w and not in S:
            D(v) = min{D(v), D(w) + c(w,v)}
    until all nodes in S 
    */
    while (num_S < num_nodes) {
        //printf("num_S=%i\n", num_S);
        int minIndex = -1;
        for (int i = 0; i < num_nodes; i++) {
            if (in_S(S, num_nodes, i + 1) == 1) continue;

            if (D[i] > 0 && minIndex == -1)
                minIndex = i;
            else if (D[i] > 0 && D[i] < D[minIndex])
                minIndex = i;
        }
        //printf("Found closest node: %i-%d\n", minIndex + 1, D[minIndex]);
        S[num_S++] = minIndex + 1;
        current = getNode_nodeList(head, minIndex + 1);
        //update
        for (int i = 0; i < current->numNeighbors; i++) {
            
            if (in_S(S, num_nodes, current->connectedTo[i]) == 1) continue;
            // not in S
            //printf("old=%i, new=%i\n", D[current->connectedTo[i] - 1], D[(current->id) - 1] + current->weights[i]);
            if (D[current->connectedTo[i] - 1] == 0) {
                D[current->connectedTo[i] - 1] = D[(current->id) - 1] + current->weights[i];
            }
            else {
                D[current->connectedTo[i] - 1] = min(D[current->connectedTo[i] - 1], D[(current->id) - 1] + current->weights[i]);
            }
            printf("updating D[%i]=%i\n", current->connectedTo[i] - 1, D[current->connectedTo[i] - 1]);
        }

        //D[minIndex] = 0;
    }

    printf("S: ");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d  ", S[i]);
    }
    printf("\nD: ");
    for (int i = 0; i < num_nodes; i++) {
        printf("%d  ", D[i]);
    }
    printf("\n");

    destroy_nodeList(head);
    fclose(file);
    return 0;
}




/*
for (int i = 0; i < current->numNeighbors; i++) {
for (int j = 0; j < num_S; j++) {
if (S[j] != current->connectedTo[i]) {
S[num_S++] = current->connectedTo[i];
D[current->connectedTo[i]] = 1;
}
}
}
*/