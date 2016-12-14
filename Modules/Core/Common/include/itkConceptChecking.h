/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
/*=========================================================================
 *
 *  Portions of this file are subject to the VTK Toolkit Version 3 copyright.
 *
 *  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 *
 *  For complete copyright, license and disclaimer of warranty information
 *  please refer to the NOTICE file at the top of the ITK source tree.
 *
 *=========================================================================*/
#ifndef itkConceptChecking_h
#define itkConceptChecking_h

#include "itkPixelTraits.h"
#include "itkNumericTraits.h"
#include <iostream>

/** Choose a concept checking implementation based on compiler abilities. */
#ifndef ITK_CONCEPT_NO_CHECKING
#if defined( _MSC_VER ) && !defined( __ICL )
#define ITK_CONCEPT_IMPLEMENTATION_VTABLE
//TST_RMV_20100730 #elif defined(__SUNPRO_CC)
//TST_RMV_20100730 #define ITK_CONCEPT_IMPLEMENTATION_VTABLE
#else
#define ITK_CONCEPT_IMPLEMENTATION_STANDARD
#endif
#endif

/** Define the concept checking implementation chosen above. */
#if defined( ITK_CONCEPT_IMPLEMENTATION_STANDARD )

/**
 * Standard instantiation-time concept check.  No run-time overhead
 * introduced.  This implementation is based on "Concept Checking:
 * Binding Parametric Polymorphism in C++" by Jeremy Siek and Andrew
 * Lumsdaine, University of Notre Dame.
 */

// Leave ()'s off the sizeof to force the caller to pass them in the
// concept argument of the itkConceptMacro.  This is necessary because
// the argument may contain commas.
#define itkConceptConstraintsMacro()    \
  template< void (Constraints::*) ( ) > \
  struct ITK_TEMPLATE_EXPORT Enforcer {};                   \
  typedef Enforcer< & Constraints::constraints > EnforcerInstantiation
#define itkConceptMacro(name, concept) enum { name = sizeof concept }

#elif defined( ITK_CONCEPT_IMPLEMENTATION_VTABLE )

/**
 * Alternate implementation for some compilers.  This introduces no
 * run-time overhead.  The "vtable" approach was invented for this
 * project by Brad King at Kitware.
 */
#define itkConceptConstraintsMacro() \
  virtual void Enforcer() { &Constraints::constraints; }
#define itkConceptMacro(name, concept) enum { name = sizeof concept }

#elif defined( ITK_CONCEPT_IMPLEMENTATION_CALL )

/** Not implemented.  */
#define itkConceptConstraintsMacro()
#define itkConceptMacro(name, concept) enum { name = 0 }

#else

/** Disable concept checking.  */
#define itkConceptConstraintsMacro()
#define itkConceptMacro(name, concept) enum { name = 0 }

#endif

namespace itk
{
/** All concept class definitions are contained in the "itk::Concept"
    namespace. */
namespace Concept
{
/**
 * Some concept implementation details are adapted from the BOOST C++
 * libraries (www.boost.org).  These are marked with "(BOOST)" in the
 * corresponding comment.
 */

/** Namespace containing concept check implementation details. */
namespace Detail
{
template< typename T >
struct ITK_TEMPLATE_EXPORT UniqueType {};
template< int >
struct ITK_TEMPLATE_EXPORT UniqueType_int {};
template< unsigned int >
struct ITK_TEMPLATE_EXPORT UniqueType_unsigned_int {};
template< bool >
struct ITK_TEMPLATE_EXPORT UniqueType_bool {};

/**
 * Concept checks may require a variable to be declared but not used.
 * This function can be called with the variable to prevent the compiler
 * warning. (BOOST)
 */
template< typename T >
inline void IgnoreUnusedVariable(T) {}

/**
 * Concept checks may require that an expression be convertible to bool.
 * Passing the expression to this function will enforce this requirement.
 * (BOOST)
 */
template< typename T >
void RequireBooleanExpression(const T & t)
{
  bool x = t;

  IgnoreUnusedVariable(x);
}
}   // namespace Detail

/** Concept requiring T to have a default constructor. (BOOST) */
template< typename T >
struct ITK_TEMPLATE_EXPORT DefaultConstructible {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      T a;

      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have a copy constructor. (BOOST) */
template< typename T >
struct ITK_TEMPLATE_EXPORT CopyConstructible {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      T  a(b);
      T *p = &a;

      const_constraints(a);
      Detail::IgnoreUnusedVariable(p);
    }

    void const_constraints(const T & a)
    {
      T        c(a);
      const T *p = &a;

      Detail::IgnoreUnusedVariable(c);
      Detail::IgnoreUnusedVariable(p);
    }

