#ifndef sequtils_types_hpp
#define sequtils_types_hpp

//
// LICENSE BIOLERPLATE
//
// In this file, the basic types and converters are 
//   defined related to DNA sequences. 
//
// 

#include <cstddef> 
#include <stdexcept>
#include <cstdint>
#include <vector>
#include <string>
#include <algorithm>

namespace rwwb {

namespace sequtils {

	// bases are expressed as uint8_t types
	//  where ACGT is 0,1,2,3 and N is -1 
	typedef int8_t base_t ; 
	
	// Copy to kmer
	//
	//
	inline bool copy_to_kmer( std::vector<base_t>& target, const std::vector<base_t>& source, std::size_t offset, std::size_t size, bool stop_on_N){
		bool rval = false ;
		std::size_t i = offset ; 
		for(size_t j=0; j<size; ++j) {
			
			i = offset + j ;
			if(i>source.size()) {
				throw std::out_of_range("") ;
			}
			
			target[j] = source[i] ;

			if(target[j] < 0){
				rval = true ;

				// direct return on Ns
				if(stop_on_N){
					return rval ;
				}
			}
		}
		return rval ;
	}

	//
	// A converter function to convert characters to bases. 
	//
	//
	//
	inline base_t char_to_base( char x ) {
		base_t r = -1 ;
		switch(x) {
		case 'A':
		case 'a':
			r = 0 ;
			break ;
		case 'C':
		case 'c':
			r = 1 ;
			break ;
		case 'G':
		case 'g':
			r = 2 ;
			break ;
		case 'T':
		case 't':
			r = 3 ;
			break ;
		default: {
				r = -1 ;
				break ;
				 }
		} ;
		return r ;
	}

	
	//
	// A converter function to convert bases to characters. 
	//
	//
	//
	inline char base_to_char( base_t x ) {
		char r ;
		switch(x) {
		case 0:
			r = 'A' ;
			break ;
		case 1:
			r = 'C' ;
			break ;
		case 2:
			r = 'G' ;
			break ;
		case 3:
			r = 'T' ;
			break ;
		default:
			r = 'N' ;
			break ;
		} ;
		return r ;
	}

	//
	// Generates a vector of bases from a string
	//
	inline std::vector<base_t> string_to_base( const std::string seq ) {
		std::vector<base_t> r = std::vector<base_t>( seq.size(), -1  ) ;
		std::transform( seq.begin(), seq.end(), r.begin(), char_to_base ) ;
		return r ;
	}

	//
	//
	//
	struct char_to_qual {
		char_to_qual( uint8_t offset ): offset(offset) {} 
		uint8_t operator()( char v ) const { 
			uint8_t dv = (uint8_t) v ;
			return dv > offset ? dv - offset : 0 ;
		} 
	private:
		uint8_t offset ;
	} ;

} ;
} ;

#endif