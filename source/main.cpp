
// STL
#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

// BOOST libraries

// own libraries
#include <sequence_matcher.hpp>
#include <rwwb/sequtils/types.hpp>
#include <rwwb/sequtils/fastq.hpp>

// Processes the reads in the input stream and writes them to the output stream
//
//
int process_reads(std::istream& hin, std::ostream& hout, const std::vector< Biomics::SequenceMatcher<rwwb::sequtils::base_t> >& adapters, std::size_t buffersize, std::size_t min_bases_left) {
    
    // prepare the read buffer
    rwwb::sequtils::read default_read = rwwb::sequtils::read() ;    
    default_read.uid = 0 ;
    default_read.name = "" ;
    default_read.sequence = std::vector<rwwb::sequtils::base_t>(0,0) ;
    default_read.quality = "" ;    
    std::vector<rwwb::sequtils::read> buffer = std::vector<rwwb::sequtils::read>(buffersize, default_read) ;     
    
    std::size_t reads_loaded = 0 ;
    std::size_t bases_left = 0 ; 
    
    // read entries from the input stream
    while(true) {
        //
        std::size_t obtained = rwwb::sequtils::reads_from_fastq(hin, reads_loaded, buffer) ;
        
        // process the reads
        for(std::size_t i=0; i<obtained; ++i){
            bases_left = buffer[i].sequence.size() ;
            
            // match the adapters
            for(std::size_t k=0; k<adapters.size(); ++k) {            
                std::size_t tmp = adapters[k].match(buffer[i].sequence) ;
                if(tmp < bases_left) {
                    bases_left = tmp ;
                }
            }
            
            // trim if necessary
            if(bases_left < buffer[i].sequence.size()) {
                
                // If too few bases are left 
                // 
                if(bases_left <= min_bases_left) {
                    // replace the sequence by N's
                    buffer[i].sequence = std::vector<rwwb::sequtils::base_t>(buffer[i].sequence.size(), -1) ;
                    buffer[i].quality = std::string(buffer[i].sequence.size(), '!') ;
                } else {
                    
                    // otherwise truncate the read                    
                    buffer[i].sequence = std::vector<rwwb::sequtils::base_t>(buffer[i].sequence.begin(), buffer[i].sequence.begin() + bases_left) ;
                    buffer[i].quality = std::string(buffer[i].quality.begin(), buffer[i].quality.begin() + bases_left) ;
                }
            }
        }
        
        // write the buffer to the output
        for(std::size_t i=0; i<obtained; ++i){
            std::vector<char> tmp(buffer[i].sequence.size(), 'N') ;
            std::transform(buffer[i].sequence.begin(), buffer[i].sequence.end(), tmp.begin(), rwwb::sequtils::base_to_char) ;            
            hout << "@" << buffer[i].name << std::endl 
                << std::string(tmp.begin(), tmp.end()) << std::endl 
                << "+" << std::endl 
                << buffer[i].quality << std::endl ; 
        }
        
        // if we didn't get the expected number of reads move on
        if(obtained != buffersize) 
            break ;                
    }   
    return 0 ;
}


//
// the main program loop
//
//
int main(int argc, char** argv) {
    //
    int return_code = 0 ;
    
    // initialize the adapters
    std::vector<Biomics::SequenceMatcher<rwwb::sequtils::base_t> > adapters ;
    adapters.push_back(
        Biomics::SequenceMatcher<rwwb::sequtils::base_t>(rwwb::sequtils::string_to_base("AGATCGGAAGAG"), 1, 1)
        ) ;
    
    // 
    return_code = process_reads(std::cin, std::cout, adapters, 1000, 25) ;
      
    //
    return return_code ;
}