// -*- c++ -*-
/* Do not edit! -- generated file */
#ifndef _SIGC_ADAPTORS_TRACK_OBJ_H_
#define _SIGC_ADAPTORS_TRACK_OBJ_H_
#include <sigc++/adaptors/adaptor_trait.h>
#include <sigc++/limit_reference.h>

namespace sigc {

/** @defgroup track_obj track_obj()
 * sigc::track_obj() tracks trackable objects, referenced from a functor.
 * It can be useful when you assign a C++11 lambda expression or a std::function<>
 * to a slot, or connect it to a signal, and the lambda expression or std::function<>
 * contains references to sigc::trackable derived objects.
 *
 * The functor returned by sigc::track_obj() is formally an adaptor, but it does
 * not alter the signature, return type or behaviour of the supplied functor.
 * Up to 7 objects can be tracked. operator()() can have up to 7 arguments.
 *
 * @par Example:
 * @code
 * struct bar : public sigc::trackable {};
 * sigc::signal<void> some_signal;
 * void foo(bar&);
 * {
 *   bar some_bar;
 *   some_signal.connect([&some_bar](){ foo(some_bar); });
 *     // NOT disconnected automatically when some_bar goes out of scope
 *   some_signal.connect(sigc::track_obj([&some_bar](){ foo(some_bar); }, some_bar);
 *     // disconnected automatically when some_bar goes out of scope
 * }
 * @endcode
 *
 * @newin{2,4}
 *
 * @ingroup adaptors
 */

/** track_obj_functor1 wraps a functor and stores a reference to a trackable object.
 * Use the convenience function track_obj() to create an instance of track_obj_functor1.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1>
class track_obj_functor1 : public adapts<T_functor>
{
public:
  typedef typename adapts<T_functor>::adaptor_type adaptor_type;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
  template <typename T_arg1=void, typename T_arg2=void, typename T_arg3=void, typename T_arg4=void, typename T_arg5=void, typename T_arg6=void, typename T_arg7=void>
  struct deduce_result_type
    { typedef typename adaptor_type::template deduce_result_type<type_trait_pass_t<T_arg1>, type_trait_pass_t<T_arg2>, type_trait_pass_t<T_arg3>, type_trait_pass_t<T_arg4>, type_trait_pass_t<T_arg5>, type_trait_pass_t<T_arg6>, type_trait_pass_t<T_arg7>>::type type; };
#endif
  typedef typename adaptor_type::result_type result_type;

  /** Constructs a track_obj_functor1 object that wraps the passed functor and
   * stores a reference to the passed trackable object.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   */
  track_obj_functor1(const T_functor& _A_func, const T_obj1& _A_obj1)
  : adapts<T_functor>(_A_func), obj1_(_A_obj1) {}

