//
//  main.c
//  HW5
//
//  Created by morgan mcmahon on 11/24/16.
//  Copyright © 2016 morgan mcmahon. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HW5_Functions.h"

#define stringLength 30


int main(int argc, const char * argv[]) {
    preProcessFile();
    
    Gate* g1;
    Gate* g2;
    Gate* g3;
    GateList list;
    list.first = NULL;
    list.last = NULL;
    list.length = 0;
    
    g1 = newGate("Gate1", or, newFan(NULL, NULL), newFan(g2, NULL), NULL);
    g2 = newGate("Gate2", and, newFan(g1, NULL), newFan(g3, NULL), NULL);
    g3 = newGate("Gate3", nand, newFan(g2, NULL), newFan(NULL, NULL), NULL);
    
    
    //addGate(&list, g1);
    //addGate(&list, g2);
    //addGate(&list, g3);
    //printGateList(&list);
    
    
    FILE *file;
    file = fopen("/Users/mlm/Google Drive/CASE/Fall 2016/EECS 318/HW5/HW5/temp.v", "r");
    //char s1[30], s2[100], s3[100];
    char s1[stringLength];
    char s2[stringLength];
    char s3[stringLength];
    Gate* g;
    
    for(int i=0; i<14; i++){
        int tagCheck(char string[]);
        fscanf(file, "%s", s1);
        //printf("|%s| \n", s1);
        int j = tagCheck(s1);
        if(j) {
            fscanf(file, "%s %s", s2, s3);
            //need to process fanin and fanout
            g = newGate(s2, getEnum(s3), newFan(NULL, NULL), newFan(NULL, NULL), NULL);
            addGate(&list,g);
        }
            
        //printf("|%s| \n", s2);
        //printf("|%s| \n", s3);
        //printf("\n");
    }
    printGateList(&list);
}



