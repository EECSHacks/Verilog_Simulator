//
//  HW5_Functions.h
//  HW5
//
//  Created by morgan mcmahon on 11/25/16.
//  Copyright © 2016 morgan mcmahon. All rights reserved.
//

#ifndef HW5_Functions_h
#define HW5_Functions_h

#define stringLength 30


/* ---------- Enum Gate Primitive Methods ---------- */

// These the enumeated verilog primitives
enum gateType {
    // Gate Primatives
    and,
    nand,
    or,
    nor,
    xor,
    xnor,
    // Transmission Gate Primitives
    not,
    buf
};

// returns the string name of the gateType enum
const char* getGateType(enum gateType type){
    switch (type)
    {
        case and: return "and";
        case nand: return "nand";
        case or: return "or";
        case nor: return "nor";
        case xor: return "xor";
        case xnor: return "xnor";
        case not: return "not";
        case buf: return "buffer";
    }
}

// given a string representation of a gate type the enum representation is returned
enum gateType getEnum(char* string){
    if (strcmp(string, "and") == 0)
        return and;
    if (strcmp(string, "nand") == 0)
        return nand;
    if (strcmp(string, "or") == 0)
        return or;
    if (strcmp(string, "nor") == 0)
        return nor;
    if (strcmp(string, "xor") == 0)
        return xor;
    if (strcmp(string, "xnor") == 0)
        return xnor;
    if (strcmp(string, "not") == 0)
        return not;
    if (strcmp(string, "buf") == 0)
        return buf;
    return 0;
}

/* ---------- Type defines ----------- */

// Gate DataStructure
typedef struct Gate {
    char name[10];              // names 5 characters and shorter are supported
    enum gateType type;         // types given by the verilog primitive gate types
    struct Fan* fanin;          // pointers to fanin gates
    struct Fan* fanout;         // pointers to fanout gates
    struct Gate* next_gate;     // pointer to next gate in the list
} Gate;

typedef struct GateList {
    Gate* first;
    Gate* last;
    int length;
} GateList;

// GateList is used to store the list of gates for the fanin and fanouts
typedef struct Fan {
    struct Gate* gate;
    struct Fan* next_fan;
} Fan;

void addFan(struct Fan* list, struct Gate* gate) {
    struct Fan* temp = list;
    while(temp->next_fan != NULL){ //while next gate exists
        temp = temp->next_fan; //move to the next gate
    }
    //temp->next_gate = gate;
}

void addGate(struct GateList* list, struct Gate* gate){
    if (list->first == NULL) {              //sets the first gate as head of the list
        list->first = gate;
        list->last = gate;
    } else {
        list->last->next_gate = gate;       //sets the point of the last gate in GateList to the new element
        list->last = gate;                  //sets the last element of GateList to the new element
        list->length = list->length + 1;    //adds one to length
    }
    gate->next_gate = NULL;                 //sets the next gate pointer of the new last gate to null
}

/* ---------- Print Methods ----------- */

//prints out the list of gates
void printGateList (struct GateList* list) {
    void printGate (Gate* gate);
    struct Gate* gate;
    gate = list->first;
    while (gate->next_gate != NULL){    // while the next gate pointer is != null
        printGate(gate);                // print the gate
        gate = gate->next_gate;         // move to next gate in list
    }
    printGate(gate);
}

//prints out the gate to the terminal
void printGate (struct Gate* gate) {
    void printFan (struct Fan* gate);
    printf("\nName: ");
    printf("%s", gate->name);
    printf("\nType: ");
    printf("%s", getGateType(gate->type));
    printf("\nFanin: ");
    printFan(gate->fanin);
    printf("\nFanout: ");
    printFan(gate->fanout);
    printf("\nNext Gate: ");
    printf("%s", gate->next_gate->name);
    printf("\n");
}

void printFan (struct Fan* list) {
    Fan* f = list;
    while (f != NULL) {
        if (f->gate != NULL)
            printf("%s", f->gate->name);
        printf("%s", ", ");
        f = f->next_fan;
    }
}

