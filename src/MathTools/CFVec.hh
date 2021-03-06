// Copyright (C) 2012 von Karman Institute for Fluid Dynamics, Belgium
//
// This software is distributed under the terms of the
// GNU Lesser General Public License version 3 (LGPLv3).
// See doc/lgpl.txt and doc/gpl.txt for the license text.

#ifndef COOLFluiD_MathTools_CFVec_hh
#define COOLFluiD_MathTools_CFVec_hh

//////////////////////////////////////////////////////////////////////////////

#include "MathTools/ArrayT.hh"
#include "MathTools/CFVecSlice.hh"
#include "MathTools/MathFunctions.hh"

//////////////////////////////////////////////////////////////////////////////

namespace COOLFluiD {

  namespace MathTools {
  
  template  <typename T, int N> class CFVec;
  
  template <typename T, int N> std::ostream& operator<< (std::ostream& out, const CFVec<T,N>& v);
  template <typename T, int N> std::istream& operator>> (std::istream& in, CFVec<T,N>& v);
  template <typename T, int N> bool operator== (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator== (const CFVec<T,N>& v1, T value);
  template <typename T, int N> bool operator!= (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator!= (const CFVec<T,N>& v1, T value);
  
#ifndef CF_HAVE_CUDA
  template <typename T, int N> bool operator<  (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator<  (const CFVec<T,N>& v1, T value);
#endif
  
  template <typename T, int N> bool operator>  (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator>  (const CFVec<T,N>& v1, T value);
  template <typename T, int N> bool operator<= (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator<= (const CFVec<T,N>& v1, T value);
  template <typename T, int N> bool operator>= (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  template <typename T, int N> bool operator>= (const CFVec<T,N>& v1, T value);
  template <typename T, int N> void copy       (const CFVec<T,N>& v1, CFVec<T,N>& v2);

//////////////////////////////////////////////////////////////////////////////
  
/// Definition of a class CFVec that implements an expression template technique
/// with statically defined size
/// @author Andrea Lani
template <typename T, int N = 0>
class CFVec : public EETVEC(ExprT,CFVec,T,N) {
public:
  
  /// Default constructor
  HHOST_DEV CFVec() : EETVEC(ExprT,CFVec,T,N)(this) {this->initialize(T());}
  
  /// Copy constructor from expression
  template <typename EXPR>
  HHOST_DEV CFVec(EETYPEV(EXPR) expr) : EETVEC(ExprT,CFVec,T,N)(this) {EVECLOOP(i, 0, N, m_data[i] = expr.at(i));}
    
  /// Copy constructor from CFVec
  HHOST_DEV CFVec(const CFVec<T,N>& orig) : EETVEC(ExprT,CFVec,T,N)(this) {copy(orig,*this);}
  
  /// Default destructor
  HHOST_DEV ~CFVec() {}
  
  /// Overloading of assignment operator(s)
#define VEC_EASSIGN_OP(__op__) \
  template <typename EXPR>						\
  HHOST_DEV const CFVec<T,N>& operator __op__ (EETYPEV(EXPR) expr) {EVECLOOP(i, 0, N, m_data[i] __op__ expr.at(i)); return *this;}
VEC_EASSIGN_OP(=)
VEC_EASSIGN_OP(+=)
VEC_EASSIGN_OP(-=)
VEC_EASSIGN_OP(*=)
VEC_EASSIGN_OP(/=)
#undef VEC_EASSIGN_OP

  /// Overloading of assignment operator(s) with constants
#define VEC_EASSIGN_OP_CONST(__op__) \
  HHOST_DEV const CFVec<T,N>& operator __op__ (T expr) {EVECLOOP(i, 0, N, m_data[i] __op__ expr); return *this;}
VEC_EASSIGN_OP_CONST(=)
VEC_EASSIGN_OP_CONST(+=)
VEC_EASSIGN_OP_CONST(-=)
VEC_EASSIGN_OP_CONST(*=)
VEC_EASSIGN_OP_CONST(/=)
#undef VEC_EASSIGN_OP_CONST

  /// copy content of another array
  template <typename ARRAY>
  HHOST_DEV void copyFrom(const ARRAY& in) {EVECLOOP(i, 0, N, m_data[i] = in[i]);}
  
  /// copy content of another array
  template <typename ARRAY>
  HHOST_DEV void copyTo(ARRAY& in) {EVECLOOP(i, 0, N, in[i] = m_data[i]);}
  
  /// @return a vector slice with fixed size
  template <int NV>
  HHOST_DEV CFVecSlice<T,NV> slice(size_t start) {return CFVecSlice<T,NV>(&m_data[start]);}
  
  /// @return a vector slice
  HHOST_DEV CFVecSlice<T,0> slice(size_t start, size_t ns) {return CFVecSlice<T,0>(&m_data[start], ns);}
  
  /// @return the raw data
  HHOST_DEV T* ptr() {return &m_data[0];}
  
  /// return the array size 
  HHOST_DEV size_t size() const {return N;}
    
  /// Overloading of the stream operator "<<" for the output.
  /// "\n"ine introduced at the end of every line of the matrix.
  friend std::ostream& operator<<
    LTGT (std::ostream& out, const CFVec<T,N>& v);
    
  /// Overloading of the stream operator ">>" for the input
  friend std::istream& operator>> 
    LTGT (std::istream& in, CFVec<T,N>& v);

  /// Overloading of the "==" operator.
  /// @return true if all elements are equal elementwise
  HHOST_DEV friend bool operator== LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);

  /// Overloading of the "==" operator.
  /// @param v given array
  /// @param value value for the comparison
  /// @return true if all elements are equal to value
  HHOST_DEV friend bool operator== LTGT (const CFVec<T,N>& v1, T value);
 
  /// Overloading of the "!=" operator.
  /// @return true if all elements are not equal elementwise
  HHOST_DEV friend bool operator!= LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);

  /// Overloading of the "!=" operator.
  /// @param v given array
  /// @param value value for the comparison
  /// @return true if at least one element is not equal to value
  HHOST_DEV friend bool operator!= LTGT (const CFVec<T,N>& v1, T value);

#ifndef CF_HAVE_CUDA    
  /// Overloading of the "<" operator.
  /// @return true if the norm of the first CFVec is < than the norm of the second CFVec.
  HHOST_DEV friend bool operator< LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  
  /// Overloading of the "<" operator.
  /// @return true if all entries of the first CFVec are < than the given value
  HHOST_DEV friend bool operator< LTGT (const CFVec<T,N>& v1, T value);
#endif
  
  /// Overloading of the ">" operator.
  /// @return true if the norm of the first CFVec is > than the norm of the second CFVec.
  HHOST_DEV friend bool operator> LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
    
  /// Overloading of the ">" operator.
  /// @return true if all entries of the first CFVec are > than the given value
  HHOST_DEV friend bool operator> LTGT (const CFVec<T,N>& v1, T value);
  
  /// Overloading of the "<=" operator.
  /// @return true if the norm of the first CFVec is <= than the norm of the second CFVec.
  HHOST_DEV friend bool operator<= LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  
  /// Overloading of the "<=" operator.
  /// @return true if all entries of the first CFVec are <= than the given value
  HHOST_DEV friend bool operator<= LTGT (const CFVec<T,N>& v1, T value);
  
  /// Overloading of the ">=" operator.
  /// @return true if the norm of the first CFVec is >= than the norm of the second CFVec.
  HHOST_DEV friend bool operator>= LTGT (const CFVec<T,N>& v1, const CFVec<T,N>& v2);
  
  /// Overloading of the ">=" operator.
  /// @return true if all entries of the first CFVec are >= than the given value
  HHOST_DEV friend bool operator>= LTGT (const CFVec<T,N>& v1, T value);
   
  /// Copy one CFVec into another one
  /// @pre v1.size() == v2.size()
  /// @param v1 source vector
  /// @param v2 destination vector
  HHOST_DEV friend void copy LTGT (const CFVec<T,N>& orig, CFVec<T,N>& dest);

  /// Normalizes this vector so that the norm is equal to one.
  /// \f$ v = v/\|v\|\f$
  HHOST_DEV void normalize() {*this /= this->norm2();}

  /// Projection of one vector onto another.
  /// @param v1 1st CFCFVector
  /// @param v2 2nd CFCFVector
  /// @post this CFVec contains the projected vector of v2 onto v1.
  template <typename V1, typename V2>  
  HHOST_DEV void proj(const V1& v1, const V2& v2)
  {
    assert(v1.size() = v2.size());
    T scale = (MathFunctions::innerProd(v1, v2) / v1.sqrNorm());
    *this = v1 * scale;
  }

  /// Projection of this CFCFVector onto a given one.
  /// @pre this and the given object must be of the same size.
  /// @param v1 the other CFCFVector
  /// @post this CFCFVector contains the projected vector of itself onto v1.
  template <typename V1> 
  HHOST_DEV void proj(const V1& v1) { proj(*this,v1);}
  
private:
  
  /// array data
  T m_data[N];
};
  
//////////////////////////////////////////////////////////////////////////////

/// Definition of a class CFVec that implements an expression template technique
/// with dynamical size
/// @author Andrea Lani
template <typename T>
class CFVec<T,0> : public EETVEC(ExprT,CFVec,T,0) {
public:
  
  /// Constructor
  HHOST_DEV CFVec(T value, size_t ns) : EETVEC(ExprT,CFVec,T,0)(this), m_owner(true), m_size(ns), m_data(NULL) 
  {
    allocate(); 
    this->initialize(value);
  }
  
  /// Constructor
  HHOST_DEV CFVec(size_t ns = 0) : EETVEC(ExprT,CFVec,T,0)(this), m_owner(true), m_size(ns), m_data(NULL) 
  {
   if (ns > 0) {
      allocate(); 
      this->initialize(T());
    }
  }
    
  // Constructor from preallocated memory
  HHOST_DEV CFVec(size_t ns, T* data) : 
    EETVEC(ExprT,CFVec,T,0)(this), m_owner(false), m_size(ns), m_data(data) {}
     
  /// Copy constructor from expression
  template <typename EXPR>
  HHOST_DEV CFVec(EETYPEV(EXPR) expr) : EETVEC(ExprT,CFVec,T,0)(this)
  {
    m_owner = true; m_size = expr.size(); allocate(); 
    EVECLOOP(i, 0, EGETSIZE1(ENMAX(0,EXPR::SIZE1)), m_data[i] = expr.at(i));
  }

  /// Copy constructor from CFVec
  HHOST_DEV CFVec(const CFVec<T,0>& orig) : 
    EETVEC(ExprT,CFVec,T,0)(this), m_owner(orig.m_owner), m_size(orig.m_size)  
  {
    if (m_owner) {allocate(); copy(orig,*this);}
    else {m_data = orig.m_data;}
  }
    
  /// Default destructor
  HHOST_DEV ~CFVec() {free();}
  
  /// Tell if the array allocate its own memory
  bool isMemoryOwner() const {return m_owner;}
  
  /// Overloading of assignment operator(s)
#define VEC0_EASSIGN_OP(__op__) \
  template <typename EXPR>						\
  HHOST_DEV const CFVec<T,0>& operator __op__ (EETYPEV(EXPR) expr)	\
    {EVECLOOP(i, 0, EGETSIZE1(ENMAX(0,EXPR::SIZE1)), m_data[i] __op__ expr.at(i)); return *this;}
VEC0_EASSIGN_OP(=)
VEC0_EASSIGN_OP(+=)
VEC0_EASSIGN_OP(-=)
VEC0_EASSIGN_OP(*=)
VEC0_EASSIGN_OP(/=)
#undef VEC0_EASSIGN_OP

   /// Overloading of assignment operator(s) with constants
#define VEC0_EASSIGN_OP_CONST(__op__) \
  HHOST_DEV const CFVec<T,0>& operator __op__ (T expr) {EVECLOOP(i, 0, size(), m_data[i] __op__ expr); return *this;}
VEC0_EASSIGN_OP_CONST(=)
VEC0_EASSIGN_OP_CONST(+=)
VEC0_EASSIGN_OP_CONST(-=)
VEC0_EASSIGN_OP_CONST(*=)
VEC0_EASSIGN_OP_CONST(/=)
#undef VEC0_EASSIGN_OP_CONST

  /// Overloading of the assignment operator "="
  /// @pre the assignee is supposed to have same size and ownership 
  /// as the given CFVec (as in std::valarray).
  HHOST_DEV const CFVec<T,0>& operator= (const CFVec<T,0>& other)
  {
    // cf_assert(&other != this);
    copy(other,*this);
    return *this;
  }

  // Constructor from preallocated memory
  HHOST_DEV void wrap(size_t ns, T* data) {m_owner = false; m_size = ns; m_data = data;}
  
  /// return the array size 
  HHOST_DEV size_t size() const {return m_size;}
  
  /// Overloading of the stream operator "<<" for the output.
  /// "\n"ine introduced at the end of every line of the matrix.
  friend std::ostream& operator<<
    LTGT (std::ostream& out, const CFVec<T,0>& v);
  
  /// Overloading of the stream operator ">>" for the input
  friend std::istream& operator>> 
    LTGT (std::istream& in, CFVec<T,0>& v);

  /// Overloading of the "==" operator.
  /// @return true if all elements are equal elementwise
  HHOST_DEV friend bool operator== LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2);

  /// Overloading of the "==" operator.
  /// @param v given array
  /// @param value value for the comparison
  /// @return true if all elements are equal to value
  HHOST_DEV friend bool operator== LTGT (const CFVec<T,0>& v1, T value);

  /// Overloading of the "!=" operator.
  /// @return true if all elements are not equal elementwise
  HHOST_DEV friend bool operator!= LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2);

  /// Overloading of the "!=" operator.
  /// @param v given array
  /// @param value value for the comparison
  /// @return true if at least one element is not equal to value
  HHOST_DEV friend bool operator!= LTGT (const CFVec<T,0>& v1, T value);

#ifndef CF_HAVE_CUDA    
  /// Overloading of the "<" operator.
  /// @return true if the norm of the first CFVec is < than the norm of the second CFVec.
  HHOST_DEV friend bool operator< LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2); 
  
  /// Overloading of the "<" operator.
  /// @return true if all entries of the first CFVec are < than the given value
  HHOST_DEV friend bool operator< LTGT (const CFVec<T,0>& v1, T value);
#endif
  
  /// Overloading of the ">" operator.
  /// @return true if the norm of the first CFVec is > than the norm of the second CFVec.
  HHOST_DEV friend bool operator> LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2);
  
  /// Overloading of the ">" operator.
  /// @return true if all entries of the first CFVec are > than the given value
  HHOST_DEV friend bool operator> LTGT (const CFVec<T,0>& v1, T value);
 
  /// Overloading of the "<=" operator.
  /// @return true if the norm of the first CFVec is <= than the norm of the second CFVec.
  HHOST_DEV friend bool operator<= LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2);
  
