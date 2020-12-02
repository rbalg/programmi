#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */

char turnim[30][13];
char turnip[30][13];

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

D = doppler
C = cefalee
U = uva
V = cerebrovascolare
S = sm
O = oncologia
P = parkinson
N = neuromuscolare
T = stroke
E = emg
B = tossina
R = reparto
*/

struct activity {
	char id;
	char name[4];
	int init;       /* giorno settimana in cui inizia */
	int length;     /* quanti giorni dura */
	char when[2];   /* M = mattina - P = pomeriggio */
	int who;        /* 1 per 1 medico - 2 se 2 medici */
	struct activity *next;
};

struct medico {
	int id;
	char name[14];
	char cap[6];
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
	case 19:
		tb1 = 3;
		break;
	case 20:
		tb1 = 5;
		break;
	case 21:
		tb1 = 6;
		break;
	case 22:
		tb1 = 0;
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
if(res < 19)
	return(0);
else if (res > 22)
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

void elenco_medici()
{
/*
FILE *fmed;
char c;
int x,y,z;

fmed = fopen("medici","r");
if(fmed == NULL)
	fmed = fopen("medici","w+");
for(x = 0;x < 14;x++) {
	fscanf(fmed,"%s",med[x]);
	if(med[x] == NULL)
		break;
	else
		fscanf(fmed,"%s",med[x]);
}
for(y = 0;y < x;y++)
	printf("med[%s] = \n",med[y]);
printf("devi modificare l'elenco (s/n)? ");
c = scanf("%c",&c);
*/
}

struct activity *lavura()
{
struct activity *rep,*su,*dop,*cef,*u1,*u2,*cer,*smmer,*sm2;

dop = malloc(sizeof(struct activity));
dop->id = 'D';
strcpy(dop->name,"dop");
dop->init = 4;           /* l'attività inizia il quinto giorno, venerdì, cioè 4 */
dop->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(dop->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
dop->who = 1;            /* per questa attività serve un solo medico */
dop->next = NULL;

cef = malloc(sizeof(struct activity));
cef->id = 'C';
strcpy(cef->name,"cef");
cef->init = 2;           /* l'attività inizia il terzo giorno, mercoledì, cioè 2 */
cef->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(cef->when,"P");   /* l'attività inizia la mattina, per cui "P" e non "M" */
cef->who = 1;            /* per questa attività serve un solo medico */
cef->next = dop;

u1 = malloc(sizeof(struct activity));
u1->id = 'U';
strcpy(u1->name,"uva");
u1->init = 1;           /* l'attività inizia il secondo giorno, martedì, cioè 1 */
u1->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(u1->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
u1->who = 1;            /* per questa attività serve un solo medico */
u1->next = cef;

u2 = malloc(sizeof(struct activity));
u2->id = 'U';
strcpy(u2->name,"uva");
u2->init = 4;           /* l'attività inizia il quinto giorno, venerdì, cioè 4 */
u2->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(u2->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
u2->who = 1;            /* per questa attività serve un solo medico */
u2->next = u1;

cer = malloc(sizeof(struct activity));
cer->id = 'V';
strcpy(cer->name,"cer");
cer->init = 1;           /* l'attività inizia il secondo giorno, martedì, cioè 1 */
cer->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(cer->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
cer->who = 1;            /* per questa attività serve un solo medico */
cer->next = u2;

smmer = malloc(sizeof(struct activity));
smmer->id = 'S';
strcpy(smmer->name,"sm");
smmer->init = 2;           
smmer->length = 1;      
strcpy(smmer->when,"M");   
smmer->who = 1;            
smmer->next = NULL;

sm2 = malloc(sizeof(struct activity));
sm2->id = 'S';
strcpy(sm2->name,"sm");
sm2->init = 2;           
sm2->length = 1;         
strcpy(sm2->when,"P");   
sm2->who = 1;            
sm2->next = smmer;

su = malloc(sizeof(struct activity));
su->id = 'T';
strcpy(su->name,"su");
su->init = 0;           /* l'attività inizia il primo giorno, lunedì, cioè 0 */
su->length = 5;         /* l'attività dura tutta settimana, per cui il valore è 5 */
strcpy(su->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
su->who = 1;            /* per questa attività serve un medico */
su->next = sm2;

rep = malloc(sizeof(struct activity));
rep->id = 'R';
strcpy(rep->name,"rep");
rep->init = 0;           /* l'attività inizia il primo giorno, lunedì, cioè 0 */
rep->length = 5;         /* l'attività dura tutta settimana, per cui il valore è 5 */
strcpy(rep->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
rep->who = 2;            /* per questa attività servono due medici */
rep->next = su;

return(rep);
}
	
struct medico *dutur()
{
struct medico *scacca, *stanzi, *sanga, *riga, *frencis, *mante, *fiumix, *costa, *borel, *bianchi, *basil, *balge, *alippa;
struct desidero *sca, *sta, *san, *rig, *fre, *man, *fiu, *cos, *bor, *bia, *bas, *bal, *ali; 
struct des_amb *stnz, *blgr;
	
scacca = malloc(sizeof(struct medico));
scacca->id = 12;
strcpy(scacca->name,"Scaccabarozzi");
strcpy(scacca->cap,"UVOTR");
scacca->guardie = 21;
scacca->notti = 15;
scacca->g_fest = 5;
scacca->r_fest = 3;
scacca->n_fest = 4;
scacca->next = NULL;
scacca->iwant = sca;

sca = malloc(sizeof(struct desidero));
strcpy(sca->ferie,"");
strcpy(sca->congre,"7-8");
strcpy(sca->riposi,"1-2-3-24");
strcpy(sca->mattine,"");
strcpy(sca->pome,"14");
strcpy(sca->notti,"");
sca->amb = NULL;

stanzi = malloc(sizeof(struct medico));
stanzi->id = 11;
strcpy(stanzi->name,"Stanzani");
strcpy(stanzi->cap,"UNER");
stanzi->guardie = 19;
stanzi->notti = 17;
stanzi->g_fest = 3;
stanzi->r_fest = 5;
stanzi->n_fest = 2;
stanzi->next = scacca;
stanzi->iwant = sta;

sta = malloc(sizeof(struct desidero));
strcpy(sta->ferie,"18-19-20-21-22");
strcpy(sta->congre,"14-15-29");
strcpy(sta->riposi,"");
strcpy(sta->mattine,"4-16");
strcpy(sta->pome,"4-6-12-16-27");
strcpy(sta->notti,"3");
sta->amb = stnz;

stnz = malloc(sizeof(struct des_amb));
strcpy(stnz->amb,"B");
strcpy(stnz->day,"28");
stnz->next = blgr;
	
sanga = malloc(sizeof(struct medico));
sanga->id = 10;
strcpy(sanga->name,"Sangalli");
strcpy(sanga->cap,"DCVTR");
sanga->guardie = 22;
sanga->notti = 19;
sanga->g_fest = 4;
sanga->r_fest = 2;
sanga->n_fest = 3;
sanga->next = stanzi;
sanga->iwant = san;

san = malloc(sizeof(struct desidero));
strcpy(san->ferie,"");
strcpy(san->congre,"");
strcpy(san->riposi,"2-16");
strcpy(san->mattine,"21-22-23");
strcpy(san->pome,"14-21-22-23");
strcpy(san->notti,"");
san->amb = NULL;

riga = malloc(sizeof(struct medico));
riga->id = 9;
strcpy(riga->name,"Rigamonti");
strcpy(riga->cap,"CUNER");
riga->guardie = 21;
riga->notti = 16;
riga->g_fest = 6;
riga->r_fest = 2;
riga->n_fest = 4;
riga->next = sanga;
riga->iwant = rig;

rig = malloc(sizeof(struct desidero));
strcpy(rig->ferie,"11-12-13-14-15");
strcpy(rig->congre,"25-26");
strcpy(rig->riposi,"");
strcpy(rig->mattine,"");
strcpy(rig->pome,"");
strcpy(rig->notti,"2-3-20");
rig->amb = NULL;

frencis = malloc(sizeof(struct medico));
frencis->id = 8;
strcpy(frencis->name,"Piamarta");
strcpy(frencis->cap,"UOPER");
frencis->guardie = 18;
frencis->notti = 17;
frencis->g_fest = 5;
frencis->r_fest = 1;
frencis->n_fest = 5;
frencis->next = riga;
frencis->iwant = fre;

fre = malloc(sizeof(struct desidero));
strcpy(fre->ferie,"25-26-27-28-29");
strcpy(fre->congre,"7-8");
strcpy(fre->riposi,"10-20");
strcpy(fre->mattine,"");
strcpy(fre->pome,"5");
strcpy(fre->notti,"5");
fre->amb = NULL;

mante = malloc(sizeof(struct medico));
mante->id = 7;
strcpy(mante->name,"Mantero");
strcpy(mante->cap,"SNR");
mante->guardie = 24;
mante->notti = 23;
mante->g_fest = 7;
mante->r_fest = 4;
mante->n_fest = 6;
mante->next = frencis;
mante->iwant = man;

man = malloc(sizeof(struct desidero));
strcpy(man->ferie,"11-12-13-14-15");
strcpy(man->congre,"27");
strcpy(man->riposi,"");
strcpy(man->mattine,"");
strcpy(man->pome,"8");
strcpy(man->notti,"8");
man->amb = NULL;

fiumix = malloc(sizeof(struct medico));
fiumix->id = 6;
strcpy(fiumix->name,"Fiumani");
strcpy(fiumix->cap,"CUR");
fiumix->guardie = 21;
fiumix->notti = 18;
fiumix->g_fest = 6;
fiumix->r_fest = 5;
fiumix->n_fest = 3;
fiumix->next = mante;
fiumix->iwant = fiu;

fiu = malloc(sizeof(struct desidero));
strcpy(fiu->ferie,"");
strcpy(fiu->congre,"");
strcpy(fiu->riposi,"2-3");
strcpy(fiu->mattine,"");
strcpy(fiu->pome,"20-26");
strcpy(fiu->notti,"26");
fiu->amb = NULL;

costa = malloc(sizeof(struct medico));
costa->id = 5;
strcpy(costa->name,"Costantino");
strcpy(costa->cap,"DCUVTR");
costa->guardie = 21;
costa->notti = 18;
costa->g_fest = 6;
costa->r_fest = 5;
costa->n_fest = 3;
costa->next = fiumix;
costa->iwant = cos;

cos = malloc(sizeof(struct desidero));
strcpy(cos->ferie,"");
strcpy(cos->congre,"7-8");
strcpy(cos->riposi,"1-9-10-13");
strcpy(cos->mattine,"");
strcpy(cos->pome,"14");
strcpy(cos->notti,"");
cos->amb = NULL;

borel= malloc(sizeof(struct medico));
borel->id = 4;
strcpy(borel->name,"Borelli");
strcpy(borel->cap,"DCUPR");
borel->guardie = 18;
borel->notti = 22;
borel->g_fest = 4;
borel->r_fest = 2;
borel->n_fest = 5;
borel->next = costa;
borel->iwant = bor;

bor = malloc(sizeof(struct desidero));
strcpy(bor->ferie,"11-12");
strcpy(bor->congre,"");
strcpy(bor->riposi,"9-10-23-24");
strcpy(bor->mattine,"");
strcpy(bor->pome,"5");
strcpy(bor->notti,"");
bor->amb = NULL;

bianchi = malloc(sizeof(struct medico));
bianchi->id = 3;
strcpy(bianchi->name,"Bianchi");
strcpy(bianchi->cap,"UOPR");
bianchi->guardie = 24;
bianchi->notti = 14;
bianchi->g_fest = 4;
bianchi->r_fest = 7;
bianchi->n_fest = 3;
bianchi->next = borel;
bianchi->iwant = bia;

bia = malloc(sizeof(struct desidero));
strcpy(bia->ferie,"4-5-6-7-8-11");
strcpy(bia->congre,"");
strcpy(bia->riposi,"");
strcpy(bia->mattine,"");
strcpy(bia->pome,"");
strcpy(bia->notti,"13");
bia->amb = NULL;

basil = malloc(sizeof(struct medico));
basil->id = 2;
strcpy(basil->name,"Basilico");
strcpy(basil->cap,"UOR");
basil->guardie = 23;
basil->notti = 19;
basil->g_fest = 4;
basil->r_fest = 7;
basil->n_fest = 4;
basil->next = bianchi;
basil->iwant = bas;

bas = malloc(sizeof(struct desidero));
strcpy(bas->ferie,"14-15-30");
strcpy(bas->congre,"7-8");
strcpy(bas->riposi,"16-17");
strcpy(bas->mattine,"");
strcpy(bas->pome,"");
strcpy(bas->notti,"");
bas->amb = NULL;

balge= malloc(sizeof(struct medico));
balge->id = 1;
strcpy(balge->name,"Balgera");
strcpy(balge->cap,"USOR");
balge->guardie = 24;
balge->notti = 22;
balge->g_fest = 5;
balge->r_fest = 6;
balge->n_fest = 4;
balge->next = basil;
balge->iwant = bal;

bal = malloc(sizeof(struct desidero));
strcpy(bal->ferie,"");
strcpy(bal->congre,"");
strcpy(bal->riposi,"16-24");
strcpy(bal->mattine,"");
strcpy(bal->pome,"");
strcpy(bal->notti,"");
bal->amb = blgr;

blgr = malloc(sizeof(struct des_amb));
strcpy(blgr->amb,"S");
strcpy(blgr->day,"14-28");
blgr->next = NULL;
	
alippa = malloc(sizeof(struct medico));
alippa->id = 0;
strcpy(alippa->name,"Aliprandi");
strcpy(alippa->cap,"DCUVTR");
alippa->guardie = 22;
alippa->notti = 20;
alippa->g_fest = 3;
alippa->r_fest = 2;
alippa->n_fest = 4;
alippa->next = balge;
alippa->iwant = ali;

ali = malloc(sizeof(struct desidero));
strcpy(ali->ferie,"");
strcpy(ali->congre,"");
strcpy(ali->riposi,"22");
strcpy(ali->mattine,"");
strcpy(ali->pome,"13-14-28");
strcpy(ali->notti,"");
ali->amb = NULL;

return(alippa);
}

struct medico *panta_rei(struct medico *current)
{	
	current = current->next;
	if(current == NULL)
		return(NULL);
	else
		return(current);	
}

struct medico *choose(struct medico *current,char c)
{
	if((strchr(current->cap,c)) != NULL)
		return(current);
	else {
		if((current = panta_rei(current)) != NULL)
			choose(current,c);
		else
			return(NULL);
		
	}
}

struct medico *choose2(struct medico *current,char c)
{
	do {
		if((strchr(current->cap,c)) != NULL)
			return(current);
		else
			current = current->next;
	} while(current != NULL);
}

int varda_se_el_va_ben(int length, char when[2], int x)
{
	int u;

	if(strcmp(when,"M") == 0) {
		for(u = 0;u < length;u++)
			if(turnim[x + u][cursor] != ' ')
				return(1);
	}
	else
		for(u = 0;u < length;u++)
			if(turnip[x + u][cursor] != ' ')
				return(1);
	return(0);
}

void fa_su_sti_turni(int dnum, struct medico *current, struct activity *todo)
{
	int x,y,z,month,cap;
	char firstday[4],c;
	struct medico *first;

	first = current;
	month = fabs(dnum/100);
	if(todo->init == 0)
		strcpy(firstday,"lun");
	else if(todo->init == 4)
		strcpy(firstday,"ven");
	else if(todo->init == 2)
		strcpy(firstday,"mer");
	else if(todo->init == 1)
		strcpy(firstday,"mar");
	for(x = 0;x < dmesi[month];x++) {
		if((strcmp(firstday,mes[x])) == 0) {  /* firstday = giorno inizio */
			if((current = choose2(current,todo->id)) != NULL) {
				printf("current = %s %c\n",current->name,todo->id);
				cursor = current->id;
				while((varda_se_el_va_ben(todo->length,todo->when,x)) == 1) {
					current = panta_rei(current);
					cursor = current->id;
				}
				printf("current = %s %d\n",current->name, cursor);
				for(z = 0;z < todo->length;z++) {
					if(x < dmesi[month]) {
						if((strcmp(todo->when,"M")) == 0)
							turnim[x++][cursor] = todo->id;
						else
							turnip[x++][cursor] = todo->id;
					}
				}	
				current = panta_rei(current);		
			}
			else {
				current = first;
				--x;          /* serve a recuperare il giorno dell'attività */
			}			
		}
	}
}

void main()
{
char data[15],risp[2],nam[4],c; 
int x, y, dnum, day1, item;
struct medico *current, *first;
struct activity *todo;

while((item = menu()) != 9) {
	if(item == 1)
		elenco_medici();
	else if(item == 2) {
		dnum = 0;
		while(dnum == 0) {
			printf("immetti mese e anno, es <gen-19> ");
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
		for(x = 1;x < 31;x++)
			printf("%d %s\n",x,mes[x]);
		scanf("%c",&c);
	}
	else if(item == 4) {
		todo = lavura();
		current = dutur();		
		first = current;
		cursor = 0;
		for(x = 0;x < 30;x++) {
			for(y = 0;y < 13;y++) {
				turnim[x][y] = ' ';
				turnip[x][y] = ' ';
			}
		}
		do {
			current = first;
			for(x = 0;x < todo->who;x++)               /* who = numero medici */
				fa_su_sti_turni(dnum,current,todo);
			todo = todo->next;
		} while(todo != NULL);
		printf("\t\t");
		current = first;
		for(y = 0;y < 13;y++) {
			strncpy(nam, current->name, 3);
			nam[3] = '\0';
			printf("%c %s  ",124,nam);    /* 124 è il codice per il trattino verticale */
			current = current->next;
		}
		printf("\n");
		for(x = 0;x < 30;x++) {
			printf("%d\t  %s = ",x+1,mes[x]);
			for(y = 0;y < 13;y++) {
				if(turnim[x][y] == '\0')
					turnim[x][y] = ' ';
				else if(turnip[x][y] == '\0')
					turnip[x][y] = ' ';
				printf("%c  %c%c  ",124,turnim[x][y],turnip[x][y]);
			}
			printf("\n");
/*
			if((current = current->next) == NULL)			
				break;			
*/
		}
		scanf("%s",risp);
	}
}
}



