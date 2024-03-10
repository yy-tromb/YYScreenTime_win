## ※I'm Japanese, so my English skills are low .  
## Now,I have already stopped developping this application. So use this [YYScreenTime_win-rs](https://github.com/yy-tromb/YYScreenTime_win-rs)  
[YYScreenTime_win-rs](https://github.com/yy-tromb/YYScreenTime_win-rs) is my app like this app that written in [Rust](https://github.com/rust-lang/rust).  
  
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
