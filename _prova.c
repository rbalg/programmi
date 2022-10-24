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
int ore[13];

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
M = emg
E = epilessia
B = tossina
R = reparto
*/

#define TRUE 1
#define FALSE 0

struct activity {
	char id;
	int altro;
	char name[4];
	int init;       /* giorno settimana in cui inizia */
	int length;     /* quanti giorni dura */
	char when[2];   /* M = mattina - P = pomeriggio - N = notte */
	int who;        /* 1 per 1 medico - 2 se 2 medici */
	struct activity *next;
};

struct medico {
	int id;
	char name[14];
	char cap[9];
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

struct activity *lavura2()
{
	FILE *fatt;
	char buf[4];
	int c;
	struct activity *att1;

	fatt = fopen("attivita.txt","r");
	if(fatt == NULL)
        return(NULL);
	att1 = malloc(sizeof(struct activity));
	fscanf(fatt,"%d",&c);
	att1->id = c;
	fscanf(fatt,"%d",&c);
	if(c == 0)
		att1->altro = FALSE;
	else
		att1->altro = TRUE;
	fscanf(fatt,"%s",buf);
	strcpy(att1->name,buf);
	fscanf(fatt,"%d",&c);
	att1->length = c;
	fscanf(fatt,"%s",buf);
	strcpy(att1->when,buf);
	fscanf(fatt,"%d",&c);
	att1->who = c;

	return(att1);
}

struct activity *lavura()
{
struct activity *rep,*su,*dop,*cef,*u1,*u2,*cer,*smmer,*sm2,*pd,*nm,*ep,*emg1,*emg2;
struct activity *emg3,*emg4,*emg5,*emg6,*emg7,*amb,*gual,*guam,*guamm,*guag,*guav;
struct activity *nlun,*nmar,*nmer,*ngio,*nven;

nven = malloc(sizeof(struct activity));              /* notte venerdì */
nven->id = 'n';
nven->altro = FALSE;
strcpy(nven->name,"not");
nven->init = 4;   
nven->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(nven->when,"N");   
nven->who = 1;            /* per questa attività serve un solo medico */
nven->next = NULL;

ngio = malloc(sizeof(struct activity));              /* notte giovedì */
ngio->id = 'n';
ngio->altro = FALSE;
strcpy(ngio->name,"not");
ngio->init = 3;   
ngio->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(ngio->when,"N");   
ngio->who = 1;            /* per questa attività serve un solo medico */
ngio->next = nven;

nmer = malloc(sizeof(struct activity));              /* notte mercoledì */
nmer->id = 'n';
nmer->altro = FALSE;
strcpy(nmer->name,"not");
nmer->init = 2;   
nmer->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(nmer->when,"N");   
nmer->who = 1;            /* per questa attività serve un solo medico */
nmer->next = ngio;

nmar = malloc(sizeof(struct activity));              /* notte martedì */
nmar->id = 'n';
nmar->altro = FALSE;
strcpy(nmar->name,"not");
nmar->init = 1;   
nmar->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(nmar->when,"N");   
nmar->who = 1;            /* per questa attività serve un solo medico */
nmar->next = nmer;

nlun = malloc(sizeof(struct activity));              /* notte lunedì */
nlun->id = 'n';
nlun->altro = FALSE;
strcpy(nlun->name,"not");
nlun->init = 0;   
nlun->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(nlun->when,"N");   /* si svolge di notte */
nlun->who = 1;            /* per questa attività serve un solo medico */
nlun->next = nmar;

guav = malloc(sizeof(struct activity));              /* amb lunedì pome */
guav->id = 'G';
guav->altro = FALSE;
strcpy(guav->name,"gua");
guav->init = 4;   
guav->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(guav->when,"M");   
guav->who = 1;            /* per questa attività serve un solo medico */
guav->next = nlun;	
	
guag = malloc(sizeof(struct activity));              /* amb lunedì pome */
guag->id = 'G';
guag->altro = FALSE;
strcpy(guag->name,"gua");
guag->init = 3;   
guag->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(guag->when,"M");   
guag->who = 1;            /* per questa attività serve un solo medico */
guag->next = guav;	
	
guamm = malloc(sizeof(struct activity));              /* amb lunedì pome */
guamm->id = 'G';
guamm->altro = FALSE;
strcpy(guamm->name,"gua");
guamm->init = 2;   
guamm->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(guamm->when,"M");   
guamm->who = 1;            /* per questa attività serve un solo medico */
guamm->next = guag;	
	
guam = malloc(sizeof(struct activity));              /* amb lunedì pome */
guam->id = 'G';
guam->altro = FALSE;
strcpy(guam->name,"gua");
guam->init = 1;   
guam->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(guam->when,"M");   
guam->who = 1;            /* per questa attività serve un solo medico */
guam->next = guamm;	
	
gual = malloc(sizeof(struct activity));              /* amb lunedì pome */
gual->id = 'G';
gual->altro = FALSE;
strcpy(gual->name,"gua");
gual->init = 0;   
gual->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(gual->when,"M");   
gual->who = 1;            /* per questa attività serve un solo medico */
gual->next = guam;

amb = malloc(sizeof(struct activity));              /* amb lunedì pome */
amb->id = 'A';
amb->altro = FALSE;
strcpy(amb->name,"amb");
amb->init = 0;   
amb->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(amb->when,"P");   
amb->who = 1;            /* per questa attività serve un solo medico */
amb->next = gual;

emg7 = malloc(sizeof(struct activity));            /* emg venerdì */ 
emg7->id = 'M';
emg7->altro = TRUE;
strcpy(emg7->name,"emg");
emg7->init = 4;   
emg7->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg7->when,"M");   
emg7->who = 1;            /* per questa attività serve un solo medico */
emg7->next = amb;

emg6 = malloc(sizeof(struct activity));            /* emg giovedì pome */
emg6->id = 'M';
emg6->altro = TRUE;
strcpy(emg6->name,"emg");
emg6->init = 3;   
emg6->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg6->when,"P");   
emg6->who = 1;            /* per questa attività serve un solo medico */
emg6->next = emg7;

