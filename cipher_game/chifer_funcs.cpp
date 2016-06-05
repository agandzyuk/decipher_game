#include "chifer.h"
#include <algorithm>

UserInput apply_strategy( const Ceil& pattern, char history_deep )
{
/*    UserInput newui;
    if ( history.size() == 0 ) 
    {
        newui = "0123";
        return newui;
    }

    std::set<int>  free_set;
    std::set<int>  prev_set;
    prev_set.insert( history.back()[0] );
    prev_set.insert( history.back()[1] );
    prev_set.insert( history.back()[2] );
    prev_set.insert( history.back()[3] );

    for( CeilsT::iterator It = invariants.begin(); It != invariants.end(); ++It )
    {
        Ceil& c = *It;
        for (char i = 0; i < 4; i++ )
            if ( prev_set.end() == prev_set.find(c[i]) )
                free_set.insert( c[i] );
    }

    for (char i = 0; i < 4; i++ )
    {
        if ( pattern[i] == 0 ) 
        {
            if ( free_set.size() == 0 )
                return UserInput();
            newui = *free_set.begin();
            free_set.erase( free_set.begin() );
        }
        else 
        {
            if( pattern[i] == 1 )
                newui.at(i) = history.back()[0];
            else if( pattern[i] == 2 )
                newui.at(i) = history.back()[1];
            else if( pattern[i] == 3 )
                newui.at(i) = history.back()[2];
            else if( pattern[i] == 4 )
                newui.at(i) = history.back()[3];
        }
    }
    return newui;*/
    return UserInput();
}

///////////////////////////////////////////////////////////

UserInput::UserInput() 
    : Ceil(), guess(-1), inplace(-1)
{}

UserInput::UserInput(const Ceil& ceil) 
    : Ceil(ceil), guess(-1), inplace(-1)
{}

UserInput::UserInput(const UserInput& ui) 
    : Ceil(ui), guess(ui.guess), inplace(ui.inplace)
{
    history = ui.history;
    min_variants = ui.min_variants;
}

UserInput::UserInput(int from_int) 
    : Ceil(from_int), guess(-1), inplace(-1)
{}

UserInput::UserInput(const char* pstr) 
    : Ceil(pstr), guess(-1), inplace(-1)
{}

UserInput::UserInput(char str[4]) 
    : Ceil(str), guess(-1), inplace(-1)
{}

UserInput::UserInput( const Ceil& ceil, int n, int p ) 
    : Ceil(ceil), guess(n), inplace(p)
{}

UserInput::UserInput(int from_int, int n, int p ) 
    : Ceil(from_int), guess(n), inplace(p)
{}

UserInput::UserInput(const char* pstr, int n, int p ) 
    : Ceil(pstr), guess(n), inplace(p)
{}

UserInput::UserInput(int gs,int plc)
: Ceil(), guess(gs), inplace(plc)
{}

bool UserInput::operator < (const UserInput& r) const
{
    int stage = history.size();
    int r_stage = r.history.size();

    if ( stage == 0 && r_stage == 0 ) 
        return (guess == r.guess ? inplace < r.inplace : guess < r.guess);

    if ( stage == r_stage ) 
        return ( *history.begin() < *r.history.begin() );

    return stage < r_stage;
}

bool UserInput::empty() const
{
    return ( 
        history.size() == 0 && 
        min_variants.size() == 0 && 
        Ceil(*this).empty() 
        );
}

