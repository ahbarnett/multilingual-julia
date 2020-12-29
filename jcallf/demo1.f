c     compile into a shared object via:
c     gfortran demo1.f -o demo1.so -shared -fPIC
c     seems like need to restart julia to take effect (reimport the .so)
      
      SUBROUTINE MULTIPLY(A,B,C)
c     multiplies two floats
      real*8 A,B,C
c     print *,'A=',A
      C = A*B
      RETURN
      end
      
      SUBROUTINE AMULTIPLY(A,B,C,n)
c     multiplies two arrays length n
      integer*8 n,i
      real*8 A(n),B(n),C(n)
      print *,'n=',n
      do i=1,n
         C(i) = A(i)*B(i)
      enddo
      RETURN
      END
