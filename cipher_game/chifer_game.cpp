#include "chifer.h"

using namespace std;


Storage storage;
CeilsT invariants;

///////////////////////////////////////////////////////////
inline int get_ceil_guess(const Ceil& var, const Ceil& from)
{
    int guess = 0;
    for(int i = 0; i < 4; ++i)
        guess += (from[i] == var[0] || from[i] == var[1] || from[i] == var[2] || from[i] == var[3]);
    return guess;
}

inline int get_ceil_inplace(const Ceil& var, const Ceil& from)
{
    int inplace = 0;
    for(int i = 0; i < 4; ++i)
        inplace += (var[i] == from[i]);
    return inplace;
}

///////////////////////////////////////////////////////////
Ceil::Ceil()
{
    memset(num, -1, 4*sizeof(int));
    str_ = "****";
}

Ceil::Ceil(const Ceil& ceil) 
{ 
    for (char i = 0; i < 4; i++ )
        num[i] = ceil[i];
    str_ = ceil.c_str();
}

Ceil::Ceil(int from_int)
{ 
    *this = from_int; 
}

Ceil::Ceil(char str[4])
{ 
    if ( str_.empty() )
        str_.resize(5);
    else {
        str_.clear();
        str_.resize(5);
    }

    for(int i = 0; i < 4; i++) 
    {
        num[i] = (str[i] != '*') ? str[i]-48 : -1;
        str_.at(i) = str[i];
    }
}

Ceil::Ceil(const char* pstr)
{ 
    if ( str_.empty() )
        str_.resize(5);
    else {
        str_.clear();
        str_.resize(5);
    }

    for(int i = 0; i < 4; i++) 
    {
        num[i] = (pstr[i] != '*') ? pstr[i]-48 : -1;
        str_.at(i) = pstr[i];
    }
}

Ceil Ceil::randomize(int accuracy)
{
    Ceil c;
    do {
        DWORD tick = GetTickCount();
        int rnd = rand();
        float y = 0.0;
        if(rnd)
            y = float(tick)/rnd - tick/rnd;
        rnd = int(y*accuracy);
        c = rnd;
        if( c[0] != c[1] && c[0] != c[2] && c[0] != c[3] &&
            c[1] != c[2] && c[1] != c[3] && c[2] != c[3] )
        {
            break;
        }
    }
    while( true );
    return c;
}

bool Ceil::empty() const
{
    return (num[0] == -1 && num[1] == -1 && num[2] == -1 && num[3] == -1);
}

int Ceil::operator [](int i) const 
{ 
    return num[i]; 
}

int& Ceil::at(int i)
{ 
    return num[i]; 
}

char Ceil::find(int val) const
{
    for ( char i = 0; i < 4; i++ )
        if ( num[i] == val )
            return i;
    return -1;
}

Ceil::operator int() const
{
    assert(num[0]>-2 && num[0]<10 );
    assert(num[1]>-2 && num[1]<10 );
    assert(num[2]>-2 && num[2]<10 );
    assert(num[3]>-2 && num[3]<10 );
    int sum = 0;
    bool negative = false;
    for( char i = 0; i < 4; i++ )
    {
        if ( num[i] == -1 )
            negative = true;
        sum = abs(num[i]) + sum*10;
    }
    return negative ? -sum : sum;
}

Ceil& Ceil::operator =(const Ceil& rval)
{ 
    for (char i = 0; i < 4; i++ )
        num[i] = rval[i];
    str_ = rval.c_str();
    return *this;
}

Ceil& Ceil::operator =(unsigned long long val)
{ 
    *this = (int)val;
    return *this;
}

const char* Ceil::c_str() const
{
    if ( str_.empty() )
        str_.resize(5);
    else {
        str_.clear();
        str_.resize(5);
    }

    for(int i = 0; i < 4; i++)
        str_.at(i) = (num[i]!=-1) ? char(num[i]+48) : '*';
    return str_.c_str(); 
}

Ceil& Ceil::operator =(char str[4])
{
    if ( str_.empty() )
        str_.resize(5);
    else {
        str_.clear();
        str_.resize(5);
    }

    for(int i = 0; i < 4; i++) 
    {
        num[i] = (str[i] != '*') ? str[i]-48 : -1;
        str_.at(i) = str[i];
    }
    return *this;
}

