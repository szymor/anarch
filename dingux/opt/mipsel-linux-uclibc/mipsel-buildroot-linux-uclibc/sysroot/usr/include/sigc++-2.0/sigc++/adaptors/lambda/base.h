// -*- c++ -*-
/* Do not edit! -- generated file */
#ifndef _SIGC_LAMBDA_BASE_HPP_
#define _SIGC_LAMBDA_BASE_HPP_
#include <sigc++/adaptors/adaptor_trait.h>
#include <sigc++/reference_wrapper.h>
#include <type_traits>

#ifndef SIGCXX_DISABLE_DEPRECATED

namespace sigc {

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// libsigc++'s lambda functions have been removed from the API.
// Some code must be kept until we can break ABI.
/** @defgroup lambdas Lambdas
 * libsigc++ ships with basic lambda functionality and the sigc::group adaptor,
 * which uses lambdas to transform a functor's parameter list.
 *
 * The lambda selectors sigc::_1, sigc::_2, ..., sigc::_7 are used to select the
 * first, second, ..., seventh argument from a list.
 *
 * @par Examples:
 * @code
 * std::cout << sigc::_1(10,20,30); // returns 10
 * std::cout << sigc::_2(10,20,30); // returns 20
 * @endcode
 *
 * Operators are defined so that, for example, lambda selectors can be used as
 * placeholders in arithmetic expressions.
 *
 * @par Examples:
 * @code
 * std::cout << (sigc::_1 + 5)(3); // returns (3 + 5)
 * std::cout << (sigc::_1 * sigc::_2)(7,10); // returns (7 * 10)
 * @endcode
 *
 * If your compiler supports C++11 lambda expressions, they are often a good
 * alternative to libsigc++'s lambda expressions. The following examples are
 * equivalent to the previous ones.
 * @code
 * [] (int x, int, int) -> int { return x; }(10,20,30); // returns 10
 * [] (int, int y, int) -> int { return y; }(10,20,30); // returns 20
 * [] (int x) -> int { return x + 5; }(3); // returns (3 + 5)
 * [] (int x, int y) -> int { return x * y; }(7,10); // returns (7 * 10)
 * @endcode
 *
 * @deprecated Use C++11 lambda expressions or %std::bind() instead.
 */

/** A hint to the compiler.
 * All lambda types publically inherit from this hint.
 *
 * @deprecated Use C++11 lambda expressions instead.
 *
 * @ingroup lambdas
 */
struct lambda_base : public adaptor_base {};

// Forward declaration of lambda.
template <class T_type> struct lambda;

namespace internal {

/** Abstracts lambda functionality.
 * Objects of this type store a value that may be of type lambda itself.
 * In this case, operator()() executes the lambda (a lambda is always a functor at the same time).
 * Otherwise, operator()() simply returns the stored value.
 *
 * @deprecated Use C++11 lambda expressions instead.
 *
 * @ingroup lambdas
 */
template <class T_type, bool I_islambda = std::is_base_of<lambda_base, T_type>::value> struct lambda_core;

/** Abstracts lambda functionality (template specialization for lambda values).
 *
 * @deprecated Use C++11 lambda expressions instead.
 *
 * @ingroup lambdas
 */
template <class T_type>
struct lambda_core<T_type, true> : public lambda_base
{
  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename T_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename T_type::result_type result_type;
  typedef T_type lambda_type;

  result_type
  operator()() const;

  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator ()(T_arg1 _A_1) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
             (_A_1);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_1) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
             (_A_1);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
             (_A_1, _A_2);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
             (_A_1, _A_2);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
             (_A_1, _A_2, _A_3);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
             (_A_1, _A_2, _A_3);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
             (_A_1, _A_2, _A_3, _A_4);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
             (_A_1, _A_2, _A_3, _A_4);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
             (_A_1, _A_2, _A_3, _A_4, _A_5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
             (_A_1, _A_2, _A_3, _A_4, _A_5);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5, T_arg6 _A_6) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
             (_A_1, _A_2, _A_3, _A_4, _A_5, _A_6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5, T_arg6 _A_6) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
             (_A_1, _A_2, _A_3, _A_4, _A_5, _A_6);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator ()(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5, T_arg6 _A_6, T_arg7 _A_7) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
             (_A_1, _A_2, _A_3, _A_4, _A_5, _A_6, _A_7);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_1, T_arg2 _A_2, T_arg3 _A_3, T_arg4 _A_4, T_arg5 _A_5, T_arg6 _A_6, T_arg7 _A_7) const
    { return value_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
             (_A_1, _A_2, _A_3, _A_4, _A_5, _A_6, _A_7);
    }
  #endif //SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD

  lambda_core() {}

  explicit lambda_core(const T_type& v)
    : value_(v) {}

  T_type value_;
};


} /* namespace internal */


// forward declarations for lambda operators other<subscript> and other<assign>
template <class T_type>
struct other;
struct subscript;
struct assign;

template <class T_action, class T_type1, class T_type2>
struct lambda_operator;

template <class T_type>
struct unwrap_lambda_type;

/** Lambda type.
 * Objects of this type store a value that may be of type lambda itself.
 * In this case, operator()() executes the lambda (a lambda is always a functor at the same time).
 * Otherwise, operator()() simply returns the stored value.
 * The assign and subscript operators are defined to return a lambda operator.
 *
 * @deprecated Use C++11 lambda expressions instead.
 *
 * @ingroup lambdas
 */
template <class T_type>
struct lambda : public internal::lambda_core<T_type>
{
  typedef lambda<T_type> self;

  lambda()
    {}

  lambda(typename type_trait<T_type>::take v)
    : internal::lambda_core<T_type>(v)
    {}

  // operators for other<subscript>
  template <class T_arg>
  lambda<lambda_operator<other<subscript>, self, typename unwrap_lambda_type<T_arg>::type> >
  operator [] (const T_arg& a) const
    { typedef lambda_operator<other<subscript>, self, typename unwrap_lambda_type<T_arg>::type> lambda_operator_type;
      return lambda<lambda_operator_type>(lambda_operator_type(this->value_, unwrap_lambda_value(a))); }

  // operators for other<assign>
  template <class T_arg>
  lambda<lambda_operator<other<assign>, self, typename unwrap_lambda_type<T_arg>::type> >
  operator = (const T_arg& a) const
    { typedef lambda_operator<other<assign>, self, typename unwrap_lambda_type<T_arg>::type> lambda_operator_type;
      return lambda<lambda_operator_type>(lambda_operator_type(this->value_, unwrap_lambda_value(a))); }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS

} /* namespace sigc */

#endif // SIGCXX_DISABLE_DEPRECATED

#endif /* _SIGC_LAMBDA_BASE_HPP_ */
