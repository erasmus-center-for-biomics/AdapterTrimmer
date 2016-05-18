#ifndef sequence_matcher_hpp
#define sequence_matcher_hpp

// STL
#include <cstddef>
#include <vector>

namespace Biomics {

    //
    // An iterative matching algorithm
    //
    //            
    template<typename T>
    std::size_t vector_matcher(const std::vector<T>& subject, const std::vector<T>& query, std::size_t mismatch_threshold, std::size_t minimum_matches) {
        
        //
        std::size_t match_location = subject.size() ;                                     
        std::size_t mismatches = 0 ;            
        std::size_t matches = 0 ;
        bool ismatch = false ;
        
        for(std::size_t i=0; i<subject.size(); ++i){
            if(subject[i] == query[0]) {
                
                // determine where to match
                ismatch = true ;                    
                mismatches = 0 ;
                matches = 0 ;                     
                for(std::size_t j=i; j<subject.size(); ++j) {
                    
                    if(j-i >= query.size())                         
                        break ;                        
                    
                    if(subject[j] != query[j-i]) {
                        mismatches += 1 ;
                        if(mismatches >= max_threshold){
                            ismatch = false ;
                            break ;
                        }
                    } else {
                        matches += 1 ; 
                    }
                }
                
                // check whether the sequence matches 
                if(ismatch && matches > minimum_matches){
                    match_location = i ;
                    break ;                        
                }                    
            }
        }
        
        //
        return match_location ;
    }
    
    template<typename T>
    class SequenceMatcher {
                    
        std::vector<T> sequence ;
        std::size_t maximum_mismatches ;
        std::size_t minimum_matches ;
        
      public:
        // Primary constructor
        //
        // 
        SequenceMatcher(std::vector<T> seq):sequence(seq), maximum_mismatches(1), minimum_matches(1) {}
        
        // Secondary constructor 
        //
        //
        SequenceMatcher(std::vector<T> seq, std::size_t threshold):sequence(seq), maximum_mismatches(threshold){}
        
        
        // Determines the point in the vector after which the sequence ought to be trimmed. 
        //
        //           
        std::size_t match(const std::vector<T>& s) const {
            return vector_matcher<T>(s, sequence, maximum_mismatches, minimum_matches) ;
        }
                            
    } ;
        
} ;
#endif