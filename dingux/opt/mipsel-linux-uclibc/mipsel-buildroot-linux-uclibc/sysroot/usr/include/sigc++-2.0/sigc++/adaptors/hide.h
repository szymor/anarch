// -*- c++ -*-
/* Do not edit! -- generated file */
#ifndef _SIGC_ADAPTORS_HIDE_H_
#define _SIGC_ADAPTORS_HIDE_H_
#include <sigc++/adaptors/adaptor_trait.h>

namespace sigc {

/** @defgroup hide hide(), hide_return()
 * sigc::hide() alters an arbitrary functor in that it adds a parameter
 * whose value is ignored on invocation of the returned functor.
 * Thus you can discard one argument of a signal.
 *
 * You may optionally specify the zero-based position of the parameter
 * to ignore as a template argument. The default is to ignore the last
 * parameter.
 * (A value of @p -1 adds a parameter at the end so sigc::hide<-1>() gives the same result as sigc::hide().)
 *
 * The type of the parameter can optionally be specified if not deduced.
 *
 * @par Examples:
 * @code
 * void foo(int, int);
 * // single argument hiding ...
 * sigc::hide(&foo)(1,2,3);     // adds a dummy parameter at the back and calls foo(1,2)
 * sigc::hide<-1>(&foo)(1,2,3); // same as sigc::hide(&foo)(1,2,3) (calls foo(1,2))
 * sigc::hide<0>(&foo)(1,2,3);  // adds a dummy parameter at the beginning and calls foo(2,3)
 * sigc::hide<1>(&foo)(1,2,3);  // adds a dummy parameter in the middle and calls foo(1,3)
 * sigc::hide<2>(&foo)(1,2,3);  // adds a dummy parameter at the back and calls foo(1,2)
 * @endcode
 *
 * The functor sigc::hide() returns can be directly passed into
 * sigc::signal::connect().
 *
 * @par Example:
 * @code
 * sigc::signal<void,int> some_signal;
 * void foo();
 * some_signal.connect(sigc::hide(&foo));
 * @endcode
 *
 * sigc::hide() can be nested in order to discard multiple arguments.
 * @par Example:
 * @code
 * // multiple argument hiding ...
 * sigc::hide(sigc::hide(&foo))(1,2,3,4); // adds two dummy parameters at the back and calls foo(1,2)
 * @endcode

 * sigc::hide_return() alters an arbitrary functor by
 * dropping its return value, thus converting it to a void functor.
 *
 * @ingroup adaptors
 */

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * Use the convenience function sigc::hide() to create an instance of sigc::hide_functor.
 *
 * The following template arguments are used:
 * - @e I_location Zero-based position of the dummy parameter (@p -1 for the last parameter).
 * - @e T_type Type of the dummy parameter.
 * - @e T_functor Type of the functor to wrap.
 *
 * @ingroup hide
 */
template <int I_location, class T_functor>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct hide_functor;
#else
struct hide_functor {};
#endif

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the last parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <-1, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
#endif // DOXYGEN_SHOULD_SKIP_THIS
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor ignoring the only argument.
   * @param _A_a1 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1)
    { return this->functor_(); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_a1)
    { return this->functor_(); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_a1, T_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
        (_A_a1); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
        (_A_a1); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (_A_a1, _A_a2); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (_A_a1, _A_a2); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a2, _A_a3); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a2, _A_a3); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a3, _A_a4); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a3, _A_a4); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the last argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 1st parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <0, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
