# tester for ArrModF module. Eg from shell run with: julia test_arrmodf.jl

push!(LOAD_PATH,".")
using ArrModF

n=Int(1e7)
x = rand(n)
y = similar(x)        # needs allocation
foomp2(x,y)           # writes into y

# note all funcs have already been called before we attempt to time them...
dummy,t = @timed foomp2(x,y)
println("foomp2 done in $t s ($(n/t/1e9) Geval/s)")
