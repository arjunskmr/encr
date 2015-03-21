#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
int reacrypt(FILE *fi, FILE *fo, char *key)
{
	unsigned char rbuff[128],wbuff[256];
	int klen = strlen(key),len,end=0,i;
	unsigned char *ptr;
	unsigned char *dat,*hash;
	size_t l2;
	//printf("%s",key);
	
	
	



	while((l2 = fread(rbuff, 1, sizeof(rbuff), fi)) > 0) {
		
    		end = l2;
		dat=rbuff;
		while(end>0)
		{
			hash=key;
			for(i = 0;i < ((klen<end)?klen:end); ++i)
	    		{
	    			*dat ^= *hash;
				dat++;
				hash++;
	    		}
			end-=klen;
		}



		len=l2;
		ptr = rbuff;		
		while(len--)
		{
			*ptr = (~*ptr);
			ptr++;
		}
		len=0;
		while(len < l2)
		{
			wbuff[2*len] = rbuff[len]/4;
			wbuff[2*len+1] = '0'+(rbuff[len++]%4);
		}
		fwrite(wbuff, 1, 2*l2, fo);
	}	
	return 0;
}
int main(int argc, char const *argv[])
{
	char ipf[32],opf[32];
	char *key;
	FILE *ifile, *ofile;
	char salt[]="$5$";
	/*unsigned long seed[2];
	char salt[] = "$1$1$1$";
  	const char *const seedchars =
    	"./0123456789ABCDEFGHIJKLMNOPQRST"
    	"UVWXYZabcdefghijklmnopqrstuvwxyz";
  	int i;

	
		seed[0] = time(NULL);
  		seed[1] = getpid() ^ (seed[0] >> 14 & 0x30000);
		for (i = 0; i < 8; i++)
    		salt[3+i] = seedchars[(seed[i/5] >> (i%5)*6) & 0x3f];*/
		if(argc >= 2)
	{
		strcpy(ipf,argv[1]);
		if(argc == 3)
			strcpy(opf,argv[2]);
		else
			strcpy(opf,strcat("op_",ipf));
		ifile = fopen(ipf,"r");
		if( ifile == NULL)
		{
			printf("Error reading File :(\n");
  		}
		ofile = fopen(opf,"w");
		if( ofile == NULL)
		{
			printf("Error writing File :(\n");
  		}
		key = crypt(getpass("Key"), salt);
		if(reacrypt(ifile,ofile,key)==0)
		{
			printf("Encryption Succes :)\n");
		}
		fclose(ifile);
		fclose(ofile);
	}
	else
		printf("Insufficient Arguments :(\n");
	return 0;
}

