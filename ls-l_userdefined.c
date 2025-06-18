#include<stdio.h>       // For standard input/output functions
#include<stdlib.h>      // For memory allocation and exit()
#include<unistd.h>      // For getcwd(), access to POSIX API
#include<dirent.h>      // For directory operations like opendir(), readdir()
#include<sys/types.h>   // For system data types used by stat and dirent
#include<sys/stat.h>    // For file info structures and macros like S_IFDIR
#include<fcntl.h>       // For file control options (not used here)
#include<pwd.h>         // For retrieving user information via getpwuid()
#include<time.h>        // For formatting file timestamps
#include<string.h>      // For string operations like strcmp(), strcpy(), strcat()

char *str[]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"}; // Month names for display
struct passwd * pass;              // Pointer to struct for user info
char fullpath[1792],path[1792];    // Buffers to hold current and full file paths
char *sfpath;                      // Pointer for storing symbolic link target
struct tm * timer;                 // Pointer for storing formatted time
struct stat st;                    // Structure to hold file statistics
struct dirent *rent;               // Pointer for reading directory entries
DIR *dir;                          // Pointer to directory stream
int year,month,hour,sec;           // Unused variables

// Function to print file type symbol based on mode
void PrintFileType(int num)
{
	switch(num)
	{
		case S_IFSOCK: printf("s"); break;  // Socket
		case S_IFLNK:  printf("l"); break;  // Symbolic link
		case S_IFREG:  printf("-"); break;  // Regular file
		case S_IFBLK:  printf("b"); break;  // Block device
		case S_IFDIR:  printf("d"); break;  // Directory
		case S_IFCHR:  printf("c"); break;  // Character device
		case S_IFIFO:  printf("p"); break;  // Named pipe
	}
}

// Function to print rwx-style permission bits for owner, group, others
void PrintOGOInfo(int num)
{
	switch(num)
	{
		case 7: printf("rwx"); break;
		case 6: printf("rw-"); break;
		case 5: printf("r-x"); break;
		case 4: printf("r--"); break;
		case 3: printf("-wx"); break;
		case 2: printf("-w-"); break;
		case 1: printf("--x"); break;
		case 0: printf("---"); break;
	}
}

// Unified function to simulate `ls -l`, `ls -a`, `ls -i`, and color-based output
void Print_ls_l_ls_a(int n,int num,int flag,int r)
{
	getcwd(path,sizeof(path)); // Get current working directory

	if((dir=opendir(path))==NULL) // Try opening directory stream
	{
		perror("opening directory failed\n"); // Print error if failed
		exit(0); // Exit the program
	}

	while((rent=readdir(dir))!=NULL) // Iterate over all entries in directory
	{
		snprintf(fullpath,sizeof(fullpath),"%s/%s",path,rent->d_name); // Build full path

		if(lstat(fullpath,&st)<0) // Get metadata using lstat
		{
			printf("reading information from file structure is failed\n"); // Error if failed
			exit(0); // Exit the program
		}

		if(n) // If -n like behavior (simple listing)
		{
			if((rent->d_name[0]=='.')&&!r) // Skip hidden files unless -a is passed
				continue;

			// Display with color based on type
			if((st.st_mode&S_IFMT)==S_IFDIR)
			{
				printf("\033[0;34m"); // Blue for directories
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if((st.st_mode&S_IFMT)==S_IFLNK)
			{
				printf("\033[0;36m"); // Cyan for links
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else if(strcmp(rent->d_name,"a.out")==0)
			{
				printf("\033[0;32m"); // Green for executables
				printf("%s\n",rent->d_name);
				printf("\033[0m");
			}
			else
			{
				printf("%s\n",rent->d_name); // Default print
			}
			continue; // Skip detailed info
		}

		if((rent->d_name[0]=='.')&&!num) // If not -a, skip hidden
			continue;

		if(flag) // If -i specified
		{
			printf("%ld   ",rent->d_ino); // Print inode number

			// Print file with color formatting
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
				printf("%s\n",rent->d_name); // Default print
			}
			continue; // Done with -i output
		}

		PrintFileType(st.st_mode&S_IFMT); // Print file type symbol
		PrintOGOInfo((st.st_mode>>6)&0x7); // Owner permissions
		PrintOGOInfo((st.st_mode>>3)&0x7); // Group permissions
		PrintOGOInfo((st.st_mode>>0)&0x7); // Others permissions
		printf(" %ld",st.st_nlink); // Link count

		pass=getpwuid(getuid()); // Get current username
		printf(" %s %s",pass->pw_name,pass->pw_name); // Show user and group

		printf(" %5ld",st.st_size); // File size

		timer=localtime(&st.st_atime); // Last access time
		printf(" %s ",str[timer->tm_mon]); // Month
		printf(" %2d",timer->tm_mday); // Day
		printf(" %-2d:%-2d",timer->tm_hour,timer->tm_min); // Hour:Minute

		// Print file name with color depending on file type
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
			strcpy(filepath,path); // Copy current path
			strcat(filepath,"/"); // Add slash
			strcat(filepath,rent->d_name); // Append filename
			sfpath=malloc(size*sizeof(char)); // Allocate buffer for symlink target
			readlink(filepath,sfpath,size+1); // Read symlink target
			printf("\033[0;36m");
			printf(" %s",rent->d_name); // Print name
			printf("\033[0m");
			printf(" ->%s",sfpath); // Print symlink destination
		}
		else
		{
			printf(" %s",rent->d_name); // Regular file
		}

		printf("\n"); // New line after each entry
	}
}

// Main function to parse command line args and call listing logic
int main(int argc,char *argv[])
{
	if(argc==1)
	{
		Print_ls_l_ls_a(1,0,0,0); // No flags: basic ls
	}
	else if(strcmp(argv[1],"-l")==0)
	{
		Print_ls_l_ls_a(0,0,0,0); // ls -l
	}
	else if((strcmp(argv[1],"-al")==0)||(strcmp(argv[1],"-la")==0))
	{
		Print_ls_l_ls_a(0,1,0,0); // ls -al or ls -la
	}
	else if(strcmp(argv[1],"-i")==0)
	{
		Print_ls_l_ls_a(0,1,1,0); // ls -i
	}
}