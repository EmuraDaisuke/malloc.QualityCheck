


#include <cstdio>
#include "./CLog.h"



CLog::~CLog() noexcept							{}

CLog::CLog()									:mpTail(maChar){}

void CLog::Tail(int s) noexcept					{ mpTail += s; }
char* CLog::Tail() const noexcept				{ return mpTail; }

void CLog::Out() const noexcept					{ std::puts(maChar); std::fflush(stdout); }

void CLog::Put() noexcept						{ Tail(std::sprintf(Tail(), " ")); }
void CLog::Put(bool v) noexcept					{ Tail(std::sprintf(Tail(), "%hhd", (v)? 1:0)); }
void CLog::Put(i8 v) noexcept					{ Tail(std::sprintf(Tail(), "%hhd", v)); }
void CLog::Put(i16 v) noexcept					{ Tail(std::sprintf(Tail(), "%hd", v)); }
void CLog::Put(i32 v) noexcept					{ Tail(std::sprintf(Tail(), "%d", v)); }
void CLog::Put(i64 v) noexcept					{ Tail(std::sprintf(Tail(), "%lld", v)); }
void CLog::Put(u8 v) noexcept					{ Tail(std::sprintf(Tail(), "%hhu", v)); }
void CLog::Put(u16 v) noexcept					{ Tail(std::sprintf(Tail(), "%hu", v)); }
void CLog::Put(u32 v) noexcept					{ Tail(std::sprintf(Tail(), "%u", v)); }
void CLog::Put(u64 v) noexcept					{ Tail(std::sprintf(Tail(), "%llu", v)); }
void CLog::Put(f32 v) noexcept					{ Tail(std::sprintf(Tail(), "%1.8f", v)); }
void CLog::Put(f64 v) noexcept					{ Tail(std::sprintf(Tail(), "%1.8f", v)); }
void CLog::Put(const char* v) noexcept			{ Tail(std::sprintf(Tail(), "%s", (v)? v:"-nullstr-")); }
void CLog::Put(const void* v) noexcept			{ Tail(std::sprintf(Tail(), "%p", v)); }
void CLog::Put(const std::string& v) noexcept	{ Tail(std::sprintf(Tail(), "%s", v.c_str())); }

void CLog::Log() noexcept						{ Tail()[0] = 0; }
