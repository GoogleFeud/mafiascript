 
let fibonacci = (n) => {
   if (n <= 1) return 1;
   return fibonacci(n - 1) + fibonacci(n - 2);
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
   let arr = [1, 2, 3, 4, 5];
   let res = fizzbuzz1(100);
   arr[arr.size()] = 6;
   arr[arr.size()] = 7;
   arr[arr.size()] = 8;
   const arr1 = arr;
   arr[arr.size()] = 9;
   arr[arr.size() - 1] /= 2;
   return arr[arr.size() - 1];
};
