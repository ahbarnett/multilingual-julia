# tester for ArrModF module. Eg from shell run with: julia test_arrmodf.jl

push!(LOAD_PATH,".")
using ArrModF

n=Int(1e7)            # reasonable size to test speed
x = rand(n)
y = similar(x)        # needs allocation

# warm up all methods...
foomp2(x,y)           # writes into y
foomp2_wrap(pointer(x),pointer(y),n)

# note all funcs have already been called before we attempt to time them...
dummy,t = @timed foomp2(x,y)
println("foomp2      done in $t s: $(n/t/1e9) Geval/s")
dummy,t = @timed foomp2_wrap(pointer(x),pointer(y),n)
println("foomp2_wrap done in $t s: $(n/t/1e9) Geval/s")
