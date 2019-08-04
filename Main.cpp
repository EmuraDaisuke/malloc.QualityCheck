


#include <cstdint>
#include <cassert>
#include <atomic>
#include <thread>
#include <array>
#include "./CLog.h"
#include "./Lapse.h"

#if MIMALLOC//[
#include "./mimalloc.h"
#elif TCMALLOC//][
#include "./tcmalloc.h"
#elif JEMALLOC//][
#include "./jemalloc.h"
#endif//]

#if 0
#define CLOG(...)   clog(__VA_ARGS__)
#else
#define CLOG(...)
#endif



constexpr std::size_t bit(int b){ return (1ULL<<b); }
constexpr std::size_t size(int b){ return (b)? bit(b-1):0; }
constexpr std::size_t KiB(std::size_t v){ return (v<<10); }
constexpr std::size_t MiB(std::size_t v){ return (v<<20); }
constexpr std::size_t GiB(std::size_t v){ return (v<<30); }

static constexpr std::size_t N = bit(7);
static constexpr std::size_t S = bit(10);
static constexpr std::size_t T = bit(3);
static constexpr std::size_t B0 = 0;
static constexpr std::size_t B1 = 16+1;



struct Value {
    std::size_t s;
    void* p;
};



class Thread final {
    public:
        using Func = void (*)(Value);
    
    public:
        ~Thread() noexcept
        {
            Wait();
            mb.store(false, std::memory_order_release);
            if (mt.joinable()) mt.join();
        }
        
        Thread()
        :mb(true)
        ,mf(0)
        ,mv{}
        ,mt(std::thread(&Thread::Work, this))
        {}
        
        void Wait() const noexcept
        {
            while (mf.load(std::memory_order_acquire));
        }
        
        void Call(Func f, Value v) noexcept
        {
            Wait();
            mv.store(v, std::memory_order_release);
            mf.store(reinterpret_cast<std::size_t>(f), std::memory_order_release);
        }
    
    private:
        void Work() noexcept
        {
            while (mb.load(std::memory_order_acquire)){
                auto f = reinterpret_cast<Func>(mf.load(std::memory_order_acquire));
                if (f){
                    f(mv.load(std::memory_order_acquire));
                    mf.store(0, std::memory_order_release);
                }
            }
        }
    
    
    private:
        std::atomic_bool mb;
        std::atomic_size_t mf;
        std::atomic<Value> mv;
        std::thread mt;
};



void Alloc(Value& rv, std::size_t s)
{
    {   // 
        rv.s = s;
        #if MIMALLOC
        rv.p = mi_malloc(rv.s);
        #elif TCMALLOC
        rv.p = tc_malloc(rv.s);
        #elif JEMALLOC
        rv.p = je_malloc(rv.s);
        #else
        rv.p = malloc(rv.s);
        #endif
        CLOG("Alloc", rv.p, rv.s);
    }
    
    #if CHECK
    {   // 
        auto c = reinterpret_cast<uint8_t>(rv.p);
        std::memset(rv.p, c, rv.s);
    }
    #endif
}



void Free(Value& rv)
{
    #if CHECK
    {   // 
        auto s = rv.s;
        auto p = static_cast<uint8_t*>(rv.p);
        auto c = reinterpret_cast<uint8_t>(p);
        bool b = true;
        for (; s; --s, ++p) b &= (c == *p);
        assert(b);
    }
    #endif
    
    {   // 
        #if MIMALLOC
        mi_free(rv.p);
        #elif TCMALLOC
        tc_free(rv.p);
        #elif JEMALLOC
        je_free(rv.p);
        #else
        free(rv.p);
        #endif
        CLOG("Free ", rv.p, rv.s);
    }
}



void testA(std::size_t s)
{
    std::array<Value, S> av;
//  Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        for (auto& v : av){ Alloc(v, s); Free(v); }
    }
}



void testB(std::size_t s)
{
    std::array<Value, S> av;
//  Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        for (auto& v : av) Alloc(v, s);
        for (auto& v : av) Free(v);
    }
}



void testC(std::size_t s)
{
    std::array<Value, S> av;
//  Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        for (auto& v : av) Alloc(v, s);
        for (auto& iv = av.rbegin(); iv != av.rend(); ++iv) Free(*iv);
    }
}



void testD(std::size_t s)
{
    std::array<Value, S> av;
//  Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        bool e;
        for (auto& v : av) Alloc(v, s);
        e = true;   for (auto& v : av){ if (e) Free(v); e = !e; }
        e = false;  for (auto& v : av){ if (e) Free(v); e = !e; }
    }
}



void testE(std::size_t s)
{
    std::array<Value, S> av;
//  Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        bool e;
        for (auto& v : av) Alloc(v, s);
        e = true;   for (auto& v : av){ if (e) Free(v); e = !e; }
        e = true;   for (auto& v : av){ if (e) Alloc(v, s); e = !e; }
        e = false;  for (auto& v : av){ if (e) Free(v); e = !e; }
        e = true;   for (auto& v : av){ if (e) Free(v); e = !e; }
    }
}



void testF(std::size_t s)
{
    std::array<Thread, T> at;
    std::size_t mt = at.size()-1;
    Thread::Func f = [](Value v){ Free(v); };
    
    std::array<Value, S> av;
    Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        std::size_t ot = 0;
        for (auto& v : av){
            Alloc(v, s);
            at[ot].Call(f, v);
            ot = ++ot & mt;
        }
    }
    for (auto& t : at) t.Wait();
}



void testG(std::size_t s)
{
    std::array<Thread, T> at;
    std::size_t mt = at.size()-1;
    Thread::Func f = [](Value v){ Free(v); };
    
    std::array<Value, S> av;
    Lapse l(__FUNCTION__, s);
    for (auto n = N; n; --n){
        for (auto& v : av) Alloc(v, s);
        
        std::size_t ot = 0;
        for (auto& v : av){
            at[ot].Call(f, v);
            ot = ++ot & mt;
        }
    }
    for (auto& t : at) t.Wait();
}



void test_A(Value v)
{
    testA(v.s);
}



void test_B(Value v)
{
    testB(v.s);
}



void test_C(Value v)
{
    testC(v.s);
}



void test_D(Value v)
{
    testD(v.s);
}



void test_E(Value v)
{
    testE(v.s);
}



void test_F(Value v)
{
    testF(v.s);
}



void test_G(Value v)
{
    testG(v.s);
}



int main(int argc, char* argv[])
{
    std::array<Thread, T> at;
    Value v;
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        Lapse l("test_A", v.s);
        for (auto& t : at) t.Call(test_A, v);
        for (auto& t : at) t.Wait();
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        Lapse l("test_B", v.s);
        for (auto& t : at) t.Call(test_B, v);
        for (auto& t : at) t.Wait();
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        Lapse l("test_C", v.s);
        for (auto& t : at) t.Call(test_C, v);
        for (auto& t : at) t.Wait();
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        Lapse l("test_D", v.s);
        for (auto& t : at) t.Call(test_D, v);
        for (auto& t : at) t.Wait();
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        Lapse l("test_E", v.s);
        for (auto& t : at) t.Call(test_E, v);
        for (auto& t : at) t.Wait();
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        test_F(v);
    }
    
    for (auto b = B0; b <= B1; ++b){
        v.s = size(b);
        test_G(v);
    }
    
    clog("***");
    return 0;
}
