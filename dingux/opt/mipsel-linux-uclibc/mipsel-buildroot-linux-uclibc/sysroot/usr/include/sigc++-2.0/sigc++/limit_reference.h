// -*- c++ -*-
/* Do not edit! -- generated file */


#ifndef _SIGC_LIMIT_REFERENCE_H_
#define _SIGC_LIMIT_REFERENCE_H_

#include <sigc++/visit_each.h>
#include <sigc++/type_traits.h>
#include <sigc++/trackable.h>

namespace sigc {

/** A limit_reference<Foo> object stores a reference (Foo&), but make sure that,
 * if Foo inherits from sigc::trackable, then visit_each<>() will "limit" itself to the
 * sigc::trackable reference instead of the derived reference. This avoids use of
 * a reference to the derived type when the derived destructor has run. That can be
 * a problem when using virtual inheritance.
 *
 * If Foo inherits from trackable then both the derived reference and the
 * sigc::trackable reference are stored, so we can later retrieve the sigc::trackable
 * reference without doing an implicit conversion. To retrieve the derived reference
 * (so that you invoke methods or members of it), use invoke(). To retrieve the trackable
 * reference (so that you can call visit_each() on it), you use visit().
 *
 * If Foo does not inherit from sigc::trackable then invoke() and visit() just return the
 * derived reference.
 *
 * This is used for bound (sigc::bind) slot parameters (via bound_argument), bound return values,
 * and, with mem_fun(), the reference to the handling object.
 *
 * - @e T_type The type of the reference.
 */
template <class T_type,
          bool I_derives_trackable =
            std::is_base_of<trackable, T_type>::value>
class limit_reference
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  limit_reference(T_type& _A_target)
    : visited(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const T_type& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline T_type& invoke() const
    { return visited; }

private:
  /** The reference.
   */
  T_type& visited;
};

/** limit_reference object for a class that derives from trackable.
 * - @e T_type The type of the reference.
 */
template <class T_type>
class limit_reference<T_type, true>
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  limit_reference(T_type& _A_target)
    : visited(_A_target),
      invoked(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const trackable& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline T_type& invoke() const
    { return invoked; }

private:
  /** The trackable reference.
   */
  trackable& visited;

  /** The reference.
   */
  T_type& invoked;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Implementation of visitor specialized for the limit_reference
 * class, to call visit_each() on the entity returned by the limit_reference's
 * visit() method.
 * @tparam T_type The type of the reference.
 * @tparam T_action The type of functor to invoke.
 * @param _A_action The functor to invoke.
 * @param _A_target The visited instance.
 */
template <class T_type, bool I_derives_trackable>
struct visitor<limit_reference<T_type, I_derives_trackable> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const limit_reference<T_type, I_derives_trackable>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.visit());
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


/** A const_limit_reference<Foo> object stores a reference (Foo&), but make sure that,
 * if Foo inherits from sigc::trackable, then visit_each<>() will "limit" itself to the
 * sigc::trackable reference instead of the derived reference. This avoids use of
 * a reference to the derived type when the derived destructor has run. That can be
 * a problem when using virtual inheritance.
 *
 * If Foo inherits from trackable then both the derived reference and the
 * sigc::trackable reference are stored, so we can later retrieve the sigc::trackable
 * reference without doing an implicit conversion. To retrieve the derived reference
 * (so that you invoke methods or members of it), use invoke(). To retrieve the trackable
 * reference (so that you can call visit_each() on it), you use visit().
 *
 * If Foo does not inherit from sigc::trackable then invoke() and visit() just return the
 * derived reference.
 *
 * This is used for bound (sigc::bind) slot parameters (via bound_argument), bound return values,
 * and, with mem_fun(), the reference to the handling object.
 *
 * - @e T_type The type of the reference.
 */
template <class T_type,
          bool I_derives_trackable =
            std::is_base_of<trackable, T_type>::value>
class const_limit_reference
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  const_limit_reference(const T_type& _A_target)
    : visited(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const T_type& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline const T_type& invoke() const
    { return visited; }

private:
  /** The reference.
   */
  const T_type& visited;
};

/** const_limit_reference object for a class that derives from trackable.
 * - @e T_type The type of the reference.
 */
template <class T_type>
class const_limit_reference<T_type, true>
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  const_limit_reference(const T_type& _A_target)
    : visited(_A_target),
      invoked(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const trackable& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline const T_type& invoke() const
    { return invoked; }

private:
  /** The trackable reference.
   */
  const trackable& visited;

  /** The reference.
   */
  const T_type& invoked;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Implementation of visitor specialized for the const_limit_reference
 * class, to call visit_each() on the entity returned by the const_limit_reference's
 * visit() method.
 * @tparam T_type The type of the reference.
 * @tparam T_action The type of functor to invoke.
 * @param _A_action The functor to invoke.
 * @param _A_target The visited instance.
 */
template <class T_type, bool I_derives_trackable>
struct visitor<const_limit_reference<T_type, I_derives_trackable> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const const_limit_reference<T_type, I_derives_trackable>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.visit());
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


/** A volatile_limit_reference<Foo> object stores a reference (Foo&), but make sure that,
 * if Foo inherits from sigc::trackable, then visit_each<>() will "limit" itself to the
 * sigc::trackable reference instead of the derived reference. This avoids use of
 * a reference to the derived type when the derived destructor has run. That can be
 * a problem when using virtual inheritance.
 *
 * If Foo inherits from trackable then both the derived reference and the
 * sigc::trackable reference are stored, so we can later retrieve the sigc::trackable
 * reference without doing an implicit conversion. To retrieve the derived reference
 * (so that you invoke methods or members of it), use invoke(). To retrieve the trackable
 * reference (so that you can call visit_each() on it), you use visit().
 *
 * If Foo does not inherit from sigc::trackable then invoke() and visit() just return the
 * derived reference.
 *
 * This is used for bound (sigc::bind) slot parameters (via bound_argument), bound return values,
 * and, with mem_fun(), the reference to the handling object.
 *
 * - @e T_type The type of the reference.
 */
template <class T_type,
          bool I_derives_trackable =
            std::is_base_of<trackable, T_type>::value>
class volatile_limit_reference
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  volatile_limit_reference(T_type& _A_target)
    : visited(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const T_type& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline volatile T_type& invoke() const
    { return visited; }

private:
  /** The reference.
   */
  T_type& visited;
};

/** volatile_limit_reference object for a class that derives from trackable.
 * - @e T_type The type of the reference.
 */
template <class T_type>
class volatile_limit_reference<T_type, true>
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  volatile_limit_reference(T_type& _A_target)
    : visited(_A_target),
      invoked(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const trackable& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline volatile T_type& invoke() const
    { return invoked; }

private:
  /** The trackable reference.
   */
  trackable& visited;

  /** The reference.
   */
  T_type& invoked;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Implementation of visitor specialized for the volatile_limit_reference
 * class, to call visit_each() on the entity returned by the volatile_limit_reference's
 * visit() method.
 * @tparam T_type The type of the reference.
 * @tparam T_action The type of functor to invoke.
 * @param _A_action The functor to invoke.
 * @param _A_target The visited instance.
 */
template <class T_type, bool I_derives_trackable>
struct visitor<volatile_limit_reference<T_type, I_derives_trackable> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const volatile_limit_reference<T_type, I_derives_trackable>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.visit());
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS


/** A const_volatile_limit_reference<Foo> object stores a reference (Foo&), but make sure that,
 * if Foo inherits from sigc::trackable, then visit_each<>() will "limit" itself to the
 * sigc::trackable reference instead of the derived reference. This avoids use of
 * a reference to the derived type when the derived destructor has run. That can be
 * a problem when using virtual inheritance.
 *
 * If Foo inherits from trackable then both the derived reference and the
 * sigc::trackable reference are stored, so we can later retrieve the sigc::trackable
 * reference without doing an implicit conversion. To retrieve the derived reference
 * (so that you invoke methods or members of it), use invoke(). To retrieve the trackable
 * reference (so that you can call visit_each() on it), you use visit().
 *
 * If Foo does not inherit from sigc::trackable then invoke() and visit() just return the
 * derived reference.
 *
 * This is used for bound (sigc::bind) slot parameters (via bound_argument), bound return values,
 * and, with mem_fun(), the reference to the handling object.
 *
 * - @e T_type The type of the reference.
 */
template <class T_type,
          bool I_derives_trackable =
            std::is_base_of<trackable, T_type>::value>
class const_volatile_limit_reference
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  const_volatile_limit_reference(const T_type& _A_target)
    : visited(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const T_type& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline const volatile T_type& invoke() const
    { return visited; }

private:
  /** The reference.
   */
  const T_type& visited;
};

/** const_volatile_limit_reference object for a class that derives from trackable.
 * - @e T_type The type of the reference.
 */
template <class T_type>
class const_volatile_limit_reference<T_type, true>
{
public:
  /** Constructor.
   * @param _A_target The reference to limit.
   */
  const_volatile_limit_reference(const T_type& _A_target)
    : visited(_A_target),
      invoked(_A_target)
    {}

