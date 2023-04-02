## ※I'm Japanese, so my English skills are low .  
## Now,I'm making.  
  
# YYScreenTime_win  
Windows ScreenTime for me  
I'm a beginner of C and winAPI, so GUI of this app may be bad.    
  
## build  
only this: `$ make`  
You have to install "make".  
If you are windows, you can install "make" with winget.  
`$ winget install GnuWin32.Make`  
  
### About Makefile
- `$ make` or `$ make build` : build  
- `$ make clean` : remove *.o *.res
- `$ make msi` : make Wix installer (MSI)  
- `$ make msiclean` : remove *.wixobj *.wixpdb  
- `$ make fullclean` : clean & msiclean  
- `$ make re` : fullclean & msi  
    
I love VSCode,so I build this with MinGW.   
