/**
 *Ashmethaa Arulanantham
 */
#include "a1.h"

int main(int argc, char *argv[]) {

    struct data *info = (struct data *)malloc(sizeof(struct data));
    info->commentRows = -1;
    info->isGName = false;
    info->isOName = false;
    info->normalRows = -1;
    info->vertexRows = -1;
    info->textureRows = -1;
    info->faceSize = -1;

    /*checks if a filename was provided*/
    if(argc == 2) {
        char *ppmFilename;
        char *objFilename;
        FILE *fp;

        objFilename = malloc(strlen((argv[1]) + 4) * sizeof(char));
        ppmFilename = malloc(strlen((argv[1]) + 4) * sizeof(char));

        strcpy(objFilename, argv[1]);
        strcpy(ppmFilename, argv[1]);

        strcat(objFilename, ".obj");
        strcat(ppmFilename, ".ppm");

        fp = fopen(objFilename, "r");

        /*checks if the filename is valid*/
        if(fp == NULL) {

            printf("There is no file with the name %s.\n", objFilename);

        } else {

            int error;

            error = connectPython(&ppmFilename);

            if(error != 0) {

                return error;

            }

            getLine(&fp, &info);

            /*closing file*/
            fclose(fp);

            formatingOBJ(&info, info->faceSize);

        }

        free(objFilename);
        free(ppmFilename);

    } else {

        printf("Filename is not provided. Program can't run.\n");

    }

    freeMemory(&info);

    printf("hit\n");
    
    return 0;
}

/**
 * gets each line of the file
 */
void getLine(FILE **fp, struct data **info) {
    char c;
    bool first = true;
    int length = 0;
    char *line;

    /*getting each character in the file*/
    while((c = getc(*fp)) != EOF) {

        /*extracts each line from the file*/
        if (c == '\n') {

            if(!(first)) {

                length = length + 2;

                /*sorts the line information*/
                sort(&line, length, info);

                length = 0;
                first = true;

                free(line);

            }

        } else if(first) {

            line = (char *) malloc(sizeof(char) * 2);

            line[length] = c;
            line[length + 1] = '\0';
            first = false;

        } else {

            length++;

            line = (char *)realloc(line, length + 2);
        
            line[length] = c;
            line[length + 1] = '\0';

        }
                
    }

    if(length != 0) {

        sort(&line, length + 2, info);
        free(line);

    }

}

/**
 * Frees memory either by free() method or another function call
 */
void freeMemory(struct data **info) {

    /*frees comments*/
    if((*info)->commentRows != -1) {

        freeingDoubleStringPointers(&((*info)->comments), (*info)->commentRows);

    }

    /*frees o name*/
    if((*info)->isOName) {

        free((*info)->oName);

    }

    /*frees g name*/
    if((*info)->isGName) {

        free((*info)->gName);

    }

    /*frees vertex values*/
    if((*info)->vertexRows != -1) {

        freeingDoubleFloatPointers(&((*info)->vertex), (*info)->vertexRows);

    }

    /*frees texture values*/
    if((*info)->textureRows != -1) {
        freeingDoubleFloatPointers(&((*info)->texture), (*info)->textureRows); 

    }

    /*frees normal values*/
    if((*info)->normalRows != -1) {

        freeingDoubleFloatPointers(&((*info)->normal), (*info)->normalRows); 

    }

    /*frees face values*/
    if((*info)->faceSize != -1) {

        freeingFaceInfo(&((*info)->faces), (*info)->faceSize);

    }

    /*frees struct*/
    free((*info));

}

/**
 * Calls other functions base on a certain criteria 
 */
void sort(char **line, int length, struct data **info) {
    int position = 0;
    char *token;
    
    position = getNextToken(&token, line, position, length);

    if(position != -1) {

        if(token[0] == '#') {

            storeComment(line, length, info);

        } else if(strcmp(token,"o") == 0 && !((*info)->isOName)) {

            storeOName(line, length ,info);

            (*info)->isOName = true;

        } else if(strcmp(token,"g") == 0 && !((*info)->isGName)) {

            storeGName(line, length ,info);

            (*info)->isGName = true;

        } else if(strcmp(token,"v") == 0) {

            storeVertex(line, position, length, info);

        } else if(strcmp(token,"vn") == 0) {

            storeNormal(line, position, length, info);

        } else if(strcmp(token,"vt") == 0) {

            storeTexture(line, position, length, info);

        } else if(strcmp(token,"f") == 0) {

            storeFace(line, position, length, info);

        }

        free(token);

    }
}

