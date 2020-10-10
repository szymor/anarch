// -*- c++ -*-
/* Do not edit! -- generated file */
#ifndef _SIGC_ADAPTORS_BIND_H_
#define _SIGC_ADAPTORS_BIND_H_
#include <sigc++/adaptors/adaptor_trait.h>
#include <sigc++/adaptors/bound_argument.h>

//TODO: See comment in functor_trait.h.
#if defined(nil) && defined(SIGC_PRAGMA_PUSH_POP_MACRO)
  #define SIGC_NIL_HAS_BEEN_PUSHED 1
  #pragma push_macro("nil")
  #undef nil
#endif

namespace sigc {

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace internal {

template <class T_arg1,class T_arg2,class T_arg3,class T_arg4,class T_arg5,class T_arg6,class T_arg7>
struct count_void
  { static const int value=0; };
template <class T_arg1,class T_arg2,class T_arg3,class T_arg4,class T_arg5,class T_arg6>
struct count_void<T_arg1,T_arg2,T_arg3,T_arg4,T_arg5,T_arg6,void>
  { static const int value=1; };
template <class T_arg1,class T_arg2,class T_arg3,class T_arg4,class T_arg5>
struct count_void<T_arg1,T_arg2,T_arg3,T_arg4,T_arg5,void,void>
  { static const int value=2; };
template <class T_arg1,class T_arg2,class T_arg3,class T_arg4>
struct count_void<T_arg1,T_arg2,T_arg3,T_arg4,void,void,void>
  { static const int value=3; };
template <class T_arg1,class T_arg2,class T_arg3>
struct count_void<T_arg1,T_arg2,T_arg3,void,void,void,void>
  { static const int value=4; };
template <class T_arg1,class T_arg2>
struct count_void<T_arg1,T_arg2,void,void,void,void,void>
  { static const int value=5; };
template <class T_arg1>
struct count_void<T_arg1,void,void,void,void,void,void>
  { static const int value=6; };
template <>
struct count_void<void,void,void,void,void,void,void>
  { static const int value=7; };

} /* namespace internal */

#endif /*DOXYGEN_SHOULD_SKIP_THIS*/


/** @defgroup bind bind(), bind_return()
 * sigc::bind() alters an arbitrary functor by fixing arguments to certain values.
 * Up to 7 arguments can be bound at a time.
 * For single argument binding, overloads of sigc::bind() are provided that let you
 * specify the zero-based position of the argument to fix with the first template parameter.
 * (A value of @p -1 fixes the last argument so sigc::bind<-1>() gives the same result as sigc::bind().)
 * The types of the arguments can optionally be specified if not deduced.
 *
 * @par Examples:
 * @code
 * void foo(int, int, int);
 * // single argument binding ...
 * sigc::bind(&foo,1)(2,3);     //fixes the last (third) argument and calls foo(2,3,1)
 * sigc::bind<-1>(&foo,1)(2,3); //same as bind(&foo,1)(2,3) (calls foo(2,3,1))
 * sigc::bind<0>(&foo,1)(2,3);  //fixes the first argument and calls foo(1,2,3)
 * sigc::bind<1>(&foo,1)(2,3);  //fixes the second argument and calls foo(2,1,3)
 * sigc::bind<2>(&foo,1)(2,3);  //fixes the third argument and calls foo(2,3,1)
 * // multi argument binding ...
 * sigc::bind(&foo,1,2)(3);     //fixes the last two arguments and calls foo(3,1,2)
 * sigc::bind(&foo,1,2,3)();    //fixes all three arguments and calls foo(1,2,3)
 * @endcode
 *
 * The functor sigc::bind() returns can be passed into
 * sigc::signal::connect() directly.
 *
 * @par Example:
 * @code
 * sigc::signal<void> some_signal;
 * void foo(int);
 * some_signal.connect(sigc::bind(&foo,1));
 * @endcode
 *
 * sigc::bind_return() alters an arbitrary functor by
 * fixing its return value to a certain value.
 *
 * @par Example:
 * @code
 * void foo();
 * std::cout << sigc::bind_return(&foo, 5)(); // calls foo() and returns 5
 * @endcode
 *
 * You can bind references to functors by passing the objects through
 * the std::ref() or std::cref() functions.
 *
 * @par Example:
 * @code
 * int some_int;
 * sigc::signal<void> some_signal;
 * void foo(int&);
 * some_signal.connect(sigc::bind(&foo, std::ref(some_int)));
 * @endcode
 *
 * If you bind an object of a sigc::trackable derived type to a functor
 * by reference, a slot assigned to the bind adaptor is cleared automatically
 * when the object goes out of scope.
 *
 * @par Example:
 * @code
 * struct bar : public sigc::trackable {} some_bar;
 * sigc::signal<void> some_signal;
 * void foo(bar&);
 * some_signal.connect(sigc::bind(&foo, std::ref(some_bar)));
 *   // disconnected automatically if some_bar goes out of scope
 * @endcode
 *
 * @ingroup adaptors
 */

/** Adaptor that binds an argument to the wrapped functor.
 * Use the convenience function sigc::bind() to create an instance of sigc::bind_functor.
 *
 * The following template arguments are used:
 * - @e I_location Zero-based position of the argument to fix (@p -1 for the last argument).
 * - @e T_type1 Type of the 1st bound argument.
 * - @e T_type2 Type of the 2nd bound argument.
 * - @e T_type3 Type of the 3rd bound argument.
 * - @e T_type4 Type of the 4th bound argument.
 * - @e T_type5 Type of the 5th bound argument.
 * - @e T_type6 Type of the 6th bound argument.
 * - @e T_type7 Type of the 7th bound argument.
 * - @e T_functor Type of the functor to wrap.
 *
 * @ingroup bind
 */
template <int I_location, class T_functor, class T_type1=nil, class T_type2=nil, class T_type3=nil, class T_type4=nil, class T_type5=nil, class T_type6=nil, class T_type7=nil>
#ifndef DOXYGEN_SHOULD_SKIP_THIS
struct bind_functor;
#else
struct bind_functor {};
#endif

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 1st argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<0, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
#endif
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>>
        (bound_.invoke(), _A_arg1);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>>
        (bound_.invoke(), _A_arg1);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (bound_.invoke(), _A_arg1, _A_arg2);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>>
        (bound_.invoke(), _A_arg1, _A_arg2);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 1st argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (bound_.invoke(), _A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 2nd argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<1, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, bound_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>>
        (_A_arg1, bound_.invoke(), _A_arg2);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>>
        (_A_arg1, bound_.invoke(), _A_arg2);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4, _A_arg5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4, _A_arg5);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 2nd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, bound_.invoke(), _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 3rd argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<2, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 3rd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, bound_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 3rd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 3rd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 3rd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4, _A_arg5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4, _A_arg5);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 3rd argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, bound_.invoke(), _A_arg3, _A_arg4, _A_arg5, _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 4th argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<3, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 4th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 4th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 4th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4, _A_arg5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4, _A_arg5);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 4th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4, _A_arg5, _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, bound_.invoke(), _A_arg4, _A_arg5, _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 5th argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<4, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 5th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 5th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke(), _A_arg5);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg5>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke(), _A_arg5);
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 5th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke(), _A_arg5, _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound_.invoke(), _A_arg5, _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 6th argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<5, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg6>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 6th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 6th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound_.invoke(), _A_arg6);
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>, type_trait_pass_t<T_arg6>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound_.invoke(), _A_arg6);
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};

