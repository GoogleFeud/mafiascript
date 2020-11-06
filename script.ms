
let fn = (a, b) => {
   let res = 5;
   loop (let i = 0, i != b, i = i + 1) {
      if (i == 3) {
         return magic;
      };
      res = res + a;
   };
   return res;
};