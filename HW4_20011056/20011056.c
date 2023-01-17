#include <stdio.h>
#include <stdlib.h>

typedef struct person{
	int id;
	char isim[20];
	char soy[20];
	int in_deg;
	int pot;
	int pot2;
	int path_num;
}PERSON;

//typedef struct ag{
//
//}AG;
int path_count;
void mat_doldur(int** mat,int size,FILE* fp,PERSON* person_arr);
void mat_yaz(int** mat,int size);
void sifirla(int**mat,int size);
void in_degree(int** mat,int size,PERSON* person_arr, int M);
void in_degreeX(int** mat,int size,PERSON* person_arr, int X);
void initialize(PERSON* person_arr,int* visited,int size);
void dfs(int source, int destination,int* visited,int**mat,PERSON* person_arr,int size);
void baglanti_hesapla(int** mat,int size,int* visited,PERSON* person_arr);
void print_takip(PERSON* person_arr,int size);
void influencer_bul(PERSON* person_arr,int size,int Y);
int main(){
	FILE* fp = fopen("socialNET.txt", "r");
	PERSON* person_arr;
	int size;
    char line[256];
    int* visited;
	int i=0,choice;
	int **mat;
	int M,X,Y;
	printf("M:");scanf("%d",&M);
	printf("X:");scanf("%d",&X);
	printf("Y:");scanf("%d",&Y);
    while (fgets(line, sizeof(line), fp)) {
       i++;
    }
    rewind(fp);
    size=i/2;
    person_arr=(PERSON*)malloc(sizeof(PERSON)*size);
    visited=(int*)malloc(sizeof(int)*size);
    mat=(int**)malloc(sizeof(int*)*size);
    for(i=0;i<size;i++){
    	mat[i]=(int*)malloc(sizeof(int)*size);
    	visited[i]=0;
	}

	initialize(person_arr,visited,size);
	sifirla(mat,size);
	mat_doldur(mat,size,fp,person_arr);
    fclose(fp);

    
    	printf("Normal 0, Detay 1:");
    	scanf("%d",&choice);
    	printf("\n");
    	initialize(person_arr,visited,size);
    	if(choice==0){
    		printf("\n\n--NORMAL MOD--\n\n");
    		in_degree(mat,size,person_arr, M);
    		baglanti_hesapla(mat,size,visited,person_arr);
    		for(i=0;i<size;i++){
    			person_arr[i].in_deg=0;
			}
			in_degreeX(mat,size,person_arr,X);
			influencer_bul(person_arr,size,Y);
		}
		else if(choice==1){
			printf("\n\n--DETAY MOD--\n\n");
			in_degree(mat,size,person_arr, M);
			baglanti_hesapla(mat,size,visited,person_arr);
			printf("\n--M ILE ELEME--\n");
			print_takip(person_arr,size);
			for(i=0;i<size;i++){
    			person_arr[i].in_deg=0;
			}
			in_degreeX(mat,size,person_arr,X);
			printf("\n--INFLUENCER--\n");
			influencer_bul(person_arr,size,Y);
		}
	
	
    return 0;

}

void mat_doldur(int** mat,int size,FILE* fp,PERSON* person_arr){
	int i=0;
	int k=0;
	int x;
	char line[256];
	char* token;




	while (fgets(line, sizeof(line), fp)) {
		if(i%2==0){
			sscanf(line,"%d,%100[^,],%100[^\n]\n",&person_arr[k].id,person_arr[k].isim,person_arr[k].soy);
			//printf("%d, %s, %s\n", person_arr[k].id, person_arr[k].isim, person_arr[k].soy);

		}
		else if(i%2==1){
			token =strtok(line, ",");
   		
   			while( token != NULL ) {

   				if(atoi(token)!=0){
   					x=atoi(token);
   					x--;
   					mat[k][x]=1;
				}
      			token = strtok(NULL, ",");
   			}
   			k++;
		}
       i++;
    }
}

void mat_yaz(int** mat,int size){
	int i,j;
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			printf("%3d",mat[i][j]);
		}
		printf("\n");
	}
}

void sifirla(int**mat,int size){
	int i,j;

	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			mat[i][j]=0;
		}
	}

}

void in_degree(int** mat,int size,PERSON* person_arr, int M){
	int i,j,k,i1;
    for (j=0; j < size; j++){
    	for(i =0; i < size; i++){
            if (person_arr[i].pot == 1){
                person_arr[j].in_deg += mat[i][j];
            }
    	}
    }

    for (i = 0; i <size;i++){
        if (person_arr[i].in_deg < M){
            person_arr[i].pot = 0;
            for ( k = 0; k < size; k++){
                mat[i][k] = 0;                  
            }

            for ( k = 0; k < size; k++){                    
                for( i1=0; i1 <size; i1++){
                    if (i1 == i){
                        mat[k][i1] = 0;
                       
                    }
                }
            }

        }
    }
}

void initialize(PERSON* person_arr,int* visited,int size){
	int i;
	for(i=0;i<size;i++){
		person_arr[i].in_deg=0;
		person_arr[i].pot=1;
		person_arr[i].path_num=0;
		person_arr[i].pot2=1;
	}
}



void print_takip(PERSON* person_arr,int size){
	int i;
	for(i=0;i<size;i++){
        if (person_arr[i].pot ==1)
            printf("%d. %s,%s %d adet takipcisi var\n\n",i+1,person_arr[i].isim,person_arr[i].soy,person_arr[i].in_deg);
	}
	printf("\n");
}

void baglanti_hesapla(int** mat,int size,int* visited,PERSON* person_arr){
	int source,destination;
    for (source = 0; source < size; source++) {
        for (destination = 0; destination < size; destination++) {
            if (source != destination) {
                
                path_count = 0;
                dfs(source, destination,visited,mat,person_arr,size);
                //printf("%d to %d :%d\n",source+1,destination+1 ,path_count);
                person_arr[destination].path_num+=path_count;
            }
        }
    }
	
}
void dfs(int source, int destination,int* visited,int**mat,PERSON* person_arr,int size)
{
	int i;
    visited[source] = 1;

    if (source == destination) {
        path_count++;
    } else {
        
        for (i = 0; i < size; i++) {
            if (mat[source][i] && !visited[i] && person_arr[i].pot!=0) {
                dfs(i, destination,visited,mat,person_arr,size);
            }
        }
    }

    // backtrack
    visited[source] = 0;
}

void influencer_bul(PERSON* person_arr,int size,int Y){
	int i;
	for(i=0;i<size;i++){
		if(person_arr[i].pot2==1 && person_arr[i].path_num>Y){
			printf("%d. %s,%s Influencer\n Baglanti Adedi:%d, takipci:%d\n\n",i+1,person_arr[i].isim,person_arr[i].soy,person_arr[i].path_num,person_arr[i].in_deg);
		}
	}
}

void in_degreeX(int** mat,int size,PERSON* person_arr, int X){
	int i,j,k,i1;
	for (j=0; j < size; j++){
    	for(i =0; i < size; i++){
            if (person_arr[i].pot2 == 1 && person_arr[i].pot==1){
                person_arr[j].in_deg += mat[i][j];
            }
    	}
    }
   for (i = 0; i <size;i++){
        if (person_arr[i].in_deg < X){
            person_arr[i].pot2 = 0;
            for ( k = 0; k < size; k++){
                mat[i][k] = 0;                  
            }

            for ( k = 0; k < size; k++){                    
                for( i1=0; i1 <size; i1++){
                    if (i1 == i){
                        mat[k][i1] = 0;
                        
                    }
                }
            }

        }
    }

}















