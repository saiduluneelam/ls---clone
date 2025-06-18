#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pwd.h>
#include<time.h>
#include<string.h>
char *str[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
struct passwd * pass;
char fullpath[1792],path[1792],*sfpath;
struct tm * timer;
struct stat st;
struct dirent *rent;
DIR *dir;
int year,month,hour,sec;
void PrintFileType(int num)
{
	switch(num)
	{
		case S_IFSOCK:printf("s");
			      break;
		case S_IFLNK:printf("l");
			     break;
		case S_IFREG:printf("-");
			     break;
		case S_IFBLK:printf("b");
			     break;
		case S_IFDIR:printf("d");
			     break;
		case S_IFCHR:printf("c");
			     break;
		case S_IFIFO:printf("p");
			     break;
	}
}
void PrintOGOInfo(int num)
{
	switch(num)
	{
		case 7:printf("rwx");
		       break;
		case 6:printf("rw-");
		       break;
		case 5:printf("r-x");
		       break;
		case 4:printf("r--");
		       break;
		case 3:printf("-wx");
		       break;
		case 2:printf("-w-");
		       break;
		case 1:printf("--x");
		       break;
		case 0:printf("---");
		       break;
	}
}
void Print_ls_l_ls_a(int n,int num,int flag,int r)
{
	getcwd(path,sizeof(path));
	if((dir=opendir(path))==NULL)
	{
		perror("opening directory failed\n");
		exit(0);
	}
	while((rent=readdir(dir))!=NULL)
	{
		snprintf(fullpath,sizeof(fullpath),"%s/%s",path,rent->d_name);
		if(lstat(fullpath,&st)<0)
		{
		printf("reading information from file structure is failed\n");
			exit(0);
		}
		if(n)
		{
			if((rent->d_name[0]=='.')&&!r)
			{
				continue;
			}
			if((st.st_mode&S_IFMT)==S_IFDIR)
			{
				printf("\033[0;34m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if((st.st_mode&S_IFMT)==S_IFLNK)
			{
				printf("\033[0;36m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if(strcmp(rent->d_name,"a.out")==0)
			{
				printf("\033[0;32m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else
			{
				printf("%s\n",rent->d_name);
			}
			continue;
		}
		if((rent->d_name[0]=='.')&&!num)
		{
			continue;
		}
		if(flag)
		{
			printf("%ld   ",rent->d_ino);
			if((st.st_mode&S_IFMT)==S_IFDIR)
			{
				printf("\033[0;34m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if((st.st_mode&S_IFMT)==S_IFLNK)
			{
				printf("\033[0;36m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if(strcmp(rent->d_name,"a.out")==0)
			{
				printf("\033[0;32m");
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else
			{
				printf("%s\n",rent->d_name);
			}
			continue;
		}
		PrintFileType(st.st_mode&S_IFMT);
		PrintOGOInfo((st.st_mode>>6)&0x7);
		PrintOGOInfo((st.st_mode>>3)&0x7);
		PrintOGOInfo((st.st_mode>>0)&0x7);
		printf(" %ld",st.st_nlink);
		pass=getpwuid(getuid());
		printf(" %s %s",pass->pw_name,pass->pw_name);
		printf(" %5ld",st.st_size);
		timer=localtime(&st.st_atime);
		printf(" %s ",str[timer->tm_mon]);
		printf(" %2d",timer->tm_mday);		
		printf(" %-2d:%-2d",timer->tm_hour,timer->tm_min);
		if((st.st_mode&S_IFMT)==S_IFDIR)
		{
			printf("\033[0;34m");
			printf(" %s",rent->d_name);
			printf("\033[0m");
		}
		else if(strcmp(rent->d_name,"a.out")==0)
		{
			printf("\033[0;32m");
			printf(" %s\n",rent->d_name);
			printf("\033[0m");
		}
		else if((st.st_mode&S_IFMT)==S_IFLNK)
		{
			int size=st.st_size;
			char filepath[100];
			strcpy(filepath,path);
			strcat(filepath,"/");
			strcat(filepath,rent->d_name);
			sfpath=malloc(size*sizeof(char));
			readlink(filepath,sfpath,size+1);
			printf("\033[0;36m");
			printf(" %s",rent->d_name);
			printf("\033[0m");
			printf(" ->%s",sfpath);
		}
		else
		{
			printf(" %s",rent->d_name);
		}
		printf("\n");
	}
}

int main(int argc,char *argv[])
{
	if(argc==1)
	{
		Print_ls_l_ls_a(1,0,0,0);
	}
	else if(strcmp(argv[1],"-l")==0)
	{
		Print_ls_l_ls_a(0,0,0,0);
	}
	else if((strcmp(argv[1],"-al")==0)||(strcmp(argv[1],"-la")==0))
	{
		Print_ls_l_ls_a(0,1,0,0);
	}
	else if(strcmp(argv[1],"-i")==0)
	{
		Print_ls_l_ls_a(0,1,1,0);
	}
	else if(strcmp(argv[1],"-a")==0)
	{
		Print_ls_l_ls_a(1,0,0,1);
	}
	
}
