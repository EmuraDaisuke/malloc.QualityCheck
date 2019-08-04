# malloc.QualityCheck
~~~
cl -DNDEBUG main.cpp clog.cpp -Ox -EHsc -Fe:Malloc.exe
cl -DNDEBUG -DMIMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:MiMalloc.exe mimalloc.lib advapi32.lib -MD -link -LTCG
cl -DNDEBUG -DTCMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:TcMalloc.exe libtcmalloc_minimal.lib
cl -DNDEBUG -DJEMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:JeMalloc.exe jemalloc.lib -I"jemalloc/include/msvc_compat"
~~~

||Stability|Portablility|
|:-:|:-:|:-:|
|malloc|o|o|
|mimalloc|o|x|
|tcmalloc|o|o|
|jemalloc|x|o|

<br>

## mimalloc (rev:70648635c6ce3cd0f67f5f03e90ba39627e89bed)
mimalloc.lib は、ビルドした環境に依存する。  
mimalloc.lib を別環境でリンクしようとした場合、以下のようなエラーとなる。  
~~~
s:\test\mimalloc\src\alloc.c : fatal error C1083: コンパイラの生成した ファイルを開けません。'S:\Test\mimalloc\ide\vs2017\..\..\out\msvc-x64\mimalloc\Release\alloc.asm':No such file or directory
~~~

<br>

## tcmalloc (rev:fe62a0baab87ba3abca12f4a621532bf67c9a7d2)

<br>

## jemalloc (rev:8a94ac25d597e439b05b38c013e4cb2d1169c681)
動作が非常に不安定。  
Segmentation fault を起こす。  
