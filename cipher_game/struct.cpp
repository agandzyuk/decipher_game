#include "chifer.h"

using namespace std;


HistoryT history;
CeilsT invariants;

///////////////////////////////////////////////////////////
Ceil::Ceil()
{
    memset(num, -1, 4*sizeof(int));
    c_str();
}

Ceil::Ceil(const Ceil& ceil) 
{ 
    *this = ceil; 
}

Ceil::Ceil(int from_int)
{ 
    *this = from_int; 
}

Ceil::Ceil(const char* pstr )
{ 
    *this = pstr; 
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

int Ceil::operator [](int i) const 
{ 
    return num[i]; 
}

int& Ceil::at(int i)
{ 
    return num[i]; 
}

char Ceil::find(int val)
{
    for ( char i = 0; i < 4; i++ )
        if ( num[i] == val )
            return i;
    return -1;
}

Ceil::operator int() const
{
    assert(num[0]!=-1);
    assert(num[1]!=-1);
    assert(num[2]!=-1);
    assert(num[3]!=-1);

    int sum = 0;
    for( char i = 0; i < 4; i++ )
        sum = num[i] + sum*10;
    return sum;
}

Ceil& Ceil::operator =(const Ceil& rval)
{ 
    for (char i = 0; i < 4; i++ )
        num[i] = rval[i];
    c_str();
    return *this;
}

Ceil& Ceil::operator =(unsigned long long val)
{ 
    *this = (int)val;
    return *this;
}

const char* Ceil::c_str()
{
    char buf[5];
    for(int i = 0; i < 4; i++)
        buf[i] = (num[i]!=-1) ? char(num[i]+48) : '*';
    buf[4] = 0;
    str_ = buf;
    return str_.c_str(); 
}

Ceil& Ceil::operator =(const char* pstr)
{
    str_ = pstr;
    for(int i = 0; i < 4; i++)
        num[i] = (pstr[i] != '*') ? pstr[i]-48 : -1;
    c_str();
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
    return ( (num[0] == ceil[0]) && (num[1] == ceil[1]) && (num[2] == ceil[2]) && (num[3] == ceil[3]) ); 
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

Binary::Binary( const char c[5] )
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

Binary& Binary::operator =( const char c[5] )
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

Pattern::Pattern(const char* pattern) 
    : Ceil(pattern) 
{}

bool Pattern::empty() const
{
    int comp = *this;
    return ( comp == -1111 );
}

Pattern& Pattern::operator =(const char* pstr)
{
    *static_cast<Ceil*>(this) = pstr;
    return *this;
}

Pattern::operator int() const
{
    assert(num[0]>-2 && num[0]<10 );
    assert(num[1]>-2 && num[1]<10 );
    assert(num[2]>-2 && num[2]<10 );
    (num[3]>-2 && num[3]<10 );
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

///////////////////////////////////////////////////////////

UserInput::UserInput() 
    : Ceil(), stage(0), guess(-1), inplace(-1)
{}

UserInput::UserInput( const Ceil& ceil, int s, int n, int p ) 
    : Ceil(ceil), stage(s), guess(n), inplace(p)
{}

UserInput::UserInput(int from_int, int s, int n, int p ) 
    : Ceil(from_int), stage(s), guess(n), inplace(p)
{}

UserInput::UserInput(const char* pattern, int s, int n, int p ) 
    : Ceil(pattern), stage(s), guess(n), inplace(p)
{}

bool UserInput::empty() const
{
    return ( stage == 0 );
}

UserInput& UserInput::operator =(const Ceil& rval)
{
    *(Ceil*)this = rval;
    return *this;
}

PredicatT& UserInput::get_predicat()
{
    guess_predicat.clear();
    inplace_predicat.clear();

    if( guess == 0 ) {
        guess_predicat.insert( "****" );
        guess_predicat.insert( *this );
        return guess_predicat;
    }

    const char* sp = this->c_str();
    char buf[5] = {sp[0], sp[1], sp[2], sp[3], 0};

    PredicatT all_3;
    for ( char i = 0; i < 4; i++ )
    {
        buf[i] = '*';
        all_3.insert( buf );
        buf[i] = sp[i];
    }

    PredicatT all_2;
    for ( char i = 0; i < 3; i++ )
        for ( char j = i+1; j < 4; j++ )
        {
            buf[i] = '*';
            buf[j] = '*';
            all_2.insert( buf );
            buf[i] = sp[i];
            buf[j] = sp[j];
        }

    PredicatT all_1;
    memset(buf,'*',4);
    for ( char i = 0; i < 4; i++ )
    {
        buf[i] = sp[i];
        all_1.insert( buf );
        buf[i] = '*';
    }

    if( guess == 1 )
        guess_predicat = all_1;
    if( guess == 2 )
        guess_predicat = all_2;
    if( guess == 3 )
        guess_predicat = all_3;
    if( guess == 4 )
        guess_predicat.insert( *this );

    if( inplace == 1 )
        inplace_predicat = all_1;
    if( inplace == 2 )
        inplace_predicat = all_2;
    if( inplace == 3 )
        inplace_predicat = all_3;
    if( inplace == 4 )
        inplace_predicat.insert( *this );

    return guess_predicat;
}

CeilsT& UserInput::get_variants()
{
    variants.clear();
    if( guess == 4 && (inplace == 3 || inplace == 4) ) 
    {
        variants.insert( *this );
        return variants;
    }
    
    guess_predicat = get_predicat();

    for( CeilsT::iterator It = invariants.begin(); It != invariants.end(); ++It )
    {
        int count = 0, inplaces = 0;

        if( inplace == 0 ) 
            if( (*It)[0] == (*this)[0] || (*It)[1] == (*this)[1] || (*It)[2] == (*this)[2] || (*It)[3] == (*this)[3] )
                continue;

        if ( (int)*It == 4567)
        {
            int br = 1;
        }
        for( PredicatT::iterator predIt = guess_predicat.begin(); predIt != guess_predicat.end(); ++predIt )
        {
            if ( guess && predIt->saticfy_xor( *It ) )
                count++;
            else if ( guess == 0 && predIt->saticfy_or( *It ) )
                count++;
            if ( count > 1 )
                break;
        }
        if( count > 1)
            continue;

        if( inplace ) 
        {
            for( PredicatT::iterator plIt = inplace_predicat.begin(); plIt != inplace_predicat.end(); ++plIt )
            {
                if ( plIt->saticfy_and( *It ) )
                    inplaces++;
                if ( inplaces > 1 )
                    break;
            }
            if ( inplaces > 1 )
                continue;
        }

        if( inplace && inplaces )
        {
            if ( count )
                variants.insert( *It );
        }
        else if ( !inplace && count )
            variants.insert( *It );
    }
    return variants;
}

/////////////////////////////////////////////////////////////////
void init_ordered_set(short start, short stop)
{
    while( start <= stop )  {
        Ceil c = start;
        if( c[0] != c[1] && c[0] != c[2] && c[0] != c[3] &&
             c[1] != c[2] && c[1] != c[3] && c[2] != c[3] )
        {
            invariants.insert(start);
        }
        start++;
    }
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


/////////////////////////////////////////////////////////////////
void apply_ui( UserInput& ui )
{
    invariants = ui.get_variants();
    history.push_back( ui );
}

/////////////////////////////////////////////////////////////////
UserInput get_input()
{
    UserInput ui;
    ui = Ceil("0123");
    ui.guess = 4;
    ui.inplace = 4;
    ui.stage = 1;
    apply_ui( ui );
    
    do {

    }
    while( ui.empty() && invariants.size() > 4 );
    return ui;
}

void statistics( UserInput& ui )
{
    CeilsT::iterator It = invariants.begin();

    int num[10] = {0};
    for( ; It != invariants.end(); ++It )
    {
        Ceil& c = *It;
        for (char i = 0; i < 4; i++ )
        {
            num[ c[ i ] ]++;
        }
    }

    printf("Numbers:");
    for (char i = 0; i < 10; i++ )
        if ( num[ i ] != 0 )
            printf("  %d:%d", i, num[i]);

    printf("\nWant to show? (y/n) ");
    char c[5] = {0};
    fflush( stdin ); 
    scanf_s("%c",&c,1);
    if (*c == 'y') 
    {
        printf("                                                        ------------\n");
        int counter = 1;
        for(It = invariants.begin(); It != invariants.end(); ++It )
        {
            Ceil& c = *It;
            if ( counter % 16 != 1 )
                printf("                                                        ");
            printf("%d %d %d %d", c[0], c[1], c[2], c[3]);
            if ( counter % 16 == 0 )
            {
                printf("\n...press any key                                        ");
                int esc = _getch();
                if ( esc == 27 || esc == 0 )
                    break;
            }
            else
                printf("\n");
            counter++;
        }
    }
}

/////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    HANDLE hWnd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = {120, 500};
    SetConsoleScreenBufferSize(hWnd, bufferSize);

    HWND console = GetConsoleWindow();
    RECT rc;
    GetWindowRect(console, &rc);
    SetWindowPos(console, 0, rc.left, rc.top, 1000, 600, SWP_SHOWWINDOW);

seif_begin:

    invariants.clear();

    int  num_vars = 10000;
    init_ordered_set( 0, 9999 );
    init_strategy_table();

    int  counter = 1;
    Ceil random = Ceil::randomize( num_vars );

    UserInput user;

    Pattern in_pattern, store;
    while (true) 
    {
        printf( "Stage %d, Variants %d, Cipher x\n", 
            counter++, invariants.size(), in_pattern.c_str() );
        printf("---------------------------------\n");

        statistics( user );

/*        user = get_input();
        if ( user.empty() ) 
        {
            printf("Nothing to propose...\n");
        }
        else 
        {
            printf( "Try to enter:    \"%s\"\n", user.c_str() );
        }
*/
        store = in_pattern;
        printf("4-sig chiefer:    ");
        do {
            char in_enter[5] = {0};
            scanf_s("%4s",in_enter,5);
            if ( strlen(in_enter) == 4 ) {
                in_pattern = in_enter;
                break;
            }
            printf("retype 4 numbers: ");
        }
        while(true);
        if ( !in_pattern.is_numeric() ) {
            printf("not all integers inside: exit!");
            break;
        }

        user = in_pattern;

        user.guess = in_pattern.contains(random[0]);
        user.guess += in_pattern.contains(random[1]);
        user.guess += in_pattern.contains(random[2]);
        user.guess += in_pattern.contains(random[3]);

        user.inplace = in_pattern.inplace(random);
        apply_ui( user );

        in_pattern.multiplie( random, store );
        if ( user == random ) {
            printf("\nCongratulations, cipher is %s!\n\n", history.back().c_str() );
            break;
        }
        printf( "                               >>  %d guess, %d in place\n\n", user.guess, user.inplace );
    }
    printf("Press \"a\" key to play once again...\n\n");
    int r = _getch();
    if (r == 'a')
        goto seif_begin;
	return 0;
}
