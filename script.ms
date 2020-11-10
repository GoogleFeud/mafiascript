 
let fibonacci = (n) => {
   if (n <= 2) return 1;
   return fibonacci(n - 1) + fibonacci(n - 2);
};

let fizzbuzz = (n) => {
   loop (let i = 1, i <= 100, i = (i + 1)) {
      if (((i % 3) == 0) && ((i % 5) == 0)) print("FizzBuzz")
      else if ((i % 3) == 0) print("Fizz")
      else if ((i % 5) == 0) print("Buzz")
      else print(i); 
   };
}; 

let fn = (a) => {
   fizzbuzz(100);
   return "Welp!";
};