#include "chifer.h"

std::string FileName = "./opt.bin";

void Storage::Save()
{
    FILE* f = 0; 
    fopen_s( &f, FileName.c_str(), "wb" );
    if ( f == 0 )
        return;

    // Number of probes what we have
    int num_probes = probes.size();
    fwrite( &num_probes, 4, 1, f );

    std::set<UserInput>::iterator It = probes.begin();
    for( ; It != probes.end(); ++It ) 
    {
        // Saving probe
        UserInput& ui = *It;
        // probe hints
        int record_guess   = ui.guess;
        fwrite( &record_guess, 4, 1, f );
        int record_inplace = ui.inplace;
        fwrite( &record_inplace, 4, 1, f );

        // number of history records
        int num_records = ui.history.size();
        fwrite( &num_records, 4, 1, f );

        HistoryT::iterator It2 = ui.history.begin();
        for( ; It2 != ui.history.end(); ++It2 )
        {
            UserInput& record = *It2;
            // history record value
            int record_value = record;
            fwrite( &record_value, 4, 1, f );
            // history record hints
            record_guess = record.guess;
            fwrite( &record_guess, 4, 1, f );
            record_inplace = record.inplace;
            fwrite( &record_inplace, 4, 1, f );
        }

        // number of digits with minimal invariance
        int min_size = ui.min_variants.size();
        fwrite( &min_size, 4, 1, f );

        // invariance value
        int min_value = 0;
        if ( min_size )
            min_value = ui.min_variants.begin()->second;
        fwrite( &min_value, 4, 1, f );

        // save digits
        MinimalsT::iterator It3 = ui.min_variants.begin();
        int* buf = new int[min_size];
        int* ptr = buf;
        for( ; It3 != ui.min_variants.end(); ++It3 )
            *(ptr++) = It3->first;
        _fwrite_nolock( buf, 4, min_size, f );
        _fflush_nolock(f);
        delete[] buf;
    }
    fclose(f);
}

void Storage::Load()
{
    FILE* f = 0; 
    fopen_s( &f, FileName.c_str(), "rb" );
    if ( f == 0 )
        return;

    // Number of probes what we have
    int num_probes = 0;
    fread( &num_probes, 4, 1, f );

    while( num_probes ) 
    {
        // probe hints
        int record_guess = -1;
        fread( &record_guess, 4, 1, f );
        int record_inplace = -1;
        fread( &record_inplace, 4, 1, f );

        UserInput ui(record_guess, record_inplace);

        // number of history records
        int num_records = 0;
        fread( &num_records, 4, 1, f );

        while( num_records )
        {
            // history record value
            int record_value = -1111;
            fread( &record_value, 4, 1, f );
            // history record hints
            record_guess = -1;
            fread( &record_guess, 4, 1, f );
            record_inplace = -1;
            fread( &record_inplace, 4, 1, f );

            ui.history.push_back( UserInput( record_value, record_guess, record_inplace ) );
            num_records--;
        }

        // number of digits with minimal invariance
        int min_size = 0;
        fread( &min_size, 4, 1, f );

        // invariance value
        int min_value = 0;
        fread( &min_value, 4, 1, f );

        // read digits
        int *buf = new int[min_size];
        unsigned int page = (unsigned int)min_size;
        unsigned int readed = 0;
        while( page ) 
        {
            readed = _fread_nolock( buf + readed, 4, page, f);
            page -= readed;
        }

        for( int i = 0; i < min_size; i++ )
            ui.min_variants.insert( MinimalsT::value_type( buf[i], min_value ) );
        delete[] buf;

        if ( !ui.empty() )
            probes.insert( ui );
        num_probes--;
    }

    fclose(f);
}
