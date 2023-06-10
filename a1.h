#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Python.h>
#define PY_SSIZE_T_CLEAN

struct data {
    char **comments;
    int commentRows;
    char *oName;
    bool isOName;
    char *gName;
    bool isGName;
    float **vertex;
    int vertexRows;
    float **texture;
    int textureRows;
    float **normal;
    int normalRows;
    struct face *faces;
    int faceSize;
};

struct face {
    int *v;
    int *vt;
    int *vn;
    int arraySize;
};

struct formating {
    int *ppmInfo;
    int *vertices;
    int *normal;
    int *texture;
    int *indices;
};

void getLine(FILE **, struct data **);
void sort(char **, int, struct data **);
int getNextToken(char **, char **, int, int);
void storeComment(char **, int , struct data **);
void freeingDoubleStringPointers(char ***, int );
void storeOName(char **, int, struct data **);
void storeGName(char **, int , struct data **);
void storeVertex(char **, int, int, struct data **);
bool isFloat(char **);
void storeTexture(char **, int, int, struct data **);
void freeingDoubleFloatPointers(float ***, int);
void storeNormal(char **, int, int, struct data **);
void storeFace(char **, int, int, struct data **);
void freeingFaceInfo(struct face **, int);
void formatingOBJ(struct data **, int);
void freeMemory(struct data **);
int connectPython(char **);
void formatingPMM(int ***, int);