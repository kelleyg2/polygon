// Boost.Polygon library polygon_interval_test.cpp file

//          Copyright Andrii Sydorchuk 2012.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// See http://www.boost.org for updates, documentation, and revision history.

#define BOOST_TEST_MODULE POLYGON_INTERVAL_TEST
#include <boost/mpl/list.hpp>
#include <boost/test/test_case_template.hpp>

#include "boost/polygon/interval_concept.hpp"
#include "boost/polygon/interval_data.hpp"
#include "boost/polygon/interval_traits.hpp"
using namespace boost::polygon;

typedef boost::mpl::list<int> test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(interval_data_test, T, test_types) {
  typedef interval_data<T> interval_type;
  interval_type interval1(1, 2);
  interval_type interval2;
  interval2 = interval1;

  BOOST_CHECK_EQUAL(interval1.low(), 1);
  BOOST_CHECK_EQUAL(interval1.high(), 2);
  BOOST_CHECK_EQUAL(interval1.get(LOW), 1);
  BOOST_CHECK_EQUAL(interval1.get(HIGH), 2);
  BOOST_CHECK(interval1 == interval2);
  BOOST_CHECK(!(interval1 != interval2));
  BOOST_CHECK(!(interval1 < interval2));
  BOOST_CHECK(!(interval1 > interval2));
  BOOST_CHECK(interval1 <= interval2);
  BOOST_CHECK(interval1 >= interval2);

  interval1.low(2);
  interval1.high(1);
  BOOST_CHECK_EQUAL(interval1.low(), 2);
  BOOST_CHECK_EQUAL(interval1.high(), 1);
  BOOST_CHECK(!(interval1 == interval2));
  BOOST_CHECK(interval1 != interval2);

  interval2.set(LOW, 2);
  interval2.set(HIGH, 1);
  BOOST_CHECK(interval1 == interval2);
}

BOOST_AUTO_TEST_CASE_TEMPLATE(interval_traits_test, T, test_types) {
  typedef interval_data<T> interval_type;

  interval_type interval = interval_mutable_traits<interval_type>::construct(1, 2);
  BOOST_CHECK_EQUAL(interval_traits<interval_type>::get(interval, LOW), 1);
  BOOST_CHECK_EQUAL(interval_traits<interval_type>::get(interval, HIGH), 2);

  interval_mutable_traits<interval_type>::set(interval, LOW, 3);
  interval_mutable_traits<interval_type>::set(interval, HIGH, 4);
  BOOST_CHECK_EQUAL(interval_traits<interval_type>::get(interval, LOW), 3);
  BOOST_CHECK_EQUAL(interval_traits<interval_type>::get(interval, HIGH), 4);
}

template <typename T>
struct Interval {
  T left;
  T right;
};

namespace boost {
namespace polygon {
  template <typename T>
  struct geometry_concept< Interval<T> > {
    typedef interval_concept type;
  };

  template <typename T>
  struct interval_traits< Interval<T> > {
    typedef T coordinate_type;

    static coordinate_type get(const Interval<T>& interval, direction_1d dir) {
      return (dir == LOW) ? interval.left : interval.right;
    }
  };

  template <typename T>
  struct interval_mutable_traits< Interval<T> > {
    typedef T coordinate_type;

    static void set(Interval<T>& interval, direction_1d dir, T value) {
      (dir == LOW) ? interval.left = value : interval.right = value;
    }

    static Interval<T> construct(coordinate_type left, coordinate_type right) {
      Interval<T> interval;
      interval.left = left;
      interval.right = right;
      return interval;
    }
  };
}  // polygon
}  // boost

BOOST_AUTO_TEST_CASE_TEMPLATE(interval_concept_test1, T, test_types) {
  typedef Interval<T> interval_type;

  interval_type interval1 = construct<interval_type>(2, 1);
  BOOST_CHECK_EQUAL(interval1.left, 1);
  BOOST_CHECK_EQUAL(interval1.right, 2);

  set(interval1, LOW, 3);
  set(interval1, HIGH, 4);
  BOOST_CHECK_EQUAL(get(interval1, LOW), 3);
  BOOST_CHECK_EQUAL(get(interval1, HIGH), 4);

  interval_type interval2 = copy_construct<interval_type>(interval1);
  BOOST_CHECK(equivalence(interval1, interval2));

  low(interval2, 1);
  high(interval2, 2);
  BOOST_CHECK_EQUAL(low(interval2), 1);
  BOOST_CHECK_EQUAL(high(interval2), 2);

  assign(interval1, interval2);
  BOOST_CHECK(equivalence(interval1, interval2));
}

BOOST_AUTO_TEST_CASE_TEMPLATE(interval_concept_test2, T, test_types) {
  typedef Interval<T> interval_type;

  interval_type interval = construct<interval_type>(1, 3);
  BOOST_CHECK_EQUAL(center(interval), 2);
  BOOST_CHECK_EQUAL(delta(interval), 2);

  flip(interval, -1);
  BOOST_CHECK_EQUAL(low(interval), -5);
  BOOST_CHECK_EQUAL(high(interval), -3);

  scale_up(interval, 2);
  BOOST_CHECK_EQUAL(low(interval), -10);
  BOOST_CHECK_EQUAL(high(interval), -6);

  scale_down(interval, 2);
  BOOST_CHECK_EQUAL(low(interval), -5);
  BOOST_CHECK_EQUAL(high(interval), -3);

  move(interval, 5);
  BOOST_CHECK_EQUAL(low(interval), 0);
  BOOST_CHECK_EQUAL(high(interval), 2);
}
