# demo julia calling fortran which uses OMP. See demomp.f
# Also times it. Barnett 12/28/20

n=Int64(1e8)
a = 10*rand(n).-5.0    # don't forget dot!
b = [0.0]       # alloc output value (ptr, since 1-size array)

println("start ccall...")    # here timed returns struct; we want 2nd element
dummy,t = @timed ccall((:sumexp_, "demomp.so"), Cvoid, (Ref{Float64},Ref{Float64},Ref{Int64}),a,b,Ref(n))
println("Fort $t s. \t ans=",b[1])

# seems like no obvious way to multithread a std dot call (SGJ wanted) :( ...
bj,tj = @timed sum(exp.(a))          # get result and native time
println("julia $tj s. \t ans=",bj)

println("relerr=",(b[1]-bj)/bj)
