#ifndef YASP_DETAIL_H
#define YASP_DETAIL_H

#include <functional>
#include <type_traits>

namespace detail
{
	template < typename T > struct deduce_type;

	template < typename R, typename C, typename... Args >
	struct deduce_type<R(C::*)(Args...) const >
	{
		using type = std::function<R(Args...)>;
	};
}

#endif