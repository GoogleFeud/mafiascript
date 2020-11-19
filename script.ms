 
let fibonacci = (n) => {
   if (n == 0) return 0;
   const arr = [0, 1];
   loop (let i=2, i <= n, i += 1) {
      arr[i] = arr[i - 1] + arr[i - 2];
   };
   return arr[n];
};

let fibonacci2 = (n) => {
   if (n < 1) return 0;
   if (n <= 2) return 1;
   return fibonacci2(n - 1) + fibonacci2(n - 2);
};

let fizzbuzz1 = (n) => {
   loop (let i = 1, i <= n, i += 1) {
      if ((i % 3) == 0 && (i % 5) == 0) print("FizzBuzz")
      else if ((i % 3) == 0) print("Fizz")
      else if ((i % 5) == 0) print("Buzz")
      else print(i); 
   };
}; 

let fizzbuzz2 = (n) => {
   let rel = "";
   loop (let i = 1, i <= 100, i += 1) {
      if ((i % 3) == 0) rel += "\nFizz"
      else if ((i % 5) == 0) rel += "\nBuzz"
      else rel += "\n" + i;
   };
   return rel;
};

let fn = (a) => {
   //fizzbuzz1(100);
   const arr = [1, 2, 3, 4, 5, 6];
   let res = "Hello";
   res = "Hell";
   res = arr;
   res[0] = 15;
   res.fib = fibonacci;
   return res.fib(10);
};