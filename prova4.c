#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
/*
#include <gtk/gtk.h>
*/
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

char turnim[31][14];
char turnip[31][14];
char errori[31];

char *mesi[] =
{
"gen",
"feb",
"mar",
"apr",
"mag",
"giu",
"lug",
"ago",
"set",
"ott",
"nov",
"dic"
};

int dmesi[] = {31,28,31,30,31,30,31,31,30,31,30,31};

/*
la prima riga della matrice è in realtà l'ultima della tabII per rispettare l'indice che parte da 0
*/

int tb2[][7] =
{
{1,2,3,4,5,6,0},
{2,3,4,5,6,0,1},
{3,4,5,6,0,1,2},
{4,5,6,0,1,2,3},
{5,6,0,1,2,3,4},
{6,0,1,2,3,4,5},
{0,1,2,3,4,5,6},
};

int tb3[] = {6,0,1,2,3,4,5};

char *giorni[] =
{
"DOM",
"lun",
"mar",
"mer",
"gio",
"ven",
"SAB"
};

int fest[12][4] =
{
{1,6,0,0},
{0,0,0,0},
{0,0,0,0},
{25,0,0,0},
{1,0,0,0},
{2,0,0,0},
{0,0,0,0},
{15,0,0,0},
{0,0,0,0},
{0,0,0,0},
{1,0,0,0},
{6,8,25,26}
};

char gfest[] =
{
"FES"
};

char *mes[] =
{
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
"",
""
};

/* elenco medici
con elenco delle relative abilità

n = notte
g = guardia
R = reparto
a = ambulatorio
p = pomeriggio
t = stroke
u = uva
d = doppler
c = cefalee
v = cerebrovascolare
S = sm
o = oncologia
P = parkinson
N = neuromuscolare
m = emg
e = epilessia
b = tossina


*/

#define TRUE 1
#define FALSE 0

struct activity {
	char id[2];
	int altro;
	char name[4];
	int init;       /* giorno settimana in cui inizia */
	int length;     /* quanti giorni dura */
	char when[2];   /* M = mattina - P = pomeriggio */
	int who;        /* 1 per 1 medico - 2 se 2 medici */
	struct activity *next;
};

struct medico {
	int id;
	char name[15];
	char cap[15];
	int guardie;
	int notti;
	int g_fest;
	int r_fest;
	int n_fest;
	struct medico *next;
	struct desidero *iwant;
};

struct desidero {
	char ferie[40];
	char congre[20];
	char riposi[20];
	char mattine[20];
	char pome[20];
	char notti[20];
	struct des_amb *amb;
};

struct des_amb {
	char amb[1];
	char day[10];
	struct des_amb *next;
};

struct current *medico;

int cursor; /* numero medico corrente */
/*
char *med[13][13] = {
{"Aliprandi","DCUVT"},
{"Balgera","USO"},
{"Basilico","UO"},
{"Bianchi","UOP"},
{"Borelli","DCUP"},
{"Costantino","DCUVT"},
{"Fiumani","CU"},
{"Mantero","SN"},
{"Piamarta","UOPE"},
{"Rigamonti","CUNE"},
{"Sangalli","DCVT"},
{"Stanzani","UNE"},
{"Scaccabarozzi","UVOT"}
};
*/

int get_day(int dnum)
{
int year, month, tb1, tab2;

month = fabs(dnum/100);
year = dnum - month * 100;
switch(year) {
	case 23:
		tb1 = 1;
		break;
	case 24:
		tb1 = 3;
		break;
	case 25:
		tb1 = 4;
		break;
	case 26:
		tb1 = 5;
		break;
	}
switch(month) {
	case 1:
		if ((year / 4) == 0)
			tab2 = tb2[tb1][5];
		else
			tab2 = tb2[tb1][6];
		break;
	case 2:
		if ((year / 4) == 0)
			tab2 = tb2[tb1][1];
		else
			tab2 = tb2[tb1][2];
		break;
	case 3: case 11:
		tab2 = tb2[tb1][2];
		break;
	case 4: case 7:
		tab2 = tb2[tb1][5];
		break;
	case 5:
		tab2 = tb2[tb1][0];
		break;
	case 6:
		tab2 = tb2[tb1][3];
		break;
	case 8:
		tab2 = tb2[tb1][1];
		break;
	case 9:
		tab2 = tb2[tb1][4];
		break;
	case 10: case 12:
		tab2 = tb2[tb1][6];
		break;
	}
return(tb3[tab2]);
}