/**
 * gets the next token in the line
 */
int getNextToken(char **token, char **line, int position, int length) {
    int i;
    bool first = true;
    int tokenSize = 0;

    for(i = position ; i < length ; i++) {

        if((*line)[i] == ' ' || (*line)[i] == '\t' || (*line)[i] == '\0') {

            if(!(first)) {

                return i;

            }

        } else if (first) {

            (*token) = (char *)malloc(sizeof(char) * 2);

            (*token)[tokenSize] = (*line)[i];
            (*token)[tokenSize + 1] = '\0';
            first = false;

        } else {

            tokenSize++;

            (*token) = (char *)realloc((*token), tokenSize + 2);

            (*token)[tokenSize] = (*line)[i];
            (*token)[tokenSize + 1] = '\0';
        }

    }

    return -1;
}

/**
 * Stores comments 
 */
void storeComment(char **line, int length, struct data **info) {
    static int row = 0;

    if (row == 0) {

        (*info)->comments = (char **)malloc(sizeof(char*));

    } else {

        (*info)->comments = (char **)realloc((*info)->comments, sizeof(char *) * (row + 1));

    }

    (*info)->comments[row] = (char *)malloc(sizeof(char) * length);

    strcpy((*info)->comments[row], (*line));

    row++;
    (*info)->commentRows = row;

}

/**
 * Frees double pointers that are strings
 */
void freeingDoubleStringPointers(char ***pointer, int size) {

    int i;

    for(i = 0 ; i < size ; i++) {

        free((*pointer)[i]);

    }

    free(*pointer);
}

/**
 * Frees double pointers that are floaters
 */
void freeingDoubleFloatPointers(float ***pointer, int size) {

    int i;

    for(i = 0 ; i < size ; i++) {

        free((*pointer)[i]);

    }

    free(*pointer);
}

/**
 * Frees the face pointer
 */
void freeingFaceInfo(struct face **pointer, int size) {

    int i;
    
    for(i = 0 ; i < size ; i++) {

        free((*pointer)[i].v);
        free((*pointer)[i].vt);
        free((*pointer)[i].vn);

    }

    free(*pointer);
}

/**
 * Stores g name
 */
void storeGName(char **line, int length, struct data **info) {

    (*info)->gName = (char *)malloc(sizeof(char) * length);

    strcpy((*info)->gName, (*line));

}

/**
 * Stores o name
 */
void storeOName(char **line, int length, struct data **info) {

    (*info)->oName = (char *)malloc(sizeof(char) * length);

    strcpy((*info)->oName, (*line));

}

/**
 * Stores all the vertex float values 
 */
void storeVertex(char **line, int position, int length, struct data **info) {

    static int size = 0;
    static int col = 0;
    char *token;

    position = getNextToken(&token, line, position, length);

    /*checks if there was another token provided and if that token was a float*/
    if(position != -1 && isFloat(&token)) {

        /*allocates space to store vertex information*/
        if(size == 0 && col == 0) {

            (*info)->vertex = (float **)malloc(sizeof(float *));
            (*info)->vertex[size] = (float *)malloc(3 * sizeof(float));


        } else if(col == 0) {

            (*info)->vertex = (float **)realloc((*info)->vertex, (size + 1) * sizeof(float *));
            (*info)->vertex[size] = (float *)malloc(3 * sizeof(float));

        }

        /*stores token into the vertex as a float value*/
        (*info)->vertex[size][col] = atof(token);

        free(token); 

        col++;

        /*determines whether all the information was extracted or another function call needs to be done*/
        if(col <= 2) {

            storeVertex(line, position, length, info);

        } else {

            size++;
            (*info)->vertexRows = size;
            col = 0;

        }

    }

}

/**
 * Store all the normal float values 
 */
