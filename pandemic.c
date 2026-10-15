#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define START_POPULATION 8

struct Menneske{
    int state; //0 = healthy 1 = infected 2 = imune/dead
    int id;
    char *name[15];
    int arbejder; //0 = no 1 = yes
};

char names[][15] = {"Per", "Bob", "Zacharias", "David", "Victoria", "Clara", "Luna", "Søren"};
int relations = 0;

struct Menneske personer[START_POPULATION];
struct Menneske relation[START_POPULATION*START_POPULATION][2];

void printRelations(){
    printf("Test");
    for(int i=0;i<relations;i++){
        printf("\n(%d, %d), (%s, %s)", relation[i][0].id, relation[i][1].id, *relation[i][0].name, *relation[i][1].name);
    }
}

void removeRelation(int n){
    for(int i=n;i<relations-1;i++){
        relation[i][0] = relation[i+1][0];
        relation[i][1] = relation[i+1][1];
    }
    relations -= 1;
}

void setup(){
    srand(time(NULL));
    for(int i=0;i<START_POPULATION;i++){
        struct Menneske person = {
            .state = 0,
            .id = i,
            .name = names[rand() % 8],
            .arbejder = rand() % 2
        };
        personer[i] = person;
    }
    //i er for index of person, og n er for number of relation
    for(int i=0;i<START_POPULATION;i++){
        struct Menneske person = personer[i];
        for(int n=0;n<rand() % 5;n++){
            struct Menneske relateredePerson = personer[rand() % START_POPULATION];
            relation[relations][0] = person;
            relation[relations][1] = relateredePerson;
            relations += 1;
        }
    }
    int relationsCopy = relations;
    for(int i=0;i<relationsCopy;i++){
        relation[relations][0] = relation[i][1];
        relation[relations][1] = relation[i][0];
        relations += 1;
    }
    for(int i=0;i<relations;i++){
        int count = 0;
        if(relation[i][0].id == relation[i][1].id){
            removeRelation(i);
            continue;
        }
        for(int n=0;n<relations;n++){
            if(relation[n][0].id == relation[i][0].id && relation[n][1].id == relation[i][1].id){
                count += 1;
            }
        }
        if(count > 1){
            removeRelation(i);
            continue;
        }
    }
    printRelations();
}

int main(){
    setup();
    return 69;
}