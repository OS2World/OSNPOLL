/*

 NPOLL.EXE is program for creating a file in the Opus outbound directory
 that will cause calls to be placed.

 You're welcome to use this program and the source code in any way you see
 fit so long as you adhere to the "freindly, legal and free" copyright that
 most of OPUS is involved with.  Please remember that the best way to force
 utility writers to stop releasing code is to abuse their generosity.

 This was <originally> written and compiled with Borland's Turbo C.


 The new Version is OSNPOLL.EXE modified by Steve Lesner @ 141/260.
 It was real simple.  I just used 2 Doscalls.lib file functions and
 shook out some possible OS/2 Errors.  Enjoy and remember to share your
 OS/2 code.  There's not much out there for us OS/2 Fans and we gotta
 make it happen!

*/


#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>


void usage(void);


	int	 i,node[25],net[25];
	char	 *sptr;
	char	 firstchar[25];
	int	 add_count=0;
	char	 *path="\\WWBBS\\MAIL\\Outbound";

		 /* I set this up as the default path to create the poll
		    files.  Keep in mind that if you do not use the
		    -P Option, you will have to change the above path
		    var to your own outbound and recompile with
		    cl /c /AS /Zpe /Zl /G2 /W3 osnpoll.c and link it
		    with doscalls and slibce(p). */

	char	 s[80];

	USHORT	 fp;
	unsigned action;



void main(argc,argv)
char    *argv[];
int     argc;

{
int	send;

/* Process the "-" switches   */
    if (argc < 2)
        usage();
    for (i=1; i<argc; i++){
        sptr=argv[i];
        if(sptr[0] == '-'){
                switch(tolower(sptr[1])){

                        case 'h' :     if (strlen(sptr)>3){
                                        firstchar[add_count]='H';
                                        sscanf(sptr+2,"%d/%d",&net[add_count],&node[add_count]);
                                        add_count++;
                                     }
                                     else printf("\n No address listed, skipping\n");
                                     break;

                        case 'n' :     if (strlen(sptr)>3){
                                        firstchar[add_count]='F';
                                        sscanf(sptr+2,"%d/%d",&net[add_count],&node[add_count]);
                                        add_count++;
                                     }
                                     else printf("\n No address listed, skipping\n");
                                     break;

                        case 'c' :     if (strlen(sptr)>3){
                                        firstchar[add_count]='C';
                                        sscanf(sptr+2,"%d/%d",&net[add_count],&node[add_count]);
                                        add_count++;
                                     }
                                     else printf("\n No address listed, skipping\n");
                                     break;


                        case 'p'  :  path = sptr+2;
                                     break;

                        default   :  cprintf("I don't understand '%s' skipping it.\r\n",sptr);
                                     break;
                }
            }
        }

 /* This is so you can enter either "C:\Opus\Outbound" or C:\Opus\Outbound\" */

        if (path[strlen(path)-1] == '\\')
                path[strlen(path)-1]='\0';



        for (send=0;send<add_count;send++){
            sprintf(s,"%s\\%04X%04X.%cLO", path,net[send],node[send],firstchar[send]);
	    if (DosOpen((char far *) s,
			(USHORT far *) &fp,
			(unsigned far *) &action,
			0l,
			0,
			0x11,
			0x11,
			0L))
	    {
		    cprintf("Can't open FLO file (%s), errno=%d\r\n",s,2);
                    usage();
            }
	    DosClose(fp);
            printf("Poll generated for %d/%d\n",net[send],node[send]);
        }
}


void usage()
{

    printf("nPOLL -[N,H,C]net/node -Ppath\n\n");
    printf("-Nnet/node ...... Mark this as a 'Normal' file for net/node\n");
    printf("-Hnet/node ...... Mark this as a 'Hold for pickup' file for net/node\n");
    printf("-Cnet/node ...... Mark this as a 'Continuous Send (Crash)' file for net/node\n");
    printf("-P ......... Path to your outbound holding area. If you don't \n");
    printf("             specify one, nPOLL will use \\WWBBS\\MAIL\\OUTBOUND.\n\n");
    printf("  You can poll more than one address just by adding more -c/-h/-o\n");
    printf("  address to the command line (up to 15), mixing their tags.\n\n");
        exit(1);
}