void UserInput::create_predicat()
{
    if( !guess_predicat.empty() && !inplace_predicat.empty() )
        return;

    if( guess == 0 || guess == 4 )
        guess_predicat.insert( *this );

    if( inplace == 4 ) {
        inplace_predicat.insert( *this );
        return;
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

    if( inplace == 0 || inplace == 1 )
        inplace_predicat = all_1;
    if( inplace == 2 )
        inplace_predicat = all_2;
    if( inplace == 3 )
        inplace_predicat = all_3;
}

CeilsT& UserInput::get_variants()
{
    variants.clear();
    create_predicat();

    for( CeilsT::iterator It = invariants.begin(); It != invariants.end(); ++It )
    {
        PredicatT::iterator gsIt = guess_predicat.begin(); 
        PredicatT::iterator pcIt = inplace_predicat.begin();

        if( guess == 0 && gsIt->saticfy_or( *It ) )
            continue;

        if ( inplace == 0 ) 
        {
            for ( ; pcIt != inplace_predicat.end(); pcIt++ )
                if ( pcIt->saticfy_and( *It ) )
                    break;
            if ( pcIt != inplace_predicat.end() )
                continue;
        }

        if ( guess ) 
        {
            bool saticfy = false;
            for( ; gsIt != guess_predicat.end(); gsIt++ )
                if( gsIt->saticfy_xor( *It ) )
                {
                    saticfy = !saticfy;
                    if( !saticfy )
                        break;
                }
            if( !saticfy )
                continue;
        }

        if ( inplace ) 
        {
            bool saticfy = false;
            for( ; pcIt != inplace_predicat.end(); pcIt++ )
                if( pcIt->saticfy_and( *It ) )
                {
                    saticfy = !saticfy;
                    if( !saticfy )
                        break;
                }
            if( !saticfy )
                continue;
        }
        variants.insert( *It );
    }
    return variants;
}

static CeilsT ordered_set;

/////////////////////////////////////////////////////////////////
CeilsT UserInput::init_ordered_set(short start, short stop)
{
    CeilsT gen;
    while( start <= stop )  {
        Ceil c = start;
        if( c[0] != c[1] && c[0] != c[2] && c[0] != c[3] &&
             c[1] != c[2] && c[1] != c[3] && c[2] != c[3] )
        {
            gen.insert(start);
        }
        start++;
    }
    return gen;
}

/////////////////////////////////////////////////////////////////
void UserInput::search_minimals(FILE* f)
{
    if ( !min_variants.empty() )
        return;

    int counter = 0, minsum = 5039;
    Ceil& own = *this;
    if ( f )
        fprintf(f, "\t%d:%d ", guess, inplace);
    else
        printf("\t%d:%d ", guess, inplace);


    int num[4][10] = {0};
    for( CeilsT::iterator It = invariants.begin(); It != invariants.end(); ++It )
    {
        Ceil& c = *It;
        for (char i = 0; i < 4; i++ )
        {
            num[i][c[i]]++;
        }
    }

    char c[5] = {0};
    for(c[0] = 48; c[0] < 58; ++c[0] ) 
    {
        if ( num[0][c[0]-48] == 0 )
            continue;
        for(c[1] = 48; c[1] < 58; ++c[1] ) 
        {
            if( c[1] == c[0] || num[1][c[1]-48] == 0 )
                continue;
            for(c[2] = 48; c[2] < 58; ++c[2] ) 
            {
                if( c[2] == c[1] || c[2] == c[0] || num[2][c[2]-48] == 0 )
                    continue;
                for(c[3] = 48; c[3] < 58; ++c[3] )
                {
                    if ( c[3] == c[2] || c[3] == c[1] || c[3] == c[0] || num[3][c[3]-48] == 0 )
                        continue;
                    if( counter++ % 504 == 0 )
                       printf(".");

                    UserInput ui( c, guess, inplace );
                    CeilsT vars = ui.get_variants();
                    if ( (vars.size() == 1) && (*vars.begin() == *this) )
                        continue;
                    if ( vars.size() == 0 || ui.empty() )
                        continue;
                    if ( minsum > (int)vars.size() )
                    {
                        minsum = vars.size();
                        min_variants.clear();
                        min_variants.insert( MinimalsT::value_type( (int)ui, minsum ) );
                    }
                    else if ( minsum == vars.size() )
                        min_variants.insert( MinimalsT::value_type( (int)ui, vars.size() ) );
                }
            }
        }
    }
    if (f)
        fprintf(f, " %d variants brings %d steps\n", min_variants.size(), min_variants.empty() ? 0 : min_variants.begin()->second);
    else
        printf(" %d variants brings %d steps\n", min_variants.size(), min_variants.empty() ? 0 : min_variants.begin()->second);
}

void UserInput::get_min_variants_random(std::vector<int>& digits, FILE* f )
{
    if ( min_variants.size() == 0 ) 
        return;
    int nums = min_variants.size();
    if ( !f )
        nums = min( min_variants.size(), 8 );
    MinimalsT::const_iterator It = min_variants.begin();
    while( nums && It != min_variants.end() )
    {
        int sz = min_variants.size();
        if ( !f && sz > 8 ) 
        {
            int rand_id = 0;
            if (sz > 1) 
                rand_id = rand() % sz;
            It = min_variants.begin();
            while( --rand_id > 0 ) It++;
            digits.push_back( It->first );
        }
        else {
            digits.push_back( It->first );
            It++;
        }
        
        nums--;
    }
}