int check_date(char *data)
{
char mese[4], anno[3];
int x,res;

strncpy(mese, data, 3);
mese[3] = '\0';
anno[0] = data[4];
anno[1] = data[5];
anno[2] = '\0';
res = atoi(anno);
if(res < 22)
	return(0);
else if (res > 26)
	return(0);
for(x = 0;x < 12;x++)
	if(strcmp(mese,mesi[x]) == 0)
		return((x + 1) * 100 + res);
return(0);
}

void build_month(int day, int dnum)
{
int month,x,y;

month = fabs(dnum/100) - 1;
/*
/*    la sottrazione di una unità serve per riportare il mese all'indice che parte da 0
*/
printf("mese = %s\n",mesi[month]);
    for(x = 0;x < 4;x++) {
        if((fest[month][x]) != 0) {
	    y = fest[month][x] - 1;
            mes[y] = gfest;
        };
    };
/*
 "x" viene inizializzato con il primo giorno del mese che viene passato alla funzione con la variabile "day"
*/
    x = day,y = 0;
    do {
        if(x == 7)
        	x = 0;
	if(mes[y] != gfest)
	        mes[y] = giorni[x];
	printf(RED"mes[%d] = %s\n",y,mes[y]);
	++y;
	++x;
    } while(y < dmesi[month]);
    printf(WHITE);
return;
}

int menu()
{
int scelta = 0;

while(scelta == 0) {
	system("clear");
	printf("1 - Controlla elenco medici\n");
	printf("2 - Prepara mese\n");
	printf("3 - Stampa mese\n");
	printf("4 - Prepara turni\n");
	printf("9 - Esci\n\n");
	printf("scegli: ");
	scanf("%d",&scelta);
	};
return(scelta);
}

void elenco_medici(struct medico *doc)
{
char c;
int x = 0;

	do {
		printf("id = %d\n",doc->id);
		printf("name = %s\n",doc->name);
		printf("cap = %s\n",doc->cap);
		doc = doc->next;
		printf("next = %s\n",doc->name);
	} while(++x < 14);
	scanf("%c",&c);
	scanf("%c",&c);
}

struct activity *lavura2()
{
	FILE *fatt;
	char buf[4],bigino[44];
	int c;
	struct activity *att1, *prosim;

	fatt = fopen("attivita.txt","r");
	if(fatt != NULL)
		do {
			fscanf(fatt,"%s",bigino);
		} while(strcmp(bigino,"---") != 0);
	if(fatt == NULL)
        return(NULL);
	else
		prosim = NULL;
		do {
			att1 = malloc(sizeof(struct activity));
			fscanf(fatt,"%s",buf);
			printf("buf = %s\n",buf);
			strcpy(att1->id,buf);
			fscanf(fatt,"%s",buf);
			c = atoi(buf);
			if(c == 0)
				att1->altro = FALSE;
			else
				att1->altro = TRUE;
			fscanf(fatt,"%s",buf);
			strcpy(att1->name,buf);
			fscanf(fatt,"%s",buf);
			c = atoi(buf);
			att1->init = c;
			fscanf(fatt,"%d",&c);
			att1->length = c;
			fscanf(fatt,"%s",buf);
			strcpy(att1->when,buf);
			fscanf(fatt,"%s",buf);
			c = atoi(buf);
			att1->who = c;
			att1->next = prosim;
			fscanf(fatt,"%s",buf);
			prosim = att1;
		} while(strcmp(buf,"*-*") != 0);
	return(att1);
}

