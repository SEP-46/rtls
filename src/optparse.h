#pragma once

#include <vector>
#include <string>
#include <unordered_map>

class OptParse
{
public:
	// Adds an option with an optional argument to parse
	// Default value is only used when the user does not specify an argument for the option
	OptParse& AddArgOption( const std::string& longName, const std::string& defaultVal = "" )
	{
		AddArgOption( longName, NO_SHORTNAME, defaultVal );
		return *this;
	}

	// Adds an option with an optional argument to parse with a shortname
	// Default value is only used when the user does not specify an argument for the option
	OptParse& AddArgOption( const std::string& longName, char shortName, const std::string& defaultVal = "" )
	{
		mOptions.push_back( { longName, shortName, defaultVal, true } );
		return *this;
	}

	// Adds an option that is used as a flag (ie. no arguments should be parsed for it)
	OptParse& AddFlagOption( const std::string& longName, char shortName = NO_SHORTNAME )
	{
		mOptions.push_back( { longName, shortName, "", false } );
		return *this;
	}

	void Process( int argc, const char** argv )
	{
		mOptionArgs.clear();
		mStrayStrings.clear();

		for( int i = 1; i < argc; i++ )
		{
			size_t argv_len = strlen( argv[i] );
			if( argv_len >= 3 && argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] != ' ' )
			{
				i = ProcessLongOption( i, argc, argv );
			}
			else if( argv_len >= 2 && argv[i][0] == '-' && argv[i][1] != ' ' )
			{
				i = ProcessShortOption( i, argc, argv );
			}
			else
			{
				AddStrayString( argv[i] );
			}
		}
	}

	// Gets number of args that aren't an argument to any options
	size_t GetArgC() const { return mStrayStrings.size(); }
	// Gets argument at specified index (args that aren't an argument to any options)
	std::string GetArg( int index ) const { return mStrayStrings[index]; };

	// Returns whether the specified option exists
	bool Exists( const std::string& opt ) const
	{
		auto it = mOptionArgs.find( opt );
		if( it == mOptionArgs.end() )
		{
			return false;
		}

		return true;
	}
	// Returns the argument for the option if it exists, or nullptr if it doesn't
	const char* Get( const std::string& opt ) const
	{
		auto it = mOptionArgs.find( opt );
		if( it == mOptionArgs.end() )
		{
			return nullptr;
		}

		return it->second.c_str();
	}
	// Returns the argument for the option if it exists, or nullptr if it doesn't
	const char* operator[]( const std::string& opt ) const
	{
		return Get( opt );
	}
private:
	int ProcessLongOption( int index, int argc, const char** argv )
	{
		std::string option = &argv[index][2]; // get the option without the -- at the start
		for( const Option& o : mOptions )
		{
			if( o.longName == option )
			{
				if( index + 1 < argc &&
					o.hasArgument &&
					argv[index + 1][0] != '-' )
				{
					mOptionArgs[option] = argv[++index];
				}
				else
				{
					mOptionArgs[option] = o.defaultVal;
				}

				break;
			}
		}

		return index;
	}
	int ProcessShortOption( int index, int argc, const char** argv )
	{
		std::string shortOptions = &argv[index][1];
		for( size_t i = 0; i < shortOptions.size(); i++ )
		{
			for( const Option& o : mOptions )
			{
				if( o.shortName == shortOptions[i] )
				{
					// No args allowed for multiple short args
					// e.g. -abcd
					if( shortOptions.size() == 1 &&
						o.hasArgument &&
						index + 1 < argc &&
						argv[index + 1][0] != '-' )
					{
						mOptionArgs[o.longName] = argv[++index];
					}
					else
					{
						mOptionArgs[o.longName] = o.defaultVal;
					}

					break;
				}
			}
		}

		return index;
	}

	void AddStrayString( const std::string& stray )
	{
		mStrayStrings.push_back( stray );
	}
private:
	static constexpr char NO_SHORTNAME = 0;

	struct Option
	{
		std::string longName;
		char shortName;
		std::string defaultVal;
		bool hasArgument;
	};

	std::vector<Option> mOptions;
	std::vector<std::string> mStrayStrings;
	std::unordered_map<std::string, std::string> mOptionArgs;
};
