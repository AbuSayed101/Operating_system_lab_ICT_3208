//Basic Linux command
/*
 pwd
 ls -lah
 cd ~/ICT-3208
 mkdir -p data/output
 touch notes.txt
 cp notes.txt notes.bak
 mv notes.bak archive.txt
 rm archive.txt
 cat notes.txt
 head -n 5 notes.txt
 tail -n 5 notes.txt
 grep -n "error" log.txt find . -name "*.cpp"
 wc -l notes.txt
 chmod +x script.sh
 ps -ef
 kill PID
 df -h
 free -h

*/

/*


example of linux command

mkdir -p ~/ICT-3208/set5/{input,output,backup}
cd ~/ICT-3208/set5

echo "Operating systems laboratory" > input/a.txt
echo "Linux shell practice" > input/b.txt
printf "CPU scheduling\nMemory management\n" > input/c.txt

grep -Rni "Linux" input
wc -l input/*.txt

cp input/a.txt output/a_copy.txt
mv output/a_copy.txt output/report.txt

ls -l output

df -h
free -h
ps -ef | head

tar -czf backup/set5_backup.tar.gz input output
*/

/*
     ******Shell - Scripting Problem******


     

*/