Ceil& Ceil::operator =(const char* pstr)
{
    if ( str_.empty() )
        str_.resize(5);
    else {
        str_.clear();
        str_.resize(5);
    }

    for(int i = 0; i < 4; i++) 
    {
        num[i] = (pstr[i] != '*') ? pstr[i]-48 : -1;
        str_.at(i) = pstr[i];
    }
    return *this;
}

Ceil& Ceil::operator =(int val)
{
    num[0] = val/1000;
    val %= 1000;
    num[1] = val/100;
    val %= 100;
    num[2] = val/10;
    val %= 10;
    num[3] = val;
    c_str();
    return *this;
}

bool Ceil::contains( int i ) const 
{
    return ( (num[0] == i) || (num[1] == i) || (num[2] == i) || (num[3] == i) ); 
}

bool Ceil::operator ==( const Ceil& ceil ) const 
{ 

    return ( (int)*this == (int)ceil ); 
}

bool Ceil::operator <( const Ceil& ceil ) const 
{ 
    return ( (int)*this < (int)ceil ); 
}

bool Ceil::is_numeric() const 
{ 
        return ( (num[0] > -2) && (num[1] > -2) && (num[2] > -2) && (num[3] > -2) && 
                 (num[0] < 10) && (num[1] < 10) && (num[2] < 10) && (num[3] < 10) ); 
}

///////////////////////////////////////////////////////////

Binary::Binary()
{
    for( char i = 0; i < 4; i++ )
        num[i] = 0;
    c_str();
}

Binary::Binary(bool b)
{
    for( char i = 0; i < 4; i++ )
        num[i] = (b == true) ? 1 : 0;
    c_str();
}

Binary::Binary( const Ceil& c )
{
    for( char i = 0; i < 4; i++ )
        num[i] = (c[i] == -1)? 0 : 1;
    c_str();
}

Binary::Binary( char c[4] )
{
    for( char i = 0; i < 4; i++ )
        num[i] = (c[i] == '*') ? 0 : 1;
    c_str();
}

Binary::Binary( int from_int )
{
    for( char i = 0; i < 4; i++ )
        num[i] = from_int & (8>>i);
    c_str();
}

Binary& Binary::operator =(bool b)
{
    for( char i = 0; i < 4; i++ )
        num[i] = (b == true) ? 1 : 0;
    c_str();
    return *this;
}

Binary& Binary::operator =( const Ceil& c )
{
    for( char i = 0; i < 4; i++ )
        num[i] = (c[i] == -1)? 0 : 1;
    c_str();
    return *this;
}

Binary& Binary::operator =( char c[4] )
{
    for( char i = 0; i < 4; i++ )
        num[i] = (c[i] == '*') ? 0 : 1;
    c_str();
    return *this;
}

Binary& Binary::operator =( int from_int )
{
    for( char i = 0; i < 4; i++ )
        num[i] = from_int & (8>>i);
    c_str();
    return *this;
}

bool Binary::operator < (const Binary& rVal) const
{
    return ( (int)*this < (int)rVal );
}

Binary::operator int() const
{
    assert( num[0] == 0 || num[0] == 1 );
    assert( num[1] == 0 || num[1] == 1 );
    assert( num[2] == 0 || num[2] == 1 );
    assert( num[3] == 0 || num[3] == 1 );

    int sum = 0;
    for( char i = 0; i < 4; i++ )
        sum += num[i] * (8>>i);
    return sum; 
}

Ceil Binary::operator&(const Ceil& c) const
{
    Ceil ret;
    for( char i = 0; i < 4; i++ )
        ret.at(i) = num[i] ? c[i] : -1;
    return ret;
}

Ceil Binary::operator|(const Ceil& c) const
{
    Ceil ret;
    for( char i = 0; i < 4; i++ )
        ret.at(i) = num[i] ? c[i] : num[i];
    return ret;
}

Ceil Binary::operator^(const Ceil& c) const
{
    Ceil ret;
    for( char i = 0; i < 4; i++ )
        ret.at(i) = num[i]^c[i];
    return ret;
}

Binary& Binary::operator&=(const Binary& c)
{
    for( char i = 0; i < 4; i++ )
        num[i] &= c[i];
    c_str();
    return *this;
}