  /** Invokes the wrapped functor.
   * @return The return value of the functor invocation.
   */
  result_type operator()()
  { return this->functor_(); }

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1>
  typename deduce_result_type<T_arg1>::type
  operator()(T_arg1 _A_arg1)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>>
      (_A_arg1);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1>
  typename deduce_result_type<T_arg1>::type
  sun_forte_workaround(T_arg1 _A_arg1)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>>
      (_A_arg1);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>>
      (_A_arg1, _A_arg2);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2>
  typename deduce_result_type<T_arg1, T_arg2>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>>
      (_A_arg1, _A_arg2);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2, typename T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>>
      (_A_arg1, _A_arg2, _A_arg3);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2, typename T_arg3>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>>
      (_A_arg1, _A_arg2, _A_arg3);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5, typename T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>, 
      type_trait_pass_t<T_arg6>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5, typename T_arg6>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>, 
      type_trait_pass_t<T_arg6>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6);
  }
  #endif

  /** Invokes the wrapped functor passing on the arguments.
   * @param _A_arg1 Argument to be passed on to the functor.
   * @param _A_arg2 Argument to be passed on to the functor.
   * @param _A_arg3 Argument to be passed on to the functor.
   * @param _A_arg4 Argument to be passed on to the functor.
   * @param _A_arg5 Argument to be passed on to the functor.
   * @param _A_arg6 Argument to be passed on to the functor.
   * @param _A_arg7 Argument to be passed on to the functor.
   * @return The return value of the functor invocation.
   */
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5, typename T_arg6, typename T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  operator()(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6, T_arg7 _A_arg7)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>, 
      type_trait_pass_t<T_arg6>, 
      type_trait_pass_t<T_arg7>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, _A_arg7);
  }

  #ifndef SIGC_TEMPLATE_SPECIALIZATION_OPERATOR_OVERLOAD
  template <typename T_arg1, typename T_arg2, typename T_arg3, typename T_arg4, typename T_arg5, typename T_arg6, typename T_arg7>
  typename deduce_result_type<T_arg1, T_arg2, T_arg3, T_arg4, T_arg5, T_arg6, T_arg7>::type
  sun_forte_workaround(T_arg1 _A_arg1, T_arg2 _A_arg2, T_arg3 _A_arg3, T_arg4 _A_arg4, T_arg5 _A_arg5, T_arg6 _A_arg6, T_arg7 _A_arg7)
  {
    return this->functor_.SIGC_WORKAROUND_OPERATOR_PARENTHESES<
      type_trait_pass_t<T_arg1>, 
      type_trait_pass_t<T_arg2>, 
      type_trait_pass_t<T_arg3>, 
      type_trait_pass_t<T_arg4>, 
      type_trait_pass_t<T_arg5>, 
      type_trait_pass_t<T_arg6>, 
      type_trait_pass_t<T_arg7>>
      (_A_arg1, _A_arg2, _A_arg3, _A_arg4, _A_arg5, _A_arg6, _A_arg7);
  }
  #endif


#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj1> obj1_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor1

/** track_obj_functor2 wraps a functor and stores 2 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor2.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2>
class track_obj_functor2 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor2 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   */
  track_obj_functor2(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor2

/** track_obj_functor3 wraps a functor and stores 3 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor3.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 * @tparam T_obj3 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3>
class track_obj_functor3 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor3 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   * @param _A_obj3 Trackable object.
   */
  track_obj_functor3(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2), obj3_(_A_obj3) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
  const_limit_reference<T_obj3> obj3_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor3

/** track_obj_functor4 wraps a functor and stores 4 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor4.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 * @tparam T_obj3 The type of a trackable object.
 * @tparam T_obj4 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4>
class track_obj_functor4 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor4 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   * @param _A_obj3 Trackable object.
   * @param _A_obj4 Trackable object.
   */
  track_obj_functor4(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2), obj3_(_A_obj3), obj4_(_A_obj4) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
  const_limit_reference<T_obj3> obj3_;
  const_limit_reference<T_obj4> obj4_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor4

/** track_obj_functor5 wraps a functor and stores 5 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor5.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 * @tparam T_obj3 The type of a trackable object.
 * @tparam T_obj4 The type of a trackable object.
 * @tparam T_obj5 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5>
class track_obj_functor5 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor5 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   * @param _A_obj3 Trackable object.
   * @param _A_obj4 Trackable object.
   * @param _A_obj5 Trackable object.
   */
  track_obj_functor5(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2), obj3_(_A_obj3), obj4_(_A_obj4), obj5_(_A_obj5) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
  const_limit_reference<T_obj3> obj3_;
  const_limit_reference<T_obj4> obj4_;
  const_limit_reference<T_obj5> obj5_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor5

/** track_obj_functor6 wraps a functor and stores 6 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor6.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 * @tparam T_obj3 The type of a trackable object.
 * @tparam T_obj4 The type of a trackable object.
 * @tparam T_obj5 The type of a trackable object.
 * @tparam T_obj6 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6>
class track_obj_functor6 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor6 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   * @param _A_obj3 Trackable object.
   * @param _A_obj4 Trackable object.
   * @param _A_obj5 Trackable object.
   * @param _A_obj6 Trackable object.
   */
  track_obj_functor6(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5, const T_obj6& _A_obj6)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2), obj3_(_A_obj3), obj4_(_A_obj4), obj5_(_A_obj5), obj6_(_A_obj6) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
  const_limit_reference<T_obj3> obj3_;
  const_limit_reference<T_obj4> obj4_;
  const_limit_reference<T_obj5> obj5_;
  const_limit_reference<T_obj6> obj6_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor6

