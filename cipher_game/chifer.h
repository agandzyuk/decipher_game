#define _CRT_RAND_S

#include <windows.h>
#include <time.h>
#include <set>
#include <list>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <assert.h>
#include <conio.h>
#include <math.h>


///////////////////////////////////////////////////////////
struct Ceil 
{
    Ceil();
    Ceil(const Ceil& ceil);
    Ceil(int from_int);
    Ceil(char str[4]);
    Ceil(const char* pstr);

    static Ceil randomize(int accuracy);
    int& at(int i);
    char find(int val) const;
    bool contains( int i ) const;
    bool is_numeric() const;
    const char* c_str() const;
    bool empty() const;

    int operator [](int i) const;
    operator int() const;
    Ceil& operator =(const Ceil& rval);
    Ceil& operator =(unsigned long long val);
    Ceil& operator =(char str[4]);
    Ceil& operator =(const char* pstr);
    Ceil& operator =(int val);
    bool operator ==( const Ceil& ceil ) const;
    bool operator <( const Ceil& ceil ) const;

protected:
    int num[4];
    mutable std::string str_;
};
typedef std::set<Ceil> CeilsT;

///////////////////////////////////////////////////////////
struct Binary : public Ceil
{
    Binary();
    Binary(bool b);
    Binary( const Ceil& c );
    Binary( char c[4] );
    Binary( int from_int );

    Binary& operator =(bool b);
    Binary& operator =( const Ceil& c );
    Binary& operator =( char c[4] );
    Binary& operator =( int from_int );
    bool operator < (const Binary& rVal) const;
    operator int() const;
    Ceil operator&(const Ceil& c) const;
    Ceil operator|(const Ceil& c) const;
    Ceil operator^(const Ceil& c) const;
    Binary& operator&=(const Binary& c);
    Binary& operator|=(const Ceil& c);
    Binary& operator^=(const Ceil& c);
    Binary& operator!=(const Binary& b);
    Binary operator!() const;
    operator bool() const;
};

typedef std::set<Binary> BinsT;
///////////////////////////////////////////////////////////

struct Pattern : public Ceil
{
    Pattern();
    Pattern(const Pattern& pattern);
    Pattern(const Ceil& ceil);
    Pattern(int from_int);
    Pattern(const char* pstr);
    Pattern(char str[4]);

    int inplace( const Ceil& chifer );
    void multiplie( const Ceil& chifer, const Ceil& old_state );
    bool saticfy_or( const Ceil& ceil ) const;
    bool saticfy_xor( const Ceil& ceil ) const;
    bool saticfy_and( const Ceil& ceil ) const;
};
typedef std::set<Pattern> PredicatT;

///////////////////////////////////////////////////////////
typedef std::map<int,int> MinimalsT;

struct UserInput : public Ceil
{
    UserInput();
    UserInput(const Ceil& ceil);
    UserInput(const UserInput& input);
    UserInput(int from_int);
    UserInput(char str[4]);
    UserInput(const char* pstr);
    UserInput(const Ceil& ceil, int n, int p );
    UserInput(int from_int, int n, int p );
    UserInput(const char* pstr, int n, int p );
    // for searching
    UserInput(int gs, int plc);

    void create_predicat();
    CeilsT& get_variants();
    bool empty() const;
    bool operator < (const UserInput&) const;
    int operator-(const UserInput&) const;
    static CeilsT init_ordered_set(short start, short stop);
    void search_minimals(FILE* f);
    void get_min_variants_random(std::vector<int>& digits, FILE* f );

    int  guess;
    int  inplace;
    PredicatT guess_predicat;
    PredicatT inplace_predicat;
    MinimalsT min_variants;
    std::list<UserInput> history;
    CeilsT variants;
};
typedef std::list<UserInput> HistoryT;

///////////////////////////////////////////////////////////
struct Storage 
{
    std::set<UserInput> probes;
    HistoryT history;
    void Save();
    void Load();
};
extern std::string FileName;

///////////////////////////////////////////////////////////
extern CeilsT    invariants;
extern UserInput apply_strategy( const Ceil& pattern, char history_deep );