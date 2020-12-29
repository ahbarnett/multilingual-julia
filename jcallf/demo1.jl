# julia try calling fortran. Pairs with demo1.f.   Barnett 12/27/20

a = [2.0]   # use arrays, len-1 for Float64
b = [4.2]
c = [NaN]   # allocates c array
ccall((:multiply_, "demo1.so"), Cvoid, (Ref{Float64},Ref{Float64},Ref{Float64}),a,b,c)
println("scalar test: err=",c[1]-a[1]*b[1])

# passing in integer too
n=Int64(1000000)
println("n=",n)
a = rand(n)
b = rand(n)
c = zeros(n)
# note Ref here...
ccall((:amultiply_, "demo1.so"), Cvoid, (Ref{Float64},Ref{Float64},Ref{Float64},Ref{Int64}),a,b,c,Ref(n))
println("array test: err =", maximum(abs.(c - a.*b)))