  /** Retrieve the entity to visit for visit_each().
   * Depending on the template specialization, this is either a derived reference, or sigc::trackable& if T_type derives from sigc::trackable.
   * @return The reference.
   */
  inline const trackable& visit() const
    { return visited; }

  /** Retrieve the reference.
   * This is always a reference to the derived instance.
   * @return The reference.
   */
  inline const volatile T_type& invoke() const
    { return invoked; }

private:
  /** The trackable reference.
   */
  const trackable& visited;

  /** The reference.
   */
  const T_type& invoked;
};

#ifndef DOXYGEN_SHOULD_SKIP_THIS
/** Implementation of visitor specialized for the const_volatile_limit_reference
 * class, to call visit_each() on the entity returned by the const_volatile_limit_reference's
 * visit() method.
 * @tparam T_type The type of the reference.
 * @tparam T_action The type of functor to invoke.
 * @param _A_action The functor to invoke.
 * @param _A_target The visited instance.
 */
template <class T_type, bool I_derives_trackable>
struct visitor<const_volatile_limit_reference<T_type, I_derives_trackable> >
{
  template <class T_action>
  static void do_visit_each(const T_action& _A_action,
                            const const_volatile_limit_reference<T_type, I_derives_trackable>& _A_target)
  {
    sigc::visit_each(_A_action, _A_target.visit());
  }
};
#endif // DOXYGEN_SHOULD_SKIP_THIS

} /* namespace sigc */

#endif /* _SIGC_LIMIT_REFERENCE_H_ */