Binary& Binary::operator|=(const Ceil& c)
{
    for( char i = 0; i < 4; i++ )
        num[i] |= c[i];
    c_str();
    return *this;
}

Binary& Binary::operator^=(const Ceil& c)
{
    for( char i = 0; i < 4; i++ )
        num[i] ^= c[i];
    c_str();
    return *this;
}

Binary& Binary::operator!=(const Binary& b)
{
    for( char i = 0; i < 4; i++ )
        num[i] = b[i] ? 0 : 1; 
    c_str();
    return *this;
}

Binary Binary::operator!() const
{
    Binary ret;
    for( char i = 0; i < 4; i++ )
        ret.at(i) = num[i] ? 0 : 1; 
    return ret;
}

Binary::operator bool() const
{
    for( char i = 0; i < 4; i++ )
        if( num[i] == 0 )
            return false;
    return true;
}
///////////////////////////////////////////////////////////

Pattern::Pattern() 
    : Ceil() 
{}

Pattern::Pattern(const Pattern& pattern) 
    : Ceil(pattern) 
{}

Pattern::Pattern(const Ceil& ceil) 
    : Ceil(ceil) 
{}

Pattern::Pattern(int from_int) 
    : Ceil(from_int) 
{}

Pattern::Pattern(const char* pstr) 
    : Ceil(pstr) 
{}

Pattern::Pattern(char str[4]) 
    : Ceil(str) 
{}

int Pattern::inplace( const Ceil& chifer )
{
    int inplace = 0;
    for (int i = 0; i < 4; i++) 
        inplace += (num[i] == chifer[i]);
    return inplace;
}

void Pattern::multiplie( const Ceil& chifer, const Ceil& old_state )
{
    for( int i = 0; i < 4; i++ ) 
        if( old_state[i] == -1 )
            num[i] = (num[i] == chifer[i]) ? chifer[i] : -1;
        else
            num[i] = old_state[i];
}

bool Pattern::saticfy_or( const Ceil& ceil ) const
{
    for( short i = 0; i < 4; i++ )
        if( num[i] != -1 && ceil.contains(num[i]) )
            return true;
    return empty();
}

bool Pattern::saticfy_xor( const Ceil& ceil ) const
{
    for ( short i = 0; i < 4; i++ )
        if( num[i] != -1 && !ceil.contains(num[i]) )
            return false;
    return true;
}

bool Pattern::saticfy_and( const Ceil& ceil ) const
{
    for ( short i = 0; i < 4; i++ )
        if( num[i] != -1 && ceil[i] != num[i] ) 
            return false;
    return true;
}

/////////////////////////////////////////////////////////////////

vector<Ceil> strategy_table;

void init_strategy_table()
{
    for(int a = 0; a < 5; a++) 
    {
        for(int b = 0; b < 5; b++) 
        {
            if( b && (b == a ) )
                continue;
            for(int c = 0; c < 5; c++) 
            {
                if( c && (c == b || c == a ) )
                    continue;
                for(int d = 0; d < 5; d++) 
                {
                    if( d && (d == c || d == b || d == a) )
                        continue;
                    Ceil cl;
                    cl.at(0) = a;
                    cl.at(1) = b;
                    cl.at(2) = c;
                    cl.at(3) = d;
                    strategy_table.push_back( cl );
                }
            }
        }
    }
}

static CeilsT rollback;
/////////////////////////////////////////////////////////////////
void apply_ui( UserInput& ui )
{
    rollback.clear();
    rollback = invariants;
    invariants = ui.get_variants();
    ui.history = storage.history;
    storage.history.push_back( ui );
}

/////////////////////////////////////////////////////////////////
void get_probe( UserInput& ui )
{
    ui.search_minimals(0);
    MinimalsT::iterator var_It = ui.min_variants.begin();
    int counter = 0;
    for( ; var_It != ui.min_variants.end(); ++var_It, ++counter ) 
    {
        printf( "%.4d\t", var_It->first );
        if (counter%8 == 7)
            printf("\n");
    }
    printf("\nshow differences?  ");
    fflush(stdin);
    char ic = 0;
    scanf_s("%c", &ic, 1);
    if ( ic == 'y' )
    {
        int chif = 0;
        fflush( stdin ); 
        printf("%d>> diff value: ", storage.history.size()+1);
        scanf_s("%d",&chif,4);
        if ( chif < 0123 || chif > 9876 )
            return;
        UserInput orig = chif;
        orig.guess = 4;
        orig.inplace = 4;
        orig.search_minimals(0);
        int countof = 0;
        MinimalsT::iterator origIt = orig.min_variants.begin();
        for (; origIt != orig.min_variants.end(); ++origIt )
        {
            if ( ui.min_variants.end() == ui.min_variants.find(origIt->first) )
            {
                printf( "\t%d", origIt->first );
                if ( ++countof % 8 == 0 )
                    printf("\n");
            }
        }
    }
    printf("\n");
}