emg5 = malloc(sizeof(struct activity));            /* emg giovedì matt */
emg5->id = 'M';
emg5->altro = TRUE;
strcpy(emg5->name,"emg");
emg5->init = 3;   
emg5->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg5->when,"M");   
emg5->who = 1;            /* per questa attività serve un solo medico */
emg5->next = emg6;

emg4 = malloc(sizeof(struct activity));            /* emg martedì pome */
emg4->id = 'M';
emg4->altro = TRUE;
strcpy(emg4->name,"emg");
emg4->init = 1;   
emg4->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg4->when,"P");   
emg4->who = 1;            /* per questa attività serve un solo medico */
emg4->next = emg5;

emg3 = malloc(sizeof(struct activity));            /* emg martedì matt */
emg3->id = 'M';
emg3->altro = TRUE;
strcpy(emg3->name,"emg");
emg3->init = 1;
emg3->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg3->when,"M");   
emg3->who = 1;            /* per questa attività serve un solo medico */
emg3->next = emg4;

emg2 = malloc(sizeof(struct activity));            /* emg lunedì pome */
emg2->id = 'M';
emg2->altro = TRUE;
strcpy(emg2->name,"emg");
emg2->init = 0;         /* l'attività inizia il primo giorno, lunedì, cioè 0 */
emg2->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg2->when,"P");   
emg2->who = 1;            /* per questa attività serve un solo medico */
emg2->next = emg3;

emg1 = malloc(sizeof(struct activity));            /* emg lunedì matt */
emg1->id = 'M';
emg1->altro = TRUE;
strcpy(emg1->name,"emg");
emg1->init = 0;         /* l'attività inizia il primo giorno, lunedì, cioè 0 */
emg1->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(emg1->when,"M");   
emg1->who = 1;            /* per questa attività serve un solo medico */
emg1->next = emg2;