/** Adaptor that binds an argument to the wrapped functor.
 * This template specialization fixes the 7th argument of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_bound>
struct bind_functor<6, T_functor, T_bound, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>::type type; };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_bound>::type>> (bound_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * bound_ is passed as the 7th argument.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, bound_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_bound>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, bound_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_bound> _A_bound)
    : adapts<T_functor>(_A_func), bound_(_A_bound)
    {}

  /// The argument bound to the functor.
  bound_argument<T_bound> bound_;
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


#ifndef DOXYGEN_SHOULD_SKIP_THIS
//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <int T_loc, class T_functor, class T_bound>
struct visitor<bind_functor<T_loc, T_functor, T_bound> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<T_loc, T_functor, T_bound>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound_);
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS

/** Adaptor that binds 1 argument(s) to the wrapped functor.
 * This template specialization fixes the last 1 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1>
struct bind_functor<-1, T_functor, T_type1, nil, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<2, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<3, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<4, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<5, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<6, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
#endif // DOXYGEN_SHOULD_SKIP_THIS
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>> (bound1_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, bound1_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound1_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 1 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, bound1_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, bound1_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1>
struct visitor<bind_functor<-1, T_functor, T_type1> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
  }
};

/** Adaptor that binds 2 argument(s) to the wrapped functor.
 * This template specialization fixes the last 2 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2>
struct bind_functor<-1, T_functor, T_type1, T_type2, nil, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<3, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<4, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<5, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<6, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>> (bound1_.invoke(), bound2_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 2 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 2 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 2 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 2 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke(), bound2_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke(), bound2_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 2 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound1_.invoke(), bound2_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, bound1_.invoke(), bound2_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
  }
};

/** Adaptor that binds 3 argument(s) to the wrapped functor.
 * This template specialization fixes the last 3 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3>
struct bind_functor<-1, T_functor, T_type1, T_type2, T_type3, nil, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<4, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<5, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<6, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>> (bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 3 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 3 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 3 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 3 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>>
        (_A_arg1, _A_arg2, _A_arg3, _A_arg4, bound1_.invoke(), bound2_.invoke(), bound3_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   * @param _A_bound3 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2, type_trait_take_t<T_type3> _A_bound3)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2), bound3_(_A_bound3)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
  bound_argument<T_type3> bound3_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2, T_type3> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2, T_type3>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
    sigc::visit_each(_A_action, _A_target.bound3_);
  }
};

/** Adaptor that binds 4 argument(s) to the wrapped functor.
 * This template specialization fixes the last 4 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4>
struct bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, nil, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<5, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<6, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>> (bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 4 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 4 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 4 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2, class T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>>
        (_A_arg1, _A_arg2, _A_arg3, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   * @param _A_bound3 Argument to bind to the functor.
   * @param _A_bound4 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2, type_trait_take_t<T_type3> _A_bound3, type_trait_take_t<T_type4> _A_bound4)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2), bound3_(_A_bound3), bound4_(_A_bound4)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
  bound_argument<T_type3> bound3_;
  bound_argument<T_type4> bound4_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2, T_type3, T_type4>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
    sigc::visit_each(_A_action, _A_target.bound3_);
    sigc::visit_each(_A_action, _A_target.bound4_);
  }
};

/** Adaptor that binds 5 argument(s) to the wrapped functor.
 * This template specialization fixes the last 5 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5>
struct bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5, nil, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>::type type; };
  template <class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal<6, T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>> (bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 5 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke());
    }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * The last 5 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1, class T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>>
        (_A_arg1, _A_arg2, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   * @param _A_bound3 Argument to bind to the functor.
   * @param _A_bound4 Argument to bind to the functor.
   * @param _A_bound5 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2, type_trait_take_t<T_type3> _A_bound3, type_trait_take_t<T_type4> _A_bound4, type_trait_take_t<T_type5> _A_bound5)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2), bound3_(_A_bound3), bound4_(_A_bound4), bound5_(_A_bound5)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
  bound_argument<T_type3> bound3_;
  bound_argument<T_type4> bound4_;
  bound_argument<T_type5> bound5_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2, T_type3, T_type4, T_type5>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
    sigc::visit_each(_A_action, _A_target.bound3_);
    sigc::visit_each(_A_action, _A_target.bound4_);
    sigc::visit_each(_A_action, _A_target.bound5_);
  }
};

/** Adaptor that binds 6 argument(s) to the wrapped functor.
 * This template specialization fixes the last 6 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6>
struct bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5, T_type6, nil> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>> (bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke(), bound6_.invoke());
  }

  /** Invokes the wrapped functor passing on the arguments.
   * The last 6 argument(s) are fixed.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke(), bound6_.invoke());
    }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <class T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
    { return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<T_arg1>, type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>>
        (_A_arg1, bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke(), bound6_.invoke());
    }
  #endif

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   * @param _A_bound3 Argument to bind to the functor.
   * @param _A_bound4 Argument to bind to the functor.
   * @param _A_bound5 Argument to bind to the functor.
   * @param _A_bound6 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2, type_trait_take_t<T_type3> _A_bound3, type_trait_take_t<T_type4> _A_bound4, type_trait_take_t<T_type5> _A_bound5, type_trait_take_t<T_type6> _A_bound6)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2), bound3_(_A_bound3), bound4_(_A_bound4), bound5_(_A_bound5), bound6_(_A_bound6)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
  bound_argument<T_type3> bound3_;
  bound_argument<T_type4> bound4_;
  bound_argument<T_type5> bound5_;
  bound_argument<T_type6> bound6_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5, T_type6> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2, T_type3, T_type4, T_type5, T_type6>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
    sigc::visit_each(_A_action, _A_target.bound3_);
    sigc::visit_each(_A_action, _A_target.bound4_);
    sigc::visit_each(_A_action, _A_target.bound5_);
    sigc::visit_each(_A_action, _A_target.bound6_);
  }
};

/** Adaptor that binds 7 argument(s) to the wrapped functor.
 * This template specialization fixes the last 7 argument(s) of the wrapped functor.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6, class T_type7>
struct bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5, T_type6, T_type7> : public adapts<T_functor>
{
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

  template <int count, class T_arg1, class T_arg2, class T_arg3, class T_arg4, class T_arg5, class T_arg6, class T_arg7>
  struct deduce_result_type_internal
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>, type_trait_pass_t<typename unwrap_reference<T_type7>::type>>::type type; };

  template <class T_arg1=void, class T_arg2=void, class T_arg3=void, class T_arg4=void, class T_arg5=void, class T_arg6=void, class T_arg7=void>
  struct deduce_result_type {
    typedef typename deduce_result_type_internal<internal::count_void<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::value,
                                                 T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type type;
  };
  typedef typename adaptor_type::result_type  result_type;

  /** Invokes the wrapped functor passing on the bound argument only.
   * @return The return value of the functor invocation.
   */
  result_type
  operator()()
  {
    //Note: The AIX compiler sometimes gives linker errors if we do not define this in the class.
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<type_trait_pass_t<typename unwrap_reference<T_type1>::type>, type_trait_pass_t<typename unwrap_reference<T_type2>::type>, type_trait_pass_t<typename unwrap_reference<T_type3>::type>, type_trait_pass_t<typename unwrap_reference<T_type4>::type>, type_trait_pass_t<typename unwrap_reference<T_type5>::type>, type_trait_pass_t<typename unwrap_reference<T_type6>::type>, type_trait_pass_t<typename unwrap_reference<T_type7>::type>> (bound1_.invoke(), bound2_.invoke(), bound3_.invoke(), bound4_.invoke(), bound5_.invoke(), bound6_.invoke(), bound7_.invoke());
  }

  /** Constructs a bind_functor object that binds an argument to the passed functor.
   * @param _A_func Functor to invoke from operator()().
   * @param _A_bound1 Argument to bind to the functor.
   * @param _A_bound2 Argument to bind to the functor.
   * @param _A_bound3 Argument to bind to the functor.
   * @param _A_bound4 Argument to bind to the functor.
   * @param _A_bound5 Argument to bind to the functor.
   * @param _A_bound6 Argument to bind to the functor.
   * @param _A_bound7 Argument to bind to the functor.
   */
  bind_functor(type_trait_take_t<T_functor> _A_func, type_trait_take_t<T_type1> _A_bound1, type_trait_take_t<T_type2> _A_bound2, type_trait_take_t<T_type3> _A_bound3, type_trait_take_t<T_type4> _A_bound4, type_trait_take_t<T_type5> _A_bound5, type_trait_take_t<T_type6> _A_bound6, type_trait_take_t<T_type7> _A_bound7)
    : adapts<T_functor>(_A_func), bound1_(_A_bound1), bound2_(_A_bound2), bound3_(_A_bound3), bound4_(_A_bound4), bound5_(_A_bound5), bound6_(_A_bound6), bound7_(_A_bound7)
    {}

  /// The argument bound to the functor.
  bound_argument<T_type1> bound1_;
  bound_argument<T_type2> bound2_;
  bound_argument<T_type3> bound3_;
  bound_argument<T_type4> bound4_;
  bound_argument<T_type5> bound5_;
  bound_argument<T_type6> bound6_;
  bound_argument<T_type7> bound7_;
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::bind_functor performs a functor on the
 * functor and on the object instances stored in the sigc::bind_functor object.
 *
 * @ingroup bind
 */