struct Probe {
    Probe() : gs(4), pc(4) {}
    Probe(int g, int p) : gs(g), pc(p) {}
    int gs;
    int pc;
    bool operator <(const Probe& r) const
    { return gs == r.gs ? pc < r.pc : gs < r.gs; }
};
typedef set<Probe> ProbesT;

struct Num44
{
//    Num44() : num(0) {}
    Num44(int n) : num(n) {}
    Num44(int n, const ProbesT& ps) : num(n), probes(ps) {}
    int num;
    ProbesT probes;
    Num44( const MinimalsT::value_type& val ) 
    { 
        num = val.first;
        if ( probes.empty() )
            probes.insert( Probe(4,4) );
    }
    string to_string()
    {
        string res;
        for( ProbesT::iterator It = probes.begin(); It != probes.end(); ++It )
        {
            char buf[10];
            if ( It == probes.begin() )
                sprintf_s(buf, 10, "%d%d", It->gs, It->pc);
            else
                sprintf_s(buf, 10, ",%d%d", It->gs, It->pc);
            res += buf;
        }
        return res;
    }

    bool operator <(const Num44& r) const
    { return num < r.num; }
};
typedef set<Num44> Nums44T;

struct Num44Assign {
    Num44Assign(Nums44T& n ) : n_(n) {}
    void operator()( const MinimalsT::value_type& val )
    {
        n_.insert( val );
    }
    Nums44T& n_;
};

#include <io.h> 

void statistics_to_file(UserInput& ui)
{
    int cnt = 1;
    string filename = "./vars.log";
    while( true ) 
    {
        if ( storage.history.size() < 2 ) 
        {
            if ( 0 == _access_s(filename.c_str(), 0) )
                _unlink( filename.c_str() );
            else
                break;
        }
        else
            if ( 0 != _access_s(filename.c_str(), 0) )
                break;

        char buf[20];
        sprintf_s(buf,20,"./vars.%d.log", cnt++);
        filename = buf;
    }
    if ( storage.history.size() < 2 )
        filename = "./vars.log";
        
    FILE *f = 0;
    fopen_s( &f, filename.c_str(), "w" );

    printf("\nsaving");
    MinimalsT mins[5][5] = {{}};
    for( int i = 0; i < 5; i++ )
        for( int j = 0; j <= i; j++ )
        {
            if ( i == 4 && (j == 3 || j == 4))
                continue;
            ui.guess   = i;
            ui.inplace = j;
            ui.min_variants.clear();
            ui.search_minimals(f);
            mins[i][j] = ui.min_variants;
            MinimalsT::iterator nIt = ui.min_variants.begin();
            for (unsigned n = 0; n < ui.min_variants.size(); ++nIt, n++ )
                if ( (n+1)%10 )
                    fprintf(f, "%.4d\t", nIt->first );
                else
                    fprintf(f, "%.4d\n", nIt->first );
            fprintf(f, "\n\n" );
        }

    ui.guess   = 4;
    ui.inplace = 4;
    ui.min_variants.clear();

    fprintf(f, "\n\n");
    ui.search_minimals(f);

    Nums44T nums44;
    std::for_each( ui.min_variants.begin(), ui.min_variants.end(), Num44Assign(nums44) );

    for( int i = 0; i < 5; i++ )
        for( int j = 0; j <= i; j++ )
        {
            if ( i == 4 && (j == 3 || j == 4))
                continue;
            int counter = 1;
            MinimalsT::iterator var_It = mins[i][j].begin();
            for( ; var_It != mins[i][j].end(); ++var_It ) 
            {
                Nums44T::iterator It = nums44.find( var_It->first );
                if( nums44.end() != It )
                    It->probes.insert( Probe(i,j) );
            }
        }

    typedef map<ProbesT,vector<Num44>> Prob2NumT;

    Prob2NumT probes2nums;
    Nums44T::iterator It = nums44.begin();
    for(; It != nums44.end(); ++It) {
        if ( probes2nums.end() == probes2nums.find( It->probes ) )
            probes2nums.insert( Prob2NumT::value_type( It->probes, vector<Num44>() ) );
        probes2nums[It->probes].push_back( Num44(It->num, It->probes) );
    }

    for (Prob2NumT::iterator pIt = probes2nums.begin(); pIt != probes2nums.end(); ++pIt)
    {
        if ( pIt->second.size() )
            fprintf( f, "%s:\n\t", pIt->second[0].to_string().c_str() );
        for(int i = 0; i < (int)pIt->second.size(); i++)
        {
            if ( (i+1)%10 )
                fprintf( f, "%.4d\t", pIt->second[i].num );
            else
                fprintf( f, "%.4d\n", pIt->second[i].num );
        }
        fprintf( f, "\n");
    }

    printf("ok!\n\n");
    fclose( f );
}

