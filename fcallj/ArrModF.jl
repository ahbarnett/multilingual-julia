module ArrModF
# example module doing something in julia to test calling from Fortran.
# Also tests julia multithreading. Also see: test_arrmodf.jl

using Base.Threads

export foomp2

println("loaded ArrModF: num threads: ",Threads.nthreads())

"""foo demo func, writes elementwise exp of float array to 2nd array, Fortran-style, multithreaded"""
function foomp2(x,y)
    @assert length(x)==length(y)       # assumes y allocated
    @threads for i in eachindex(x)
        y[i] = exp(x[i])
    end
end

end   # module