#endif // DOXYGEN_SHOULD_SKIP_THIS
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor ignoring the only argument.
   * @param _A_a1 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1)
    { return this->functor_(); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_a1)
    { return this->functor_(); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1, T_arg2 _A_a2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>>
        (_A_a2); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>>
        (_A_a2); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a2, _A_a3); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a2, _A_a3); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a2, _A_a3, _A_a4); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a2, _A_a3, _A_a4); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a2, _A_a3, _A_a4, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a2, _A_a3, _A_a4, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }
  #endif

  /** Invokes the wrapped functor, ignoring the 1st argument.
   * @param _A_a1 Argument to be ignored.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a2, _A_a3, _A_a4, _A_a5, _A_a6, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a2, _A_a3, _A_a4, _A_a5, _A_a6, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 2nd parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <1, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_a1, T_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
        (_A_a1); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>>
        (_A_a1); }
  #endif

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @param _A_a3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a3); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a3); }
  #endif

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a3, _A_a4); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a3, _A_a4); }
  #endif

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a3, _A_a4, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a3, _A_a4, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a3, _A_a4, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a3, _A_a4, _A_a5, _A_a6); }
  #endif

  /** Invokes the wrapped functor, ignoring the 2nd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be ignored.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a3, _A_a4, _A_a5, _A_a6, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a3, _A_a4, _A_a5, _A_a6, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 3rd parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <2, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 3rd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (_A_a1, _A_a2); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (_A_a1, _A_a2); }
  #endif

  /** Invokes the wrapped functor, ignoring the 3rd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @param _A_a4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a4); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a4); }
  #endif

  /** Invokes the wrapped functor, ignoring the 3rd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a4, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a4, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the 3rd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a4, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a4, _A_a5, _A_a6); }
  #endif

  /** Invokes the wrapped functor, ignoring the 3rd argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be ignored.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a4, _A_a5, _A_a6, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a4, _A_a5, _A_a6, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 4th parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <3, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 4th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a2, _A_a3); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_a1, _A_a2, _A_a3); }
  #endif

  /** Invokes the wrapped functor, ignoring the 4th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be ignored.
   * @param _A_a5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the 4th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be ignored.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a5, _A_a6); }
  #endif

  /** Invokes the wrapped functor, ignoring the 4th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be ignored.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a5, _A_a6, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a5, _A_a6, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 5th parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <4, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 5th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a3, _A_a4); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_a1, _A_a2, _A_a3, _A_a4); }
  #endif

  /** Invokes the wrapped functor, ignoring the 5th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be ignored.
   * @param _A_a6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5, T_arg6 _A_a6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a6); }
  #endif

  /** Invokes the wrapped functor, ignoring the 5th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be ignored.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a6, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5, T_arg6 _A_a6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a6, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 6th parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <5, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg7>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 6th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5); }
  #endif

  /** Invokes the wrapped functor, ignoring the 6th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be ignored.
   * @param _A_a7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a7); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6, T_arg7 _A_a7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg7>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a7); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};

/** Adaptor that adds a dummy parameter to the wrapped functor.
 * This template specialization ignores the value of the 7th parameter in operator()().
 *
 * @ingroup hide
 */
template <class T_functor>
struct hide_functor <6, T_functor> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor, ignoring the 7th argument.
   * @param _A_a1 Argument to be passed on to the functor.
   * @param _A_a2 Argument to be passed on to the functor.
   * @param _A_a3 Argument to be passed on to the functor.
   * @param _A_a4 Argument to be passed on to the functor.
   * @param _A_a5 Argument to be passed on to the functor.
   * @param _A_a6 Argument to be passed on to the functor.
   * @param _A_a7 Argument to be ignored.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_a1, T_arg2 _A_a2, T_arg3 _A_a3, T_arg4 _A_a4, T_arg5 _A_a5, T_arg6 _A_a6, T_arg7)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_a1, _A_a2, _A_a3, _A_a4, _A_a5, _A_a6); }
  #endif


  /** Constructs a hide_functor object that adds a dummy parameter to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   */
  explicit hide_functor(const T_functor& _A_func)
    : adapts<T_functor>(_A_func)
    {}
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#ifndef DOXYGEN_SHOULD_SKIP_THIS
//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::hide_functor performs a functor on the
 * functor stored in the sigc::hide_functor object.
 *
 * @ingroup hide
 */
template <int I_location, class T_functor>
struct visitor<hide_functor<I_location, T_functor> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const hide_functor<I_location, T_functor>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS

/** Creates an adaptor of type sigc::hide_functor which adds a dummy parameter to the passed functor.
 * The optional template argument @e I_location specifies the zero-based
 * position of the dummy parameter in the returned functor (@p -1 stands for the last parameter).
 *
 * @param _A_func Functor that should be wrapped.
 * @return Adaptor that executes @e _A_func, ignoring the value of the dummy parameter.
 *
 * @ingroup hide
 */
template <int I_location, class T_functor>
inline hide_functor<I_location, T_functor>
hide(const T_functor& _A_func)
  { return hide_functor<I_location, T_functor>(_A_func); }

/** Creates an adaptor of type sigc::hide_functor which adds a dummy parameter to the passed functor.
 * This overload adds a dummy parameter at the back of the functor's parameter list.
 *
 * @param _A_func Functor that should be wrapped.
 * @return Adaptor that executes @e _A_func, ignoring the value of the last parameter.
 *
 * @ingroup hide
 */
template <class T_functor>
inline hide_functor<-1, T_functor>
hide(const T_functor& _A_func)
  { return hide_functor<-1, T_functor> (_A_func); }

} /* namespace sigc */
#endif /* _SIGC_ADAPTORS_HIDE_H_ */
