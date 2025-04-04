/*Remoção de nó (baseiada na ppp)
*Adicionadas: clearskip(), remove_node().
*/
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

void clearskip();
void remove(char *from, char *to);

void main(void){
	char from[20], to[20], c1[20], c2[20];
	int d
	setup();
	printf("De? ");
	gets(from);
	printf("Para? ");
	gets(to);
	do{
		isflight(from, to);
		route(to);
		if(tos>0)pop(c1, c2, &d)
		clearskip();/*Limpa os flags de salto.*/
		remove_node(c1, c2);/*Remove o nó.*/
		tos = 0;/*reinicializa a pilha de retorno.*/	
	}while(getche()!='q');
}

/*Inicializa o banco de dados de vôos.*/
void setup(void){
	assert_flight("New York", "Chicago",      1000);
	assert_flight("Chicago",  "Denver",       1000);
	assert_flight("New York", "Toronto",       800);
	assert_flight("New York", "Denver",       1900);
	assert_flight("Toronto",  "Calgary",      1500);
	assert_flight("Toronto",  "Los Angeles",  1800);
	assert_flight("Toronto",  "Chicago",       500);
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

/*Dado from, encontre anywhere.*/
int find(char *from, char *anywhere){
	find_pos =0;
	while(find_pos<f_pos){
		if(!strcmp(flight[find_pos].from,from)&&
			!flight[find_pos].skip){
				strcpy(anywhere, flight[find_pos].to);
				flight[find_pos].skip = 1;
				return flight[find_pos].distance;
			}
			find_pos++;
	}
	return 0;
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

void clearskip(){
	for(int t=0;t<f_pos;t++)
		flight[t].skip=0;	
}

void remove_node(char *from, char *to){
	for(int t=0;t<f_pos;t++){
		if(!strcmp(flight[t].from,from)&&
		!strcmp(flight[t].to, to)){
			strcpy(flight[t].from, "");
			return;
		}
	}
}