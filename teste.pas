program teste (input, output);
	var x,y: integer;
	procedure A(var z: integer; w: integer);
		var t: integer;
		begin
			t := z + w * x * y
		end;
	function factorial(n: integer) : integer;
		begin
			if n > 2 then
				factorial := factorial(n-1) * n
			else
				factorial := 1
		end;
	procedure testeWhile(n: integer);
		begin
			while n < 1 do n := n - 1
		end;
	procedure testeRepeat(n: integer);
		begin
			repeat n := n - 1 until n < 1
		end;
	begin
		testeRepeat(10);
		testeWhile(10);
		x := factorial(3)
	end.