/////////////////////////////////////////////////////////////////
void get_input()
{
    int stage = storage.history.size()+1;
    if ( stage < 2 )
        return;

    printf("\nsearching optimals...\n");
    for(int i = 0; i < 5; i++ ) 
    {
        for(int j = 0; j <= i; j++ )
        {
            UserInput ui( storage.history.back() );
            ui.history = storage.history;
            ui.guess = i;
            ui.inplace = j;
//            set<UserInput>::iterator probe_in_cache = storage.probes.find( ui );
//            if ( storage.probes.end() == probe_in_cache )
            {
                ui.search_minimals(0);
                storage.probes.insert( ui );

                vector<int> digits;
                digits.clear();
                ui.get_min_variants_random(digits, 0);
                vector<int>::iterator var_It = digits.begin();
                for( ; var_It != digits.end(); ++var_It )
                    printf( "%.4d\t", (int)*var_It );
                printf("\n");
            }
/*            else
            {
                printf("\t%d:%d (in cache) ... %d variants brings %d steps\n", 
                    i,j,
                    probe_in_cache->min_variants.size(), 
                    probe_in_cache->min_variants.begin()->second 
                    );
            }*/
        }
    }

/*    if ( cache_changed )
    {
        printf("You have a changes in cache. Save it? ");
        char ch_enter[5] = {0};
        fflush(stdin);
        scanf_s("%1c",ch_enter,1);
        if ( ch_enter[0] == 'y')
            storage.Save();
    }

    printf("Type probe ** want to see (random): ");
    int in_enter = 0;
    fflush(stdin);
    scanf_s("%d",&in_enter);

    UserInput ui( storage.history.back() );
    ui.history = storage.history.back().history;
    ui.guess = in_enter/10;
    ui.inplace = in_enter%10;
    set<UserInput>::iterator probe = storage.probes.find( ui );
    if ( probe != storage.probes.end() )
    {
        std::vector<int> digits;
    }
    else
        printf("probe %.2d not found in storage...\n", in_enter);
        */
}

void statistics( UserInput& ui )
{
    int stage = storage.history.size()+1;
    if ( stage < 2 )
        return;

    CeilsT::iterator It = invariants.begin();

    int num[4][10] = {0};
    for( ; It != invariants.end(); ++It )
    {
        Ceil& c = *It;
        for (char i = 0; i < 4; i++ )
        {
            num[i][c[i]]++;
        }
    }

    for (char i = 0; i < 10; i++ ) {
        if ( num[0][i] == 0 && num[1][i] == 0 && num[2][i] == 0 && num[3][i] == 0 )
            continue;
        printf("%d:\t%d\t%d\t%d\t%d\n", i, num[0][i], num[1][i], num[2][i], num[3][i] );
    }
}