    T b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to be convertible to T2. (BOOST) */
template< typename T1, typename T2 >
struct ITK_TEMPLATE_EXPORT Convertible {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      T2 b = static_cast< T2 >( a );

      Detail::IgnoreUnusedVariable(b);
    }

    T1 a;
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operator =.  (BOOST) */
template< typename T >
struct ITK_TEMPLATE_EXPORT Assignable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = a;
      const_constraints(a);
    }

    void const_constraints(const T & b)
    {
      a = b;
    }

    T a;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators < and <= with a right-hand operator
    of type T2.  (BOOST) */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT LessThanComparable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::RequireBooleanExpression(a < b);
      Detail::RequireBooleanExpression(a <= b);
    }

    T1 a;
    T2 b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators > and >= with a right-hand operator
    of type T2.  (BOOST) */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT GreaterThanComparable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::RequireBooleanExpression(a > b);
      Detail::RequireBooleanExpression(a >= b);
    }

    T1 a;
    T2 b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators == and != with a right-hand operator
    of type T2.  (BOOST) */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT EqualityComparable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
CLANG_PRAGMA_PUSH
CLANG_SUPPRESS_Wfloat_equal
      Detail::RequireBooleanExpression(a == b);
      Detail::RequireBooleanExpression(a != b);
CLANG_PRAGMA_POP
    }

    T1 a;
    T2 b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators <, >, <=, >=, ==, != with a
    right-hand operator of type T2. (BOOST) */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT Comparable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::RequireBooleanExpression(a < b);
      Detail::RequireBooleanExpression(a > b);
      Detail::RequireBooleanExpression(a <= b);
      Detail::RequireBooleanExpression(a >= b);
CLANG_PRAGMA_PUSH
CLANG_SUPPRESS_Wfloat_equal
      Detail::RequireBooleanExpression(a == b);
      Detail::RequireBooleanExpression(a != b);
CLANG_PRAGMA_POP
    }

    T1 a;
    T2 b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators +, -, in the form
    T1 op T2 = T3.  */
template< typename T1, typename T2 = T1, typename T3 = T1 >
struct ITK_TEMPLATE_EXPORT AdditiveOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = static_cast< T3 >( b + c );
      a = static_cast< T3 >( b - c );
      const_constraints(b, c);
    }

    void const_constraints(const T1 & d, const T2 & e)
    {
      a = static_cast< T3 >( d + e );
      a = static_cast< T3 >( d - e );
    }

    T3 a;
    T1 b;
    T2 c;
  };

  itkConceptConstraintsMacro();
};


/** Concept requiring T1 to have operators +=, -= in the form
    T2 op= T1.  */
template< typename T1, typename T2 = T1>
struct ITK_TEMPLATE_EXPORT AdditiveAndAssignOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a += c;
      a -= c;
      const_constraints(c);
    }

    void const_constraints(const T1 & d)
    {
      a += d;
      a -= d;
    }

    T2 a;
    T1 c;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operator * in the form T1 op T2 = T3. */
template< typename T1, typename T2 = T1, typename T3 = T1 >
struct ITK_TEMPLATE_EXPORT MultiplyOperator {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = static_cast< T3 >( b * c );
      const_constraints(b, c);
    }

    void const_constraints(const T1 & d, const T2 & e)
    {
      a = static_cast< T3 >( d * e );
    }

    T3 a;
    T1 b;
    T2 c;
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operator  *= in the form T2 op= T1. */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT MultiplyAndAssignOperator {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a *= b;
      const_constraints(b);
    }

    void const_constraints(const T1 & d)
    {
      a *= d;
    }

    T2 a;
    T1 b;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operators / the form T1 op T2 = T3. */
template< typename T1, typename T2 = T1, typename T3 = T1 >
struct ITK_TEMPLATE_EXPORT DivisionOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = static_cast< T3 >( b / c );
      const_constraints(b, c);
    }

    void const_constraints(const T1 & d, const T2 & e)
    {
      a = static_cast< T3 >( d / e );
    }

    T3 a;
    T1 b;
    T2 c;
  };

  itkConceptConstraintsMacro();
};


/** Concept requiring T to have operators /= in the form T2 op= T1. */
template< typename T1, typename T2 = T1 >
struct ITK_TEMPLATE_EXPORT DivisionAndAssignOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a /= c;
      const_constraints(c);
    }

    void const_constraints(const T1 & d)
    {
      a /= d;
    }

    T1 c;
    T2 a;
  };

  itkConceptConstraintsMacro();
};