template <class T_functor, class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6, class T_type7>
struct visitor<bind_functor<-1, T_functor, T_type1, T_type2, T_type3, T_type4, T_type5, T_type6, T_type7> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const bind_functor<-1, T_functor,  T_type1, T_type2, T_type3, T_type4, T_type5, T_type6, T_type7>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.bound1_);
    sigc::visit_each(_A_action, _A_target.bound2_);
    sigc::visit_each(_A_action, _A_target.bound3_);
    sigc::visit_each(_A_action, _A_target.bound4_);
    sigc::visit_each(_A_action, _A_target.bound5_);
    sigc::visit_each(_A_action, _A_target.bound6_);
    sigc::visit_each(_A_action, _A_target.bound7_);
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


/** Creates an adaptor of type sigc::bind_functor which binds the passed argument to the passed functor.
 * The optional template argument @e I_location specifies the zero-based
 * position of the argument to be fixed (@p -1 stands for the last argument).
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @return Adaptor that executes @e _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <int I_location, class T_bound1, class T_functor>
inline bind_functor<I_location, T_functor, T_bound1>
bind(const T_functor& _A_func, T_bound1 _A_b1)
{
  return bind_functor<I_location, T_functor, T_bound1>
           (_A_func, _A_b1);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 1 argument(s) of the passed functor.
 * This function overload fixes the last 1 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1>
bind(const T_functor& _A_func, T_type1 _A_b1)
{ return bind_functor<-1, T_functor,
                    T_type1>
                    (_A_func, _A_b1);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 2 argument(s) of the passed functor.
 * This function overload fixes the last 2 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2>
                    (_A_func, _A_b1, _A_b2);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 3 argument(s) of the passed functor.
 * This function overload fixes the last 3 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @param _A_b3 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_type3, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2, T_type3 _A_b3)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3>
                    (_A_func, _A_b1, _A_b2, _A_b3);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 4 argument(s) of the passed functor.
 * This function overload fixes the last 4 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @param _A_b3 Argument to bind to @e _A_func.
 * @param _A_b4 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_type3, class T_type4, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2, T_type3 _A_b3, T_type4 _A_b4)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4>
                    (_A_func, _A_b1, _A_b2, _A_b3, _A_b4);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 5 argument(s) of the passed functor.
 * This function overload fixes the last 5 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @param _A_b3 Argument to bind to @e _A_func.
 * @param _A_b4 Argument to bind to @e _A_func.
 * @param _A_b5 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2, T_type3 _A_b3, T_type4 _A_b4, T_type5 _A_b5)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5>
                    (_A_func, _A_b1, _A_b2, _A_b3, _A_b4, _A_b5);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 6 argument(s) of the passed functor.
 * This function overload fixes the last 6 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @param _A_b3 Argument to bind to @e _A_func.
 * @param _A_b4 Argument to bind to @e _A_func.
 * @param _A_b5 Argument to bind to @e _A_func.
 * @param _A_b6 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5,
                    T_type6>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2, T_type3 _A_b3, T_type4 _A_b4, T_type5 _A_b5, T_type6 _A_b6)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5,
                    T_type6>
                    (_A_func, _A_b1, _A_b2, _A_b3, _A_b4, _A_b5, _A_b6);
}

/** Creates an adaptor of type sigc::bind_functor which fixes the last 7 argument(s) of the passed functor.
 * This function overload fixes the last 7 argument(s) of @e _A_func.
 *
 * @param _A_func Functor that should be wrapped.
 * @param _A_b1 Argument to bind to @e _A_func.
 * @param _A_b2 Argument to bind to @e _A_func.
 * @param _A_b3 Argument to bind to @e _A_func.
 * @param _A_b4 Argument to bind to @e _A_func.
 * @param _A_b5 Argument to bind to @e _A_func.
 * @param _A_b6 Argument to bind to @e _A_func.
 * @param _A_b7 Argument to bind to @e _A_func.
 * @return Adaptor that executes _A_func with the bound argument on invokation.
 *
 * @ingroup bind
 */
template <class T_type1, class T_type2, class T_type3, class T_type4, class T_type5, class T_type6, class T_type7, class T_functor>
inline bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5,
                    T_type6,
                    T_type7>
bind(const T_functor& _A_func, T_type1 _A_b1, T_type2 _A_b2, T_type3 _A_b3, T_type4 _A_b4, T_type5 _A_b5, T_type6 _A_b6, T_type7 _A_b7)
{ return bind_functor<-1, T_functor,
                    T_type1,
                    T_type2,
                    T_type3,
                    T_type4,
                    T_type5,
                    T_type6,
                    T_type7>
                    (_A_func, _A_b1, _A_b2, _A_b3, _A_b4, _A_b5, _A_b6, _A_b7);
}


} /* namespace sigc */

#ifdef SIGC_NIL_HAS_BEEN_PUSHED
  #undef SIGC_NIL_HAS_BEEN_PUSHED
  #pragma pop_macro("nil")
#endif
#endif /* _SIGC_ADAPTORS_BIND_H_ */