typedef std::map<int,Ceil> SetOfProbT;
typedef std::map<int,CeilsT> MapOfProbT;
void calc_matrix_mode( int row )
{
    UserInput mceil;
    SetOfProbT allProbes;
    MapOfProbT allProbesMap;
    int counter = 0;
    printf("please wait...  ");

    int countof = 0;    
    for(CeilsT::iterator It1 = invariants.begin(); It1 != invariants.end(); ++It1 )
    {
        set<int> inputs;
        int nrow = 1;
        countof = 0;
        CeilsT::iterator It2 = invariants.begin();
        for(; It2 != invariants.end(); ++It2, ++nrow )
        {
            int probe = get_ceil_guess( *It1, *It2 )*10 + get_ceil_inplace( *It1, *It2 );
            if ( inputs.end() != inputs.find(probe) ) 
            {
                countof++;
                if ( row > 0 && nrow == row )
                    break;
            }
            else
                inputs.insert(probe);
        }
        if( It2 != invariants.end() )
            continue;

        allProbes.insert( SetOfProbT::value_type(countof+1, *It1 ) );
        if ( allProbesMap.end() == allProbesMap.find( countof+1 ) ) 
            allProbesMap.insert( MapOfProbT::value_type(countof+1, CeilsT()) );
        allProbesMap[countof+1].insert( *It1 );
    }

    countof = 0;
    for( int a = 0; a < 10; a++ ) {
        countof = 0;
        for( int b = 0; b < 10; b++ ) {
//            if ( b == a )
//                continue;
            countof = 0;
            for( int c = 0; c < 10; c++ ) {
//                if ( c == b || c == a )
//                    continue;
                countof = 0;
                for( int d = 0; d < 10; d++ ) {
//                    if ( d == a || d == b || d == c )
//                        continue;
                    countof = 0;

                    float tmp = float(100)/10000*counter;
                    int percents = (int)tmp;
                    if ( counter%100 ) {
                        if ( percents < 10 )
                            printf("\b\b%d%%", percents);
                        else
                            printf("\b\b\b%d%%", percents);
                    }

                    counter++;
                    mceil.at(0) = a;
                    mceil.at(1) = b;
                    mceil.at(2) = c;
                    mceil.at(3) = d;
                    set<int> inputs;
                    int nrow = 1;
                    CeilsT::iterator iIt = invariants.begin();
                    for(; iIt != invariants.end(); ++iIt, ++nrow )
                    {
                        int probe = get_ceil_guess( mceil, *iIt )*10 + get_ceil_inplace( mceil, *iIt );
                        if ( inputs.end() != inputs.find(probe) ) 
                        {
                            countof++;
                            if ( row > 0 && nrow == row )
                                break;
                        }
                        else
                            inputs.insert(probe);
                    }
                    if( iIt != invariants.end() )
                        continue;

                    allProbes.insert( SetOfProbT::value_type(countof+1, mceil ) );

                    if ( invariants.end() != invariants.find( mceil ) ) 
                    {
                        if ( allProbesMap.end() == allProbesMap.find( countof+1 ) ) 
                            allProbesMap.insert( MapOfProbT::value_type(countof+1, CeilsT()) );
                        allProbesMap[countof+1].insert( mceil );
                    }

                    if (countof == 0)
                        break;
                }
                if ( countof == 0 )
                    break;
            }
            if ( countof == 0 )
                break;
        }
        if ( countof == 0 )
            break;
    }
    printf("\b\b\b. done!\n\n  var   prob   \n-------------\n");

    SetOfProbT::iterator pIt = allProbes.begin();
    for(; pIt != allProbes.end(); ++pIt )
        printf("  %d\t%.4d\n", pIt->first, (int)pIt->second );

    int ncase = -1;
    while( true )
    {
        if( ncase == -1 )
            printf("choose a case or enter xxxx\n");
        else
            printf("case variants <v>\n>> ");
        char scase[6] = {0};
        fflush(stdin);
        printf("s>> ");
        scanf_s("%s",&scase,5);
        if ( scase[0] == 'v' || scase[0] == 'V' ) 
        {
            MapOfProbT::iterator mIt = allProbesMap.find( ncase );
            if ( mIt != allProbesMap.end() )
            {
                int ccc = 1;
                CeilsT::iterator mpIt = mIt->second.begin();
                for(; mpIt != mIt->second.end(); ++mpIt, ++ccc )
                    if ( ccc%8 )
                        printf("%.2d\t", (int)*mpIt );
                    else
                        printf("%.2d\n", (int)*mpIt );
            }
            ncase = -1;
            continue;
        }
        else if ( scase[0] < 48 || scase[0] > 58 ) {
            printf("no case\n");
            break;
        }
        
        int len = strlen(scase);
        ncase = 0;
        for( int j = 0; j < len; j++ )
            ncase = ncase*10 + (scase[j]-48);

        Ceil variantCeil;
        if ( len != 4 ) {
            pIt = allProbes.find( ncase );
            if ( pIt == allProbes.end() ) {
                printf("no case\n");
                break;
            }
        }
        else {
            pIt = allProbes.begin();
            for( ; pIt != allProbes.end(); ++pIt )
                if ( (int)pIt->second == ncase ) {
                    ncase = pIt->first;
                    break;
                }
            if ( pIt == allProbes.end() ) {
                variantCeil = ncase;
                ncase = -1;
            }
        }

        CeilsT::iterator iIt = invariants.begin();
        for(; iIt != invariants.end(); ++iIt )
        {
            int probe;
            if ( variantCeil.empty() )
                probe = get_ceil_guess( allProbes[ncase], *iIt )*10 + get_ceil_inplace( allProbes[ncase], *iIt );
            else
                probe = get_ceil_guess( variantCeil, *iIt )*10 + get_ceil_inplace( variantCeil, *iIt );

            printf("%.2d: %.4d\n", probe, (int)*iIt );
        }
    }
}

