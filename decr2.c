#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <crypt.h>
int reacrypt(FILE *fi, FILE *fo, char *key)
{
	unsigned char wbuff[128],rbuff[256],*wptr;
	int klen = strlen(key),len,i,flag, end;
	unsigned char *ptr;
	unsigned char *dat,*hash;
	size_t l2;
	//printf("%s",key);
	while((l2 = fread(rbuff, 1, sizeof(rbuff), fi)) > 0) {
		len=0;
		wptr = wbuff;		
		while(len<l2) //Joining
		{
						
			*wptr = rbuff[2*len]*4 + (rbuff[2*len+1]-'0');
			wptr++; len++;
			
		}		
		ptr = wbuff;		
		while(len--)
		{
			*ptr =(~*ptr);
			ptr++;
		}
		
		end = l2/2;
		dat=wbuff;
		
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
			
		fwrite(wbuff, 1, l2/2, fo);
	}	
	return 0;
}
int main(int argc, char const *argv[])
{
	char ipf[32],opf[32]; 
	char *key;
	FILE *ifile, *ofile;
	int flag;
	char salt[]="$5$";
	/*
	unsigned long seed[2];
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
  		key=crypt(getpass("Key"),salt);
		flag=reacrypt(ifile,ofile,key);
		if(flag==0)
		{
			printf("Decryption Succes :)\n");
		}
		
		fclose(ifile);
		fclose(ofile);
	}
	else
		printf("Insufficient Arguments :(\n");
	return 0;
}

