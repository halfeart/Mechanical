#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <conio.h>
#include <ctype.h>


#define	atom_V	15.85e-30 
#define	pi		3.1415926
#define nclusterMax	2000
#define LC		3.165
#define boxX	150
#define boxY	150
#define boxZ	150
double boxV;//#define boxV	(boxX*boxY*boxZ*LC*LC*LC*1.0e-30)//(pow(150*3.165,3)*1.0e-30)
#define frames	2147483647

double V;
double a;

FILE *p;
FILE *r;



int 	Vclusters[nclusterMax]={0};


int		Iclusters[nclusterMax]={0};





double	diameter[nclusterMax]={0};

double 	alpha_V[nclusterMax];
double 	alpha_I[nclusterMax];
double  NV[nclusterMax];
double  NI[nclusterMax]; //in m-3





double	N_to_R(int n)
{
	return	pow(3.0*n*atom_V/4.0/pi,1.0/3.0);//meter
}

double	R_to_alphaV(double d)
{
	return	1.0-exp(-pow(d/4.0e-9,3.0/2.0));
}

double	R_to_alphaI(double d)
{
	return	1.0-exp(-pow(d/4.0e-9,3.3));
}

void dose()
{	puts("//////////////////injection DOSE/////////////////");
    
    int i=0,k=0;char temp[100];

	char key0[100]="injection";
    char key1[100]="GRID_MIN\0";
    char key2[100]="GRID_MAX\0";
    char key3[100]="a0\0";
    
	int s1,s2,s3;
  
    
    while(fscanf(p,"%s",temp)!=EOF)
    {	
        if(strcmp(temp,key0)==0)
        {
			k++;
            for(i=0;i<2;i++)fscanf(p,"%s",temp);
            fprintf(r,"%s\n",temp);
        }
        
    }
    printf("%d\n",k);
    
    
}

void parameter()
{	puts("//////////////////BOX INFO/////////////////");
    
    int i;char temp[100];

	char key0[100]="#";
    char key1[100]="GRID_MIN\0";
    char key2[100]="GRID_MAX\0";
    char key3[100]="a0\0";
    
	int s1,s2,s3;
    
    
    for(i=0;i<nclusterMax;i++)
    {
        diameter[i]=2.0*N_to_R(i+1);
		//printf("%e\n",diameter[i]);
        alpha_V[i]=R_to_alphaV(diameter[i]);
        //printf("%e\n",alpha_V[i]);
        alpha_I[i]=R_to_alphaI(diameter[i]);
        //printf("%e\n",alpha_I[i]);
    }
    
    //for(i=0;i<2;i++)fgets(temp,1000,p);
    while(fscanf(p,"%s",temp)!=EOF)
    {	
        if(strcmp(temp,key0)==0){fgets(temp,1000,p);continue;}
        //puts(temp);
        if(strcmp(temp,key1)==0)
        {
			fscanf(p,"%s",temp);
            fscanf(p,"%s",temp);
            s1=atoi(temp);
            fscanf(p,"%s",temp);
            s2=atoi(temp);
            fscanf(p,"%s",temp);
            s3=atoi(temp);
            
          
        }
        if(strcmp(temp,key2)==0)
        {
			fscanf(p,"%s",temp);
			fscanf(p,"%s",temp);
            s1=atoi(temp)-s1;
            fscanf(p,"%s",temp);
            s2=atoi(temp)-s2;
            fscanf(p,"%s",temp);
            s3=atoi(temp)-s3;
            printf("%d %d %d\n",s1,s2,s3);
            printf("%e\n",V=(double)(s1*s2)*(double)(s3));
        }
        if(strcmp(temp,key3)==0)
        {
			fscanf(p,"%s",temp);
			fscanf(p,"%s",temp);
            printf("%f\n",a=atof(temp));
        }
        
    }printf("%e\tm^3\n",boxV=(V*a*a*a*1.0e-30));
    
}


void init()
{
	int i;
    for(i=0;i<nclusterMax;i++)
    {
		Vclusters[i]=0;
		Iclusters[i]=0;
               
    }
}
void density()
    {
    int i;
    for(i=0;i<nclusterMax;i++)
    {
        NV[i]=Vclusters[i]*1.0/boxV;//=V/N
        NI[i]=Iclusters[i]*1.0/boxV;//=I/N
        
    }
    }

double 	delta_t(double a,double N,double d)
{
	return 135.0*a*sqrt(N*d);//pasca
}