void storeNormal(char **line, int position, int length, struct data **info) {

    static int size = 0;
    static int col = 0;
    char *token;

    position = getNextToken(&token, line, position, length);

    /*checks if there was another token provided and if that token was a float*/
    if(position != -1 && isFloat(&token)) {

        /*allocates space to store normal information*/
        if(size == 0 && col == 0) {

            (*info)->normal = (float **)malloc(sizeof(float *));
            (*info)->normal[size] = (float *)malloc(3 * sizeof(float));


        } else if(col == 0) {

            (*info)->normal = (float **)realloc((*info)->normal, (size + 1) * sizeof(float *));
            (*info)->normal[size] = (float *)malloc(3 * sizeof(float));

        }

        /*stores token into the normal as a float value*/
        (*info)->normal[size][col] = atof(token);

        free(token); 

        col++;

        /*determines whether all the information was extracted or another function call needs to be done*/
        if(col <= 2) {

            storeNormal(line, position, length, info);

        } else {

            size++;
            (*info)->normalRows = size;
            col = 0;

        }

    }

}

/**
 * Store all the texture float values 
 */
void storeTexture(char **line, int position, int length, struct data **info) {

    static int size = 0;
    static int col = 0;
    char *token;

    position = getNextToken(&token, line, position, length);

    /*checks if there was another token provided and if that token was a float*/
    if(position != -1 && isFloat(&token)) {

        /*allocates space to store texture information*/
        if(size == 0 && col == 0) {

            (*info)->texture = (float **)malloc(sizeof(float *));
            (*info)->texture[size] = (float *)malloc(2 * sizeof(float));


        } else if(col == 0) {

            (*info)->texture = (float **)realloc((*info)->texture, (size + 1) * sizeof(float *));
            (*info)->texture[size] = (float *)malloc(2 * sizeof(float));

        }

        /*stores token into the texture as a float value*/
        (*info)->texture[size][col] = atof(token);

        free(token); 

        col++;

        /*determines whether all the information was extracted or another function call needs to be done*/
        if(col <= 1) {

            storeTexture(line, position, length, info);

        } else {

            size++;
            (*info)->textureRows = size;
            col = 0;

        }

    }

}

/**
 * Store all the faces int values 
 */
void storeFace(char **line, int position, int length, struct data **info) {

    static int size = 0;
    static int col = 0;
    int charLocation = 0;
    int slash = 0;
    char *token;

    position = getNextToken(&token, line, position, length);

    /*checks if there was another token provided*/
    if(position != -1) {

        /*allocates space to store faces information*/
        if(size == 0 && col == 0) {

            (*info)->faces = (struct face*)malloc(sizeof(struct face));
            (*info)->faces[size].v = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].vt = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].vn = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].arraySize = 3;
            size++;

        } else if(col == 0) {

            (*info)->faces = (struct face*)realloc((*info)->faces, (size + 1) * sizeof(struct face));
            (*info)->faces[size].v = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].vt = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].vn = (int *)malloc(3 * sizeof(int));
            (*info)->faces[size].arraySize = 3;
            size++;

        } else if(col == 3) {

            (*info)->faces[size - 1].v = (int *)realloc((*info)->faces[size - 1].v, 4 * sizeof(int));
            (*info)->faces[size - 1].vt = (int *)realloc((*info)->faces[size - 1].vt, 4 * sizeof(int));
            (*info)->faces[size - 1].vn = (int *)realloc((*info)->faces[size - 1].vn, 4 * sizeof(int));
            (*info)->faces[size - 1].arraySize = 4;
            
        }

        /*stores default value of -1 as the temp values for face vertex, face texture, and face normal*/
        (*info)->faces[size - 1].v[col] = -1;
        (*info)->faces[size - 1].vt[col] = -1;
        (*info)->faces[size - 1].vn[col] = -1;

        /*separates each value in the token to provide information on the face vertex, face texture, and face normal*/
        if((isdigit(token[0]))) {

            /*loops through each character in the token*/
            while(token[charLocation] != '\0') {

                char *temp;
                int tempSize = 1;
                temp = (char *)malloc(sizeof(char) * 2);

                if(charLocation == 0) {

                    /*collects the information for the face vertex*/
                    temp[tempSize] = '\0';
                    temp[tempSize - 1] = token[charLocation];

                    while((token[charLocation + 1] != '\0') && (token[charLocation + 1] != '/')) {

                        tempSize++;
                        charLocation++;

                        temp = (char *)realloc(temp, (tempSize + 1) * sizeof(char));

                        temp[tempSize] = '\0';
                        temp[tempSize - 1] = token[charLocation];
                            
                    }
                    
                    (*info)->faces[size - 1].v[col] = atoi(temp);

                } else if(token[charLocation] == '/') {

                    if(token[charLocation + 1] == '/') {

                        /*collects information for vertex normal*/
                        charLocation = charLocation + 2;

                        temp[tempSize] = '\0';
                        temp[tempSize - 1] = token[charLocation];

                        while(token[charLocation + 1] != '\0') {

                            tempSize++;
                            charLocation++;

                            temp = (char *)realloc(temp, (tempSize + 1) * sizeof(char));

                            temp[tempSize] = '\0';
                            temp[tempSize - 1] = token[charLocation];
                            
                        }
                        
                        (*info)->faces[size - 1].vn[col] = atoi(temp);

                    } else  {

                        /*collects info for either the face texture or face normal*/
                        slash++;
                        charLocation++;

                        temp[tempSize] = '\0';
                        temp[tempSize - 1] = token[charLocation];

                        while((token[charLocation + 1] != '\0') && (token[charLocation + 1] != '/')) {

                            tempSize++;
                            charLocation++;

                            temp = (char *)realloc(temp, (tempSize + 1) * sizeof(char));

                            temp[tempSize] = '\0';
                            temp[tempSize - 1] = token[charLocation];
                            
                        }

                        /*determines what information was collected and store it in the correct variable*/
                        if(slash == 1) {
                           
                            (*info)->faces[size - 1].vt[col] = atoi(temp);

                        } else {
                           
                            (*info)->faces[size - 1].vn[col] = atoi(temp);

                        }

                    }

                }

                charLocation++;

                free(temp);

            }
                
        }

        col++;

        storeFace(line, position, length, info);

        free(token);

    }

    col = 0;
    (*info)->faceSize = size;

}