/** track_obj_functor7 wraps a functor and stores 7 references to trackable objects.
 * Use the convenience function track_obj() to create an instance of track_obj_functor7.
 *
 * @tparam T_functor The type of functor to wrap.
 * @tparam T_obj1 The type of a trackable object.
 * @tparam T_obj2 The type of a trackable object.
 * @tparam T_obj3 The type of a trackable object.
 * @tparam T_obj4 The type of a trackable object.
 * @tparam T_obj5 The type of a trackable object.
 * @tparam T_obj6 The type of a trackable object.
 * @tparam T_obj7 The type of a trackable object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6, typename T_obj7>
class track_obj_functor7 : public track_obj_functor1<T_functor, T_obj1>
{
public:
  /** Constructs a track_obj_functor7 object that wraps the passed functor and
   * stores references to the passed trackable objects.
   * @param _A_func Functor.
   * @param _A_obj1 Trackable object.
   * @param _A_obj2 Trackable object.
   * @param _A_obj3 Trackable object.
   * @param _A_obj4 Trackable object.
   * @param _A_obj5 Trackable object.
   * @param _A_obj6 Trackable object.
   * @param _A_obj7 Trackable object.
   */
  track_obj_functor7(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5, const T_obj6& _A_obj6, const T_obj7& _A_obj7)
  : track_obj_functor1<T_functor, T_obj1>(_A_func, _A_obj1), obj2_(_A_obj2), obj3_(_A_obj3), obj4_(_A_obj4), obj5_(_A_obj5), obj6_(_A_obj6), obj7_(_A_obj7) {}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
//protected:
  // public, so that visit_each() can access it.
  const_limit_reference<T_obj2> obj2_;
  const_limit_reference<T_obj3> obj3_;
  const_limit_reference<T_obj4> obj4_;
  const_limit_reference<T_obj5> obj5_;
  const_limit_reference<T_obj6> obj6_;
  const_limit_reference<T_obj7> obj7_;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

}; // end class track_obj_functor7


