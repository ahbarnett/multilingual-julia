# tester for ArrMod module. Eg from shell run with: julia test_arrmod.jl

push!(LOAD_PATH,".")
using ArrMod

n=Int(1e7)
x = rand(n)

# warm up all methods...
foo(x)
foomp(x)
y = similar(x)        # output needs allocation
foomp2(x,y)           # writes into y
foomp2_wrap(pointer(x),pointer(y),n)

# time all methods...
@time y = foo(x)
@time y = foomp(x)
@time foomp2(x,y)
@time foomp2_wrap(pointer(x),pointer(y),n)