  /// Overloading of the "<=" operator.
  /// @return true if all entries of the first CFVec are <= than the given value
  HHOST_DEV friend bool operator<= LTGT (const CFVec<T,0>& v1, T value);
  
  /// Overloading of the ">=" operator.
  /// @return true if the norm of the first CFVec is >= than the norm of the second CFVec.
  HHOST_DEV friend bool operator>= LTGT (const CFVec<T,0>& v1, const CFVec<T,0>& v2);
  
  /// Overloading of the ">=" operator.
  /// @return true if all entries of the first CFVec are >= than the given value
  HHOST_DEV friend bool operator>= LTGT (const CFVec<T,0>& v1, T value);

  /// Copy one CFVec into another one
  /// @pre v1.size() == v2.size()
  /// @param v1 source vector
  /// @param v2 destination vector
  HHOST_DEV friend void copy LTGT (const CFVec<T,0>& orig, CFVec<T,0>& dest);

  /// @return a vector slice with fixed size
  template <int NV>
  HHOST_DEV CFVecSlice<T,NV> slice(size_t start) {return CFVecSlice<T,NV>(&m_data[start]);}
  
  /// @return a vector slice
  HHOST_DEV CFVecSlice<T,0> slice(size_t start, size_t ns) {return CFVecSlice<T,0>(&m_data[start], ns);}

