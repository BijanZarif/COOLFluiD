#include "BaseRunLib.hh"

#include <tvmet/Vector.h>
#include <tvmet/Matrix.h>

using namespace std;

template < typename TYPE , int SIZE >
class RunTVMET : public BaseRunLib
{
  public:

  typedef tvmet::Vector<TYPE,SIZE>      DEVector;
  typedef tvmet::Matrix<TYPE,SIZE,SIZE> GEMatrix;

  RunTVMET () {};

  protected:

  void compute()
  {
   using namespace tvmet;

      for(int i = 0; i < nbelems; ++i)
      {

        GEMatrix& a = *A[i];
        DEVector& x = *X[i];
        DEVector& y = *Y[i];
        DEVector& z = *Z[i];

        z = 2*prod(a,x) + 1.5*y;

#if 0
        cout << "A = " << a << endl;
        cout << "x = " << x << endl;
        cout << "y = " << y << endl;
        cout << "z = 2*A*x + 1.5*y = " << z << endl;
#endif
      }
  }

  void init()
  {
      A = new GEMatrix*[nbelems];
      for(int i = 0; i < nbelems; ++i)
      {
          A[i] = new GEMatrix;
          init(*A[i]);
      }

      X = new DEVector*[nbelems];
      Y = new DEVector*[nbelems];
      Z = new DEVector*[nbelems];
      for(int i = 0; i < nbelems; ++i)
      {
          X[i] = new DEVector;
          Y[i] = new DEVector;
          Z[i] = new DEVector;
          init(*X[i]);
          init(*Y[i]);
      }
  }

  void finalize()
  {
      for(int i = 0; i < nbelems; ++i)
          delete A[i];
      delete [] A;

      for(int i = 0; i < nbelems; ++i)
      {
          delete X[i];
          delete Y[i];
          delete Z[i];
      }
      delete [] X;
      delete [] Y;
      delete [] Z;
  }

  private: // helper functions
  void init (GEMatrix& A)
  {
    for (int i=0; i<SIZE; ++i){
      for (int j=0; j<SIZE ; ++j){ A(i,j)= (i+1)*(j+1); }
    }
  }

  void init (DEVector& v)
  {
    for (int j=0; j<SIZE ; ++j){ v(j) = j+1; }
  }

  private: // data

    GEMatrix ** A;
    DEVector ** X;
    DEVector ** Y;
    DEVector ** Z;
};
