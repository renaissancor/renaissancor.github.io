# 1. Create a new user;
sudo adduser newuser

# 2. List all the currently running processes
ps aux
ps -le
#    Display some top resource consuming processes
top
q #  exit

# 3. Display the characteristics of the CPU and the available memory
#    Display CPU info
lscpu
cat /proc/cpuinfo

#    Display memory info
free -h
cat /proc/meminfo

# 4. Redirect random output read from a system device into two different files
#    Read random bytes from /dev/urandom
head -c 100 /dev/urandom > random_alpha.txt
head -c 100 /dev/urandom > random_omega.txt

# 5. Concatenate the two previous files and display it
cat random_alpha.txt random_omega.txt > random_sigma.txt

# 6. Read the content of the resulting file as hexadecimal values
xxd random_sigma.txt

# 7. Go at the root of the kernel source you downloaded in l2 and use a single command to find all
#    the files in with the word mutex in their name and containing the whole word nest_lock;

find . -type f -name '*mutex*' -exec grep -lw 'nest_lock' {} \;
