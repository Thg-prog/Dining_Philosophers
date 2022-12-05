//
//  main.c
//  Five-Dining_philosophers
//
//  Created by Антон on 05.12.2022.
//
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
 
typedef struct philosoph_t {
    int number;
    unsigned left;
    unsigned right;
    unsigned time;
} philosoph_t;
 
typedef struct table_t {
    pthread_mutex_t forks[5];
} table_t;
 
typedef struct philosoph_args {
     philosoph_t *philosoph;
     const table_t *table;
} philosoph_args;
 
pthread_mutex_t entry_point = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t entry_point1 = PTHREAD_MUTEX_INITIALIZER;
int whoeat[5]={0,0,0,0,0};
void init_philosoph(philosoph_t *philosoph, int number,unsigned left,unsigned right,unsigned time) {
    philosoph->number = number;
    philosoph->left = left;
    philosoph->right = right;
    philosoph->time=time;
}
 
void init_table(table_t *table) {
    size_t i;
    for (i = 0; i < 5; i++) {
        pthread_mutex_init(&table->forks[i], NULL);
    }
}

void* dining(void *args) {
    philosoph_args *arg = (philosoph_args*) args;
    philosoph_t *philosoph = arg->philosoph;
    table_t *table = arg->table;
    while (1){
//        берем вилки и едим
        sleep(philosoph->time);
        pthread_mutex_lock(&entry_point);
        pthread_mutex_lock(&table->forks[philosoph->left]);
        pthread_mutex_lock(&table->forks[philosoph->right]);
        pthread_mutex_unlock(&entry_point);
        whoeat[(philosoph->number)-1]=1;
        //printf("%i начал есть\n", philosoph->number);
        puts("*********************");
        for(int i=0;i<5;i++){
            if (whoeat[i]==1){
                printf("Философ %i ест\n", i+1);
            }else{
                printf("Философ %i размышляет\n", i+1);
            }
        }
        puts("*********************");
        pthread_mutex_lock(&entry_point1);
//        ложим вилки на место и молчим
        sleep(philosoph->time);
        pthread_mutex_unlock(&table->forks[philosoph->right]);
        pthread_mutex_unlock(&table->forks[philosoph->left]);
        //printf("%i закончил ecть \n", philosoph->number);
        whoeat[(philosoph->number)-1]=0;
        pthread_mutex_unlock(&entry_point1);
        
    }
}
 
int main() {
    pthread_t threads[5];
    philosoph_t philosophs[5];
    philosoph_args args[5];
    table_t table;
    init_table(&table);
//    подаем порядковый номер философа, левую и правую вилку, задержку по времени еды:)
    init_philosoph(&philosophs[0], 1, 0, 1,10);
    init_philosoph(&philosophs[1], 2, 1, 2,1);
    init_philosoph(&philosophs[2], 3, 2, 3,10);
    init_philosoph(&philosophs[3], 4, 3, 4,1);
    init_philosoph(&philosophs[4], 5, 4, 0,3);
    for (int i = 0; i < 5; i++) {
        args[i].philosoph = &philosophs[i];
        args[i].table = &table;
    }
    for (int i = 0; i < 5; i++) {
        pthread_create(&threads[i], NULL, dining, &args[i]);
    }
    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }
    
    
    return 0;
}
