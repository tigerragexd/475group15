//
//  main.cpp
//  475-p4
//
//  Created by Alexander Athan on 3/12/14.
//  Copyright (c) 2014 Alex. All rights reserved.
//

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "uberzahl.h"

//c = exponent, a = base, p,q self expl.
uberzahl originalModExp(uberzahl c, uberzahl a, uberzahl p, uberzahl q);
uberzahl chineseModExp(uberzahl c, uberzahl a, uberzahl p, uberzahl q);
uberzahl origMont(uberzahl c, uberzahl a, uberzahl p, uberzahl q);
uberzahl chinMont(uberzahl c, uberzahl a, uberzahl p, uberzahl q);

uberzahl originalModExp(uberzahl c, uberzahl a, uberzahl p, uberzahl q){
	uberzahl z = 1;
	uberzahl n = p*q;

	unsigned int numBits = c.bitLength();

	unsigned int currentBit = 1;

	for (unsigned int i = 0; i < numBits; i++){

		z = (z*z) % n;

		currentBit = c.bit(numBits-i-1);

        if (currentBit == 1){
            z = (z*a) % n;
        }
        
	}

	return z;
}

uberzahl chineseModExp(uberzahl c, uberzahl a, uberzahl p, uberzahl q){
	uberzahl dp = c % (p-1);
	uberzahl dq = c % (q-1);

	uberzahl T = q.inverse(p);
	

	uberzahl m1 = originalModExp(dp,a,p,1);
	uberzahl m2 = originalModExp(dq,a,q,1);
    
	uberzahl u  = ((m1-m2)*T) % p;
	uberzahl m = m2 + u*q;
    
	return m;
}

uberzahl montRed(uberzahl T, uberzahl R, uberzahl M){

	uberzahl m = (T)*(R-(M.inverse(R))); //T*R^(-1) % n
	if (m>=R){
		uberzahl bitDiff = (m.bitLength()-R.bitLength());
		uberzahl two = 2;
		m = m & (two.exp(bitDiff)-1); //bitwise and with 2^(bitDiff)-1 1's
	}
	//std::cout << "m: " << m << "\n";
	uberzahl t = (T + m*M)/R;

	//std::cout << "t: " << t << "\n";
	if (t >= M)
		return(t-M);
	//*************
	return t;
}

uberzahl montMultiply(uberzahl a, uberzahl b, uberzahl R, uberzahl n){
	uberzahl aBar = a*R % n;
	uberzahl bBar = b*R % n;
	uberzahl T = aBar * bBar;

	std::cout << "M: " << n << "\n";
	std::cout << "R: " << R << "\n";
	std::cout << "T: " << T << "\n";
	std::cout << "M': " << n-(n.inverse(R)) << "\n";

	
	uberzahl zBar = montRed(T,R,n);

	return zBar;	

}

uberzahl origMont(uberzahl c, uberzahl a, uberzahl p, uberzahl q){
	uberzahl z = 1;
	uberzahl n = p*q;

	unsigned int numBits = c.bitLength();

	unsigned int currentBit = 1;
	
	uberzahl two = 2;

	uberzahl R = 2;
	R = two.exp(n.bitLength());
	

	for(unsigned int i = 0; i < numBits; i++){
		//z = z*z % n;
		z = montMultiply(z,z,R,n);
		currentBit = c.bit(numBits-i-1);

        if (currentBit == 1){
			z = montMultiply(z,a,R,n); 
        }

	}

	return z;
}
//***************************
int main()
{
	//uberzahl c = 3533;
	//uberzahl x = 9726;
	//uberzahl n = 11413;
	uberzahl p = 13;
	uberzahl q = 11;
	uberzahl a = 20;
	uberzahl c = 8;

	uberzahl result = 1;
    
	result = originalModExp(c,a,p,q);
    std::cout << "original: " << result << "\n";
	result = chineseModExp(c,a,p,q);
    std::cout << "chinese: " << result << "\n";
	result = origMont(c,a,p,q);
    std::cout << "origMont: " << result << "\n";
    return 0;
}



