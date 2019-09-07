The GAP 4 package `4ti2gap`
===========================

1 Package description
---------------------
This package intends to use the tools included in 4ti2 from the gap shell.
So far it provides (limited) access to

* groebner,
* minimize,
* normalform,
* markov, 
* walk, 
* zbasis, 
* hilbert,
* graver,
* zsolve.

We took as initial template [NormalizInterface](https://github.com/fingolfin/NormalizInterface), and got plenty of help from Sebastian Gutshe.

2 Installation preparation
--------------------------

To run this package you need 4ti2 installed with the components groebner and zsolve enabled, configured with the option --enable-shared.

In case you want to use this package with *hpc-gap*, you should append the option --with-hpcgap, to the `configure` command used in order to build the *4ti2gap* binaries.

The library GMP must be installed with the c++ option enabled, and also 4ti2 and GAP should be installed with multiple precision arithmetic enabled.

If you have GMP and 4ti2 installed in your system, you can try to build 4ti2gap in its folder. The following commands will try to find a GMP installed in /usr or /usr/local. (Note that the installation folders for GAP and 4ti2 should be specified with the appropiate options.)

	./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=yes
	make

Alternativelly, if GMP is installed in another folder, the value of --with-gmp should be this path of this folder.

If you want to use 4ti2gap with GMP, best way to get it working is the following. Compile gmp with

	./configure --prefix=whereyouwanttoinstallgmp --enable-cxx

Compile gap with

	./configure --with-gmp=whereyouinstalledgmp
	make

Then compile 4ti2 with

	./configure --enable-shared --prefix=whereyouwanttoinstall4ti2 --with-gmp=whereyouinstalledgmp
	make
	make install

In some systems 4ti2 does not find the GNU Linear Programming Kit needed by the groebner component. Adding the path to GLPK to the previous configure command line may help:

	...$ ./configure --enable-shared --prefix=whereyouwanttoinstall4ti2 --with-gmp=whereyouinstalledgmp --with-glpk=whereglpkisinstalled

Finally go to 4ti2gap folder and do

	./autogen.sh

and next

	./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=whereyouinstalledgmp

	make

If the compilation process fails with the error `fatal error: 'glpk.h' file not found`, then it is necessary to specify the glpk path to include files before the configure command using CPPFLAGS:

  CPPFLAGS="-I whereitisinstalledglpk/include" ./configure --with-gaproot=whereyouinstalledgap --with-4ti2=whereyouinstalled4ti2 --with-gmp=whereyouinstalledgmp

	make

3 Documentation and tests
-------------------------

### Gröbner bases

groebner is called in the following way:

`GroebnerBasis4ti2( matrix , order )`, where matrix is a matrix with integer entries and order is either an order matrix or "lex", "grlex" "grevlex".

```gap
gap> GroebnerBasis4ti2([[3,5,7]],[[3,5,7]]);

[ [ -4, 1, 1 ], [ -3, -1, 2 ], [ -1, 2, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"lex");

[ [ 0, 7, -5 ], [ 1, -2, 1 ], [ 1, 5, -4 ], [ 2, 3, -3 ], [ 3, 1, -2 ], [ 4, -1, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"grevlex");

[ [ -1, 2, -1 ], [ 3, 1, -2 ], [ 4, -1, -1 ] ]

gap> GroebnerBasis4ti2([[3,5,7]],"grlex");  

[ [ 0, 7, -5 ], [ 1, -2, 1 ], [ 1, 5, -4 ], [ 2, 3, -3 ], [ 3, 1, -2 ], [ 4, -1, -1 ] ]
```
If GMP is installed, `GroebnerBasis4ti2( matrix , order )` uses integer multiple precision arithmetic.


### Integer linear program minimal solution

minimize is called in the following way:

`Minimize4ti2( matrix, lattice, cost, sol, sign )`, where

- matrix is optional as an empty list [] only if lattice basis is given,

- lattice basis also optional as an empty list [] only if matrix is given,

- the cost vector should be a matrix with 1 row or a flat list,

- sol is an integer solution to specify a fiber (same format as cost vector),

- the sign are the constraints of the variables (1 o 0 for non-negative and free variable, resp., optional as an empty list [], and the default is all non-negative).


```gap
gap> Minimize4ti2([[3,5,7]],[],[[1,1,1]],[3,6,9],[1,1,1]);
[ [ 2, 1, 13 ] ]
gap> GroebnerBasis4ti2([[3,5,7]]);
[ [ -4, 1, 1 ], [ -3, -1, 2 ], [ -1, 2, -1 ] ]
gap> Minimize4ti2([],last,[[1,1,1]],[3,6,9],[1,1,1]);
[ [ 2, 1, 13 ] ]
```

### Normal form

normalform is called in the following way:

`Normalform4ti2(matrix, lattice, groebner, cost, feas, sign)` where

- matrix is optional as an empty list [], only if lattice basis is given,

- lattice basis also optional as an empty list [], only if matrix is given, 

- a Gröbner basis (the differences of the exponents) of the lattice ideal

- the cost vector should be a matrix with 1 row or a flat list, also optional as an empty list [] (default is degrevlex),

- feas is a list of solutions of the problem (the ones we want to compute their normal form),

- sign are the constraints of the variables (1 or 0 for non-negative and free variable, resp., optional as an empty list [], and the default is all non-negative).

```gap
gap> g:=GroebnerBasis4ti2([[3,5,7]]);;
gap> Normalform4ti2([[3,5,7]],[],g,[[1,1,1]],[[10,11,23],[7,0,0]],[[1,1,1]]);
[ [ 1, 1, 34 ], [ 0, 0, 3 ] ]
```

### Markov bases

markov is called in the following way:

`MarkovBasis4ti2(matrix, lattice, sign, weights, weights_max, zsol)` where

- matrix is optional as an empty list [], only if lattice basis is given,

- lattice basis also optional as an empty list [] only if matrix is given,

- sign constraints of the variables (1 or 0 for non-negative and free variable, resp., optional as an empty list [], and the default is all non-negative).

- weights vector used for truncation (optional as an empty list []).

- weights_max are the maximum weights used for truncation (optional as an empty list [], needed if weights is given).

- zsol is an integer solution to specify a fiber (optional as an empty list, the integer solution is used for truncation).

```gap
gap> lat:=[[-3, 2, -2, 0, 0, 0, -1, 1, 0, 1, 1, 3, 0], [1, 1, 2, 0, 1, -1, -1, 1, -2, 0, -2, -3, 1], [-3, 1, -4, 1, 0, -1, 2, 2, 2, 0, 2, 1, -1], [21, -21, 8, -2, -4, 5, 14, -15, 9, -8, -1, -10, -3], [42, -43, 15, -2, -7, 10, 25, -25, 17, -18, 1, -24, -6], [-1, 0, 4, -1, 3, -2, 1, -1, 0, -1, 0, -2, 1], [-83, 63, -41, 6, 9, -8, -44, 45, -17, 28, 17, 59, 5], [1, -1, -1, 2, 3, -1, -1, -2, 3, -2, 1, -1, 0], [-5, 3, -1, 1, -4, 3, 1, 1, 2, 0, 1, 2, -1]];
gap> zsol:=[1, 1, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0];
gap> MarkovBasis4ti2([], lat, [], [], [], zsol);
[ [ 0, 4, 3, 0, 0, -3, 0, -1, -1, 0, 2, -1, -1 ], [ 1, 1, 2, 0, 1, -1, -1, 1, -2, 0, -2, -3, 1 ], [ 3, -2, 2, 0, 0, 0, 1, -1, 0, -1, -1, -3, 0 ], [ 3, 4, 1, -1, -2, -1, 0, -2, -2, 1, -1, 0, 0 ] ]
```

### Integer linear program minimal solution

walk is called in the following way:

`Walk4ti2(matrix, lattice, gro_start, cost_start, cost, zsol, sign)`

- matrix is optional as an empty list [], only if lattice basis is given,

- lattice basis also optional as an empty list [], only if matrix is given, 

- gro_start is the starting Groebner basis (needed), 

- cost_start is the starting cost vector (optional as an empty list [], default is degrevlex),

- cost is the target cost vector (needed according to 4ti2 source code),

- zsol is an integer solution to specify a fiber (according to 4ti2 source code, optional as an empty list []), 

- sign constraints of the variables (1 or 0 for non-negative and free variable, resp., optional as an empty list [], and the default is all non-negative).

```gap
gap> mat:=[[12223, 12224, 36674, 61119, 85569]];;
gap> gro_start := [[-7336, 3, 2444, 0, 0], [-7334, -2, 2445, 0, 0], [-2, 5, -1, 0, 0], [-1, -4, 0, 1, 0], [-1, -3, -1, 0, 1]];;
gap> cost_start := [[-1, 0, 0, 0, 0]];;
gap> cost := [[1, 1, 0, 0, 0]];;
gap> Walk4ti2(mat, [], gro_start, cost_start, cost, [], []);
[ [-2, 2, 2, 3, -3 ], [ -2, 3, 1, 2, -2 ], [ -2, 4, 0, 1, -1 ], ....  [ 7317, -1, -2450, 7, 0 ], [ 7318, 3, -2450, 6, 0 ], [ 7320, -2, -2449, 6, 0 ], [ 7321, 2, -2449, 5, 0 ], [ 7323, -3, -2448, 5, 0 ], [ 7324, 1, -2448, 4, 0 ], [ 7326, -4, -2447, 4, 0 ], [ 7327, 0, -2447, 3, 0 ], [ 7330, -1, -2446, 2, 0 ], [ 7331, 3, -2446, 1, 0 ], [ 7333, -2, -2445, 1, 0 ], [ 7334, 2, -2445, 0, 0 ], [ 7336, -3, -2444, 0, 0 ] ]
```


### Lattice bases

zbasis is called in the following way:

`ZBasis4ti2(matrix)` 

```gap
gap> ZBasis4ti2([[15, 4, 14, 19, 2, 1, 10, 17], [18, 11, 13, 5, 16, 16, 8, 19], [11, 7, 8, 19, 15, 18, 14, 6], [17, 10, 13, 17, 16, 14, 15, 18]]);
[ [ -13, 1, 64, 0, 48, -36, -17, -35 ], [ -154, 0, 551, -4, 380, -273, -131, -265 ], [ -44, 0, 168, -3, 115, -84, -37, -83 ], [ 121, 0, -452, 0, -321, 231, 115, 222 ] ]
```

### Hilbert bases

hilbert actually only returns the homogenous output. It is called in the following way:

`HilbertBasis4ti2( problem )`, where problem is a list to specify the input components of the "problem" (see the 4ti2 manual):

```gap
gap> problem:=[ "mat", [ [ 1, -3, 9, -1, 81 ],
[ -9, 27, 81, -9, 1 ], [ 1, -3, 9, 1, -81 ] ],
"rel", [ "=", "<", ">"], "sign", [1, 0, 1, 0, -1] ];;
gap> HilbertBasis4ti2(problem);
rec( zhom := [ [ 3, 1, 0, 0, 0 ], [ 0, -364, 0, -1095, -27 ], [ 1, 0, 0, 1, 0 ],
	[ 1, -121, 0, -365, -9 ], [ 0, -1, 0, 3, 0 ], [ 0, -27, 0, -81, -2 ],
  [ 0, -14, 0, -39, -1 ], [ 2, -13, 0, -40, -1 ], [ 0, 0, 1, 9, 0 ] ] )

gap> problem:=["mat", [[1, 1, 1, -1, -1, -1, 0, 0, 0 ],
[1, 1, 1, 0, 0, 0, -1, -1, -1], [0, 1, 1, -1, 0, 0, -1, 0, 0],
[1, 0, 1, 0, -1, 0, 0, -1,  0], [ 1, 1, 0, 0, 0, -1, 0, 0, -1],
[ 0, 1, 1, 0, -1, 0, 0, 0, -1], [ 1, 1, 0, 0, -1, 0, -1, 0, 0]],
"rel", [[0, 0 ,0 ,0, 0, 0, 0]],
"sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];;
gap> HilbertBasis4ti2(problem);
rec( zfree := [ [ 2, 2, -1, -2, 1, 4, 3, 0, 0 ],
	[ 0, -1, 1, 1, 0, -1, -1, 1, 0 ], [ -1, 0, 1, 2, 0, -2, -1, 0, 1 ] ],
	zhom := [  ] )

gap> problem:=["mat", [[1, -31, -1, 1], [-111, 5, 10, 25]]];;
gap> HilbertBasis4ti2(problem);
rec( zhom := [ [ 35, 0, 136, 101 ], [ 15, 1, 36, 52 ], [ 195, 34, 0, 859 ],
	[ 110, 19, 4, 483 ], [ 25, 4, 8, 107 ] ] )
```

If GMP is installed, `HilbertBasis4ti2gmp( problem )` uses integer multiple precision arithmetic.

### Graver bases

graver actually only returns the homogenous output. It is called in the following way:
GraverBasis4ti2( problem ), where problem is a list to specify the input components of the "problem" (see the 4ti2 manual):

```gap
gap> problem:=["mat", [[1, 1, 1, 1, 1], [1, 2, 3, 4, 5]]];; GraverBasis4ti2( problem );

[ [ 1, -2, 1, 0, 0 ], [ 2, -3, 0, 1, 0 ], [ 3, -4, 0, 0, 1 ],
  [ 1, -1, -1, 1, 0 ], [ 2, -2, -1, 0, 1 ], [ 1, -1, 0, -1, 1 ],
  [ 0, 1, -2, 1, 0 ], [ 0, 1, -1, -1, 1 ], [ 1, 0, -2, 0, 1 ],
  [ 1, -2, 0, 2, -1 ], [ 0, 0, 1, -2, 1 ], [ 1, 0, -1, -2, 2 ],
  [ 0, 1, 0, -3, 2 ], [ 1, 0, -3, 2, 0 ], [ 1, 0, 0, -4, 3 ],
  [ 0, 2, -3, 0, 1 ] ]
```

If GMP is installed, `GraverBasis4ti2gmp( problem )` uses integer multiple precision arithmetic.

### ZSolve

zsolve performs the same computations as 4ti2 the command line version. Its input is expressed the same way as GraverBasis4ti2 and HilbertBasis4ti2. The output is a record with the components that gives the explicit description of all solutions (see the 4ti2 manual):

```gap
gap> problem:=["mat",[[0, 0, 0, 0, 0, 3, -4, -1, 2], [0, 0, 0, 0, 1, -1, 1, 0, -1], [0, 0, 0, 1, 2, 0, 0, -1, -2], [0, 0, 1, 0, 1, 0, 0, -1, -1], [0, 1, 2, 0, 0, 0, 0, -1, -2], [1, 0, 2, 0, 0, 0, 0, -2, -1], [-2, 0, -2, 0, 0, 0, 0, 3, 0], [-2, 0, 0, 0, 0, 0, 0, 1, 0], [0, 0, -2, 0, 0, 0, 0, 1, 0], [0, 0, 0, 0, 0, 0, 0, -1, 0]], "rel", [[0, 0, 0, 0, 0, 0, -1, -1, -1, -1]], "sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];; ZSolve4ti2(problem);

rec( zhom := [ [ 1, 0, 2, 2, 1, 0, 0, 2, 1 ], [ 2, 0, 1, 0, 1, 2, 1, 2, 0 ],
      [ 1, 2, 0, 0, 1, 2, 2, 0, 1 ], [ 1, 1, 1, 1, 1, 1, 1, 1, 1 ],
      [ 0, 2, 1, 2, 1, 0, 1, 0, 2 ] ],
  zinhom := [ [ 0, 0, 0, 0, 0, 0, 0, 0, 0 ] ] )

gap> ZSolve4ti2(["mat",[[0, 0]],"sign",[[0,0]]]);

rec( zfree := [ [ 1, 0 ], [ 0, 1 ] ], zhom := [  ], zinhom := [ [ 0, 0 ] ] )

gap> ZSolve4ti2(["mat",TransposedMat([[2,0],[0,2],[1,2],[2,1]]),"sign",
[1,1,1,1],"rhs",[[15,15]]]);
rec( zhom := [  ], zinhom := [ [ 0, 3, 1, 7 ], [ 2, 4, 1, 5 ], [ 4, 5, 1, 3 ],
[ 6, 6, 1, 1 ], [ 1, 2, 3, 5 ], [ 3, 3, 3, 3 ], [ 3, 0, 7, 1 ], [ 5, 4, 3, 1 ],
[ 4, 2, 5, 1 ], [ 2, 1, 5, 3 ], [ 0, 0, 5, 5 ] ] )

```

If GMP is installed, `ZSolve4ti2gmp( problem )` uses integer multiple precision arithmetic.


4 Roadmap
---------
1.- See if we can get rid of --enable-shared

2.- Produce a 32 bit version that uses gmp and can be compiled under cygwin.
