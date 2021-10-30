find . \( -name '*.c' -o -name '*.h' -o -path './dependencies' -prune \) -type f >> formatter_files.txt
uncrustify -c firmware_style.cfg -F formatter_files.txt --no-backup
rm formatter_files.txt
