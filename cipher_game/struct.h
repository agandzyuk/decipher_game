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
    Ceil(const char* pstr );

    static Ceil randomize(int accuracy);
    int& at(int i);
    char find(int val);
    bool contains( int i ) const;
    bool is_numeric() const;
    const char* c_str();

    int operator [](int i) const;
    operator int() const;
    Ceil& operator =(const Ceil& rval);
    Ceil& operator =(unsigned long long val);
    Ceil& operator =(const char* pstr);
    Ceil& operator =(int val);
    bool operator ==( const Ceil& ceil ) const;

protected:
    int num[4];
    std::string str_;
};
typedef std::set<Ceil> CeilsT;

///////////////////////////////////////////////////////////
struct Binary : public Ceil
{
    Binary();
    Binary(bool b);
    Binary( const Ceil& c );
    Binary( const char c[5] );
    Binary( int from_int );

    Binary& operator =(bool b);
    Binary& operator =( const Ceil& c );
    Binary& operator =( const char c[5] );
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
    Pattern(const char* pattern);
    bool empty() const;

    Pattern& operator =(const char* pstr);
    operator int() const;
    int inplace( const Ceil& chifer );
    void multiplie( const Ceil& chifer, const Ceil& old_state );
    bool saticfy_or( const Ceil& ceil ) const;
    bool saticfy_xor( const Ceil& ceil ) const;
    bool saticfy_and( const Ceil& ceil ) const;
};
typedef std::set<Pattern> PredicatT;

///////////////////////////////////////////////////////////
struct UserInput : public Ceil
{
    UserInput();
    UserInput( const Ceil& ceil, int s, int n, int p );
    UserInput(int from_int, int s, int n, int p );
    UserInput(const char* pattern, int s, int n, int p );

    PredicatT& get_predicat();
    CeilsT& get_variants();
    bool empty() const;
    UserInput& operator =(const Ceil& rval);

    int  stage;
    int  guess;
    int  inplace;
    PredicatT guess_predicat;
    PredicatT inplace_predicat;
    CeilsT variants;
    Pattern applied;
};
typedef std::list<UserInput> HistoryT;


///////////////////////////////////////////////////////////
extern HistoryT  history;
extern CeilsT    invariants;

extern UserInput apply_strategy( const Ceil& pattern, char history_deep );