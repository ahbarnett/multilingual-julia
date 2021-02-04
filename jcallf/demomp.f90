!  Demo calling OMP routine in Fortran90.
!  Compile into a shared object; see makefile.

SUBROUTINE sumexp(a,ans,n)
  ! sums the exp of double array a, length n
  use omp_lib               ! needed to access types of omp_ calls
  implicit none
  integer*8 n,i
  real*8 a(n),ans(1)
  
  ans=0.0
  print *,'f90 nthreads=',omp_get_max_threads()
  !$omp parallel do schedule(static) shared(a,n) private(i) reduction(+:ans)
  do i=1,n
     ans = ans + exp(a(i))
  enddo
  !$omp end parallel do
end SUBROUTINE sumexp
