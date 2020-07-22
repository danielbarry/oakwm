#pragma once

#include <chrono>
#include <ctime>
#include <string>

#define CLASS() std::string(__FILE__).substr(std::string(__FILE__).find_last_of('/') + 1)

#define CHEADER() printf("[\033[1;33m%lli\033[0m] \033[1;35m%s\033[0m->\033[1;34m%s()\033[0m::\033[1;36m%i\033[0m ", LOG::CURRENT_TIME_NANOS(), CLASS().c_str(), __FUNCTION__, __LINE__)
#define PHEADER() ("[" + std::to_string(LOG::CURRENT_TIME_NANOS()) + "] " + CLASS() + "->" + __FUNCTION__ + "()::" + std::to_string(__LINE__) + " ")

#define CLOG(s)     {CHEADER(); printf("%s\n", s);}
#define CLOGB(s,b)  {CHEADER(); printf("%s0x%02X\n", s, b);}
#define CLOGI(s,i)  {CHEADER(); printf("%s%i\n", s, i);}
#define CLOGL(s,l)  {CHEADER(); printf("%s%lli\n", s, l);}
#define CLOGF(s,f)  {CHEADER(); printf("%s%f\n", s, f);}
#define CLOGM(s,m)  {CHEADER(); printf("%s%s\n", s, m);}
#define CWARN(s)    {CHEADER(); printf("\033[1;37;1;41m%s\033[0m\n", s);}
#define CWARNB(s,b) {CHEADER(); printf("\033[1;37;1;41m%s\033[0m0x%02X\n", s, b);}
#define CWARNI(s,i) {CHEADER(); printf("\033[1;37;1;41m%s\033[0m%i\n", s, i);}
#define CWARNL(s,l) {CHEADER(); printf("\033[1;37;1;41m%s\033[0m%lli\n", s, l);}
#define CWARNF(s,f) {CHEADER(); printf("\033[1;37;1;41m%s\033[0m%f\n", s, f);}
#define CWARNM(s,m) {CHEADER(); printf("\033[1;37;1;41m%s\033[0m%s\n", s, m);}

#define PLOG(s)     (PHEADER() + std::string(s)).c_str()
#define PLOGB(s,b)  (PHEADER() + std::string(s) + std::to_string(b)).c_str()
#define PLOGI(s,i)  (PHEADER() + std::string(s) + std::to_string(i)).c_str()
#define PLOGL(s,l)  (PHEADER() + std::string(s) + std::to_string(l)).c_str()
#define PLOGF(s,f)  (PHEADER() + std::string(s) + std::to_string(f)).c_str()
#define PLOGM(s,m)  (PHEADER() + std::string(s) + m).c_str()
#define PWARN(s)    (PHEADER() + "[!!] " + std::string(s)).c_str()
#define PWARNB(s,b) (PHEADER() + "[!!] " + std::string(s) + std::to_string(b)).c_str()
#define PWARNI(s,i) (PHEADER() + "[!!] " + std::string(s) + std::to_string(i)).c_str()
#define PWARNL(s,l) (PHEADER() + "[!!] " + std::string(s) + std::to_string(l)).c_str()
#define PWARNF(s,f) (PHEADER() + "[!!] " + std::string(s) + std::to_string(f)).c_str()
#define PWARNM(s,m) (PHEADER() + "[!!] " + std::string(s) + m).c_str()

#define LOG(s)     {CLOG(s);     LOG::WRITE_DISK_LOG(FILENAME(),PLOG(s));}
#define LOGB(s,b)  {CLOGB(s,b);  LOG::WRITE_DISK_LOG(FILENAME(),PLOGB(s,b));}
#define LOGI(s,i)  {CLOGI(s,i);  LOG::WRITE_DISK_LOG(FILENAME(),PLOGI(s,i));}
#define LOGL(s,l)  {CLOGL(s,l);  LOG::WRITE_DISK_LOG(FILENAME(),PLOGL(s,l));}
#define LOGF(s,f)  {CLOGF(s,f);  LOG::WRITE_DISK_LOG(FILENAME(),PLOGF(s,f));}
#define LOGM(s,m)  {CLOGM(s,m);  LOG::WRITE_DISK_LOG(FILENAME(),PLOGM(s,m));}
#define WARN(s)    {CWARN(s);    LOG::WRITE_DISK_LOG(FILENAME(),PWARN(s));}
#define WARNB(s,b) {CWARNB(s,b); LOG::WRITE_DISK_LOG(FILENAME(),PWARNB(s,b));}
#define WARNI(s,i) {CWARNI(s,i); LOG::WRITE_DISK_LOG(FILENAME(),PWARNI(s,i));}
#define WARNL(s,l) {CWARNL(s,l); LOG::WRITE_DISK_LOG(FILENAME(),PWARNL(s,l));}
#define WARNF(s,f) {CWARNF(s,f); LOG::WRITE_DISK_LOG(FILENAME(),PWARNF(s,f));}
#define WARNM(s,m) {CWARNM(s,m); LOG::WRITE_DISK_LOG(FILENAME(),PWARNM(s,m));}

#define FILENAME() ("log-" + std::to_string(LOG::GET_START_TIME_NANOS()) + ".txt").c_str()

class LOG{
  private:
    static long long START_TIME_NANOS;

  public:
    /**
     * CURRENT_TIME_MILLIS()
     *
     * Get the current wall clock time in milliseconds since epoch.
     *
     * @return Number of milliseconds at the time of the request.
     **/
    static long long CURRENT_TIME_MILLIS(){
      return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    /**
     * CURRENT_TIME_MICROS()
     *
     * Get the current wall clock time in microseconds since epoch.
     *
     * @return Number of microseconds at the time of the request.
     **/
    static long long CURRENT_TIME_MICROS(){
      return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    /**
     * CURRENT_TIME_NANOS()
     *
     * Get the current wall clock time in nanoseconds since epoch.
     *
     * @return Number of nanoseconds at the time of the request.
     **/
    static long long CURRENT_TIME_NANOS(){
      return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }

    /**
     * GET_START_TIME_NANOS()
     *
     * Get the start time of the program in nanoseconds.
     *
     * @return The start time of the program in nanoseconds.
     **/
    static long long GET_START_TIME_NANOS(){
      return START_TIME_NANOS;
    }

    /**
     * WRITE_DISK_LOG()
     *
     * Append a string to a file on the disk in log format.
     *
     * @param name The name of the file to be appended to.
     * @param str The string to be appended to the file.
     **/
    static void WRITE_DISK_LOG(const char* name, const char* str){
      FILE *fout;
      fout = fopen(name, "a");
      if(fout){
        fprintf(fout, "%s\n", str);
        fclose(fout);
      }else{
        WARNM("Unable to write disk log -> ", name);
      }
    }
};

long long LOG::START_TIME_NANOS = LOG::CURRENT_TIME_NANOS();
