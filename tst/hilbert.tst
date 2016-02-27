gap> magic33:=["mat", [ [ 1, 1, 1, -1, -1, -1, 0, 0, 0 ],
> [ 1, 1, 1, 0, 0, 0, -1, -1, -1 ], [ 0, 1, 1, -1, 0, 0, -1, 0, 0 ],
> [ 1, 0, 1, 0, -1, 0, 0, -1, 0 ], [ 1, 1, 0, 0, 0, -1, 0, 0, -1 ],
> [ 0, 1, 1, 0, -1, 0, 0, 0, -1 ], [ 1, 1, 0, 0, -1, 0, -1, 0, 0 ] ] ];;
gap> HilbertBasis4ti2(magic33);
rec( zhom := [ [ 2, 0, 1, 0, 1, 2, 1, 2, 0 ], [ 1, 0, 2, 2, 1, 0, 0, 2, 1 ],
[ 0, 2, 1, 2, 1, 0, 1, 0, 2 ], [ 1, 2, 0, 0, 1, 2, 2, 0, 1 ],
[ 1, 1, 1, 1, 1, 1, 1, 1, 1 ] ] )
gap> problem:=[ "mat", [ [ 1, -3, 9, -1, 81 ], [ -9, 27, 81, -9, 1 ],
> [ 1, -3, 9, 1, -81 ] ], "rel", [ "=", "<", ">"], "sign", [1, 0, 1, 0, -1] ];;
gap> HilbertBasis4ti2(problem);
rec( zhom := [ [ 3, 1, 0, 0, 0 ], [ 0, -364, 0, -1095, -27 ], [ 1, 0, 0, 1, 0 ],
	[ 1, -121, 0, -365, -9 ], [ 0, -1, 0, 3, 0 ], [ 0, -27, 0, -81, -2 ],
  [ 0, -14, 0, -39, -1 ], [ 2, -13, 0, -40, -1 ], [ 0, 0, 1, 9, 0 ] ] )
gap> problem:=["mat", [[1, 1, 1, -1, -1, -1, 0, 0, 0 ],
> [1, 1, 1, 0, 0, 0, -1, -1, -1], [0, 1, 1, -1, 0, 0, -1, 0, 0],
> [1, 0, 1, 0, -1, 0, 0, -1,  0], [ 1, 1, 0, 0, 0, -1, 0, 0, -1],
> [ 0, 1, 1, 0, -1, 0, 0, 0, -1], [ 1, 1, 0, 0, -1, 0, -1, 0, 0]],
> "rel", [[0, 0 ,0 ,0, 0, 0, 0]],
> "sign", [[0, 0, 0, 0, 0, 0, 0, 0, 0]]];;
gap> HilbertBasis4ti2(problem);
rec( zfree := [ [ 2, 2, -1, -2, 1, 4, 3, 0, 0 ],
	[ 0, -1, 1, 1, 0, -1, -1, 1, 0 ], [ -1, 0, 1, 2, 0, -2, -1, 0, 1 ] ],
	zhom := [  ] )