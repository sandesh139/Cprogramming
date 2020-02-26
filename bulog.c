// #define MAIN

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <wchar.h>
#include <pat.h>
#include "bu.h"

#define     szCBUF      1023
#define     szWBUF      2047

#define       CR        0x0D
#define       LF        0x0A


typedef     wchar_t wchar;

static char *dl = ":\\ ";
static char *ok = "success";
static char *rp = "recovery point";


void logName( int app, char *path )
{
    int     n;
    char   *tmp;


    //  Need to know which application & its working path
    if(( app IS UNKNOWN ) OR ( path IS NULL ))
         return;

    //  If Log filename is already known, do not replace it
    if(( strlen( bu.log )) isNOT ZERO )
         return;


    //  Typically, the log filename is in the folder 'Logs' relative to
    //  the base working folder with name '<app>.log.txt', where '<app>'
    //  is the name of the installed application (BESR or SSR)

    //  If path ends w/slash (or backslash), then just add 'Logs/<app>.log.txt'
    //  otherwise, need to add a slash before 'Logs'  [DMC allows filenames to
    //  contain either forward or back slashes as well as a mix]

    n=strlen( path );
    if(( *( path+n ) IS symSLASH ) OR ( *( path+n ) IS symSLASH ))
        sprintf( gBuf, "%sLogs/%s.log.txt", path,swName[app] );
    else
        sprintf( gBuf, "%s/Logs/%s.log.txt", path,swName[app] );

    tmp = strdup( gBuf );   memset( gBuf,0x00,szBUF );


    //  Set 'tmp' as the log filename only if it can be read
    if(( fileExists( forREADING,tmp )) IS TRUE )
    {
        strcpy( bu.log,tmp );
        bu.inf |= infLOG;
    }

    free( tmp );

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

            ++i;
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