  /// @return the raw data
  HHOST_DEV T* ptr() {return m_data;}
  
  /// resize the array 
  HHOST_DEV void resize(size_t ns, T init = T()) 
  {free(); m_size = ns; allocate(); this->initialize(init);}
  
  /// Normalizes this vector so that the norm is equal to one.
  /// \f$ v = v/\|v\|\f$
  HHOST_DEV void normalize() {*this /= this->norm2();}
 
  /// Projection of one vector onto another.
  /// @param v1 1st CFCFVector
  /// @param v2 2nd CFCFVector
  /// @post this CFVec contains the projected vector of v2 onto v1.
  template <typename V1, typename V2>  
  HHOST_DEV void proj(const V1& v1, const V2& v2)
  {
    assert(v1.size() == v2.size());
    T scale = (MathFunctions::innerProd(v1, v2) / v1.sqrNorm());
    *this = v1 * scale;
  }

  /// Projection of this CFCFVector onto a given one.
  /// @pre this and the given object must be of the same size.
  /// @param v1 the other CFCFVector
  /// @post this CFCFVector contains the projected vector of itself onto v1.
  template <typename V1> 
  HHOST_DEV void proj(const V1& v1) { proj(*this,v1); }
  
private: // helper functions
  
  /// allocate the memory
  HHOST_DEV void allocate() {if (size() > 0) {assert(m_owner); m_data = new T[m_size];}}
  