int get_clusters()
{
	
    
    char temp[6000];
	int i,j,s,k=0;
    int skip=3;
    char m[2]="\0";
    char t[100]="\0";
	char time[100]="\0";
		double step;
    
	double	MV,MI,MV2,MI2; //two ways of superposition
	double  t1,t2;
  
    for(i=0;i<3;i++)fgets(temp,5000,p); 
    while(fscanf(p,"%d",&step)!=EOF)//while(fgets(temp,1000,p))
    {	
        fprintf(r,"%d\t%d\t",k,step);
        for(i=0;i<1;i++)fscanf(p,"%s",time);
        fprintf(r,"%s\t",time);
        for(i=0;i<6;i++)fscanf(p,"%s",temp);
        fprintf(r,"%s\t",temp);
        for(i=0;i<1;i++)fscanf(p,"%s",temp);
        
        //printf("%d\t",step);
        //puts(time);

		//fprintf(r,"%d\t%s\t%e\n",step,temp,time);
        
        //system("pause");
        //puts(fgets(temp,1000,p));//time

		for(i=0;i<4;i++)fgets(temp,5000,p); 
   
		init();
		
		i=0;s=0;
        //system("cls");
		puts("///////////////pick VAC////////////////////");
		for(j=0;;j++)
		{	
			if(temp[i]=='\r'||temp[i]=='\n')break;
			//printf("S: %d ",s);
			for(;;i++)
			{
				if(temp[i]=='\t'||temp[i]==' '){s++;break;}
				m[0]=temp[i];
				strcat(t,m);
			}
			//puts(t);
			Vclusters[s-1]=atoi(t);
			strcpy(t,"\0");
        
			i++;   
		}
    
		if(s==0)Vclusters[0]=0;
		//printf("%d\n",Vclusters[0]);
		//puts("vac:");

		if(s==0)puts("0");
		for(i=0;i<s;i++)printf("%d\t",Vclusters[i]);
		puts("\n");
    
		fgets(temp,5000,p);
		fgets(temp,5000,p);
        
		s=0;i=0;
		puts("///////////////pick SIA////////////////////");
		for(j=0;;j++)
		{	
			if(temp[i]=='\r'||temp[i]=='\n')break;
			//printf("S: %d ",s);
			for(;;i++)
			{
				if(temp[i]=='\t'||temp[i]==' '){s++;break;}
				m[0]=temp[i];
				strcat(t,m);
			}
			//puts(t);
			Iclusters[s-1]=atoi(t);
			strcpy(t,"\0");
			
			i++;   
		}
     
		if(s==0)Iclusters[0]=0;
		//printf("%d\n",Vclusters[0]);
		for(i=0;i<2;i++)fgets(temp,5000,p);
		//puts("sia:");
    
		if(s==0)puts("0");
		for(i=0;i<s;i++)printf("%d\t",Iclusters[i]);
		puts("\n");
        
        puts("/////////calculate mechanical//////////////");
        
        density();
        MV=MI=MV2=MI2=0;
        //++k;
        
        for(i=0;i<nclusterMax;i++)
		{
			//printf("%e %e %e\n",alpha_I[i],NI[i],diameter[i]);
			MV+=delta_t(alpha_V[i],NV[i],diameter[i]);			
            MI+=delta_t(alpha_I[i],NI[i],diameter[i]);
            MV2+=pow(delta_t(alpha_V[i],NV[i],diameter[i]),2);	
            MI2+=pow(delta_t(alpha_I[i],NI[i],diameter[i]),2);	
            
			//if(k==117)printf("%e\n",MI);
		}printf("k %d \ntime\t%s\ndt_V %e \ndt_I %e\nDT %5.3f %5.3f Mpa ",k,time,MV,MI,t1=(MV+MI)*1.0e-6,t2=sqrt(MV2+MI2)*1.0e-6);
			//printf("dt %e\n",delta_t(alpha_V[20],NV[20],diameter[20]));
		//system("pause");
        fprintf(r,"%5.3f\t%5.3f\n",t1,t2);
        
        puts("\n");
        if(++k>frames)break;
    }
    
    return 0;
    
}
    






int main()
{
	
    r=fopen("mechanical.txt","w");
    fprintf(r,"No.\tsteps\te_T\tc_T\td_t_line\td_t_sqrt\n");
    p=fopen("read_initial.txt","r");
    parameter();
    p=fopen("output.txt","r");
    get_clusters();
    p=fopen("react.txt","r");
    r=fopen("dose.txt","w");
    dose();
    
    fclose(p);
    fclose(r);
	//system("pause");
	return 0;
}