void matrix_mode()
{
    if ( invariants.empty() )
    {
        printf("m<<");
        return;
    }

/*	7693	7936	9673	9736	
    invariants.clear();
    invariants.insert(3796);
    invariants.insert(3976);
    invariants.insert(6793);
    invariants.insert(6973);
    invariants.insert(7693);
    invariants.insert(7936);
  */  
//    invariants.insert(4602);
//    invariants.insert(4620);

    printf("                                                     --+------------\n");
    CONSOLE_SCREEN_BUFFER_INFO console;
    ::GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&console);

    int counter = 1;
    for(CeilsT::iterator It = invariants.begin(); It != invariants.end(); ++It )
    {
        Ceil& c = *It;
        if ( counter == 1 )
            printf("Calculate min set <s>                                1 | %d %d %d %d\n", c[0], c[1], c[2], c[3]);
        else if ( counter == 2 )
            printf("Row number out of vars (xx)                          2 | %d %d %d %d\n", c[0], c[1], c[2], c[3]);
        else if ( counter == 3 )
            printf("Exit and continue <any>                              3 | %d %d %d %d\n", c[0], c[1], c[2], c[3]);
        else if ( counter < 10 )
            printf("                                                     %d | %d %d %d %d\n", counter, c[0], c[1], c[2], c[3]);
        else if ( counter < 100 )
            printf("                                                    %d | %d %d %d %d\n", counter, c[0], c[1], c[2], c[3]);
        else if ( counter < 1000 )
            printf("                                                   %d | %d %d %d %d\n", counter, c[0], c[1], c[2], c[3]);
        else 
            printf("                                                  %d | %d %d %d %d\n", counter, c[0], c[1], c[2], c[3]);
        counter++;
        if ( invariants.size() > 50 && counter > 2 && counter < (int)invariants.size()-1 ) {
            printf("                                                    xx | . . . .\n");
            counter = invariants.size()-1;
            It = invariants.end();
            --It; --It; --It;
        }
    }
    
    console.dwCursorPosition.Y += 4;
    ::SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), console.dwCursorPosition);

    bool prev_menu = false;
    while( true ) 
    {
        char in_enter[3] = {0};
        fflush( stdin );
        if ( !prev_menu )
            printf("m>> ");
        else {
            printf("m<< ");
            prev_menu = false;
        }

        scanf_s("%s", &in_enter, 3);
        
        if ( in_enter[0] > 47 && in_enter[0] < 59 )
        {
            int row = in_enter[0]-48;
            if ( in_enter[1] > 47 && in_enter[1] < 59 )
                row = row * 10 + in_enter[0] - 48;
            calc_matrix_mode( row );
            prev_menu = true;
        }
        else if ( in_enter[0] == 's' )
        {
            calc_matrix_mode( 0 );
            prev_menu = true;
        }
        else
            break;
    }
}

static float bank = 2.4f;
static float bet = 0.0f;

