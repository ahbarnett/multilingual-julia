# tester for ArrMod module. Eg from shell run with: julia test_arrmod.jl

push!(LOAD_PATH,".")
using ArrMod

n=Int(1e7)
x = rand(n)
foo(x)
foomp(x)

y = similar(x)        # needs allocation
foomp2(x,y)           # writes into y

# note all funcs have already been called before we attempt to time them...
@time y = foo(x)
@time y = foomp(x)
@time foomp2(x,y)
@time foomp2_len(x,y,n)
@time foomp3(x,y,n)
