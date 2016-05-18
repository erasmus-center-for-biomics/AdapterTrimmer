#ifndef sequtils_streamcounter_hpp
#define sequtils_streamcounter_hpp

// STL
#include <cstddef>
#include <iostream>
#include <mutex>

//
// This file contains wrappers for stream with added counters.
//

namespace rwwb {

namespace sequtils {

	class IStreamCounter {
	public:
		std::istream& handle ;
		std::size_t counter ;

		IStreamCounter(std::istream& h): handle(h), counter(0) {}
	} ;

	class ProtectedIStreamCounter: public IStreamCounter {
	public:
		std::mutex mutex ;
		ProtectedIStreamCounter(std::istream& h): IStreamCounter(h), mutex() {}
	} ;


	class OStreamCounter {
	public:
		std::ostream& handle ;
		std::size_t counter ;

		OStreamCounter(std::ostream& h): handle(h), counter(0) {}
	} ;

	
	class ProtectedOStreamCounter: public OStreamCounter {
	public:
		std::mutex mutex ;
		ProtectedOStreamCounter(std::ostream& h): OStreamCounter(h), mutex() {}
	} ;


}

}

#endif