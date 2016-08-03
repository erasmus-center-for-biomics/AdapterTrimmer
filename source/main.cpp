
// STL
#include <cstddef>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>

// BOOST libraries
#include <boost/program_options.hpp>

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
    std::istream hin ;
    std::ostream hout ;
    
    // the input parameters 
    std::string file_input = "-" ;
    std::string file_output = "-" ;
    std::string file_adapters = "-" ;
    std::vector<std::string> adapter_sequences(0) ;
    std::size_t maximum_mismatches = 2 ;
    std::size_t minimum_matches = 1 ;
    std::size_t minimum_bases_remaining = 25 ;    
    std::size_t buffer_size = 1000 ;
        
    // parse the command-line parameters
    boost::program_options::options_description desc("Allowed options") ;
    desc.add_options()
        ("help", "Produce the help message")
        ("input,i", boost::program_options::value<std::string>(&file_input), "The input FastQ file")
        ("output,o", boost::program_options::value<std::string>(&file_output), "The output FastQ file")
        ("adapter,a", boost::program_options::value< std::vector<std::string> >(&adapter_sequences), "The adapter sequence to trim")
        ("adapter-file,f", boost::program_options::value<std::string>(&file_adapters), "A file with adapter sequences")
        ("maximum-mismatches", boost::program_options::value<std::size_t>(&maximum_mismatches), "The maximum number of mismatches")
        ("minimum-matches", boost::program_options::value<std::size_t>(&minimum_matches), "The minimum number of matching bases")
        ("minimum-bases-remaining", boost::program_options::value<std::size_t>(&minimum_bases_remaining), "The minimum number of bases in the reads remaining")
        ("buffer-size", boost::program_options::value<std::size_t>(&buffer_size), "The size of the read buffer to process") ;    
    boost::program_options::positional_options_description p ;
	boost::program_options::variables_map vm ;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(desc).positional(p).run(), vm) ;
	boost::program_options::notify(vm) ;
    
    // check the command-line arguments
    if( vm.count("help") ) {
		std::cerr << "Usage" << std::endl ; 
		std::cerr << desc << std::endl; 
		return 0 ;
	}    
    if(file_input == "-"){            
        hin = std::cin ;
    } else { 
        hin = (std::istream) std::ifstream(file_input, std::ifstream::in) ;
    }   
    if(file_output == "-"){
        hout = std::cout ;
    } else {
        hout = (std::ostream) std::ofstream(file_output, std::ifstream::out) ;
    }
    if(file_adapters != "-"){
        
    }      
    
    // initialize the adapters
    std::vector<Biomics::SequenceMatcher<rwwb::sequtils::base_t> > adapters ;
    for(std::size_t i=0; i<adapter_sequences.size(); ++i){
        adapters.push_back(Biomics::SequenceMatcher<rwwb::sequtils::base_t>(rwwb::sequtils::string_to_base(adapter_sequences[i]), maximum_mismatches, minimum_matches)) ;    
    }        
    
    // process the reads from standard in 
    return_code = process_reads(hin, hout, adapters, buffer_size, minimum_bases_remaining) ;
      
      
    // cleanup the opened files
    if(file_input == "-"){
        hin.close() ;
    }
    if(file_output != "-"){
        hout.close() ;
    }
          
    //
    return return_code ;
}