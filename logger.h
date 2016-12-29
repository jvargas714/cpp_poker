#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>  // ofstream
#include <iomanip>  // std::put_time
#include <iostream> // std::cerr 
#include <cstdio>
#include <cstring>  // std::strcat
#include <cstdlib>
#include <ctime>    // std::strftime
#include <memory>   // unique_ptr
#include <chrono>   // std::chrono::system_clock::now() and std::time_point
/*
 * TODO:: set module in logger as well, use #define to do so
 */
// some macros for log prefix
#define D       "::DEBUG::"
#define V       "::VERBOSE"
#define E       "::ERROR::"
#define I       "::INFO::"
#define fstr()  std::string(__func__) + "(): "
#define FN      fstr()

// helper functions
std::string now();

class logger
{
public:
	std::ostream* strm;
	std::string log_filename;
	std::string mode;

	// ctor, cpy ctor, and assignment operator
	logger() : log_filename( "" ), mode( D )
	{
		strm = &std::cout;
	}

	logger( std::string name ) : log_filename( name ), mode( D )
	{
		strm = new std::ofstream( log_filename );
	}

	logger( const logger& lg ) 
	{
		strm 			= lg.strm;
		log_filename 	= lg.log_filename;
		mode 			= lg.mode;
	}

	logger& operator=( const logger& lg )
	{ return *this; }

	~logger()
	{
		std::cerr << "~logger(): flushing and closing " << log_filename << std::endl;
		strm->flush();
		delete strm;
	}

	inline void flush_log( bool new_line = false )
	{
		if ( new_line )
		{
			*strm << "\n";
		}
		strm->flush();
	}

	inline void set_mode( std::string md = "DEBUG" )
	{
		mode = md;
	}

	template<typename toPrint>
	std::ostream& operator<<( const toPrint& msg )
	{
		if( strm == &std::cout )
		{
			std::cout << now() << mode << " " << msg;
		}
		else
		{
			std::ofstream* fstrm = static_cast<std::ofstream*>( strm );
			if ( fstrm->is_open() && mode == "OFF" )
			{
				std::cerr << "Mode is set to OFF, closing log_file: " << log_filename << std::endl;
				fstrm->flush();
				fstrm->close();
			}
			else if ( fstrm->is_open() )
			{
				*fstrm << now() << mode << " " << msg;
				fstrm->flush();
			}
		}
		return *strm;
	}

	inline void log( std::string&& msg, bool log_prefix = true )
	{
		if( strm == &std::cout )
		{
			std::cout << now() << mode << " " << msg;
		}
		else
		{
			std::ofstream* fstrm = static_cast<std::ofstream*>( strm );
			if ( fstrm->is_open() )
			{
				if ( log_prefix )
				{
					*fstrm << now() << mode << " " << msg;
				}
				else
				{
					*fstrm << msg;
				}
			}
			else
			{
				return;
			}
			fstrm->flush();
		}
	}
};
#endif
