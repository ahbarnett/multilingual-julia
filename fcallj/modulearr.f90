! example of calling Julia func in a user module from Fortran (via C wrapper).
! This func is foomp2 (elementwise exp of 1D array), also tested in ../ccallj
! Also does timing, and test, for kicks.
! Note the wrapper has 3 routines: julia_setup, julia_foomp2, julia_cleanup.
! Barnett 2/3/21

program modulearr
  implicit none
  integer*8 n, i, ier
  real*8 err,maxerr
  real*8, allocatable:: x(:), y(:)

  ! for timing...
  integer*8 t1,t2,crate
  real t

  call system_clock(t1)
  call julia_setup(ier)
  call system_clock(t2,crate)
  t = (t2-t1)/float(crate)
  print *,'julia_setup done in ',t,' sec, ier=',ier
  if (ier.ne.0) then
     error stop
  endif
  
  n = int8(5.0e7)
  ! I & O arrays...
  call system_clock(t1)
  allocate(x(n))
  print *,'filling input array, n=',n
  do i=1,n
     x(i) = 1.0 + float(i)/n
  enddo
  allocate(y(n))
  call system_clock(t2)
  t = (t2-t1)/float(crate)
  print *,'done in ',t,' sec. Calling foomp2...'

  ! do it (note: n must be int*8)
  call julia_foomp2(x,y,n)
  
  call system_clock(t1)
  t = (t1-t2)/float(crate)
  print *,'julia call done, time: ',t,' sec: ',n/t/1.0E9,' Geval/s'

  ! math check
  maxerr = 0.0
  do i=1,n
     err = abs(exp(x(i))-y(i))
     if (err.ge.maxerr) then
        maxerr = err
     endif
  enddo
  call system_clock(t2)
  t = (t2-t1)/float(crate)
  print *,'max err = ',maxerr, '(took ',t,' sec singlethreaded Fortran)'

  call julia_cleanup
end program modulearr