struct medico *dutur2()
{
	FILE *fatt;
	char buf[15];
	int c = 13;
	struct medico *doc, *prosim, *first;

	fatt = fopen("medici.txt","r");
	if(fatt == NULL)
        return(NULL);
	else {
		prosim = NULL;
		do {
			doc = malloc(sizeof(struct medico));
			if(c == 13)
				first = doc;
			doc->id = c--;
			fscanf(fatt,"%s",buf);
			strcpy(doc->name,buf);
			fscanf(fatt,"%s",buf);
			strcpy(doc->cap,buf);
			doc->next = prosim;
			fscanf(fatt,"%s",buf);
			prosim = doc;
		} while(strcmp(buf,"*-*") != 0);
		first->next = doc;
	}
	return(doc);
}

struct medico *panta_rei(struct medico *current)
{

	current = current->next;
	cursor = current->id;
	return(current);	
}

int catel(struct medico *current,struct activity *todo, int x, int var)
{
	char *p;
	int c,u;

	c = todo->id[0];
	p = strchr(current->cap,c);
	if(p == NULL)
		return(1);
	else {
		if(x > 0)								/* controlla che non sia il primo giorno del mese */
			if(turnim[x - 1][current->id] == 'n')
				return(1);
		for(u = x;u < x + var;u++) {
			if(todo->when[0] == 'M')
				if(turnim[u][current->id] != ' ')
					return(1);
			if(todo->when[0] == 'P')
				if(turnip[u][current->id] != ' ')
					return(1);
			if(todo->id[0] == 'n')
				if((x + 1) < 31)
					if(turnim[x + 1][current->id] != ' ')
						return(1);
		}
	}
	return(0);
}

int check_guardie(struct medico *current)
{
	struct medico *scelto;
	int g;

	scelto = current;
	g = current->guardie;
	while((current = current->next) != NULL)
		if(current->guardie < g)
			return(1);
	return(0);
}

int vardafest(struct activity *todo, int x)
{
	if((strcmp(mes[x],"FES")) == 0)
		if((todo->id[0] == 'r') || (todo->id[0] == 'R'))
			return(-1);
	else
		return(0);
}

void ciapa_chi(int dnum, struct medico *current, struct activity *todo)
{
	char firstday[4],c[2];
	int month,x,z,y,temp,var,num,buf,buf2,result;
	struct medico *first;

	first = current;
	switch(todo->init)
	{
		case 0:
			strcpy(firstday,"lun");
			break;
		case 1:
			strcpy(firstday,"mar");
			break;
		case 2:
			strcpy(firstday,"mer");
			break;
		case 3:
			strcpy(firstday,"gio");
			break;
		case 4:
			strcpy(firstday,"ven");
			break;
	}
	month = fabs(dnum/100);
	var = num = 0;
/*	printf("month = %d\n",month); */
	for(x = 0;x < dmesi[month - 1];x++) {
		if(var == 0) {
   			result = strcmp(firstday,mes[x]);             /* cerca il primo giorno dell'attività */
   			if(todo->init == -1)                          /* se l'attività è presente sempre e non inizia in un giorno specifico */
   				result = 0;
			if(todo->id[0] == 'a') {
				if((strcmp(mes[x],"SAB")) == 0)
					result = 1;
				else if((strcmp(mes[x],"DOM")) == 0)
					result = 1;
				else if((strcmp(mes[x],"FES")) == 0)
					result = 1;
			}
			if(result == 0) {
				num = todo->who;							/* numero di medici per una attività */
				var = todo->length; 							/* imposta la lunghezza del periodo dell'attività */
				if((x + todo->length) > dmesi[month - 1])                        /* controllo per l'ultimo giorno del mese */
					var = dmesi[month - 1] - x;
				buf = var;
				buf2 = x;
			}	
		}
		if(num > 0) {
			z = 0;
			do {
				result = catel(current,todo,x,var);          /*cerca il medico adatto e controlla se è libero*/
				if(result == 1) {
					current = current->next;
					++z;
				}
				if(z > 28) {
					errori[x] ==todo->id[0];
					break;
				}
			} while(result == 1);
		}
		if(result == 0) {
			result = vardafest(todo,x);
			if(result == -1)
				++x;
			else {
				if((strcmp(todo->when,"P")) != 0) {
					turnim[x][current->id] = todo->id[0];
					if(todo->altro == FALSE)            /* FALSE se pome deve essere libero */
						turnip[x][current->id] = '*';
				}
				else {
					turnip[x][current->id] = todo->id[0];
					if(todo->altro == FALSE)            /* FALSE se matt deve essere libera */
						turnim[x][current->id] = '*';
				}
				if(x < dmesi[month - 1]) {
					if(todo->id[0] == 'n') {
						turnim[x + 1][current->id] = 's';
						turnip[x + 1][current->id] = '*';	
					}
				}
			}
			if(--var == 0) {
				if(--num == 0)
					current = current->next;
				else {
					var = buf;
					if(buf2 > 0)
						x = --buf2;
				}
			}
		}
	}
}

