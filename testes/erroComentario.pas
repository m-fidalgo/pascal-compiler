program teste (input, output);
	{comentario
	var x,y: real;
	procedure A(var a: integer; w: integer);
		var t, x: integer;
		begin
			t := x + w;
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
	procedure testeRepeat(b: integer);
		begin
			repeat b := b - 1 until b < 1
		end;
	begin
		testeRepeat(10);
		testeWhile(10);
		x := factorial(3)
	end.