/* ---------- Constructors ---------- */

// Gate constructor
Gate* newGate(char name[], enum gateType type, struct Fan* fin, struct Fan* fout, struct Gate* next_gate){
    Gate* p = malloc(sizeof(*p));
    strcpy(p->name, name);
    p->type = type;
    p->fanin = fin;
    p->fanout = fout;
    p->next_gate = next_gate;
    return p;
}

// Fan constructor
Fan* newFan(struct Gate* gate, struct Fan* next_fan) {
    Fan* p = malloc(sizeof(*p));
    p->gate = gate;
    p->next_fan = next_fan;
    return p;
}


/* ---------- Parser Methods ---------- */

//returns a 1 if the string is a gate
int tagCheck(char string[stringLength]){
    if (strcmp(string, "and") == 0)
        return 1;
    if (strcmp(string, "nand") == 0)
        return 1;
    if (strcmp(string, "or") == 0)
        return 1;
    if (strcmp(string, "nor") == 0)
        return 1;
    if (strcmp(string, "xor") == 0)
        return 1;
    if (strcmp(string, "xnor") == 0)
        return 1;
    if (strcmp(string, "not") == 0)
        return 1;
    if (strcmp(string, "buf") == 0)
        return 1;
    return 0;
}




/* ---------- Preprocessor ---------- */

/*
 * Reads in file input.v in working directory
 * Processes the file and writes the result in a temporary file
 * -The goal is to get rid of variations between files like white space and comments
 */
void preProcessFile(){
    FILE *in;
    FILE *temp;
    in = fopen("/Users/mlm/Google Drive/CASE/Fall 2016/EECS 318/HW5/HW5/input.v", "r");
    temp = fopen("/Users/mlm/Google Drive/CASE/Fall 2016/EECS 318/HW5/HW5/temp.v", "w");
    
    // flags used for parsing
    int open_paren = 0;
    int comment = 0;
    int block_comment = 0;
    int semicolon = 0;
    char last = 0;
    char car = fgetc(in); // temporary variable for each caracter read
    int print = 0; // blooean to decide if car should be printed
    
    while (car != EOF ){
        /*
         * Gets rid of:
         *  - all tabs in the file
         *  - all spaces inside and new lines characters inside parethesis
         *  - all blank lines
         */
        
        print = 1;
        
        //deals with semicolons
        if (car == ';'){
            semicolon = 1;
            fputc(';', temp);
        }
        if ( (car < 123) && (car > 64) && semicolon ){
            semicolon = 0;
            fputc('\n', temp);
        }
        
        if (car=='\t')                                          // deletes all tabs
            print = 0;
        else if (open_paren && ((car ==' ') || (car=='\n')))    // deletes all all seperators inside parenthesis
            print = 0;
        else if (comment || block_comment)                      // deletes all comments
            print = 0;
        else if (semicolon)                                     // deletes everthing between ______
            print = 0;
        else if ( (last == ' ') && (car == ' ') )
            print = 0;
        else if ( (last == ',') && (car == ' ') )
            print = 0;
        
        if (print)
            fputc(car, temp);
        
        //reset flags if needed
        if (car == ')')
            open_paren = 0;                             // flag is reset
        
        //set flags based on current character
        if (car == '(')
            open_paren = 1;                             // flag is set
        
        //checks for comments blocks and sets flags
        if ( (last == '/') && (car == '/') ) {
            comment = 1;                                // entering comment text
        } else if ( (last == '/') && (car == '*') ) {
            block_comment = 1;                          // entering block comment
        }
        
        //resets block comment flags
        if (car == '\n')                                // comment end with line end
            comment = 0;
        if ( (last == '*') && (car == '/') )
            block_comment = 0;                          // end of block comment reached
        
        last = car;                                     // store car value
        car = fgetc(in);                                // get next character from file
    }
    
    // closes both files
    fclose(in);
    fclose(temp);
}

#endif /* HW5_Functions_h */
