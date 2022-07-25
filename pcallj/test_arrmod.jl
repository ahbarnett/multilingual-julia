# tester for ArrMod module. Eg from shell run with: julia test_arrmod.jl

push!(LOAD_PATH,".")
using ArrMod

using Printf
using BenchmarkTools
BenchmarkTools.DEFAULT_PARAMETERS.seconds = 1

n=Int(1e7)            # reasonable size to test speed
x = rand(n)
y = similar(x)        # needs allocation

# warm up all methods...
foomp2(x,y)           # writes into existing y
y = foomp(x)          # allocates y

# note all funcs have already been called before we attempt to time them...
#dummy,t = @timed foomp2(x,y)
#t = @elapsed foomp2(x,y)
#@printf "foomp2      done in %.3g s: %.3g Geval/s\n" t n/t/1e9

b = @benchmark y=foomp(x)
t = median(b.times/1e9)
@printf "benchmark foomp       median %.3g s: %.3g Geval/s\n" t n/t/1e9
b = @benchmark foomp2(x,y)
t = median(b.times/1e9)
@printf "benchmark foomp2      median %.3g s: %.3g Geval/s\n" t n/t/1e9

