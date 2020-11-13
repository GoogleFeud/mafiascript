 
let fibonacci = (n) => {
   if (n <= 1) return 1;
   return fibonacci(n - 1) + fibonacci(n - 2);
};

let fizzbuzz1 = (n) => {
   loop (let i = 1, i <= n, i = (i + 1)) {
      if (((i % 3) == 0) && ((i % 5) == 0)) print("FizzBuzz")
      else if ((i % 3) == 0) print("Fizz")
      else if ((i % 5) == 0) print("Buzz")
      else print(i); 
   };
}; 

let fizzbuzz2 = (n) => {
   let rel = "";
   loop (let i = 1, i <= 100, i = (i + 1)) {
      if ((i % 3) == 0) {
         rel += "Fizz";
      }
      else if ((i % 5) == 0) {
         rel += "Buzz";
      }
      else {
         rel += i;
      };
   };
   return rel;
};

let fnThatReturnsFn = () => {
   let a = 3000;
   return [a](b) => a + 35 + b;
};

let fn = (a) => {
   let res = fizzbuzz2(100);
   return res;
};

let accessTest = () => {
   let obj = {
      "a": (a, b) => {
         return {
            "res": [a, b](c) => {
            return a + b + c;
           }
         };
      }
   };
   let arr = [5];
   let arrAccess = 0;
   return obj.a(10, 10).res(arr[arrAccess]);
};