  /// free the memory
  HHOST_DEV void free() {if (m_owner && m_size > 0) {delete [] m_data; m_data = NULL; m_size = 0;}}
    
private:
  
  /// boolean flag to know if CFVec has ownership on the data
  bool m_owner;
  
  /// array size
  size_t m_size;
  
  /// array data
  T* m_data;
};
  
//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
std::ostream& operator<< (std::ostream& out, const CFVec<T,N>& v)
{
  const size_t size = v.size();
  for (size_t i = 0; i < size; ++i)
    out << v.m_data[i] << " " ;
  return out;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
std::istream& operator>> (std::istream& in, CFVec<T,N>& v)
{
  const size_t size = v.size();
  for (size_t i = 0; i < size; ++i)
    in >> v.m_data[i];
  return in;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator== (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  assert(v1.size() == v2.size());
  const size_t size = v1.size();
  for (size_t i = 0; i < size; ++i) {
    if (v1.m_data[i] != v2.m_data[i]) {
      return false;
    }
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator== (const CFVec<T,N>& v, T value)
{
  const size_t size = v.size();
  for (size_t i = 0; i < size; ++i) {
    if (v[i] != value)  return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator!= (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  return !(v1 == v2);
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator!= (const CFVec<T,N>& v, T value)
{
  return !(v == value);
}

//////////////////////////////////////////////////////////////////////////////

#ifndef CF_HAVE_CUDA
template <typename T, int N>
bool operator< (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  return (v1.sqrNorm() < v2.sqrNorm());
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator< (const CFVec<T,N>& v1, const T value)
{
 const size_t size = v1.size();
 for (size_t i = 0; i < size; ++i) {
   if (v1[i] >= value) return false;
 }
 return true;
}
#endif

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator> (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  return v1.sqrNorm() > v2.sqrNorm();
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator> (const CFVec<T,N>& v1, T value)
{
  const size_t size = v1.size();
  for (size_t i = 0; i < size; ++i) {
    if (v1[i] <= value) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator<= (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  return !(v1.sqrNorm() > v2.sqrNorm());
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator<= (const CFVec<T,N>& v1, T value)
{
  const size_t size = v1.size();
  for (size_t i = 0; i < size; ++i) {
    if (v1[i] > value) return false;
  }
 return true;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator>= (const CFVec<T,N>& v1, const CFVec<T,N>& v2)
{
  return !(v1.sqrNorm() < v2.sqrNorm());
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
bool operator>= (const CFVec<T,N>& v1, T value)
{
  const size_t size = v1.size();
  for (size_t i = 0; i < size; ++i) {
    if (v1[i] < value) return false;
  }
  return true;
}

//////////////////////////////////////////////////////////////////////////////

template <typename T, int N>
void copy (const CFVec<T,N>& orig, CFVec<T,N>& dest)
{
  assert(orig.size() == dest.size());
  const size_t size = orig.size();
  for (size_t i = 0; i < size; ++i)
    dest.m_data[i] = orig.m_data[i];
}

//////////////////////////////////////////////////////////////////////////////

 } // namespace MathTools

}   // namespace COOLFluiD

//////////////////////////////////////////////////////////////////////////////

#endif
