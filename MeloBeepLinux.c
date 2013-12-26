#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ao/ao.h>
#include <math.h>

#define M_PI 3.14159265358979

const char *progname = "MeloBeepLinux";

typedef struct tone{  //struktura ton, laicky:
  int f, t;           //vyska v hertzich, delka v milisekundach
  float vol;          //hlasitost, od 0 do 1
  float (*func)(float volume, int freq);      //barva, tvar signalu
  char *buffer;       //buffer se vzorky
  int buf_size;       //velikost toho bufferu
}Ttone;

void uvod();
void help();
void vypis(int del, short tecka, short tecka2, short krizek, char ton, int okt,
            short triol, short odmlka, short text, char slovo[50]);
void vypis_lig(int del, short tecka, short tecka2, short triol);
void Play(Ttone tone, ao_device *device, ao_sample_format format);
void ao_end(ao_device *device);

int main(int argc, char **argv)
{
    FILE *fr;
    char *nazev;                    //nazev souboru pro otevreni a prehrani
    /*uvod();
    fgets(nazev,51,stdin);          //fgets kontroluje preteceni, na rozdil od gets
    
    if(nazev[strlen(nazev)-1]=='\n') nazev[strlen(nazev)-1] = '\0';
    //pokud se zada mene nez 50 znaku, fgets precte i '\n', proto je to treba zamenit za koncovou znacku '\0'*/
    if(argc == 1) {       //spusteno bez argumentu
      help();
      return -1;
    }

    nazev = (char *)malloc(sizeof(char)*strlen(argv[1]));     //sizeof(char) je vzdy 1, ale pro lepsi citelnost
    nazev = argv[1];
    fr = fopen(nazev, "r");
    if(fr==NULL) {
      fprintf(stderr, "Soubor %s se nepodarilo otevrit.\n", nazev);
      return 1;
    } 
      
    int tempo = 0;
    char s_tempo[3];        //string_tempo - tempo se funkci fgets nacte jako retezec

    fgets(s_tempo, 4, fr);            //tempo musi byt na zacatku souboru
    tempo = atoi(s_tempo);            //funkce atoi() prevadi retezec na cislo

    if(tempo<30 || tempo>240) {
      tempo = 120;
      fprintf(stderr, "Zadane tempo neni v rozmezi 30-240 dob/min,proto bylo zvoleno tempo 120 dob/min.\n");
    }
    int ctvrtka = 60000/tempo;  //tempo se rovna poctu ctvrtovych not za minutu(60000ms)                                
    //printf("Tempo je %d, delka ctvrtove noty je %dms.\n", tempo, ctvrtka);
    
    Ttone tone;
    int c = 0, i = 1;                        //c - nacitany znak, i - pocitadlo not
    short _16_ = 0, _32_ = 0, triol = 0;                  //promenne _16_ a _32_ pro noty 16 a 32, triol pro triolku
    short tecka = 0, tecka2 = 0, krizek = 0, pauza = 0, ctvrt_t, odmlka = 0, ligatura = 0, text = 0;
    int del, okt = 0, t_lig = 0;
    char ton; 
    char slovo[100];
    int n = 0;

    //inicializace pro zvuk
    ao_device *device;
    ao_sample_format format;
    int default_driver;
    char *buffer;
    int buf_size;
    int sample;

    ao_initialize();

    /* -- Setup for default driver -- */
    default_driver = ao_default_driver_id();

    memset(&format, 0, sizeof(format));   //struktura se vynuluje
    format.bits = 16;
    format.channels = 2;
    format.rate = 44100;
    format.byte_format = AO_FMT_LITTLE;

    /* -- Open driver -- */
    device = ao_open_live(default_driver, &format, NULL /* no options */);
    if (device == NULL) {
      fprintf(stderr, "Error opening device.\n");
      return 1;
    }

    buf_size = format.bits/8 * format.channels * format.rate * 4;
    buffer = calloc(buf_size, sizeof(char));
    tone.buf_size = buf_size;
    tone.buffer = buffer;

//------------------------------------------------------------------------------
    while(c!=EOF) {
      c = getc(fr);
      switch (c) {
                 case '1':
                      tone.t = ctvrtka*4;
		                  _16_ = 1;
		                  del = 1;
                      break;
                 case '2':
                      tone.t = ctvrtka*2;
                      del = 2;          
                      break;
                 case '3':
                      _32_ = 1;
                      break;
                 case '4':
                      tone.t = ctvrtka;
                      del = 4;
                      break;
                 case '8':
                      tone.t = ctvrtka/2;
                      del = 8;       
                      break;
                 case '\n':
                 case '\t':
                 case ' ':
                 case 13:               //carriage return z Windowsu
                      continue;
                 case EOF:
                      return 0;                
                 default:
                      fprintf(stderr, "TChyba v zapisu noty cislo %d. Chybny znak je >> %i <<\n", i, c);
                      getchar();
                      break;
          }      
      c = getc(fr);
      
      if(_16_==1) {
        if(c=='6') { 
          tone.t = ctvrtka/4;
          _16_ = 0;
          del = 16;
          c = getc(fr);
        }  
      }  
        
      if(_32_==1) {
        if(c=='2') {
          tone.t = ctvrtka/8;     
          _32_ = 0;
          del = 32;
          c = getc(fr);
        }
        else
          fprintf(stderr, "32Chyba v zapisu noty cislo %d.\n", i);
      }
      
      if(c=='.') {
        ctvrt_t = (tone.t/4);
        tone.t = tone.t * 1.5;
        tecka = 1;
        c = getc(fr);
      }
      
      if(tecka==1 && c=='.') {
        tone.t = tone.t + ctvrt_t;
        tecka2 = 1;
        c = getc(fr);        
      }
//-----ne pro ligaturu-----      
      if(!ligatura) {
        if(c=='#') {
          krizek = 1;
          c = getc(fr);
        } 
            
        switch (c) {
             case 'c':
                  tone.f = 131;                    
                  break;
             case 'd':
                  tone.f = 147;                     
                  break;        
             case 'e':
                  tone.f = 165;                      
                  break;        
             case 'f':
                  tone.f = 174;
                  break;      
             case 'g':
                  tone.f = 196;                     
                  break;      
             case 'a':
                  tone.f = 220;                                        
                  break;      
             case 'h':
             case 'b':
                  tone.f = 247;                      
                  break;
             case '-':
                  pauza = 1;
                  ton = '-';
                  break;
             case '\n':
             case 13:               //carriage return z Windowsu
                  continue;
             case EOF:
                  return 0;
             default:
                  fprintf(stderr, "FChyba v zapisu noty cislo %d.\n", i);
                  break;
        }
        ton = c;
        c = getc(fr);
      
        switch (c) {                                  //roznasobovani frekvence do jednotlivych oktav
             case '1':
                  tone.f = tone.f * 2;
                  okt = 1;
                  c = getc(fr);                      
                  break;
             case '2':
                  tone.f = tone.f * 4;
                  okt = 2;
                  c = getc(fr);                     
                  break;
             case '3':
                  tone.f = tone.f * 8;
                  okt = 3;
                  c = getc(fr);                     
                  break;
             case 't':
                  tone.t = (tone.t*2)/3;
                  triol = 1;
                  c = getc(fr);
                  break;
             case '+':
                  t_lig += tone.t;
                  vypis(del,tecka,tecka2,krizek,ton,okt,triol,odmlka,text,slovo);
                  printf("+");
                  tecka = 0, tecka2 = 0, triol = 0;
                  ligatura = 1;
                  continue;
             case '[':
                  n = 0;
                  slovo[0] = 0;                    //smazani predchoziho slova
                  while((c = getc(fr))!=']') {
                    if(n>100) {
                      fprintf(stderr, "\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
                      getchar();
                      ao_end(device);
                      return 1;          
                    }
                    slovo[n] = c;
                    n++;                                       
                  }
                  slovo[n] = '\0';
                  c = 0;
                  text = 1;
                  break;
             case '*':
                  odmlka = 1;
                  break;
             case ' ':
             case '\n':
             case '\t':
             case 13:
             case EOF:
                  break;                         
             default:
                  fprintf(stderr, "OChyba v zapisu noty cislo %d.\n", i);
                  break;
        }      
      }      
//-----/!ligatura-----

//-----------Slovo-----------      
      if(c=='[') {
        n = 0;
        slovo[0] = 0;                    //smazani predchoziho slova
        while((c = getc(fr))!=']') {
          if(n>100) {
            fprintf(stderr, "\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
            ao_end(device);
            return 1;          
          }
          slovo[n] = c;
          n++;                             
        }
        slovo[n] = '\0';
        c = 0;
        text = 1;                   
      }
//----------/Slovo-----------                  
      if(triol==0 && c=='t') {   //triola po oktave
        tone.t = (tone.t*2)/3;
        triol = 1;
        c = getc(fr);
      }
//-----------Slovo-----------      
      if(c=='[') {
        n = 0;
        slovo[0] = 0;                    //smazani predchoziho slova
        while((c = getc(fr))!=']') {
          if(n>100) {
            fprintf(stderr, "\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
            ao_end(device);
            return 1;          
          }
          slovo[n] = c;
          n++;                           
        }
        slovo[n] = '\0';
        c = 0;
        text = 1;                   
      }      
//----------/Slovo-----------                     
      if(ligatura) {
        if(!text)
          vypis_lig(del, tecka, tecka2, triol);      
        t_lig += tone.t;                                                   //celkova delka spojenych not
      }
          
      if(c=='+') {
        if(!t_lig) {
          vypis(del,tecka,tecka2,krizek,ton,okt,triol,odmlka,text,slovo);        //provede jen poprve  
          t_lig += tone.t; 
        }                                                            
        printf("+");
        tecka = 0, tecka2 = 0, triol = 0;
        ligatura = 1;
        continue;           
      }
      
      if(ligatura) {
        tone.t = t_lig;
        printf("\t");
      }
      if(c=='*')
        odmlka = 1;
//-----------Slovo-----------
      c = getc(fr);        
      if(c=='[') {
        n = 0;
        slovo[0] = 0;                    //smazani predchoziho slova
        while((c = getc(fr))!=']') {
          if(n>100) {
            fprintf(stderr, "\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
            ao_end(device);
            return 1;          
          }
          slovo[n] = c;
          n++;                             
        }
        slovo[n] = '\0';
        c = 0;
        text = 1;                             
      }
      else
        fseek(fr,-1,SEEK_CUR);  
//----------/Slovo-----------              
      if(krizek)
        tone.f = tone.f * pow(2, 1/12.0);
      
      if(!ligatura) {
        vypis(del,tecka,tecka2,krizek,ton,okt,triol,odmlka,text,slovo);          //obycejny vypis, bez ligatury
        if(!text)
          printf("\t");
      }

      //Play()

      if(pauza) {
        tone.vol = 0;
        Play(tone, device, format);
      }
        //printf("\tt = %d\n", t);
      else {
        tone.vol = 1;
        if(odmlka) tone.t = 1;
        Play(tone, device, format);
        // ao_close(device);
        //printf("\tf = %d, t = %d\n", tone.f, tone.t);
      }
      
      tecka = 0, tecka2 = 0, triol = 0;
      krizek = 0, okt = 0;
      pauza = 0, odmlka = 0, ligatura = 0, t_lig = 0, text = 0, n = 0, slovo[0] = 0;
      i++;                              
    }
//------------------------------------------------------------------------------    
    printf("\n");
    ao_end(device);
    return 0; 
}

void uvod() {     
    printf("\t\t   ___  ___     _      ______\n");
    printf("\t\t   |  \\/  |    | |     | ___ \\ \n");
    printf("\t\t   | .  . | ___| | ___ | |_/ / ___  ___ _ __\n");
    printf("\t\t   | |\\/| |/ _ \\ |/ _ \\| ___ \\/ _ \\/ _ \\ '_ \\ \n");
    printf("\t\t   | |  | |  __/ | (_) | |_/ /  __/  __/ |_) |\n");
    printf("\t\t   \\_|  |_/\\___|_|\\___/\\____/ \\___|\\___| .__/\n");
    printf("\t\t                                       | |\n");
    printf("\t\t                                       |_|");
    printf("\n\nVitejte v programu MeloBeep.\n");
    printf("Tento program prehrava melodie zapsane v textovych souborech s priponou txt.");
    printf("\nZadejte nazev souboru (soubor musi byt ve stejne slozce s programem) : \n\t");
    printf("...\b\b\b"); 
}

void help() {
    printf("Zadejte soubor pro prehrani jako argument programu.\n");
    printf("Napr. $./%s mozart.txt\n", progname);
}

void vypis(int del,short tecka, short tecka2, short krizek, char ton, int okt, short triol, short odmlka, short text, char slovo[50]) {
  if(!text) {  
    printf("%d", del);
    if(tecka) printf(".");
    if(tecka && tecka2) printf(".");
    if(krizek) printf("#");
    printf("%c", ton);
    if(okt) printf("%d", okt);
    if(triol) printf("t");      
    if(odmlka) printf("*");   
  }
  else
    printf("%s", slovo);    
}

void vypis_lig(int del, short tecka, short tecka2, short triol) {
  printf("%d", del);
  if(tecka) printf(".");
  if(tecka && tecka2) printf(".");
  if(triol) printf("t");  
}
/*funkce Play zapouzdruje prehravani, jeji parametry:
struktura Ttone obsahuje: frekvenci, delku, ukazatel na matematickou funkci
dale je tam zarizeni pro prehravani a struktura formatu zvuku
Popis: matematicka funkce naplni prehravaci buffer hodnotami - bude urcovat
       vysku, hlasitost a barvu tonu
naplneny buffer se preda funkci ao_play, ktera hodnoty posle na zvukovy vystup*/
void Play(Ttone tone, ao_device *device, ao_sample_format format)
{
    int i, length, sample;
    for (i = 0; i < format.rate * 4; i++) {
      sample = (int)(tone.vol * 32768.0 *
      sin(2 * M_PI * tone.f * ((float) i/(format.rate))));    //ctyri sekundy sinusu

      /* Put the same stuff in left and right channel */
      //intove cislo se dava do charoveho bufferu
      tone.buffer[4*i] = tone.buffer[4*i+2] = sample & 0xff;
      tone.buffer[4*i+1] = tone.buffer[4*i+3] = (sample >> 8) & 0xff;
    }
    /* -- Play -- */
    length = (int)(tone.buf_size / (4000 / tone.t));
    printf("\n");
    ao_play(device, tone.buffer, length);
}

void ao_end(ao_device *device) {
    ao_close(device);
    ao_shutdown();
}