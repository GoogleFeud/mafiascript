
let fn = (a, b) => {
   let res = 0;
   loop (let i = 0, i != b, i = i + 1) {
      res = res + a;
   };
   return res;
};