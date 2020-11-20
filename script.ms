 
let fibonacci = (n) => {
   if (!n) return 0;
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
   loop (let i = 1, i <= n, i++) {
      if ((i % 3) == 0 && (i % 5) == 0) print("FizzBuzz")
      else if ((i % 3) == 0) print("Fizz")
      else if ((i % 5) == 0) print("Buzz")
      else print(i); 
   };
}; 


const fnThatTakesObj = (obj) => {
   return obj.a + obj.b;
};

const res = (array, x) => {
  if (array.size() >= 10) return true
  else return false;
};

let fn = (a) => {
   const arr = [].fill(null, 50);
   let mapped = arr.map((el, index) => fibonacci(index));
   return mapped.reduce((acc, i) => acc + i);
};