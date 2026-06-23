//cred_man.cpp
#include "blake2.h"
#include "aes256.h"
#include <fstream>
#include <string>
#include <iostream>

bool is_stream_open = false;

//HARDCODED - YOU CAN CHANGE IT FOR YOUR OWN SALT
std::string SALT_CREDS = "N7s97s&Tnpf9&(*)697pbDF&P9bSFO*&9t7nos7notfs*O89ST(fs7g&r9sd53Y#@3sTg0"; 

byte keyBytes[32] = {
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
    0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,
    0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F
};

byte ivBytes[16] = {
    0x0F,0x0E,0x0D,0x0C,
    0x0B,0x0A,0x09,0x08,
    0x07,0x06,0x05,0x04,
    0x03,0x02,0x01,0x00
};

// open login file
short open_login_file(char mode)
{
    try
    {
        if (mode == 'r')
        {
            std::fstream str("CRED.LGN", std::ios::in);
            if (!str.is_open()) return -1;           // open failed
            is_stream_open = true;
            return 0;
        }
        else if (mode == 'f')
        {
            std::fstream str("CRED.LGN", std::ios::in | std::ios::out | std::ios::app);
            if (!str.is_open()) return -1;           // open failed
            is_stream_open = true;
            return 0;
        }
        else
        {
        	std::cout << "Invalid mode provided! Programmer error! \n";
            return -2; // invalid mode
        }
    }
    catch (const std::ios_base::failure&)
    {
        is_stream_open = false;
        std::cout << "Stream/IO exception! Check if program has permission to access the file! \n";
        return -3; // stream/IO exception
    }
    catch (...)
    {
        is_stream_open = false;
        std::cout << "Unknown error! \n";
        return -4; // unknown error
    }
}

// write to login file
short write_login_file(char mode, const std::string& data)
{
    try
    {
        std::fstream str;

        if (mode == 'r') // read
        {
            str.open("CRED.LGN", std::ios::in);
        }
        else if (mode == 'f') // full
        {
            str.open("CRED.LGN", std::ios::in | std::ios::out | std::ios::app);
        }
        else
        {
            return -2; // invalid mode
        }

        if (!str.is_open()) return -1; // open failed

        // optionally throw on bad writes
        // str.exceptions(std::ios::badbit | std::ios::failbit);

        str << data;
        if (!str.good()) return -5; // write failed

        is_stream_open = true;
        return 0;
    }
    catch (const std::ios_base::failure&)
    {
        is_stream_open = false;
        return -3; // stream/IO exception
    }
    catch (...)
    {
        is_stream_open = false;
        return -4; // unknown error
    }
}

// function called from BUTTON_CB for actually saving password	
short save_password_from_text_field(std::string plaintext_password, std::string plaintext_username, std::short role)
{
	if (is_stream_open)
	{
		std::string writeline = blake2s_hash(plaintext_username + "," + plaintext_password + "," + role + "," + SALT_CREDS);
		short rc = write_login_file('f', writeline);
    	if (rc != 0) {
        	std::cout << "write_login_file failed with code " << rc << "\n";
        	return rc;
    	}
    	else
    	{
    		std::cout << "Function ran succesfully (no exit code) \n";
		}
		
	}
	else if (!is_stream_open)
	{
		std::cout << "Filestream is not open! Function cannot proceed! \n";
		return -1; //code for filestream not open
	}
	else 
	{
		std::cout << "Unknown filestream state! Function did not run... \n";
		return -2; //code for unknown filestream state
	}
}

// function for checking if string in file exists
bool file_contains_string(const std::string& searched_string)
{
    if (!is_stream_open) return false;

    std::string line;
    while (std::getline(str, line))
    {
        if (line.find(searched_string) != std::string::npos)
            return true;
    }
    return false;
}

// function that gets user input and: 1. gets hash 2. checks if it exists in CRED.LGN
bool verify_login(std::string plaintext_password, std::string plaintext_username, std::short role)
{
	
}




