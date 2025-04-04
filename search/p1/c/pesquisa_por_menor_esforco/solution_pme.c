/*Esta se baseia na ppp com o acréscimo de heurísticas.*/
#include <stdio.h>
#include <string.h>

#define MAX 100

/*Estrutura do banco de dados sobre os vôos.*/
struct FL{
	char from[20];
	char to[20];
	int distance;
	char skip; /*usado no retorno.*/
};

struct Fl flight[MAX];/*matriz se estrutura do db*/

int f_pos = 0;/*número de entradas do db dos vôos*/
int find_pos= 0;/*índice de pesquisa no db dos vôos*/
int tos = 0; /*topo da pilha*/

struct stack{
	char from[20];
	char to[20];
	int dist;
};

struct stack bt_stack[MAX]; /*pilha de retorno.*/

void setup(void), route(char *to);
void assert_flight(char *from, char *to, int dist);
void push(char *from, char *to, int dist);
void pop(char *from, char *to, int dist);
void isflight(char *from , char *to);
int find(char *from, char *anywhere);
int match(char *from, char to);

void main(void){
	char from[20], to[20];
	
	setup();
	printf("De? ");
	gets(from);
	printf("Para? ");
	gets(to);
	
	isflight(from, to);
	route(to);
}

/*Inicializa o banco de dados de vôos.*/
void setup(void){
	assert_flight("New York", "Chicago",     1000);
	assert_flight("Chicago",  "Denver",      1000);
	assert_flight("New York", "Toronto",      800);
	assert_flight("New York", "Denver",      1900);
	assert_flight("Toronto",  "Calgary",     1500);
	assert_flight("Toronto",  "Los Angeles", 1800);
	assert_flight("Toronto",  "Chicago",      500);
	assert_flight("Denver",   "Urbana",       1000);
	assert_flight("Denver",   "Houston",      1500);
	assert_flight("Houston",  "Los Angeles",  1500);
	assert_flight("Denver",   "Los Angeles",  1000);
}

/*Coloca os fatos no banco de dados*/
void assert_flight(char *from, char *to, int dist){
	if(f_pos<MAX){
		strcpy(flight[f_pos].from, from);
		strcpy(flight[f_pos].to, to);
		flight[f_pos].distance= dist;
		flight[f_pos].skip = 0;
		f_pos++;
	}
	else printf("Banco de dados de vôos cheio.\n");
}

/*Mostra a rota e a distância total.*/
void route(char *to){
	int dist, t;
	
	dist = 0;
	t= 0;
	while(t<tos){
		printf("%s para ", bt_stack[t].from);
		dist += bt_stack[t].dist;
		t++;
	}
	printf("%s\n",to);
	printf("A distância é: %d.\n",dist);
}

/*Se há o vôo entre from e to, então devolve a distância do vôo; caso contrário,
*devolve 0.*/
int match(char *from, char *to){
	register int t;
	
	for(t=f_pos; t>-1; t--)
		if(!strcmp(flight[t].from,from)&&
		!strcmp(flight[t].to, to))return flight[t].distance;
		
	return 0;/*não encontrou.*/	
}

/*Dado from, encontre  o anywhere mais próximo.*/
int find(char *from, char *anywhere){
	int pos  = 0;
	int dist = 32000;
	find_pos = 0;
	
	while(find_pos<f_pos){
		if(!strcmp(flight[find_pos].from,from)&&!flight[find_pos].skip){
			if(dist>flight[find_pos].distance){
				dist = flight[find_pos].distance;
				pos  = find_pos;
			}
		}
		find_pos++;
	}
	if(pos){
		strcpy(anywhere, flight[pos].to);
		flight[pos].skip = 1;
	}
}

/*Determina se há uma rota entre from e to*/
void isflight(char *from, char *to){
	int d, dist;
	char anywhere[20];
	
	/*vê se está no destino*/
	if(d=match(from, to)){
		push(from, to);
		return;
	}
	
	/*tenta outra conexâo*/
	if(dist=find(from,anywhere)){
		push(from, to, dist);
		isflight(anywhere, to);
	}
	else if(tos>0){
		/*retorna*/
		pop(from, to,&dist);
		isflight(from, to);
	}
}

/*Rotinas de pilha*/
void push(char *from, char *to, int dist){
	if(tos<MAX){
		strcpy(bt_stack[tos].from, from);
		strcpy(bt_stack[tos].to, to);
		bt_stack[tos].dist=dist;
		tos++;
	}
	else printf("Pilha cheia.\n");
}

void pop(char *from, char *to, int dist){
	if(tos>0){
		tos--;
		strcpy(from, bt_stack[tos].from);
		strcpy(to, bt_stack[tos].to);
		*dist=bt_stack[tos].dist;
	}
	else printf("Pilha vazia.\n");
}