/**
 * determines if the token provided is a float
 */
bool isFloat(char **token) {

    int position = 0;
    bool firstMinus = true;
    bool firstDot = true;

    while((*token)[position] != '\0') {

        if((*token)[position] == '.') {

            if(!(firstDot)) {

                return false;

            }

            firstDot = false;

        } else if ((*token)[position] == '-') {

            if(!(firstMinus)) {

                return false;

            }

            firstMinus = false;

        } else if(isdigit((*token)[position]) == 0){

            return false;

        }
        
        position++;

    }

    return true;

}

/**
 * formating the normal, texture, and vertex to the faces values
 */
void formatingOBJ(struct data **info, int size) {

    int i, x;
    int vPosition = 0;
    int vnPosition = 0; 
    int vtPosition = 0;
    int indicesPosition = 0;
    struct formating formatingObj;

    formatingObj.vertices = (int *)malloc(sizeof(int) * 3 * size * (*info)->faces[0].arraySize);
    formatingObj.indices = (int *)malloc(sizeof(int) * size * (*info)->faces[0].arraySize);
    formatingObj.texture = (int *)malloc(sizeof(int) * 2 * size * (*info)->faces[0].arraySize);
    formatingObj.normal = (int *)malloc(sizeof(int) * 3 * size * (*info)->faces[0].arraySize);

    for( i = 0 ; i < size ; i++ ) {
        
        for( x = 0 ; x < (*info)->faces[i].arraySize ; x++ ) {

            formatingObj.vertices[vPosition] = (*info)->vertex[(*info)->faces[i].v[x] - 1][0];
            formatingObj.vertices[vPosition + 1] = (*info)->vertex[(*info)->faces[i].v[x]][0];
            formatingObj.vertices[vPosition + 2] = (*info)->vertex[(*info)->faces[i].v[x] +1][0];
            formatingObj.indices[indicesPosition] = indicesPosition + 1;

            indicesPosition++;
            vPosition = vPosition + 3;

        }

        for( x = 0 ; x < (*info)->faces[i].arraySize ; x++ ) {

            if((*info)->faces[i].vt[x] != -1) {

                formatingObj.texture[vtPosition] = (*info)->texture[(*info)->faces[i].vt[x] - 1][0];
                formatingObj.texture[vtPosition + 1] = (*info)->texture[(*info)->faces[i].vt[x]][0];

                vtPosition = vtPosition + 2;

            }

        }

        for( x = 0 ; x < (*info)->faces[i].arraySize ; x++ ) {

            if((*info)->faces[i].vn[x] != -1) {

                formatingObj.normal[vnPosition] = (*info)->normal[(*info)->faces[i].vn[x] - 1][0];
                formatingObj.normal[vnPosition + 1] = (*info)->normal[(*info)->faces[i].vn[x]][0];
                formatingObj.normal[vnPosition + 2] = (*info)->normal[(*info)->faces[i].vn[x] + 1][0];
                
                vnPosition = vnPosition + 3;

            }

        }
        
    }

    free(formatingObj.vertices);
    free(formatingObj.indices);
    free(formatingObj.texture);
    free(formatingObj.normal);

}

