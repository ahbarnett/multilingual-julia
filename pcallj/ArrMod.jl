module ArrMod
# example module doing something in julia to an array, to test calling from Py.
# Also see: test_arrmod.jl

using Base.Threads

export foomp,foomp2

println("loaded ArrMod: num threads: ",Threads.nthreads())

"""foo demo func, alloc & returns elementwise exp of float array, multithreaded"""
function foomp(x)
    y = similar(x)                 # alloc but don't fill output
    @threads for i in eachindex(x)                 # works when called from C
        y[i] = exp(x[i])
    end
    y
end

"""foo demo func, writes elementwise exp of an array into 2nd array, multithreaded"""
function foomp2(x,y)
    @assert length(x)==length(y)       # assumes y allocated
    @threads for i in eachindex(x)
        y[i] = exp(x[i])
    end
end

end   # module

