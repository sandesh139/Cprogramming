#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pat.h"
#include "bu.h"
#include "stdarg.h"
#include "wchar.h"


#define szCBUF 1023
#define szWBUF 2047
#define CR 0x0D
#define LF 0x0A
#define  NEWLINE     '\n'

typedef wchar_t wchar;



static char *dl = ":\\";
static char *ok = "success";
static char *rp = "recovery point";
static char *okVeritas = "drive-based";


void dbxOut (char *fmt, ...)
{


    char *cr;
    va_list args;
    printf("\n");
    va_start(args, fmt);
    va_end( args);
    if( (cr = strchr(fmt, NEWLINE)) IS NULL )
        printf("\n");
    return;

}
void buLog(){
    int i, j, k, n;
    int d, m, y;
    char *p;
    char c,cs[szCBUF];
    wchar w, ws[szWBUF];
    FILE *fp;
    int check = 0;


    if((fp = fopen(bu.log, "rb")) IS NULL){
        bu.state = buFAIL;
        bu.msg = "Failed to open log";
        dbxOut("buLog :: Unable to open the log %s ", bu.log);
        return;
    }
    if( strstr(bu.log, "Veritas")){
	check = 1;
    } else {
	check =0;
    }
    //dbxOut("Successfully opened log file!! \n \n");
    fread(&w, szWID, ONE, fp);
    i = j = n =0;
    memset(cs, 0x00, szCBUF);
    while((w = fgetwc(fp)) isNOT WEOF)
    {

        if( w IS CR){

            continue;
        }
        else if (w IS LF)
        {

            ++i;;
            wcstombs(cs, ws, szCBUF);
            _strlwr(cs);
            if(((( strstr(cs, rp)) isNOT NULL) AND
                ((p = strstr(cs,dl)) isNOT NULL)AND
                (( strstr( cs,ok )) isNOT NULL ) AND
                ( *(p-1) isNOT symSTAR ))
               OR ((strstr(cs, okVeritas)) isNOT NULL))
            {

                if( check == 1){
			k =0;

		    } else {
			k =*( p-1 ) - smA;
			}

			if(( k >= 0 ) AND ( k <= 25 )){

                if((strstr(cs, ok)) isNOT NULL)
                {
                /* k = *(p-1) -smA;
                if((k>=0) AND (k<= 25))
                    */

                k = 0;
                if((p = strchr(cs, symSLASH)) isNOT NULL)
                    *p =symSPACE;

                if((p = strchr(cs, symSLASH)) isNOT NULL)
                    *p = symSPACE;

                if((sscanf(cs, "%d%d%d", &m, &d, &y)) IS 3){
                    bu.when[k].d = d;
                    bu.when[k].m = m;
                    bu.when[k].y =y;
                    bu.when[k].ymd =100*(100*y + m)+d;
                    printf("Successful Backup on %d %d %d", y,m,d);
                    dbxOut(" 'bulog()' :: Successful Backup on %d %d %d", y,m,d);

                }

                }
			}

            }
            j =ZERO;
            memset(cs , 0x00, szCBUF);
            memset(ws, 0x00,szWBUF);
        }
        else ws[j++] =w;
        }
        fclose(fp);
    return;
}


int main()
    {
     strcpy(bu.log, "Veritas System Recovery.log.txt");
     buLog();
     return (1);
    }
