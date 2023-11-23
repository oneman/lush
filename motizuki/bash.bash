ls -Q *pdf > ls_pdfs.tx
bash ls_pdfs.bash
python forpdfs.py | bash
./forpdfs.pdf | bash