/////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {90, 300};
    SetConsoleScreenBufferSize(hWnd, bufferSize);

    HWND console = GetConsoleWindow();
    RECT rc;
    GetWindowRect(console, &rc);
    SetWindowPos(console, 0, rc.left, rc.top, 700, 700, SWP_SHOWWINDOW);

seif_begin:
/*    int numOfOk = 0;
    int numOfFail = 0;
    int numOfBet = 1;
    for (int i = 0; i < 200; i++) 
    {
        if ( numOfBet % 3 == 0 )
        {
            bet = 1.0f;
            numOfFail = 0;
            numOfOk   = 0;
        }
        else if( numOfBet % 6 == 0 )
        {
            bet = 5.0f;
            numOfFail = 0;
            numOfOk   = 0;
        }
        else
            bet = 0.25f;

        bank -= bet;
        numOfBet++;
        int t = Ceil::randomize( 9999 );
        if ( t%2 ) {
            bank += bet * 1.9f;
            numOfOk++;
        }
        else {
            numOfFail++;
        }
    }
*/

    invariants.clear();
    storage.Load();

    int  num_vars = 10000;
    invariants = UserInput::init_ordered_set( 0123, 9876 );

    int  counter = 0;
    Ceil random = Ceil::randomize( num_vars );
    UserInput user;

    Pattern in_pattern, store;
    while (true) 
    {
        counter++;

        printf( "Stage %d Variants %d\n", 
            counter, invariants.size(), in_pattern.c_str() );
        printf("-------------------------\n");

        statistics( user );
        get_input();

        store = in_pattern;
        printf("\n");
        do {
            printf("Chifer(xxxx), probe(xx), <r>, <m>, <f>, <p>:\n%d>> ", counter);

            char in_enter[5] = {0};
            fflush(stdin);
            scanf_s("%s",&in_enter,5);
            if ( in_enter[0] == 'r' ) 
            {
                printf("you have a rollback\n%d>>", counter);
                invariants = rollback;
                counter--;
            }
            if ( in_enter[0] == 'f' ) 
            {
                UserInput ui2 = store;
                statistics_to_file( ui2 );
            }
            if ( in_enter[0] == 'm' ) 
            {
                matrix_mode();
            }
            if ( in_enter[0] == 'p' ) 
            {
                storage.history.clear();
                goto seif_begin;
            }
            else if ( strlen(in_enter) == 2 )
            {
                int chif = 0;
                fflush( stdin ); 
                printf("probe value:\n%d>>", counter);
                scanf_s("%d",&chif);

                UserInput ui2 = chif;
                ui2.guess = in_enter[0]-48;
                ui2.inplace = in_enter[1]-48;
                get_probe(ui2);
                continue;
            }
            else if ( strlen(in_enter) == 4 ) 
            {
                in_pattern = in_enter;
                break;
            }
            printf("... \n");
        }
        while(true);
        if ( (int)in_pattern > 9999 ) 
        {
            printf("Error input! exiting!");
            break;
        }

        user = in_pattern;

        printf("%d>> guess: ", counter);
        int guess = 0;
        scanf_s("%d",&guess,1);
        printf("%d>> inplace: ", counter);
        int inplace = 0;
        scanf_s("%d",&inplace,1);
        user.guess   = guess;
        user.inplace = inplace;
/*
        user.guess = in_pattern.contains(random[0]);
        user.guess += in_pattern.contains(random[1]);
        user.guess += in_pattern.contains(random[2]);
        user.guess += in_pattern.contains(random[3]);

        user.inplace = in_pattern.inplace(random);
*/
        apply_ui( user );

        in_pattern.multiplie( random, store );
/*        if ( user == random ) {
            bank += bet * 1.07f;
            printf("\nCongratulations, cipher is %s!\nBank %.2f UAH\n\n", storage.history.back().c_str(), bank );
            counter = 0;
            break;
        }
        else if( counter == 5 ) {
            printf("\nYou loose :(, chifer is %.4d!\nBank %.2f UAH\n\n", (int)random, bank );
            counter = 0;
            break;
        }
        printf( "                        >>>>>>  %d guess, %d in place\n\n", user.guess, user.inplace );
        */
    }
    printf("Press \"a\" key to play once again...\n\n");
    int r = _getch();
    if (r == 'a') {
        storage.history.clear();
        goto seif_begin;
    }
	return 0;
}
