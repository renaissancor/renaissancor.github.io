# CCW Counter Clockwise Algorithm 

Reference Problem 

- [Baekjoon 2166](https://www.acmicpc.net/problem/2166)
- [Baekjoon 2162](https://www.acmicpc.net/problem/2162)

Cross Product, aka Outer Product, aka Vector Product (외적) 

$$ 
CCW(A, B, C) = (x_2 - x_1)(y_3 - y_1) - (y_2 - y_1)(x_3 - x_1)
$$

`>0` Counterclockwise 
`<0` Clockwise 
`==0` Collinear 

## Linear Inersectoin  

$CCW(A, B, C) \times CCW(A, B, D) \leq 0$ 

$CCW(C, D, A) \times CCW(C, D, B) \leq 0$ 

Means, two lines `AB` and `CD` intersects. 

However, there is one more probality, in same line. 
Meaning that, 

$CCW(A, B, C) == CCW(A, B, D) == CCW(C, D, A) == CCW(C, D, B)$ 

