#ifndef sequtils_fastq_hpp
#define sequtils_fastq_hpp

// STL
#include <cstddef>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

// own headers
#include <rwwb/sequtils/types.hpp> 
#include <rwwb/sequtils/streamcounter.hpp> 

namespace rwwb {

namespace sequtils {
	
	bool fastq(std::istream &h, std::string &nm, std::vector<base_t> &seq, std::string &qual) {
		std::string l = "" ;
		
		// name
		nm = std::getline( h, l ).good() && !l.empty() ? l.substr(1) : "" ;
		if( nm == "" ) 
			return false ;

		// sequence 
		if( std::getline(h, l).good() ) {
			seq = std::vector<base_t>( l.size(), -1 ) ;
			std::transform(l.begin(), l.end(), seq.begin(), char_to_base ) ;
		} else {
			return false ;
		}

		// +
		if( ! std::getline( h, l ).good() ) {		
			return false ;
		}

		// quality
		if( std::getline( h, l ).good() ) {
			qual = l ;
		} else { 
			return false ;
		}

		return true ;
	}

	//
	// A continuous reader with minimal copies
	//
	//
	//

	typedef struct __read__ {
		std::size_t uid ;
		std::string name ;
		std::vector<base_t> sequence ;
		std::string quality ;
	} read ;

	//
	// Get multiple reads from a FastQ file. Rhe number of reads 
	//   to obtain is determined  by the size of reads.
	//
	// 

	std::size_t reads_from_fastq(std::istream& handle, std::size_t& counter, std::vector<read>& reads) {
		
		// the number of reads obtained
		std::size_t rval = counter ;
		
		// the l variable
		std::string l = "" ;

		for(std::size_t i=0; i<reads.size(); ++i) {
			
			// read header
			if(!std::getline(handle, l).good())
				reads[i].name = l.substr(1) ;
			
			// sequence
			if(!std::getline(handle, l).good())
				break ;

			reads[i].sequence.resize(l.size()) ;
			std::transform(l.begin(), l.end(), reads[i].sequence.begin(), char_to_base ) ;

			// +
			if(!std::getline(handle, l).good())
				break ;

			if(!std::getline(handle, l).good())
				reads[i].quality = l ;

			// set the uid
			counter += 1 ;
			reads[i].uid = counter ;
			
			std::cerr << reads[i].uid << "\t" << reads[i].name << std::endl ;
		}

		// return the number of reads from a fastq.
		return counter - rval ;
	}

	std::size_t reads_from_fastq(IStreamCounter& current, std::vector<read>& reads) {
		return reads_from_fastq(current.handle, current.counter, reads);
	}

	std::size_t reads_from_fastq(ProtectedIStreamCounter& current, std::vector<read>& reads) {
		std::lock_guard<std::mutex> lock(current.mutex) ;
		return reads_from_fastq(current.handle, current.counter, reads);
	}

} ;
} ;
#endif