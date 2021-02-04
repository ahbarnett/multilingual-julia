! example of calling Julia func in a user module from Fortran (via C wrapper).
! The function is foomp2 (elementwise exp of 1D array) also tested in ../ccallj
! Also does timing, for kicks.
! Barnett 12/29/20

program modulearr
  implicit none
  integer*8 n, i, ier
  real*8 err,maxerr
  real*8, allocatable:: x(:), y(:)

  ! for timing...
  integer*8 t1,t2,crate
  real*8 t

  n = int(1.0e8)
  ! I & O arrays...
  allocate(x(n))
  print *,'filling input array, n=',n
  call system_clock(t1)
  do i=1,n
     x(i) = 1.0 + float(i)/n
  enddo
  call system_clock(t2,crate)
  t = (t2-t1)/float(crate)
  allocate(y(n))
  print *,'done in ',t,' sec. Calling wrapper...'

  ! this wrapper is specific to the func. We'd instead want general wrappers...
  call julia_foomp2(x,y,n,ier)
  
  call system_clock(t1)
  t = (t1-t2)/float(crate)
  print *,'julia call done, ier=',ier,' (should be 0)'
  print *,'time: ',t,' sec (',n/t/1.0D9,' Geval/s)'

  ! math check
  maxerr = 0.0
  do i=1,n
     err = abs(exp(x(i))-y(i))
     if (err.ge.maxerr) then
        maxerr = err
     endif
  enddo
  print *,'max err = ',maxerr
  
end program modulearr
