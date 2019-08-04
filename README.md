# malloc.QualityCheck
~~~
cl -DNDEBUG main.cpp clog.cpp -Ox -EHsc -Fe:Malloc.exe
cl -DNDEBUG -DMIMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:MiMalloc.exe mimalloc.lib advapi32.lib -MD -link -LTCG
cl -DNDEBUG -DTCMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:TcMalloc.exe libtcmalloc_minimal.lib
cl -DNDEBUG -DJEMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:JeMalloc.exe jemalloc.lib -I"jemalloc/include/msvc_compat"
~~~