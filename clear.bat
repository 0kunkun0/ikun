:: 用于清除当前目录下不必要的文件(如MSVC编译时生成的.obj, .ilk, .pdb等文件)
@echo off
del *.ilk
del *.pdb
del *.obj
del *.idb
del *.exp
del *.ilk