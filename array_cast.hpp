/* The following code declares helper function array_cast<...>(...)
 *
 * (C) Copyright Herve Martin 2009.
 *
 * 13 Mar 2009 - Initial Revision (Herve Martin)
 *
 * 13 Mar, 2009
 */

#ifndef ARRAY_CAST_HPP
#define ARRAY_CAST_HPP

#include <boost/assert.hpp>
#include <cstddef>

namespace utils {

	/* definition of the array_cast<T,N>( INPUT& data ) function.
	*
	* PURPOSE: cast any data of INPUT type into a C-style array of T type items which is N large.
	* 
	* COMMENT: most of the time N should take value sizeof(INPUT)/sizeof(T) to have the size of the result array matching the size of the INPUT data.
	* ( assuming that sizeof(INPUT) is divisible by sizeof(T) )
	* in that case use of array_cast<T>(INPUT& data) function should be preferred.
	*/
	template<class T, std::size_t N, class INPUT> T (&array_cast( INPUT& data ))[N]
	{
		return *(T(*)[N]) &data;
	}
	template<class T, std::size_t N, class INPUT> const T (&array_cast( const INPUT& data ))[N]
	{
		return *(const T(*)[N]) &data;
	}
	template<class T, std::size_t N, class INPUT> volatile T (&array_cast( volatile INPUT& data ))[N]
	{
		return *(volatile T(*)[N]) &data;
	}
	template<class T, std::size_t N, class INPUT> const volatile T (&array_cast( const volatile INPUT& data ))[N]
	{
		return *(const volatile T(*)[N]) &data;
	}

	/* definition of the array_cast<N>(INPUT& data) function and its derivatives.
	*
	* PURPOSE: cast any data of INPUT type into a C-style array of byte (unsigned char) items which is N large.
	* 
	* COMMENT: most of the time N should take value sizeof(INPUT)
	* to make matching size of INPUT data and the size of the resulting array and
	* in that case use of array_cast(INPUT& data) function should be preferred.
	*/ 
	template<std::size_t N, class INPUT> unsigned char (&array_cast( INPUT& data ))[N]
	{
		return *(unsigned char(*)[N]) &data;
	}
	template<std::size_t N, class INPUT> const unsigned char (&array_cast( const INPUT& data ))[N]
	{
		return *(const unsigned char(*)[N]) &data;
	}
	template<std::size_t N, class INPUT> volatile unsigned char (&array_cast( volatile INPUT& data ))[N]
	{
		return *(volatile unsigned char(*)[N]) &data;
	}
	template<std::size_t N, class INPUT> const volatile unsigned char (&array_cast( const volatile INPUT& data ))[N]
	{
		return *(const volatile unsigned char(*)[N]) &data;
	}

	/* definition of the array_cast<T>(INPUT& data) function and its derivatives.
	*
	* PURPOSE: cast any data of INPUT type into a C-style array of T type items which will be sizeof(INPUT)/sizeof(T) large.
	* sizeof(INPUT) shall be divisible by sizeof(T),
	* if not, array_cast<T,N> can rather be used to force the size of the resulting array
	* ( thus it can be possible to choose if the resulting array will be shorter or larger than the INPUT data)
	*/ 
	template<class T, class INPUT> T (&array_cast( INPUT& data ))[sizeof(INPUT)/sizeof(T)]
	{
		// sizeof(INPUT) shall be divisible by sizeof(T), see comment above for further details.
		BOOST_STATIC_ASSERT(( ::boost::math::static_gcd<sizeof(INPUT), sizeof(T)>::value == sizeof(T) ));
		return *(T(*)[sizeof(INPUT)/sizeof(T)]) &data;
	}
	template<class T, class INPUT> const T (&array_cast( const INPUT& data ))[sizeof(INPUT)/sizeof(T)]
	{
		// sizeof(INPUT) shall be divisible by sizeof(T), see comment above for further details.
		BOOST_STATIC_ASSERT(( ::boost::math::static_gcd<sizeof(INPUT), sizeof(T)>::value == sizeof(T) ));
		return *(const T(*)[sizeof(INPUT)/sizeof(T)]) &data;
	}
	template<class T, class INPUT> volatile T (&array_cast( volatile INPUT& data ))[sizeof(INPUT)/sizeof(T)]
	{
		// sizeof(INPUT) shall be divisible by sizeof(T), see comment above for further details.
		BOOST_STATIC_ASSERT(( ::boost::math::static_gcd<sizeof(INPUT), sizeof(T)>::value == sizeof(T) ));
		return *(volatile T(*)[sizeof(INPUT)/sizeof(T)]) &data;
	}
	template<class T, class INPUT> const volatile T (&array_cast( const volatile INPUT& data ))[sizeof(INPUT)/sizeof(T)]
	{
		// sizeof(INPUT) shall be divisible by sizeof(T), see comment above for further details.
		BOOST_STATIC_ASSERT(( ::boost::math::static_gcd<sizeof(INPUT), sizeof(T)>::value == sizeof(T) ));
		return *(const volatile T(*)[sizeof(INPUT)/sizeof(T)]) &data;
	}

	/* definition of the array_cast(INPUT& data) function and its derivatives.
	*
	* PURPOSE: cast any data of INPUT type into a C-style array of byte (unsigned char) items which will be sizeof(INPUT) large.
	*/ 
	template<class INPUT> unsigned char (&array_cast( INPUT& data ))[sizeof(INPUT)]
	{
		return *(unsigned char(*)[sizeof(INPUT)]) &data;
	}
	template<class INPUT> const unsigned char (&array_cast( const INPUT& data ))[sizeof(INPUT)]
	{
		return *(const unsigned char(*)[sizeof(INPUT)]) &data;
	}
	template<class INPUT> volatile unsigned char (&array_cast( volatile INPUT& data ))[sizeof(INPUT)]
	{
		return *(volatile unsigned char(*)[sizeof(INPUT)]) &data;
	}
	template<class INPUT> const volatile unsigned char (&array_cast( const volatile INPUT& data ))[sizeof(INPUT)]
	{
		return *(const volatile unsigned char(*)[sizeof(INPUT)]) &data;
	}

} /* namespace utils */

#endif /*ARRAY_CAST_HPP*/
