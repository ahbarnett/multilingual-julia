module ArrMod
# example module doing something in julia to test calling from C.
# Also see test_arrmod.jl

using Base.Threads                     # use OMP

export foo, foomp, foomp2

"""foo demo func, returns elementwise exp of a float array."""
foo(x) = exp.(x)

"""foo demo func, alloc & returns elementwise exp of float array, multithreaded (well, that fails, so not yet)"""
function foomp(x)       # ::Array{Float64,1})
    y = similar(x)                 # alloc but don't fill output
#    @threads for i in eachindex(x)       # segfaults when called from C!
    for i in eachindex(x)                 # works when called from C
        y[i] = exp(x[i])
    end
    y
end

"""foo demo func, writes elementwise exp of float array to 2nd array, Fortran-style, multithreaded"""
function foomp2(x,y)
    @assert length(x)==length(y)      # assumes y allocated
    @threads for i in eachindex(x)         # use OMP
        y[i] = exp(x[i])
    end
end

end   # module

