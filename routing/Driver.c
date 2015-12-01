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
    free(nodeList->connectedTo);
    free(nodeList->weights);

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
        last = &node;
    }
}

void insert_minHeap(struct linked_list *node, int dest, int weight) {
    // MIN HEAP STUFF
}

void insert_nodeList(struct linked_list *head, int a, int b, int weight) {
    if (a == b) return;
    struct linked_list *current, *prev;
    current = head;
    while (current != NULL && current->id != (a < b) ? a : b) {
        prev = current;
        current = current->next;
    }
    if (current == NULL) {
        newNode_nodeList(&prev, (a < b) ? a : b);
        current = prev->next;
    }
    insert_minHeap(current, (a < b) ? a : b, weight);
}


int main(int argc, char* argv[])
{
    char *filename;

    if (argc > 1)
        filename = argv[1];
    else
        filename = "input.txt";

    FILE* file = fopen(filename, "r");
    char line[256];

    while (fgets(line, sizeof(line), file)) {
        /* note that fgets don't strip the terminating \n, checking its
        presence would allow to handle lines longer that sizeof(line) */
        //printf("%s", line);
        char* token = strtok(line, "$");
        while (token) {
            printf("token: %s\n", token);
            token = strtok(NULL, "$");
        }
    }

    fclose(file);

    return 0;
}