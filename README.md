# malloc.QualityCheck
~~~
cl -DNDEBUG main.cpp clog.cpp -Ox -EHsc -Fe:Malloc.exe
cl -DNDEBUG -DMIMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:TestMiMalloc.exe mimalloc.lib advapi32.lib -MD -link -LTCG
cl -DNDEBUG -DTCMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:TestTcMalloc.exe libtcmalloc_minimal.lib
cl -DNDEBUG -DJEMALLOC main.cpp clog.cpp -Ox -EHsc -Fe:TestJeMalloc.exe jemalloc.lib -I"jemalloc/include/msvc_compat"
~~~