
📄 README.txt

🔧 Project: User-Defined `ls` Command in C

This project replicates core functionality of the UNIX `ls` command using system-level C programming. It supports various options like:

- `-l` for long listing
- `-a` for showing hidden files
- `-i` for inode numbers
- `-la` or `-al` for combined options

✅ Features

- ✅ Directory traversal using `opendir()` and `readdir()`
- ✅ File metadata extraction using `lstat()`
- ✅ Inode number display with `-i` option
- ✅ Color-coded output:
  - Blue for directories
  - Green for executables
  - Cyan for symbolic links
- ✅ Symbolic link targets displayed using `readlink()`
- ✅ File type detection via `st_mode & S_IFMT`
- ✅ Permission bits for Owner/Group/Others displayed using bit manipulation
- ✅ Hidden files handled using `-a` flag
- ✅ User and group names fetched using `getpwuid()`
- ✅ Formatted timestamps using `localtime()` and `struct tm`
- ✅ Command-line argument parsing for option handling
- ✅ Safe path formatting using `snprintf()`

🧱 Technologies Used

- Language: C
- Platform: Linux
- APIs: POSIX (`dirent.h`, `sys/stat.h`, `pwd.h`)
- Build: gcc

▶️ How to Compile & Run

```bash
gcc ls-l_userdefined.c -o myls
./myls           # default output
./myls -l        # long listing
./myls -a        # all files including hidden
./myls -i        # show inode numbers
./myls -la       # long listing + hidden
```

📁 Output Example (with `-l` flag)

-rw-r--r-- 1 user user  4096 Apr 14 10:00 file.txt
drwxr-xr-x 2 user user  4096 Apr 14 09:50 folder/
lrwxrwxrwx 1 user user     9 Apr 14 11:00 link -> file.txt

🧠 Skills Demonstrated

- System-level programming
- Bitwise operations for permissions
- ANSI escape sequences for color formatting
- Manual implementation of common shell tool behavior
- Directory and file metadata handling
- Use of dynamic memory and string manipulation functions
