rsa_mul(y: *int, a: *int, b: *int, n: int) {
	var c: int;
	var i: int;
	var j: int;

	i = 0;
	loop {
		if i == 2 * n {
			break;
		}

		y[i] = 0;

		i = i + 1;
	}

	i = 0;
	loop {
		if i == n {
			break;
		}

		j = 0;
		c = 0;
		loop {
			if j == n {
				break;
			}

			c = c + (a[i] & (-1 >> 32)) * (b[j] & (-1 >> 32));
			c = c + y[i + j];
			y[i + j] = c & (-1 >> 32);

			c = c >> 32;
			j = j + 1;
		}

		j = j + i;

		loop {
			if j == 2 * n {
				break;
			}

			c = c + y[j];
			y[j] = c & (-1 >> 32);

			j = j + 1;
			c = c >> 32;
		}

		i = i + 1;
	}
}

rsa_trymod(r: *int, d: *int, q: int, n: int): int {
	var i: int;
	var a: int;
	var s: int;

	i = 0;
	a = 0;
	s = 1;
	loop {
		if i == n {
			break;
		}

		a = a + (d[i] & (-1 >> 32)) * (q & (-1 >> 32));
		s = s + ((~a) & (-1 >> 32));
		s = s + (r[i] & (-1 >> 32));

		i = i + 1;
		a = a >> 32;
		s = s >> 32;
	}

	s = s + ((~a) & (-1 >> 32));
	s = s + (r[i] & (-1 >> 32));
	s = s >> 32;

	return ~s & ((q + (-1 >> 32)) >> 32);
}

rsa_domod(r: *int, d: *int, q: int, n: int) {
	var i: int;
	var a: int;
	var s: int;

	i = 0;
	a = 0;
	s = 1;
	loop {
		if i == n {
			break;
		}

		a = a + (d[i] & (-1 >> 32)) * (q & (-1 >> 32));
		s = s + ((~a) & (-1 >> 32));
		s = s + (r[i] & (-1 >> 32));
		r[i] = s & (-1 >> 32);

		i = i + 1;
		a = a >> 32;
		s = s >> 32;
	}

	s = s + ((~a) & (-1 >> 32));
	s = s + (r[i] & (-1 >> 32));
	r[i] = s & (-1 >> 32);
}

// Based on https://ridiculousfish.com/blog/posts/labor-of-division-episode-iv.html
rsa_mod(r: *int, x: *int, d: *int, n: int) {
	var i: int;
	var j: int;
	var nd: int;
	var k: int;
	var h: int;
	var q: int;
	var a: int;

	// Find the size of d
	nd = n;
	loop {
		if (d[nd - 1] & (- 1 >> 32)) != 0 || nd == 0 {
			break;
		}

		nd = nd - 1;
	}

	// Divide by a single word
	if (nd == 1) {
		i = 2 * n - 1;
		a = 0;
		loop {
			a = ((a << 32) + (x[i] & (- 1 >> 32))) % (d[nd - 1] & (- 1 >> 32));
			r[i] = 0;

			if i == 0 {
				break;
			}

			i = i - 1;
		}

		r[0] = a;
		return;
	}

	// Find the fudge factor
	h = d[nd - 1];
	k = 0;
	loop {
		if h & (1 << 31) != 0 {
			break;
		}

		h = h << 1;

		k = k + 1;
	}
	h = h | ((d[nd - 2] & (- 1 >> 32)) << k) >> 32;

	// Copy x to r
	i = 0;
	loop {
		if i == 2 * n {
			break;
		}

		r[i] = x[i] & (- 1 >> 32);

		i = i + 1;
	}
	r[i] = 0;

	i = 2 * n;
	j = 2 * n - nd;
	loop {
		a = (r[i] << (k + 32))
			| (r[i - 1] << k)
			| ((r[i - 2] << k) >> 32);

		// Compute guess
		q = a / h;

		// Bump it down
		q = q - (q >> 32);

		// Try twice since it is at most +2 from the lower guess
		q = q - rsa_trymod(&r[j], d, q, n);
		q = q - rsa_trymod(&r[j], d, q, n);

		rsa_domod(&r[j], d, q, n);

		if j == 0 {
			break;
		}

		i = i - 1;
		j = j - 1;
	}
}

rsa_sel(y: *int, x: *int, n: int, s: int) {
	var i: int;

	loop {
		if i == n {
			break;
		}

		y[i] = ((y[i] & ~s) | (x[i] & s)) & (-1 >> 32);

		i = i + 1;
	}
}

rsa_pow(y: *int, x: *int, d: *int, m: *int, t: *int, n: int) {
	var nd: int;
	var i: int;
	var e: int;

	y[0] = 1;

	i = 1;
	loop {
		if i == 2 * n {
			break;
		}

		y[i] = 0;

		i = i + 1;
	}

	nd = n - 1;
	loop {
		i = 0;
		e = d[nd];
		loop {
			if i == 32 {
				break;
			}

			rsa_mul(t, y, y, n);
			rsa_mod(y, t, m, n);

			rsa_mul(t, y, x, n);
			rsa_mod(t, t, m, n);

			rsa_sel(y, t, n, -((e >> 31) & 1));

			i = i + 1;
			e = e << 1;
		}

		if nd == 0 {
			break;
		}

		nd = nd - 1;
	}
}