/** Concept requiring T1 to have operators &, |, and ^ in the form
    T1 op T2 = T3.  */
template< typename T1, typename T2 = T1, typename T3 = T1 >
struct ITK_TEMPLATE_EXPORT BitwiseOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = static_cast< T3 >( b & c );
      a = static_cast< T3 >( b | c );
      a = static_cast< T3 >( b ^ c );
      a &= static_cast< T3 >( c );
      a |= static_cast< T3 >( c );
      a ^= static_cast< T3 >( c );
      const_constraints(b, c);
    }

    void const_constraints(const T1 & d, const T2 & e)
    {
      a = static_cast< T3 >( d & e );
      a = static_cast< T3 >( d | e );
      a = static_cast< T3 >( d ^ e );
      a &= static_cast< T3 >( e );
      a |= static_cast< T3 >( e );
      a ^= static_cast< T3 >( e );
    }

    T3 a;
    T1 b;
    T2 c;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 to have operators []  in the form T1 [] T2 = T3.  */
template< typename T1, typename T2 = T1, typename T3 = T1 >
struct ITK_TEMPLATE_EXPORT BracketOperator {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = static_cast< T3 >( b[c] );
      const_constraints(b, c);
    }

    void const_constraints(const T1 & d, const T2 & e)
    {
      a = static_cast< T3 >( d[e] );
    }

    T3 a;
    T1 b;
    T2 c;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operator !.  */
template< typename T >
struct ITK_TEMPLATE_EXPORT NotOperator {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a = !a;
    }

    T a;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have operators ++ and --.  */
template< typename T >
struct ITK_TEMPLATE_EXPORT IncrementDecrementOperators {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      a++;
      a--;
      ++a;
      --a;
    }

    T a;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to be writable to an ostream.  */