#ifndef DOXYGEN_SHOULD_SKIP_THIS
//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor1 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor1 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1>
struct visitor<track_obj_functor1<T_functor, T_obj1> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor1<T_functor, T_obj1>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor2 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor2 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2>
struct visitor<track_obj_functor2<T_functor, T_obj1, T_obj2> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor2<T_functor, T_obj1, T_obj2>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor3 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor3 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3>
struct visitor<track_obj_functor3<T_functor, T_obj1, T_obj2, T_obj3> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor3<T_functor, T_obj1, T_obj2, T_obj3>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
    sigc::visit_each(_A_action, _A_target.obj3_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor4 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor4 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4>
struct visitor<track_obj_functor4<T_functor, T_obj1, T_obj2, T_obj3, T_obj4> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor4<T_functor, T_obj1, T_obj2, T_obj3, T_obj4>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
    sigc::visit_each(_A_action, _A_target.obj3_);
    sigc::visit_each(_A_action, _A_target.obj4_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor5 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor5 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5>
struct visitor<track_obj_functor5<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor5<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
    sigc::visit_each(_A_action, _A_target.obj3_);
    sigc::visit_each(_A_action, _A_target.obj4_);
    sigc::visit_each(_A_action, _A_target.obj5_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor6 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor6 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6>
struct visitor<track_obj_functor6<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor6<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
    sigc::visit_each(_A_action, _A_target.obj3_);
    sigc::visit_each(_A_action, _A_target.obj4_);
    sigc::visit_each(_A_action, _A_target.obj5_);
    sigc::visit_each(_A_action, _A_target.obj6_);
  }
};

//template specialization of visitor<>::do_visit_each<>(action, functor):
/** Performs a functor on each of the targets of a functor.
 * The function overload for sigc::track_obj_functor7 performs a functor
 * on the functor and on the trackable object instances stored in the
 * sigc::track_obj_functor7 object.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6, typename T_obj7>
struct visitor<track_obj_functor7<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6, T_obj7> >
{
  template <typename T_action>
  static void do_visit_each(const T_action& _A_action,
                            const track_obj_functor7<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6, T_obj7>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.functor_);
    sigc::visit_each(_A_action, _A_target.obj1_);
    sigc::visit_each(_A_action, _A_target.obj2_);
    sigc::visit_each(_A_action, _A_target.obj3_);
    sigc::visit_each(_A_action, _A_target.obj4_);
    sigc::visit_each(_A_action, _A_target.obj5_);
    sigc::visit_each(_A_action, _A_target.obj6_);
    sigc::visit_each(_A_action, _A_target.obj7_);
  }
};

#endif // DOXYGEN_SHOULD_SKIP_THIS

/** Creates an adaptor of type sigc::track_obj_functor1 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1>
inline track_obj_functor1<T_functor, T_obj1>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1)
{
  return track_obj_functor1<T_functor, T_obj1>
    (_A_func, _A_obj1);
}

/** Creates an adaptor of type sigc::track_obj_functor2 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2>
inline track_obj_functor2<T_functor, T_obj1, T_obj2>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2)
{
  return track_obj_functor2<T_functor, T_obj1, T_obj2>
    (_A_func, _A_obj1, _A_obj2);
}

/** Creates an adaptor of type sigc::track_obj_functor3 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @param _A_obj3 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3>
inline track_obj_functor3<T_functor, T_obj1, T_obj2, T_obj3>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3)
{
  return track_obj_functor3<T_functor, T_obj1, T_obj2, T_obj3>
    (_A_func, _A_obj1, _A_obj2, _A_obj3);
}

/** Creates an adaptor of type sigc::track_obj_functor4 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @param _A_obj3 Trackable object.
 * @param _A_obj4 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4>
inline track_obj_functor4<T_functor, T_obj1, T_obj2, T_obj3, T_obj4>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4)
{
  return track_obj_functor4<T_functor, T_obj1, T_obj2, T_obj3, T_obj4>
    (_A_func, _A_obj1, _A_obj2, _A_obj3, _A_obj4);
}

/** Creates an adaptor of type sigc::track_obj_functor5 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @param _A_obj3 Trackable object.
 * @param _A_obj4 Trackable object.
 * @param _A_obj5 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5>
inline track_obj_functor5<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5)
{
  return track_obj_functor5<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5>
    (_A_func, _A_obj1, _A_obj2, _A_obj3, _A_obj4, _A_obj5);
}

/** Creates an adaptor of type sigc::track_obj_functor6 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @param _A_obj3 Trackable object.
 * @param _A_obj4 Trackable object.
 * @param _A_obj5 Trackable object.
 * @param _A_obj6 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6>
inline track_obj_functor6<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5, const T_obj6& _A_obj6)
{
  return track_obj_functor6<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6>
    (_A_func, _A_obj1, _A_obj2, _A_obj3, _A_obj4, _A_obj5, _A_obj6);
}

/** Creates an adaptor of type sigc::track_obj_functor7 which wraps a functor.
 * @param _A_func Functor that shall be wrapped.
 * @param _A_obj1 Trackable object.
 * @param _A_obj2 Trackable object.
 * @param _A_obj3 Trackable object.
 * @param _A_obj4 Trackable object.
 * @param _A_obj5 Trackable object.
 * @param _A_obj6 Trackable object.
 * @param _A_obj7 Trackable object.
 * @return Adaptor that executes _A_func() on invocation.
 *
 * @newin{2,4}
 *
 * @ingroup track_obj
 */
template <typename T_functor, typename T_obj1, typename T_obj2, typename T_obj3, typename T_obj4, typename T_obj5, typename T_obj6, typename T_obj7>
inline track_obj_functor7<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6, T_obj7>
track_obj(const T_functor& _A_func, const T_obj1& _A_obj1, const T_obj2& _A_obj2, const T_obj3& _A_obj3, const T_obj4& _A_obj4, const T_obj5& _A_obj5, const T_obj6& _A_obj6, const T_obj7& _A_obj7)
{
  return track_obj_functor7<T_functor, T_obj1, T_obj2, T_obj3, T_obj4, T_obj5, T_obj6, T_obj7>
    (_A_func, _A_obj1, _A_obj2, _A_obj3, _A_obj4, _A_obj5, _A_obj6, _A_obj7);
}


} /* namespace sigc */
#endif /* _SIGC_ADAPTORS_TRACK_OBJ_H_ */