ep = malloc(sizeof(struct activity));            /* amb epilessia mercoledì pome */
ep->id = 'E';
ep->altro = TRUE;
strcpy(ep->name,"epi");
ep->init = 2;           /* l'attività inizia il terzo giorno, mercoledì, cioè 2 */
ep->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(ep->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
ep->who = 1;            /* per questa attività serve un solo medico */
ep->next = emg1;

nm = malloc(sizeof(struct activity));            /* amb neuromuscolare venerdì pome */
nm->id = 'N';
nm->altro = TRUE;
strcpy(nm->name,"nm");
nm->init = 4;           /* l'attività inizia il quinto giorno, venerdì, cioè 4 */
nm->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(nm->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
nm->who = 1;            /* per questa attività serve un solo medico */
nm->next = ep;

pd = malloc(sizeof(struct activity));            /* amb parkinson giovedì pome */
pd->id = 'P';
pd->altro = TRUE;
strcpy(pd->name,"pd");
pd->init = 3;           /* l'attività inizia il quarto giorno, giovedì, cioè 3 */
pd->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(pd->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
pd->who = 1;            /* per questa attività serve un solo medico */
pd->next = nm;

dop = malloc(sizeof(struct activity));            /* amb doppler venerdì matt */
dop->id = 'D';
dop->altro = TRUE;
strcpy(dop->name,"dop");
dop->init = 4;           /* l'attività inizia il quinto giorno, venerdì, cioè 4 */
dop->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(dop->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
dop->who = 1;            /* per questa attività serve un solo medico */
dop->next = pd;

cef = malloc(sizeof(struct activity));            /* amb cefalee mercoledì pome */
cef->id = 'C';
cef->altro = TRUE;
strcpy(cef->name,"cef");
cef->init = 2;           /* l'attività inizia il terzo giorno, mercoledì, cioè 2 */
cef->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(cef->when,"P");   /* l'attività inizia la mattina, per cui "P" e non "M" */
cef->who = 1;            /* per questa attività serve un solo medico */
cef->next = dop;

u1 = malloc(sizeof(struct activity));            /* amb uva martedì matt */
u1->id = 'U';
u1->altro = TRUE;
strcpy(u1->name,"uva");
u1->init = 1;           /* l'attività inizia il secondo giorno, martedì, cioè 1 */
u1->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(u1->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
u1->who = 1;            /* per questa attività serve un solo medico */
u1->next = cef;

u2 = malloc(sizeof(struct activity));            /* amb uva venerdì pome */
u2->id = 'U';
u2->altro = TRUE;
strcpy(u2->name,"uva");
u2->init = 4;           /* l'attività inizia il quinto giorno, venerdì, cioè 4 */
u2->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(u2->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
u2->who = 1;            /* per questa attività serve un solo medico */
u2->next = u1;

cer = malloc(sizeof(struct activity));            /* amb cerebrovascolare martedì pome */
cer->id = 'V';
cer->altro = TRUE;
strcpy(cer->name,"cer");
cer->init = 1;           /* l'attività inizia il secondo giorno, martedì, cioè 1 */
cer->length = 1;         /* l'attività dura un solo giorno, per cui il valore è 1 */
strcpy(cer->when,"P");   /* l'attività inizia il pomeriggio, per cui "P" e non "M" */
cer->who = 1;            /* per questa attività serve un solo medico */
cer->next = u2;

smmer = malloc(sizeof(struct activity));
smmer->id = 'S';
smmer->altro = TRUE;
strcpy(smmer->name,"sm");
smmer->init = 2;           
smmer->length = 1;      
strcpy(smmer->when,"M");   
smmer->who = 1;            
smmer->next = cer;

sm2 = malloc(sizeof(struct activity));
sm2->id = 'S';
sm2->altro = TRUE;
strcpy(sm2->name,"sm");
sm2->init = 2;           
sm2->length = 1;         
strcpy(sm2->when,"P");   
sm2->who = 1;            
sm2->next = smmer;

su = malloc(sizeof(struct activity));
su->id = 'T';
su->altro = FALSE;
strcpy(su->name,"su");
su->init = 0;           /* l'attività inizia il primo giorno, lunedì, cioè 0 */
su->length = 5;         /* l'attività dura tutta settimana, per cui il valore è 5 */
strcpy(su->when,"M");   /* l'attività inizia la mattina, per cui "M" e non "P" */
su->who = 1;            /* per questa attività serve un medico */
su->next = sm2;

rep = malloc(sizeof(struct activity));
rep->id = 'R';
rep->altro = FALSE;
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
struct medico *med, *med1, *med2, *med3, *med4, *med5, *med6, *med7, *med8, *med9, *med10, *med11, *med12, *med13, *med14;
/*
struct desidero *sca, *sta, *san, *rig, *fre, *man, *fiu, *cos, *bor, *bia, *bas, *bal, *ali; 
struct des_amb *stnz, *blgr;
*/
FILE *fmed;
int x;
char doc[14];

/* alloca le aree di memoria per le strutture dei medici */

	med1 = malloc(sizeof(struct medico));
	med2 = malloc(sizeof(struct medico));
	med3 = malloc(sizeof(struct medico));
	med4 = malloc(sizeof(struct medico));
	med5 = malloc(sizeof(struct medico));
	med6 = malloc(sizeof(struct medico));
	med7 = malloc(sizeof(struct medico));
	med8 = malloc(sizeof(struct medico));
	med9 = malloc(sizeof(struct medico));
	med10 = malloc(sizeof(struct medico));
	med11 = malloc(sizeof(struct medico));
	med12 = malloc(sizeof(struct medico));
	med13 = malloc(sizeof(struct medico));
	med14 = malloc(sizeof(struct medico));

/* apre il file contenente i dati dei medici */

	fmed = fopen("medici.txt","r");
    if(fmed == NULL)
        return(NULL);
    else {
		med = malloc(sizeof(struct medico));
		for(x = 0;x < 14;x++) {
    		fscanf(fmed,"%s",doc);
			printf("%s\n",doc);
			med->id = x;
			strcpy(med->name,doc);
			strcpy(med->cap,"nGAUR");
			med->guardie = 0;
			med->notti = 0;
			med->g_fest = 0;
			med->r_fest = 0;
			med->n_fest = 0;
			med->next = NULL;
			med->iwant = NULL;
			switch(x)
			{
				case 0:
					med1 = med;
					break;
				case 1:
					med2 = med;
					break;
				case 2:
					med3 = med;
					break;
				case 3:
					med4 = med;
					break;
				case 4:
					med5 = med;
					break;
				case 5:
					med6 = med;
					break;
				case 6:
					med7 = med;
					break;
				case 7:
					med8 = med;
					break;
				case 8:
					med9 = med;
					break;
				case 9:
					med10 = med;
					break;
				case 10:
					med11 = med;
					break;
				case 11:
					med12 = med;
					break;
				case 12:
					med13 = med;
					break;
				case 13:
					med14 = med;
					break;
			}	
        }
		med14->next = NULL;
    }
	return(med1);
}

struct medico *panta_rei(struct medico *current)
{	
	current = current->next;
	if(current == NULL)
		return(NULL);
	else
		return(current);	
}

int choose2(struct medico *current,char c)
{
	int x;

	for(x = 0;current->cap[x] != '\0';x++) {
		if(current->cap[x] == c)
			return(0);
	}
	return(1);
}

int varda_se_el_va_ben(int length, char when[2], int x)
{
	int u;

	if(strcmp(when,"M") == 0) {
		for(u = 0;u < length;u++)
			if(turnim[x + u][cursor] != ' ')
				return(1);
	}
	else if(strcmp(when,"N") == 0) {
		if(turnim[x][cursor] != ' ')
			return(1);
		if(turnip[x][cursor] != ' ')
			return(1);
	}
	else
		for(u = 0;u < length;u++)
			if(turnip[x + u][cursor] != ' ')
				return(1);
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

ferie(struct medico *current, int month)
{
struct desidero *holiday;

	printf("%s\n",current->iwant->ferie);
}

int fa_su_sti_turni(int dnum, struct medico *current, struct activity *todo)
{
	int x,y,z,month,cap,result,counter,found,written;
	char firstday[4],c;
	struct medico *first;

	first = current;
	counter = found = written = 0; 
	month = fabs(dnum/100);
	if(todo->init == 0)
		strcpy(firstday,"lun");
	else if(todo->init == 4)
		strcpy(firstday,"ven");
	else if(todo->init == 3)
		strcpy(firstday,"gio");
	else if(todo->init == 2)
		strcpy(firstday,"mer");
	else if(todo->init == 1)
		strcpy(firstday,"mar");
/*
	ferie(current,month);
*/
	for(x = 0;x < dmesi[month];x++) {
		if((strcmp(firstday,mes[x])) == 0) {  /* firstday = giorno inizio */
			++found;
			do {
				cursor = current->id;
				while((choose2(current,todo->id)) == 1) {
					current = current->next;
					if(current == NULL) {
						current = first;
						++counter;
					}
				}
				cursor = current->id;
				result = varda_se_el_va_ben(todo->length,todo->when,x);
				if(counter > 3)
					exit;
				if(result == 0) {
					if(todo->id == 'G') {                                 /* controlla che le guardie non siano troppo vicine */
						if((check_guardie(current)) == 0) {
							++current->guardie;
							for(y = x - 2;y < x + 2;y++) {
								if(turnim[y][cursor] == 'G') {
									result = 1;
									--current->guardie;
								}
							}
							if(turnip[x][cursor] != ' ')
							result = 1;
						}
						else
							result = 1;
					}
					else if(todo->id == 'R') {                                 /* impedisce 2 settimane di giro */
						if(x > 5) {
							for(y = x - 4;y < x + 8;y++)
								if(turnim[y][cursor] == 'R')
									result = 1;
						}
						else {
							for(y = x;y < x + 8;y++)
								if(turnim[y][cursor] == 'R')
									result = 1;
						}
						
					}
					else if(todo->id == 'N') {
						 if(turnip[x][cursor] != ' ')
							result = 1;
					}
					else if(todo->id == 'n') {
						if(x > 4) {
							for(y = x - 4;y < x + 4;y++)
								if(turnip[y][cursor] == 'n')
									result = 1;
						}
						else {
							for(y = x;y < x + 4;y++)
								if(turnip[y][cursor] == 'n')
									result = 1;
						}
					}
				}
				if(result == 1) {
					if((current = panta_rei(current)) == NULL) {
						current = first;
						++counter;
					}
				}
				if(counter > 3)
					break;
			} while(result == 1);     
			for(z = 0;z < todo->length;z++) {
				if(x < dmesi[month]) {
					if((strcmp(todo->when,"M")) == 0) {
						turnim[x][cursor] = todo->id;
						++ore[x];
						if(todo->altro == FALSE)            /* FALSE se pome deve essere libero */
							turnip[x][cursor] = '*';
						++x;
						++written;
					}
					else {
						turnip[x][cursor] = todo->id;
						++ore[x];
						if(todo->altro == FALSE)            /* FALSE se matt deve essere libera */
							turnim[x][cursor] = '*';
						++x;
						++written;
					}
					if(todo->id == 'n') {
						turnim[x][cursor] = 's';
						turnip[x][cursor] = '*';
					}
				}
			}
			if((current = panta_rei(current)) == NULL)
				current = first;
		}
	}
	printf("written = %d found = %d\n",written,found);
	if(written < found)
		return(1);
	else
		return(0);
}

/*

char * ges_file()
{
	FILE *fmed;
	char c,med[14],medici[14][14];
	int x;

	fmed = fopen("medici.txt","r");
    if(fmed == NULL)
        return(NULL);
    else {
        for(x = 0;x < 14;x++) {
        	fscanf(fmed,"%s",med);
			printf("%s\n",med);
            if(strcmp(med,"") == 0)
                break;
			else {
				strcpy(medici[x], med);
			}
        }
    }
	return(medici);
}
*/

int crea_file()
{
	FILE *fmed;
	char c,med[14];
	int x;

	fmed = fopen("medici.txt","w");
    for(x = 0;x < 14;x++) {
		printf("inserisci medico %d ",x);
		scanf("%s",med);
       	fprintf(fmed,"%s\n",med);
    }
	return(1);
}

int menu()
{
int scelta = 0;

while(scelta == 0) {
/*
    system("clear");
*/
    printf("1 - File\n");
    printf("2 - Prepara mese\n");
    printf("3 - Stampa mese\n");
    printf("4 - Prepara turni\n");
    printf("9 - Esci\n\n");
    printf("scegli: ");
    scanf("%d",&scelta);
    };
return(scelta);
}

int menu2()
{
int scelta = 0;

while(scelta == 0) {
    system("clear");
    printf("1 - Carica medici\n");
    printf("2 - Aggiorna medici\n");
    printf("3 - Nuovo elenco medici\n");
    printf("4 - Ferie\n");
    printf("9 - Esci\n\n");
    printf("scegli: ");
    scanf("%d",&scelta);
    };
return(scelta);
}
void main()
{
char data[15],risp[2],nam[4],c,*medici[14][14]; 
int x, y, dnum, day1, item;
struct medico *current, *first;
struct activity *todo;

if((current = dutur()) == NULL)
	EXIT_FAILURE;
while((item = menu()) != 9) {
/*
        while(item = menu2() != 9) {
            if(item == 1) {
                if(x == NULL) {
					printf("file inesistente\n");
					crea_file();
				}
				else {
					printf("il file contiene %d medici\n",x);
					scanf("%d",&x);
					scanf("%d",&x);
				}
			}
            else if(item == 9)
                break;
        }

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
            }
        }
    }
*/
	if(item == 2) {
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
		for(x = 0;x < 31;x++)
			printf("%d %s\n",x + 1,mes[x]);
		scanf("%c",&c);
		scanf("%c",&c);
	}
	else if(item == 4) {
		todo = lavura();
/*
		current = dutur();		
*/
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
				if(fa_su_sti_turni(dnum,current,todo) != 0)
					printf("problemi con %s\n",todo->name);
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