template< typename T >
struct ITK_TEMPLATE_EXPORT OStreamWritable {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      std::cout << a;
    }

    T a;
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to be signed. */
template< typename T >
struct ITK_TEMPLATE_EXPORT Signed {
  typedef Signed Self;
  itkStaticConstMacro(IsSigned, bool, NumericTraits< T >::is_signed);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< true >                             TrueT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(IsSigned) > SignedT;
    void constraints()
    {
      SignedT a = TrueT();

      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T1 and T2 to be the same type. */
template< typename T1, typename T2 >
struct ITK_TEMPLATE_EXPORT SameType {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::UniqueType< T1 > a = Detail::UniqueType< T2 >();
      Detail::IgnoreUnusedVariable(a);
    }
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring D1 and D2 to be the same dimension. */
template< unsigned int D1, unsigned int D2 >
struct ITK_TEMPLATE_EXPORT SameDimension {
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_unsigned_int< D1 > DT1;
    typedef Detail::UniqueType_unsigned_int< D2 > DT2;
    void constraints()
    {
      DT1 a = DT2();

      Detail::IgnoreUnusedVariable(a);
    }
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring T to have NumericTraits */
template< typename T >
struct ITK_TEMPLATE_EXPORT HasNumericTraits {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::UniqueType< typename NumericTraits< T >::ValueType >();
      Detail::UniqueType< typename NumericTraits< T >::PrintType >();
      Detail::UniqueType< typename NumericTraits< T >::AbsType >();
      Detail::UniqueType< typename NumericTraits< T >::AccumulateType >();
      Detail::UniqueType< typename NumericTraits< T >::RealType >();
      Detail::UniqueType< typename NumericTraits< T >::ScalarRealType >();
      Detail::UniqueType< typename NumericTraits< T >::FloatType >();
      T    a;
      bool b;

      // Test these methods that take an instance of T to
      // allow for types with variable length.
      a = NumericTraits< T >::NonpositiveMin( a );
      a = NumericTraits< T >::ZeroValue( a );
      a = NumericTraits< T >::OneValue( a );

      b = NumericTraits< T >::IsPositive(a);
      b = NumericTraits< T >::IsNonpositive(a);
      b = NumericTraits< T >::IsNegative(a);
      b = NumericTraits< T >::IsNonnegative(a);
      Detail::IgnoreUnusedVariable(a);
      Detail::IgnoreUnusedVariable(b);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have PixelTraits */
template< typename T >
struct ITK_TEMPLATE_EXPORT HasPixelTraits {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::UniqueType< typename PixelTraits< T >::ValueType >();
      unsigned int a = PixelTraits< T >::Dimension;
      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have a trait called ValueType */
template< typename T >
struct ITK_TEMPLATE_EXPORT HasValueType {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::UniqueType< typename T::ValueType >();
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have Zero */
template< typename T >
struct ITK_TEMPLATE_EXPORT HasZero {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      T a;

      a = NumericTraits< T >::ZeroValue();
      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to have JoinTraits */
template< typename T1, typename T2 >
struct ITK_TEMPLATE_EXPORT HasJoinTraits {
  struct ITK_TEMPLATE_EXPORT Constraints {
    void constraints()
    {
      Detail::UniqueType< typename JoinTraits< T1, T2 >::ValueType >();
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring D1 and D2 to be the same dimension or D2-1 = D1. */
template< unsigned int D1, unsigned int D2 >
struct ITK_TEMPLATE_EXPORT SameDimensionOrMinusOne {
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_unsigned_int< D1 >     Type1;
    typedef Detail::UniqueType_unsigned_int< D1 - 1 > Type2;

    void f(Type1) {}
    void f(Type2, int = 0) {}

    void constraints()
    {
      Detail::UniqueType_unsigned_int< D2 > tt;
      this->f(tt);
    }
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring D1 and D2 to be the same dimension or D2-1 = D1. */
template< unsigned int D1, unsigned int D2 >
struct ITK_TEMPLATE_EXPORT SameDimensionOrMinusOneOrTwo {
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_unsigned_int< D1 >     Type1;
    typedef Detail::UniqueType_unsigned_int< D1 - 1 > Type2;
    typedef Detail::UniqueType_unsigned_int< D1 - 2 > Type3;

    void f(Type1) {}
    void f(Type2, int = 0) {}
    void f(Type3, int = 0, int = 0) {}

    void constraints()
    {
      Detail::UniqueType_unsigned_int< D2 > tt;
      this->f(tt);
    }
  };
  itkConceptConstraintsMacro();
};

/** Concept requiring T to be integer. */
template< typename T >
struct ITK_TEMPLATE_EXPORT IsInteger {
  typedef IsInteger Self;
  itkStaticConstMacro(Integral, bool, NumericTraits< T >::is_integer);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< true >                             TrueT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(Integral) > IntegralT;
    void constraints()
    {
      IntegralT a = TrueT();

      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};


/** Concept requiring T to be an unsigned integer. */
template< typename T >
struct ITK_TEMPLATE_EXPORT IsUnsignedInteger {
  typedef IsUnsignedInteger Self;
  itkStaticConstMacro(Unsigned, bool, !NumericTraits< T >::is_signed);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< true >                             TrueT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(Unsigned) > UnsignedT;
    void constraints()
    {
      UnsignedT a = TrueT();

      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};


/** Concept requiring T to be non-integer. */
template< typename T >
struct ITK_TEMPLATE_EXPORT IsNonInteger {
  typedef IsNonInteger Self;
  itkStaticConstMacro(NonIntegral, bool, NumericTraits< T >::is_integer);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< false >                               FalseT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(NonIntegral) > NonIntegralT;
    void constraints()
    {
      NonIntegralT a = FalseT();

      Detail::IgnoreUnusedVariable(a);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to be floating point. */
template< typename T >
struct ITK_TEMPLATE_EXPORT IsFloatingPoint {
  typedef IsFloatingPoint Self;
  itkStaticConstMacro(Integral, bool, NumericTraits< T >::is_integer);
  itkStaticConstMacro(IsExact, bool, NumericTraits< T >::is_exact);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< false >                            FalseT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(Integral) > IntegralT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(IsExact) >  ExactT;
    void constraints()
    {
      IntegralT a = FalseT();
      ExactT    b = FalseT();

      Detail::IgnoreUnusedVariable(a);
      Detail::IgnoreUnusedVariable(b);
    }
  };

  itkConceptConstraintsMacro();
};

/** Concept requiring T to be fixed point. */
template< typename T >
struct ITK_TEMPLATE_EXPORT IsFixedPoint {
  typedef IsFixedPoint Self;
  itkStaticConstMacro(Integral, bool, NumericTraits< T >::is_integer);
  itkStaticConstMacro(IsExact, bool, NumericTraits< T >::is_exact);
  struct ITK_TEMPLATE_EXPORT Constraints {
    typedef Detail::UniqueType_bool< true >                             TrueT;
    typedef Detail::UniqueType_bool< false >                            FalseT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(Integral) > IntegralT;
    typedef Detail::UniqueType_bool< itkGetStaticConstMacro(IsExact) >  ExactT;
    void constraints()
    {
      IntegralT a = FalseT();
      ExactT    b = TrueT();

      Detail::IgnoreUnusedVariable(a);
      Detail::IgnoreUnusedVariable(b);
    }
  };

  itkConceptConstraintsMacro();
};
} // end namespace Concept
} // end namespace itk

#endif