void main(int argc, char *argv[])
{
char data[15],risp[2],c; 
int x, y, dnum, day1, days, item;
struct medico *current, *first;
struct activity *todo;
FILE *fatt;
/*
GtkWidget *window;

gtk_init(&argc, &argv);
window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
gtk_widget_show(window);
g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_main_quit), NULL);  
gtk_main();
*/
first = current = dutur2();
while((item = menu()) != 9) {
	if(item == 1)
		elenco_medici(current);
	else if(item == 2) {
		dnum = 0;
		while(dnum == 0) {
			printf("immetti mese e anno, es <gen-23> ");
			scanf("%s",data);
			dnum = check_date(data);
			if (dnum == 0)
				printf("formato data errato\n");
		};
		printf("dnum = %d\n",dnum);
		day1 = get_day(dnum);
		printf("il primo giorno del mese selezionato è = %s\n",giorni[day1]);
		build_month(day1,dnum);
/*
		printf("tutto OK\n");
		scanf("%s",risp);
*/
	}
	else if(item == 3) {
		for(x = 0;x < 31;x++)
			printf("%d %s\n",x + 1,mes[x]);
		scanf("%c",&c);
		scanf("%c",&c);
	}
	else if(item == 4) {
		fatt = fopen("attivita.txt","r");
		if(fatt == NULL)
        	break;
		else
			todo = lavura2(fatt);	
		cursor = 0;
		for(x = 0;x < 31;x++) {
			for(y = 0;y < 14;y++) {
				turnim[x][y] = ' ';
				turnip[x][y] = ' ';
				errori[x] = ' ';
			}
		}
		do {
			current = first;
			printf("todo->name = %s\n",todo->name);
			ciapa_chi(dnum,current,todo);
			todo = todo->next;
			printf("todo->id = %s\n",todo->id);
		} while(todo != NULL);
		printf("\t\t");
		current = first;
		for(y = 0;y < 14;y++) {
			printf("%c %.3s  ",124,current->name);    /* 124 è il codice per il trattino verticale */
			current = current->next;
			cursor = current->id;
		}
		printf("%c\n",124);
		y = fabs(dnum/100);
		days = dmesi[y-1];
		for(x = 0;x < days;x++) {
			if(strcmp(mes[x],"SAB") == 0)
				printf("%s",RED);
			else if(strcmp(mes[x],"DOM") == 0)
				printf("%s",RED);
			else if(strcmp(mes[x],"FES") == 0)
				printf("%s",RED);
			else
				printf("%s",RESET);
			printf("%d\t  %s = ",x+1,mes[x]);
			for(y = 0;y < 14;y++) {
/*				
				if(turnim[x][y] == '\0')
					turnim[x][y] = ' ';
				else if(turnip[x][y] == '\0')
					turnip[x][y] = ' ';
*/
				printf("%c  %c%c  ",124,turnim[x][y],turnip[x][y]);

			}
			printf("%c\n",124);
/*
			if((current = current->next) == NULL)			
				break;			
*/
		}
		scanf("%s",risp);
	}
}
}



