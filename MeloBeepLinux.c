#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ao/ao.h>
void uvod();
void vypis(int del, short tecka, short tecka2, short krizek, char ton, int okt, short triol, short odmlka, short text, char slovo[50]);
void vypis_lig(int del, short tecka, short tecka2, short triol);
int main()
{
    FILE *fr;
    char nazev[50];
    uvod();
    fgets(nazev,51,stdin);          //fgets kontroluje preteceni, na rozdil od gets
    
    if(nazev[strlen(nazev)-1]=='\n') nazev[strlen(nazev)-1] = '\0';
    //pokud se zada mene nez 50 znaku, fgets precte i '\n', proto je to treba zamenit za koncovou znacku '\0'
    
    fr = fopen(nazev, "r");
    if(fr==NULL) {
      printf("\n\tSoubor %s se nepodarilo otevrit.\n", nazev);
      return 1;
    } 
      
    int tempo = 0;
    char s_tempo[3];        //string_tempo - tempo se funkci fgets nacte jako retezec

    fgets(s_tempo, 4, fr);            //tempo musi byt na zacatku souboru
    tempo = atoi(s_tempo);            //funkce atoi() prevadi retezec na cislo

    if(tempo<30 || tempo>240) {
      tempo = 120;
      printf("Zadane tempo neni v rozmezi 30-240 dob/min,proto bylo zvoleno tempo 120 dob/min.");
    }
    int ctvrtka = 60000/tempo;  //tempo se rovna poctu ctvrtovych not za minutu(60000ms)                                
    printf("\n\tTempo je %d, delka ctvrtove noty je %dms.\n", tempo, ctvrtka);
    
    int c = 0, t, f, i = 1;                        //c - nacitany znak, t - delka dane noty, f - frekvence tonu, i - pocitadlo not
    short _16_ = 0, _32_ = 0, triol = 0;                  //promenne _16_ a _32_ pro noty 16 a 32, triol pro triolku
    short tecka = 0, tecka2 = 0, krizek = 0, pauza = 0, ctvrt_t, odmlka = 0, ligatura = 0, text = 0;
    int del, okt = 0, t_lig = 0;
    char ton; 
    char slovo[100];
    int n = 0;
//------------------------------------------------------------------------------
    while(c!=EOF) {
      c = getc(fr);
      switch (c) {
                 case '1':
                      t = ctvrtka*4;
		                  _16_ = 1;
		                  del = 1;
                      break;
                 case '2':
                      t = ctvrtka*2;
                      del = 2;          
                      break;
                 case '3':
                      _32_ = 1;
                      break;
                 case '4':
                      t = ctvrtka;
                      del = 4;
                      break;
                 case '8':
                      t = ctvrtka/2;
                      del = 8;       
                      break;
                 case '\n':
                 case '\t':
                 case ' ':
                 case 13:               //carriage return v Linuxu
                      continue;
                 case EOF:
                      return 0;                
                 default:
                      printf("TChyba v zapisu noty cislo %d. Chybny znak je >> %i <<\n", i, c);
                      getchar();
                      break;
          }      
      c = getc(fr);
      
      if(_16_==1) {
        if(c=='6') { 
          t = ctvrtka/4;
          _16_ = 0;
          del = 16;
          c = getc(fr);
          }  
      }  
        
      if(_32_==1) {
        if(c=='2') {
          t = ctvrtka/8;     
          _32_ = 0;
          del = 32;
          c = getc(fr);
        }
        else
          printf("32Chyba v zapisu noty cislo %d.\n", i);
      }
      
      if(c=='.') {
        ctvrt_t = (t/4);
        t = t * 1.5;
        tecka = 1;
        c = getc(fr);
      }
      
      if(tecka==1 && c=='.') {
        t = t + ctvrt_t;
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
                  f = 131;                    
                  break;
             case 'd':
                  f = 147;                     
                  break;        
             case 'e':
                  f = 165;                      
                  break;        
             case 'f':
                  f = 174;
                  break;      
             case 'g':
                  f = 196;                     
                  break;      
             case 'a':
                  f = 220;                                        
                  break;      
             case 'h':
             case 'b':
                  f = 247;                      
                  break;
             case '-':
                  pauza = 1;
                  ton = '-';
                  break;
             case '\n':
                  continue;
             case EOF:
                  return 0;
             default:
                  printf("FChyba v zapisu noty cislo %d.\n", i);
                  break;
        }
        ton = c;
        c = getc(fr);
      
        switch (c) {                                  //roznasobovani frekvence do jednotlivych oktav
             case '1':
                  f = f * 2;
                  okt = 1;
                  c = getc(fr);                      
                  break;
             case '2':
                  f = f * 4;
                  okt = 2;
                  c = getc(fr);                     
                  break;
             case '3':
                  f = f * 8;
                  okt = 3;
                  c = getc(fr);                     
                  break;
             case 't':
                  t = (t*2)/3;
                  triol = 1;
                  c = getc(fr);
                  break;
             case '+':
                  t_lig += t;
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
                      printf("\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
                      getchar();
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
             case EOF:
                  break;                         
             default:
                  printf("OChyba v zapisu noty cislo %d.\n", i);
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
            printf("\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
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
        t = (t*2)/3;
        triol = 1;
        c = getc(fr);
      }
//-----------Slovo-----------      
      if(c=='[') {
        n = 0;
        slovo[0] = 0;                    //smazani predchoziho slova
        while((c = getc(fr))!=']') {
          if(n>100) {
            printf("\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
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
        t_lig += t;                                                   //celkova delka spojenych not
      }
          
      if(c=='+') {
        if(!t_lig) {
          vypis(del,tecka,tecka2,krizek,ton,okt,triol,odmlka,text,slovo);        //provede jen poprve  
          t_lig += t; 
        }                                                            
        printf("+");
        tecka = 0, tecka2 = 0, triol = 0;
        ligatura = 1;
        continue;           
      }
      
      if(ligatura) {
        t = t_lig;
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
            printf("\nU noty cislo %d byl prekrocen max. pocet znaku slova pro jednu notu - max.100 znaku.", i);
            getchar();
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
        f = f * pow(2, 1/12.0);
      
      if(!ligatura) {
        vypis(del,tecka,tecka2,krizek,ton,okt,triol,odmlka,text,slovo);          //obycejny vypis, bez ligatury
        if(!text)
          printf("\t");
      }
      
      /*if(pauza)
        //Sleep(t);
        printf("\tt = %d\n", t);
      else {
        Beep(f,t);
        if(odmlka)
          Sleep(1);
        printf("\tf = %d, t = %d\n", f, t);
      }*/
      
      tecka = 0, tecka2 = 0, triol = 0;
      krizek = 0, okt = 0;
      pauza = 0, odmlka = 0, ligatura = 0, t_lig = 0, text = 0, n = 0, slovo[0] = 0;
      i++;                              
    }
//------------------------------------------------------------------------------    
    printf("\n");
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