/**
 * connects c to python and receives information from it
 */
int connectPython(char **ppmFilename) {

    PyObject *pName, *pModule, *pFunc, *pArgs, *pValue, *pList, *pTempValue;

    Py_Initialize();
    pName = PyUnicode_DecodeFSDefault("readppm");
    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if(pModule != NULL) {

        pFunc = PyObject_GetAttrString(pModule, "openPPMFile");

        if(pFunc && PyCallable_Check(pFunc)) {

            /*creates the arguments for the python function*/
            pArgs = PyTuple_New(2);
            pValue = PyUnicode_FromString((*ppmFilename));
            pList = PyList_New(3);

            if(!pValue || pList == NULL) {

                Py_DECREF(pArgs);
                Py_DECREF(pModule);
                Py_DECREF(pList);

                printf("Error with argument for python.\n");

                return 1;

            }

            /*inserts values into the python list created to for the parameter*/
            pTempValue = PyLong_FromLong(-1);

            PyList_SetItem(pList, 0, pTempValue);
            PyList_SetItem(pList, 1, pTempValue);
            PyList_SetItem(pList, 2, pTempValue);

            Py_DECREF(pTempValue);

            PyTuple_SetItem(pArgs, 0, pValue);
            PyTuple_SetItem(pArgs, 1, pList);

            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);

            /*extracts the the list information returned from the python code*/
            if(pValue != NULL) {

                int size = PyList_Size(pList);
                int i;
                int count = 0;
                int rowPosition = 0;
                int **pythonList;
                pythonList = (int **)malloc(sizeof(int));
                pythonList[0] = (int *)malloc(sizeof(int) * 3);

                for(i = 0 ; i < size ; i++) {

                    if(count == 3) {

                        int z;
                        rowPosition++;
                        count = 0;

                        pythonList = (int **)realloc(pythonList, (rowPosition + 1) * sizeof(int *));
                        pythonList[rowPosition] = (int *)malloc(sizeof(int) * 3);

                        for(z = 0 ; z < 3 ; z++) {
                            pythonList[rowPosition][z] = 0;
                        }

                    }

                    PyObject *pTemp = PyList_GetItem(pList, i);
                    pythonList[rowPosition][count] = PyLong_AsLong(pTemp);
                    Py_DECREF(pTemp);

                    count++;

                }

                free(pythonList);

                formatingPMM(&pythonList, rowPosition);

            } else {

                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                Py_DECREF(pList);
                            
                printf("Call to python failed.\n");

                return 1;
            }
                    
        } else {

            printf("Can't find python function.\n");

        }

        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
                
    } else {

        printf("Module not found\n");

        return 1;

    }

    if(Py_FinalizeEx() < 0) {
        return 120;
    }

    return 0;
}

/**
 * formating the information provided by the ppm file
 */
void formatingPMM(int ***pythonList, int size) {

    struct formating format;
    int i, x;
    int count = 0;

    format.ppmInfo = (int *)malloc((size * sizeof(int) * 3) - (4 * sizeof(int)) + (size * sizeof(int)) );

    for(i = 3 ; i < size ; i++) {


        for(x = 0 ; x < 3 ; x++) {

            format.ppmInfo[count] = (*pythonList)[i][x];
            count++;
        }

        format.ppmInfo[count] = 255;
        count++;

    }

    free(format.ppmInfo);

}