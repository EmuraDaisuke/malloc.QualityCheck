#pragma once



#include <string>



using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using f32 = float;
using f64 = double;



class CLog final {
    public:
        ~CLog() noexcept;
        
        CLog();
        
        void Out() const noexcept;
        
        template <class Head, class... Tail>
        void Log(Head&& head, Tail&&... tail) noexcept
        {
            Put(head); Put();
            return Log(std::forward<Tail>(tail)...);
        }
    
    private:
        void Tail(int s) noexcept;
        char* Tail() const noexcept;
        
        void Put() noexcept;
        void Put(bool v) noexcept;
        void Put(i8 v) noexcept;
        void Put(i16 v) noexcept;
        void Put(i32 v) noexcept;
        void Put(i64 v) noexcept;
        void Put(u8 v) noexcept;
        void Put(u16 v) noexcept;
        void Put(u32 v) noexcept;
        void Put(u64 v) noexcept;
        void Put(f32 v) noexcept;
        void Put(f64 v) noexcept;
        void Put(const char* v) noexcept;
        void Put(const void* v) noexcept;
        void Put(const std::string& v) noexcept;
        
        void Log() noexcept;
    
    
    private:
        char maChar[4096];
        char* mpTail;
};



template <class... Args>
void clog(Args... args) noexcept
{
    CLog v;
    v.Log(args...);
    v.Out();
}
